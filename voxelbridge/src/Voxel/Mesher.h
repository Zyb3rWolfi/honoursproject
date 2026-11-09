//
// Created by antonscereskevics on 06/11/2026.
//

#ifndef VOXELBRIDGE_MESHER_H
#define VOXELBRIDGE_MESHER_H
#include <cstdint>
#include <vector>

#include "World.h"
#include "../Gfx/Vertex.h"

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
};

class Mesher {
public:
    MeshData BuildChunkMesh(const World& world, ChunkCoord coord) const;
};


#endif //VOXELBRIDGE_MESHER_H
