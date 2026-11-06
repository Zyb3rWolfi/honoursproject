//
// Created by anton on 03/11/2026.
//

#ifndef VOXELBRIDGE_CHUNK_H
#define VOXELBRIDGE_CHUNK_H
#include "VoxelConfig.h"


class Chunk {
public:
    VoxelId Get(int x, int y, int z) const;
    bool Set(int lx, int ly, int lz, VoxelId id);
    bool IsDirty() const { return m_DirtyFlag;}
    void ClearDirty() { m_DirtyFlag = false; }

    void MarkDirty() { m_DirtyFlag = true; }

    int FilledCount() const { return m_filledCount; }
    bool IsEmpty() const { return m_filledCount == 0; }
private:
    static int Index(int lx, int ly, int lz);
    std::array<VoxelId, CHUNK_VOLUME> m_Voxels = {};
    bool m_DirtyFlag = false;
    int m_filledCount = 0;
};


#endif //VOXELBRIDGE_CHUNK_H
