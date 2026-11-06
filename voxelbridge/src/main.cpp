#include "Core/Application.h"

#include <cstdio>
#include <exception>

#include "Voxel/Voxeltests.h"

int main() {
    try {
        RunVoxelTests();

        Application app;
        app.Run();
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Fatal error: %s\n", e.what());
        return 1;
    }
    return 0;
}
