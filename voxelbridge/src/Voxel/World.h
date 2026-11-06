//
// Created by anton on 03/11/2026.
//

#ifndef VOXELBRIDGE_WORLD_H
#define VOXELBRIDGE_WORLD_H
#include "VoxelConfig.h"
#include "Chunk.h"
#include "ChunkCoord.h"
#include "Coordinates.h"
#include <cstddef>
#include <unordered_map>
#include <vector>


class World {
public:
    using ChunkMap = std::unordered_map<ChunkCoord, Chunk>;

    VoxelId GetVoxel(int wx, int wy, int wz) const;
    bool SetVoxel(int wx, int wy, int wz, VoxelId id);

    // Returns a pointer to the chunk at the given coordinates, or nullptr if it doesn't exist.
    Chunk* GetChunk(const ChunkCoord& c);
    const Chunk* GetChunk(const ChunkCoord& c) const;

    std::vector<ChunkCoord> DirtyChunks() const;
    std::size_t ChunkCount() const { return m_Chunks.size(); }
private:
    ChunkMap m_Chunks;
};


#endif //VOXELBRIDGE_WORLD_H
