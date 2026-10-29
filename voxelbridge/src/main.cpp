#include "Core/Application.h"

#include <cstdio>
#include <exception>

int main() {
    try {
        Application app;
        app.Run();
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Fatal error: %s\n", e.what());
        return 1;
    }
    return 0;
}