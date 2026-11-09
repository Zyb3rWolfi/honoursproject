//
// Created by antonscereskevics on 03/11/2026.
//

#ifndef VOXELBRIDGE_PICKING_H
#define VOXELBRIDGE_PICKING_H
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <cmath>

#include "PickResult.h"
#include "../Voxel/World.h"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

class Picking {
public:
    Picking();
    ~Picking();

    bool IntersectRayPlaneY(const Ray& ray, float planeY, glm::vec3& outHitPoint);
    Ray ScreenToRay(glm::vec2 ndc, const glm::mat4& view, const glm::mat4& projection);

    PickResult Pick(const World& world, const Ray& ray, float maxDistance = 100.0f);
};


#endif //VOXELBRIDGE_PICKING_H
