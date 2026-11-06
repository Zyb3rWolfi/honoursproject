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

    return it->second.Set(loc.lx, loc.ly, loc.lz, id);
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


