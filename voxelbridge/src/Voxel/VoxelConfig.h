//
// Created by anton on 03/11/2026.
//

#ifndef VOXELBRIDGE_VOXELCONFIG_H
#define VOXELBRIDGE_VOXELCONFIG_H
#include <string>
#include <glm\glm.hpp>

inline constexpr int CHUNK_SIZE = 16;

class VoxelConfig {
    public:
    std::uint8_t VoxelId;
    const std::string Name = "AIR";

};


#endif //VOXELBRIDGE_VOXELCONFIG_H

