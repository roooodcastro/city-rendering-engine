#include "ChunkGenerator.h"

Chunk *ChunkGenerator::generateChunk(City *city, const Vector2 &position) {
    std::cout << "Started generating Chunk " << position << std::endl;
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

    Chunk *chunk = new Chunk(position);
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
                        //(*it)->getClosestIntersectionsTo(newInter, 10);
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
                        //std::cout << "Substituted " << newInter->getPosition() << " with " << neighbourSubstitute->getPosition() << " at " << newInter->getWorldPosition() << std::endl;
                        chunk->removeIntersection(newInter);
                        delete newInter;
                        newInter = neighbourSubstitute;
                        chunk->addIntersection(newInter);
                    }
                    //std::cout << "Intersection " << newInter->getWorldPosition() << std::endl;
                    gridLayout.generateRoads(chunk, newInter);
                    // Get 10 closest intersections to newIntPos    O(n)
                    // Create roads between newIntPos and these 10  O(10)
                    // Check which of those cross each other and existing roads, remove them    O(10*log10)
                    // Check which of those cross other intersections, remove them              O(10*10)
                    // Apply a random factor to the remaining roads, so some may not be added   O(10)
                    // Add the ones that manage to get to the end
                }
            }
        }
    }

    /*
     * Generate City Blocks
     */

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