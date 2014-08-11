#include "CityBlock.h"

CityBlock::CityBlock(float density) : Entity() {
    vertices = new std::vector<Intersection*>();
    //model = Model::getOrCreate("cube", "resources/meshes/cube.obj", false);
    //shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        //"resources/shaders/fragLight.glsl", false);
    this->numChunksSharing = 0;
    this->density = density;

    // Use density info to choose the type of this CityBlock
    // I guess there's no easy better way to do this except for these ugly ifs ):
    Vector3 centrePos = getCentralPosition();
    int posSum = (int) (centrePos.x + centrePos.y + centrePos.z); // Used to choose between types with same density
    if (density < 0) {
        type = CITY_BLOCK_VOID;
        this->maximumPerimeterPerBuilding = 9999.0f; // There won't be anything here, so why split?
    } else if (density < 0.4f) {
        // At this point, if may be either a park or low density commercial or residential block
        // Use mods to choose between the three
        if (posSum % 7 == 0) {
            type = CITY_BLOCK_PARK; // 14% chance
            this->maximumPerimeterPerBuilding = 9999.0f; // We want the park to occupy the whole block
        }
        else if (posSum % 3 == 0) {
            type = CITY_BLOCK_COMMERCIAL_LOW; // 33% chance
            this->maximumPerimeterPerBuilding = 150.0f;
        } else {
            type = CITY_BLOCK_RESIDENTIAL_LOW; // About 53% chance
            this->maximumPerimeterPerBuilding = 128.0f;
        }
    } else if (density < 0.8) {
        // It can be a park or medium density commercial or residential block
        if (posSum % 13 == 0) {
            type = CITY_BLOCK_PARK; // 8% chance
            this->maximumPerimeterPerBuilding = 9999.0f; // We want the park to occupy the whole block
        }
        else if (posSum % 2 == 0) {
            type = CITY_BLOCK_COMMERCIAL_MEDIUM; // 50% chance
            this->maximumPerimeterPerBuilding = 256.0f;
        } else {
            type = CITY_BLOCK_RESIDENTIAL_MEDIUM; // About 42% chance
            this->maximumPerimeterPerBuilding = 200.0f;
        }
    } else if (density < 0.9) {
        if (posSum % 3 == 0) {
            type = CITY_BLOCK_RESIDENTIAL_HIGH; // 33% chance
            this->maximumPerimeterPerBuilding = 300.0f;
        } else {
            type = CITY_BLOCK_COMMERCIAL_HIGH; // About 67% chance
            this->maximumPerimeterPerBuilding = 400.0f;
        }
    } else {
        // For densities higher than 0.8, we have only a very small chance of having a residential block
        if (posSum % 11) {
            type = CITY_BLOCK_RESIDENTIAL_HIGH; // 9% chance
            this->maximumPerimeterPerBuilding = 400.0f;
        } else {
            type = CITY_BLOCK_COMMERCIAL_HIGH; // About 91% chance
            this->maximumPerimeterPerBuilding = 512.0f;
        }
    }
}

CityBlock::~CityBlock(void) {
    if (vertices != nullptr) {
        vertices->clear();
        delete vertices;
        vertices = nullptr;
    }
}

void CityBlock::update(float millisElapsed) {
    if (posChanged || rotChanged || scaleChanged) {
        calculateModelMatrix(Vector3(), Vector3(), Vector3(1, 1, 1), false, false, false);
    }
    // Update the distanceToCamera if that's changed, and if it is, also update the distance of the children.
    if (Naquadah::getInstance()->getCurrentScene()->getCamera()->hasChanged()) {
        Camera *camera = Naquadah::getInstance()->getCurrentScene()->getCamera();
        Vector3 dir = position - camera->getPosition();
        distanceToCamera = Vector3::dot(dir, dir);
        for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); it++) {
            (*it)->update(millisElapsed);
        }
    }
}

void CityBlock::addVertice(Intersection *intersection) {
    this->vertices->push_back(intersection);
    this->position = getCentralPosition();
    this->posChanged = true;
}

void CityBlock::unloadOpenGL() {
    auto itEnd = childEntities->end();
    for (auto it = childEntities->begin(); it != itEnd; it++) {
        Model *model = (*it)->getModel();
        if (model != nullptr && model->getNumUsers() <= 1) {
            model->unload();
        }
    }
}

void CityBlock::generateBuildings() {
    if (density > 0) {
        // First we define the CityBlock's RenderRadius. This is the best place to do this as all the vertices (should) be
        // added already.
        Vector2 minPos = Vector2((float) MAX_INT, (float) MAX_INT);
        Vector2 maxPos = Vector2();

        // TODO: Move the roadWidth and pavementWidth to Road.h, and make them relative to the road type and size
        float roadWidth = 10.0f;
        float pavementWidth = 5.0f;

        // The usable area of the CityBlock, inset to allow space for the road and pavement.
        std::vector<Vector2> usableArea = std::vector<Vector2>();
        for (auto it = vertices->begin(); it != vertices->end(); it++) {
            Vector2 vertex = Vector2((*it)->getPosition().toVec2(Vector3(0, 1, 0)));
            usableArea.push_back(vertex);
            // To help define the RenderRadius
            minPos.x = min(minPos.x, vertex.x);
            minPos.y = min(minPos.y, vertex.y);
            maxPos.x = max(maxPos.x, vertex.x);
            maxPos.y = max(maxPos.y, vertex.y);
        }
        this->renderRadius = (maxPos - minPos).getLength() / 2.0f;

        usableArea = Geom::insetPolygon(usableArea, roadWidth / 2.0f + pavementWidth);

        // Split the usable area into lots
        std::vector<Building*> buildingLots = splitLots(usableArea, usableArea);

        // TODO: Tell each lot its type, according to its size and the type of this CityBlock, and "build" the Building.
        auto itEnd = buildingLots.end();
        for (auto it = buildingLots.begin(); it != itEnd; it++) {
            addChild(*it);
            (*it)->constructGeometry();
        }
    }
}

