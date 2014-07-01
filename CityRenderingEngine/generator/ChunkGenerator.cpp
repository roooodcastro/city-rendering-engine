#include "ChunkGenerator.h"
#include "gridlayouts/ManhattanGridLayout.h"

Chunk *ChunkGenerator::generateChunk(City *city, const Vector2 &position) {
    // First we check if position is valid (if both X and Y are multiple of 1000)
    if ((int) position.x % 1000 != 0 || (int) position.y % 1000 != 0) {
        return nullptr;
    }
    // And also if this chunk doesn't already exists
    if (FileIO::fileExists(Chunk::getFileName(position))) {
        return nullptr;
    }
    Profiler::getTimer(3)->startMeasurement();

    /*
     * Generate Intersections and Roads
     */

    //TODO: Put this condition on the different grid layout generators. Each may have different limits
    int minDistanceIntersections = 50; // Minimum distance between intersections

    Chunk *chunk = new Chunk(position, city);
    std::vector<Chunk*> neighbourChunks = city->getNeighbourChunks(chunk, true);
    ManhattanGridLayout gridLayout = ManhattanGridLayout(Vector2(), Vector2());
    int numSubChunks = Chunk::CHUNK_SIZE / Chunk::SUBCHUNK_SIZE;
    float subChunkSize = (float) Chunk::SUBCHUNK_SIZE;
    for (int i = 0; i < numSubChunks; i++) {
        for (int j = 0; j < numSubChunks; j++) {
            // Calculate the position of the new intersection on this subchunk
            gridLayout.posMin = Vector2(i * subChunkSize, j * subChunkSize);
            gridLayout.posMax = Vector2((i + 1) * subChunkSize, (j + 1) * subChunkSize);
            Vector2 intersectionPos = gridLayout.getIntersectionPosition();
            if (intersectionPos.x >= 0 && intersectionPos.y >= 0) {
                // If there's an intersection in this subchunk, check if it has enough distance from the others
                intersectionPos += position; // Convert to World Position
                auto intersectionsBegin = chunk->getIntersections()->begin();
                auto intersectionsEnd = chunk->getIntersections()->end();
                bool tooClose = false;
                Vector3 newIntPos = Vector3(intersectionPos.x, 0, intersectionPos.y);
                for (auto it = intersectionsBegin; it != intersectionsEnd; it++) {
                    Intersection *intersection = (*it);
                    // Chunk position for the new intersection position (0 to 1000)
                    if ((intersection->getPosition() - newIntPos).getLength() < minDistanceIntersections) {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose) {
                    // If the new intersections is sufficiently distant from all the others, proceed
                    Intersection *newInter = new Intersection(newIntPos);
                    chunk->addIntersection(newInter);
                    Intersection *neighbourSubstitute = nullptr;
                    // Check if the new Intersection is too close to any Intersection on the neighbours
                    for (auto it = neighbourChunks.begin(); it != neighbourChunks.end(); it++) {
                        Intersection *closest = (*it)->getClosestIntersectionTo(newInter);
                        if (closest != nullptr) {
                            float distance = (newInter->getWorldPosition() - closest->getWorldPosition()).getLength();
                            if (distance < minDistanceIntersections) {
                                // The new Intersection is too close to an Intersection on an neighbour. Use the
                                // neighbour's Intersection instead
                                neighbourSubstitute = closest;
                            }
                        }
                    }
                    if (neighbourSubstitute != nullptr) {
                        chunk->removeIntersection(newInter);
                        delete newInter;
                        newInter = neighbourSubstitute;
                        chunk->addIntersection(newInter);
                    }
                    gridLayout.generateRoads(chunk, newInter);
                }
            }
        }
    }

    /*
     * Generate City Blocks and Buildings
     */

    auto itEnd = chunk->getIntersections()->end();
    for (auto it = chunk->getIntersections()->begin(); it != itEnd; it++) {
        CityBlock *cityBlock = gridLayout.generateCityBlock(chunk, (*it));
        if (cityBlock != nullptr) {
            cityBlock->generateBuildings();
        }
    }

    // Use convex hull algorithm called Gift wrapping to find the city blocks given N intersections.
    // This won't work for non-convex city blocks
    // Use the grid layout to get a preferred angle for the build of the city block. If rectangular city blocks
    // are expected, the angle should be 90 degrees. If triangular city blocks are expected, the angle should be less
    // than 60, and if polygon-like, big city blocks are expected, the angle should be greater than 90 degrees.
    // When including a new intersection on the city block, make a triangle with the new,the last intersection and the
    // first one added, and check if another intersection is within this triangle. If it is, use it instead of the
    // chosen one.


    // Iterate through the intersections, performing a breadth-first tree search running along the roads to do a loop
    // and find the smalest path going out and back to the starting intersection, using at least three intersections.

    // Verify that the city block generated doesn't already exist and doesn't overlap other city blocks.

    //std::cout << "Chunk " << position << " generated" << std::endl;

    Profiler::getTimer(3)->finishMeasurement();
    Profiler::getTimer(3)->resetCycle();
    std::cout << chunk->getChunkPos() << " generated in " << Profiler::getTimer(3)->getAverageTime() << "ms" << std::endl;
    return chunk;

    /* Chunk Generator Algorithm: */

    /*
     * 1 - We try to load the adjacent chunks to detect invalid intersections near the borders.
     * 
     * 2 - Iterate over the 10x10 grid of partial chunks, calculate the grid layout for each one and call the generator
     * algorithm of the correct grid layout to generate intersections and roads for each one of the partial chunks.
     * NOTE: Consider Voronoi Diagram for the GridLayout selector
     * 
     * 3 - Connect this chunk's intersections with the adjacent chunk's intersections, if the corresponding adjacent
     * chunk exists.
     * 
     * 4 - Generate empty city blocks and add everything generated so far to the scene, so it can render something
     * while we generate the buildings
     * 
     * 5 - Use the grid layout and city zone to assign a city block type to each city block
     * 
     * 6 - Generate the buildings for all city blocks
     * 
     * 7 - Add the new buildings to the scene so it can render everything
     * 
     * 8 - Save this chunk to its file
     * 
     * 9 - Update the adjacent chunks with the new edge city blocks, intersections and roads
     */
}

int ChunkGenerator::getGridLayout(const Vector2 &position) {
    return 1; // 1 will be the future ManhattanGridLayout
}