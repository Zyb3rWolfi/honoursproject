//
// Created by anton on 03/11/2026.
//

#include "Chunk.h"

int Chunk::Index(int lx, int ly, int lz) {
    // Out-of-range local coordinates are a programming error, not user input.
    assert(lx >= 0 && lx < CHUNK_SIZE);
    assert(ly >= 0 && ly < CHUNK_SIZE);
    assert(lz >= 0 && lz < CHUNK_SIZE);
    return lx + CHUNK_SIZE * (ly + CHUNK_SIZE * lz);
}

VoxelId Chunk::Get(int x, int y, int z) const {
    return m_Voxels[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
}

bool Chunk::Set(int lx, int ly, int lz, VoxelId id) {

    const int index = Index(lx, ly, lz);
    const VoxelId old = m_Voxels[index];


    // If old is the same
    if (old == id) {
        return false; // No change needed
    }

    // Empty -> Filled or Filled -> Empty
    if (old == AIR && id != AIR) {
        ++m_filledCount;
    } else if (old != AIR && id == AIR) {
        --m_filledCount;
    }

    m_Voxels[index] = id;
    m_DirtyFlag = true;
    return true;

}
