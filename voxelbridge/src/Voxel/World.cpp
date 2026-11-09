//
// Created by anton on 03/11/2026.
//

#include "World.h"

VoxelId World::GetVoxel(int wx, int wy, int wz) const {
    const VoxelLocation loc = WorldToVoxelLocation(wx, wy, wz);

    const auto it = m_Chunks.find(loc.chunk);
    if (it == m_Chunks.end()) {
        return AIR;
    }

    return it->second.Get(loc.lx, loc.ly, loc.lz);
}

bool World::SetVoxel(int wx, int wy, int wz, VoxelId id) {
    const VoxelLocation loc = WorldToVoxelLocation(wx, wy, wz);

    auto it = m_Chunks.find(loc.chunk);
    if (it == m_Chunks.end()) {
        if (id == AIR) {
            return false; // No need to create a chunk for empty space
        }
        it = m_Chunks.emplace(loc.chunk, Chunk()).first;
    }

    const bool changed = it->second.Set(loc.lx, loc.ly, loc.lz, id);
    if (!changed) {
        return false;
    }

    // A boundary edit changes whether the adjacent chunk needs a face.
    constexpr int directions[6][3] = {
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0},
        {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
    };
    for (const auto& direction : directions) {
        const int lx = loc.lx + direction[0];
        const int ly = loc.ly + direction[1];
        const int lz = loc.lz + direction[2];
        if (lx < 0 || lx >= CHUNK_SIZE ||
            ly < 0 || ly >= CHUNK_SIZE ||
            lz < 0 || lz >= CHUNK_SIZE) {
            const ChunkCoord neighbour{
                loc.chunk.x + direction[0],
                loc.chunk.y + direction[1],
                loc.chunk.z + direction[2]
            };
            if (auto neighbourIt = m_Chunks.find(neighbour); neighbourIt != m_Chunks.end()) {
                neighbourIt->second.MarkDirty();
            }
        }
    }

    return true;
}

Chunk* World::GetChunk(const ChunkCoord& coord) {
    const auto it = m_Chunks.find(coord);
    return it == m_Chunks.end() ? nullptr : &it->second;
}

const Chunk* World::GetChunk(const ChunkCoord& coord) const {
    const auto it = m_Chunks.find(coord);
    return it == m_Chunks.end() ? nullptr : &it->second;
}

std::vector<ChunkCoord> World::DirtyChunks() const {
    std::vector<ChunkCoord> result;
    for (const auto& [coord, chunk] : m_Chunks) {
        if (chunk.IsDirty())
            result.push_back(coord);
    }
    return result;
}

