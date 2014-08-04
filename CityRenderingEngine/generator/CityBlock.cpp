#include "CityBlock.h"

CityBlock::CityBlock(float density) : Entity() {
    vertices = new std::vector<Intersection*>();
    //model = Model::getOrCreate("cube", "resources/meshes/cube.obj", false);
    //shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        //"resources/shaders/fragLight.glsl", false);
    this->maximumPerimeterPerBuilding = 250.0f;
    this->numChunksSharing = 0;
    this->type = CITY_BLOCK_COMMERCIAL_HIGH;
    this->density = density;
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
        std::vector<Building*> buildingLots = splitLots(usableArea);

        // TODO: Tell each lot its type, according to its size and the type of this CityBlock, and "build" the Building.
        auto itEnd = buildingLots.end();
        for (auto it = buildingLots.begin(); it != itEnd; it++) {
            addChild(*it);
            (*it)->constructGeometry();
        }
    }
}

std::vector<Building*> CityBlock::splitLots(std::vector<Vector2> &lotPolygon) {
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
            // Create an empty building and return a vector containing it.
            Building *newBuilding = new Building(lotPolygon, this);
            std::vector<Building*> buildings = std::vector<Building*>();
            buildings.push_back(newBuilding);
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
            std::vector<Building*> buildings = splitLots(lotA);
            std::vector<Building*> buildingsB = splitLots(lotB);
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