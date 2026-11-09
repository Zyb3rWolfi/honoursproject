//
// Created by antonscereskevics on 06/11/2026.
//

#include "Mesher.h"

struct Face {
    int       nx, ny, nz;   // normal, which is also the direction of the neighbour to test
    glm::vec3 origin;
    glm::vec3 u;
    glm::vec3 v;
};

const Face FACES[6] = {
    // normal        origin           u                 v
    {  1,  0,  0, {1, 0, 1}, { 0, 0, -1}, {0, 1,  0} },   // +X
    { -1,  0,  0, {0, 0, 0}, { 0, 0,  1}, {0, 1,  0} },   // -X
    {  0,  1,  0, {0, 1, 1}, { 1, 0,  0}, {0, 0, -1} },   // +Y
    {  0, -1,  0, {0, 0, 0}, { 1, 0,  0}, {0, 0,  1} },   // -Y
    {  0,  0,  1, {0, 0, 1}, { 1, 0,  0}, {0, 1,  0} },   // +Z
    {  0,  0, -1, {1, 0, 0}, {-1, 0,  0}, {0, 1,  0} },   // -Z
};

glm::vec3 VoxelColor(VoxelId id) {
    static const glm::vec3 palette[] = {
        {0.90f, 0.40f, 0.20f},   // id 1: orange
        {0.35f, 0.65f, 0.35f},   // id 2: green
        {0.40f, 0.55f, 0.90f},   // id 3: blue
        {0.85f, 0.80f, 0.35f},   // id 4: yellow
        {0.70f, 0.70f, 0.70f},   // id 5: grey
    };
    constexpr int count = static_cast<int>(sizeof(palette) / sizeof(palette[0]));
    return palette[(static_cast<int>(id) - 1) % count];
}

// 4 vertices + 6 indices (two triangles) for one face of the voxel at 'pos'.
void AddFace(MeshData& mesh, const glm::vec3& pos, const Face& f, const glm::vec3& color) {
    const auto base = static_cast<std::uint32_t>(mesh.vertices.size());
    const glm::vec3 normal(static_cast<float>(f.nx), static_cast<float>(f.ny), static_cast<float>(f.nz));
    const glm::vec3 p0 = pos + f.origin;

    mesh.vertices.push_back({p0,                 normal, color});
    mesh.vertices.push_back({p0 + f.u,           normal, color});
    mesh.vertices.push_back({p0 + f.u + f.v,     normal, color});
    mesh.vertices.push_back({p0 + f.v,           normal, color});

    for (std::uint32_t i : {0u, 1u, 2u, 0u, 2u, 3u})
        mesh.indices.push_back(base + i);
}



MeshData Mesher::BuildChunkMesh(const World& world, ChunkCoord coord) const {
    MeshData mesh;

    const Chunk* chunk = world.GetChunk(coord);
    if (chunk == nullptr || chunk->IsEmpty()) {
        return mesh;
    }

    const WorldPos origin = ChunkOrigin(coord);

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                const VoxelId id = chunk->Get(x, y, z);
                if (id == AIR) {
                    continue; // Skip empty voxels
                }

                const glm::vec3 pos(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                const glm::vec3 color = VoxelColor(id);

                for (const Face& f : FACES) {
                    const int wx = origin.x + x + f.nx;
                    const int wy = origin.y + y + f.ny;
                    const int wz = origin.z + z + f.nz;

                    if (world.GetVoxel(wx, wy, wz) == AIR) {
                        AddFace(mesh, pos, f, color);
                    }
                }
            }
        }
    }
    return mesh;
}
