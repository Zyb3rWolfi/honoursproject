//
// Created by anton on 03/11/2026.
//

#ifndef VOXELBRIDGE_CHUNKCOORD_H
#define VOXELBRIDGE_CHUNKCOORD_H
#include "glm/vec3.hpp"

struct ChunkCoord {
    int x = 0, y = 0, z = 0;

    bool operator==(const ChunkCoord& c) const {
        return x == c.x && y == c.y && z == c.z;
    }
};

// If we reference ChunkCord, use this
namespace std {
    template<>
    struct hash<ChunkCoord> {
        size_t operator()(const ChunkCoord& c) const noexcept {
            std::size_t h = static_cast<std::size_t>(c.x) * 73856093u;
            h ^= static_cast<size_t>(c.y) * 193496639u;
            h ^= static_cast<size_t>(c.z) * 83492791u;
            return h;
        }
    };
}


#endif //VOXELBRIDGE_CHUNKCOORD_H
