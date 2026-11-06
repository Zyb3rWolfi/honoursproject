//
// Created by anton on 04/11/2026.
//

#ifndef VOXELBRIDGE_COORDINATES_H
#define VOXELBRIDGE_COORDINATES_H
#include <glm/glm.hpp>

#include "ChunkCoord.h"
#include "VoxelConfig.h"

// This is the voxels position inside the world
struct WorldPos {
    int x = 0, y = 0, z = 0;
};

// This is in which chunk the voxel is in and its position inside the voxel
struct VoxelLocation {
    ChunkCoord chunk;
    int lx = 0, ly = 0, lz = 0;
};

// Floor divisonm rounds toward negaitive infifty
inline int FloorDiv(int value, int divisor) {
    int q = value / divisor;
    if (value % divisor < 0) --q;
    return q;
}

// One axis: which chunk does this world co ordinate fall in?
inline int WorldToChunk(int world) {
    return FloorDiv(world, CHUNK_SIZE);
}

// Posotion inside the chunk
inline int WorldToLocal(int world) {
    return world - WorldToChunk(world) * CHUNK_SIZE;
}

inline VoxelLocation WorldToVoxelLocation(int wx, int wy, int wz) {
    VoxelLocation loc;
    loc.chunk = ChunkCoord{WorldToChunk(wx), WorldToChunk(wy), WorldToChunk(wz)};
    loc.lx = WorldToLocal(wx);
    loc.ly = WorldToLocal(wy);
    loc.lz = WorldToLocal(wz);
    return loc;
}

inline WorldPos ChunkOrigin(const ChunkCoord& c) {
    return WorldPos{c.x * CHUNK_SIZE, c.y * CHUNK_SIZE, c.z * CHUNK_SIZE};
}

#endif //VOXELBRIDGE_COORDINATES_H
