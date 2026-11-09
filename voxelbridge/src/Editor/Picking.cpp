//
// Created by antonscereskevics on 03/11/2026.
//

#include "Picking.h"
#include <glm/glm.hpp>

Picking::Picking() {}
Picking::~Picking() {}

Ray Picking::ScreenToRay(glm::vec2 ndc, const glm::mat4 &view, const glm::mat4 &projection) {

    const glm::mat4 &mvp = glm::inverse(projection * view);

    glm::vec4 nearTarget(ndc.x, ndc.y, -1.0f, 1.0f);
    glm::vec4 farTarget(ndc.x, ndc.y, 1.0f, 1.0f);

    glm::vec4 worldNear = mvp * nearTarget;
    glm::vec4 worldFar = mvp * farTarget;

    worldNear /= worldNear.w;
    worldFar /= worldFar.w;

    Ray ray;
    ray.origin = glm::vec3(worldNear);
    ray.direction = normalize(glm::vec3(worldFar - worldNear));

    return ray;

}

bool Picking::IntersectRayPlaneY(const Ray& ray, float planeY, glm::vec3& outHitPoint) {
    // Avoid division by zero if ray is parallel to the plane
    if (std::abs(ray.direction.y) < 1e-6f)
        return false;

    float t = (planeY - ray.origin.y) / ray.direction.y;
    if (t < 0.0f) // Intersection is behind the camera
        return false;

    outHitPoint = ray.origin + t * ray.direction;
    return true;
}

PickResult Picking::Pick(const World &world, const Ray &ray, float maxDistance) {
    PickResult result;

    // IF there is no direction
    if (glm::dot(ray.direction, ray.direction) < 1e-12f) {
        return result;
    };

    const glm::vec3 o = ray.origin;
    const glm::vec3 d = glm::normalize(ray.direction);

    constexpr float INF = std::numeric_limits<float>::infinity();

    float tGround = INF;
    float limit = maxDistance;

    if (o.y > 0.0f && d.y < -1e-6f) {
        tGround = -o.y / d.y;
        if (tGround < limit) {
            limit = tGround;
        }
    }

    // Setting up the grid walk

    const float origin[3] = {o.x, o.y, o.z};
    const float dir[3] = {d.x, d.y, d.z};

    int cell[3]; // The cell the ray currently is at
    int step[3]; // The step direction for each axis
    float tDelta[3]; // distance along the ray to cross a voxel boundary for each axis
    float tMax[3]; // distance along the ray to the next voxel boundary for each axis

    for (int a = 0; a < 3; a++) {
        cell[a] = static_cast<int>(std::floor(origin[a]));

        if (dir[a] > 0.0f) {
            step[a]   = 1;
            tDelta[a] = 1.0f / dir[a];
            tMax[a]   = (static_cast<float>(cell[a] + 1) - origin[a]) / dir[a];
        } else if (dir[a] < 0.0f) {
            step[a]   = -1;
            tDelta[a] = -1.0f / dir[a];
            tMax[a]   = (static_cast<float>(cell[a]) - origin[a]) / dir[a];
        } else {
            step[a]   = 0;               // parallel to this axis: never crosses its grid lines
            tDelta[a] = INF;
            tMax[a]   = INF;
        }

    }

    while (true) {
        int axis = 0;
        if (tMax[1] < tMax[axis]) axis = 1;
        if (tMax[2] < tMax[axis]) axis = 2;

        const float t = tMax[axis];          // where the ray crosses into the next cell
        if (t >= limit)
            break;                           // too far, or we've reached the ground

        cell[axis] += step[axis];
        tMax[axis] += tDelta[axis];

        if (world.GetVoxel(cell[0], cell[1], cell[2]) != AIR) {
            result.hit      = true;
            result.solid    = true;
            result.voxel    = glm::ivec3(cell[0], cell[1], cell[2]);
            result.faceNormal   = glm::ivec3(0, 0, 0);
            result.faceNormal[axis] = -step[axis];   // we entered through the face opposite to our movement
            result.distance = t;
            return result;
        }
    }

    // ---- Nothing solid in front of the ground: report the ground if it is in range ----
    if (tGround <= maxDistance) {
        const float px = o.x + d.x * tGround;
        const float pz = o.z + d.z * tGround;
        result.hit      = true;
        result.solid    = false;
        result.voxel    = glm::ivec3(static_cast<int>(std::floor(px)), -1,
                                     static_cast<int>(std::floor(pz)));
        result.faceNormal   = glm::ivec3(0, 1, 0);
        result.distance = tGround;
    }
    return result;


}

