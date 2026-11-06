//
// Created by anton on 03/11/2026.
//

#ifndef VOXELBRIDGE_VOXELCONFIG_H
#define VOXELBRIDGE_VOXELCONFIG_H
#include <string>
#include <glm/glm.hpp>

#include <cstdint>

inline constexpr int CHUNK_SIZE   = 16;
inline constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;   // 4096

using VoxelId = std::uint8_t;           // a voxel is just a small number
inline constexpr VoxelId AIR = 0;       // 0 means empty

#endif //VOXELBRIDGE_VOXELCONFIG_H