std::vector<Building*> CityBlock::splitLots(std::vector<Vector2> &lotPolygon, std::vector<Vector2> &originalLot) {
    int numSides = (int) lotPolygon.size();
    if (numSides >= 3) { // We need to have at least a triangle to do this
        // Calculate the perimeter of the lot and find the longest side
        Vector2 longA, longB;
        float perimeter = 0.0f;
        float longestLength = 0.0f;
        for (int i = 1; i < numSides + 1; i++) {
            Vector2 a = lotPolygon.at(i - 1);
            Vector2 b = (i == numSides) ? lotPolygon.at(0) : lotPolygon.at(i);
            float length = (b - a).getLength();
            perimeter += length;
            if (length > longestLength) {
                longA = a;
                longB = b;
                longestLength = length;
            }
        }
        if (perimeter < maximumPerimeterPerBuilding) {
            // First we iterate through all of the lot sides, and then through all of the CityBlock sides.
            // We do this to check which lot sides connects to the road, if any.
            bool connectsToRoad = false;
            float biggestConnection = 0.0f;
            int biggestConnAIndex = 0;
            Vector2 roadNormal = Vector2();
            for (int i = 1; i < numSides + 1; i++) {
                if (connectsToRoad) break;
                Vector2 buildA = lotPolygon.at(i - 1);
                Vector2 buildB = (i == numSides) ? lotPolygon.at(0) : lotPolygon.at(i);
                int blockSides = (int) originalLot.size();
                for (int j = 1; j < blockSides + 1; j++) {
                    Vector2 blockA = originalLot.at(j - 1);
                    Vector2 blockB = (j == blockSides) ? originalLot.at(0) : originalLot.at(j);
                    if (Geom::isPointOnLine(blockA, blockB, buildA) && Geom::isPointOnLine(blockA, blockB, buildB)) {
                        // If both of the lot side points are on the CityBlock's side line, then this side of the lot
                        // connects to the road.
                        connectsToRoad = true;
                        // We then compare the connections to find the largest conneting side, this will be the front
                        // of the building.
                        float sideLength = (buildB - buildA).getLength();
                        if (sideLength > biggestConnection) {
                            biggestConnection = sideLength;
                            biggestConnAIndex = i - 1;
                            roadNormal = Geom::vec2Normal(buildA, buildB);
                        }
                        break;
                    }
                }
            }
            // Now order the vertices to ensure the first two make up the largest road connection line.
            std::vector<Vector2> orderedLotPolygon = std::vector<Vector2>();
            for (int i = biggestConnAIndex; i < numSides + biggestConnAIndex; i++) {
                int index = i < numSides ? i : i - numSides;
                orderedLotPolygon.push_back(lotPolygon.at(index));
            }

            std::vector<Building*> buildings = std::vector<Building*>();
            if (connectsToRoad) {
                // Create an empty building and return a vector containing it.
                Building *newBuilding = new Building(orderedLotPolygon, this, true, roadNormal);
                buildings.push_back(newBuilding);
            }
            return buildings;
        } else {
            // Split this lot into 2 smaller lots and return their combined Building vectors.
            Vector2 midPoint = (longA + longB) / 2.0f;
            Vector2 diff = longB - longA;
            Vector2 perpA = midPoint; // perpAperpB is the split line
            Vector2 perpB = perpA + Vector2(-diff.y, diff.x);
            // TODO: Deviate perpA and perpB by -/+ 10% of the maximumPerimeter
            std::vector<Vector2> lotA = std::vector<Vector2>();
            std::vector<Vector2> lotB = std::vector<Vector2>();
            bool currentIsA = true;
            for (int i = 1; i < numSides + 1; i++) {
                Vector2 a = lotPolygon.at(i - 1);
                Vector2 b = (i == numSides) ? lotPolygon.at(0) : lotPolygon.at(i);
                Vector2 intersect = Geom::lineSegmentIntersection(perpA, perpB, a, b);
                if (intersect.x != MAX_INT) {
                    // The split line intersects this side
                    lotA.push_back(intersect);
                    lotB.push_back(intersect);
                    currentIsA = !currentIsA; // Switch between filling one lot or the other.
                }
                currentIsA ? lotA.push_back(b) : lotB.push_back(b);
            }
            // Now call it recursively and return the join between the 2 lots.
            std::vector<Building*> buildings = splitLots(lotA, originalLot);
            std::vector<Building*> buildingsB = splitLots(lotB, originalLot);
            buildings.reserve(buildings.size() + buildingsB.size());
            buildings.insert(buildings.end(), buildingsB.begin(), buildingsB.end());
            return buildings;
        }
    }
    // If we're trying to split a lot that has less than 3 sides, it's not a valid lot so we just ignore it
    return std::vector<Building*>();
}

Vector3 CityBlock::getCentralPosition() {
    int numVertices = (int) vertices->size();
    Vector3 centralPosition = Vector3();
    for (int i = 0; i < numVertices; i++) {
        centralPosition += vertices->at(i)->getPosition();
    }
    return centralPosition / (float) numVertices;
}