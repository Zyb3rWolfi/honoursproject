#include "Voxeltests.h"

#include "Chunk.h"
#include "ChunkCoord.h"
#include "Coordinates.h"
#include "VoxelConfig.h"
#include "World.h"

#include <cstdio>
#include <cstdlib>

// Own macro instead of assert(): assert() is compiled out in Release builds,
// which would make the tests "pass" by never running.
#define CHECK(cond)                                                           \
    do {                                                                      \
        if (!(cond)) {                                                        \
            std::fprintf(stderr, "VOXEL TEST FAILED: %s  (%s:%d)\n",          \
                         #cond, __FILE__, __LINE__);                          \
            std::abort();                                                     \
        }                                                                     \
    } while (0)

static_assert(CHUNK_SIZE == 16, "These tests hard-code expected values for CHUNK_SIZE == 16");

// ---------------------------------------------------------------- Coordinates

static void TestTable() {
    struct Row { int world, chunk, local; };
    const Row rows[] = {
        {     0,   0,  0},
        {    15,   0, 15},
        {    16,   1,  0},
        {    -1,  -1, 15},
        {   -16,  -1,  0},
        {   -17,  -2, 15},
        {  1000,  62,  8},
        { -1000, -63,  8},
    };
    for (const Row& r : rows) {
        CHECK(WorldToChunk(r.world) == r.chunk);
        CHECK(WorldToLocal(r.world) == r.local);
    }
}

static void TestRoundTrip() {
    for (int x = -1000; x <= 1000; ++x) {
        const int chunk = WorldToChunk(x);
        const int local = WorldToLocal(x);
        CHECK(local >= 0 && local < CHUNK_SIZE);
        CHECK(chunk * CHUNK_SIZE + local == x);
    }
}

static void TestAxesIndependent() {
    const VoxelLocation loc = WorldToVoxelLocation(-1, 16, -17);
    CHECK(loc.chunk.x == -1 && loc.chunk.y == 1 && loc.chunk.z == -2);
    CHECK(loc.lx == 15 && loc.ly == 0 && loc.lz == 15);
}

static void TestChunkOrigin() {
    for (int cx = -3; cx <= 3; ++cx)
    for (int cy = -3; cy <= 3; ++cy)
    for (int cz = -3; cz <= 3; ++cz) {
        const ChunkCoord c{cx, cy, cz};
        const WorldPos o = ChunkOrigin(c);

        VoxelLocation a = WorldToVoxelLocation(o.x, o.y, o.z);
        CHECK(a.chunk == c);
        CHECK(a.lx == 0 && a.ly == 0 && a.lz == 0);

        VoxelLocation b = WorldToVoxelLocation(o.x + CHUNK_SIZE - 1,
                                               o.y + CHUNK_SIZE - 1,
                                               o.z + CHUNK_SIZE - 1);
        CHECK(b.chunk == c);
        CHECK(b.lx == CHUNK_SIZE - 1 && b.ly == CHUNK_SIZE - 1 && b.lz == CHUNK_SIZE - 1);
    }
}

// ---------------------------------------------------------------------- Chunk

static void TestChunk() {
    // 1. Fresh chunk: everything is air, clean, empty
    Chunk fresh;
    for (int z = 0; z < CHUNK_SIZE; ++z)
    for (int y = 0; y < CHUNK_SIZE; ++y)
    for (int x = 0; x < CHUNK_SIZE; ++x)
        CHECK(fresh.Get(x, y, z) == AIR);
    CHECK(!fresh.IsDirty());
    CHECK(fresh.IsEmpty());

    // 2. Corners round trip
    Chunk c;
    CHECK(c.Set(0, 0, 0, 1));
    CHECK(c.Set(15, 0, 0, 2));
    CHECK(c.Set(0, 15, 0, 3));
    CHECK(c.Set(0, 0, 15, 4));
    CHECK(c.Set(15, 15, 15, 5));
    CHECK(c.Get(0, 0, 0) == 1);
    CHECK(c.Get(15, 0, 0) == 2);
    CHECK(c.Get(0, 15, 0) == 3);
    CHECK(c.Get(0, 0, 15) == 4);
    CHECK(c.Get(15, 15, 15) == 5);

    // 3. No overlap: fill every voxel, the count must be exactly CHUNK_VOLUME
    Chunk full;
    for (int z = 0; z < CHUNK_SIZE; ++z)
    for (int y = 0; y < CHUNK_SIZE; ++y)
    for (int x = 0; x < CHUNK_SIZE; ++x)
        CHECK(full.Set(x, y, z, 1));
    CHECK(full.FilledCount() == CHUNK_VOLUME);

    // 4. Neighbours stay clean (catches swapped axes in the index formula)
    Chunk n;
    n.Set(1, 0, 0, 7);
    CHECK(n.Get(0, 1, 0) == AIR);
    CHECK(n.Get(0, 0, 1) == AIR);
    CHECK(n.Get(1, 1, 0) == AIR);

    // 5. Dirty flag
    Chunk d;
    CHECK(!d.IsDirty());
    d.Set(2, 2, 2, 9);
    CHECK(d.IsDirty());
    d.ClearDirty();
    CHECK(!d.IsDirty());
    CHECK(!d.Set(2, 2, 2, 9));      // same value: no change
    CHECK(!d.IsDirty());
    CHECK(d.Set(2, 2, 2, AIR));     // solid -> air is a change
    CHECK(d.IsDirty());

    // 6. Filled count
    Chunk k;
    k.Set(0, 0, 0, 1);
    k.Set(1, 0, 0, 1);
    k.Set(2, 0, 0, 1);
    CHECK(k.FilledCount() == 3);
    k.Set(1, 0, 0, 5);              // overwrite solid with a different solid
    CHECK(k.FilledCount() == 3);
    k.Set(2, 0, 0, AIR);
    CHECK(k.FilledCount() == 2);
    k.Set(0, 0, 0, AIR);
    k.Set(1, 0, 0, AIR);
    CHECK(k.IsEmpty());
}

// ---------------------------------------------------------------------- World

static void TestWorld() {
    // 1. Empty world: reads give air and do not create chunks
    {
        World w;
        CHECK(w.ChunkCount() == 0);
        CHECK(w.GetVoxel(0, 0, 0) == AIR);
        CHECK(w.GetVoxel(-1, -1, -1) == AIR);
        CHECK(w.GetVoxel(1000, -1000, 5) == AIR);
        CHECK(w.ChunkCount() == 0);
    }

    // 2. Writing air to a missing chunk does nothing
    {
        World w;
        CHECK(!w.SetVoxel(3, 3, 3, AIR));
        CHECK(w.ChunkCount() == 0);
    }

    // 3. Round trip at awkward positions
    {
        World w;
        struct P { int x, y, z; VoxelId id; };
        const P pts[] = {
            {0, 0, 0, 1}, {15, 15, 15, 2}, {16, 0, 0, 3}, {-1, -1, -1, 4},
            {-16, 0, 0, 5}, {-17, 0, 0, 6}, {1000, -1000, 5, 7},
        };
        for (const P& p : pts) CHECK(w.SetVoxel(p.x, p.y, p.z, p.id));
        for (const P& p : pts) CHECK(w.GetVoxel(p.x, p.y, p.z) == p.id);
    }

    // 4. Chunk counts
    {
        World w;
        w.SetVoxel(0, 0, 0, 1);
        w.SetVoxel(5, 5, 5, 1);
        CHECK(w.ChunkCount() == 1);        // same chunk

        World b;
        b.SetVoxel(-1, 0, 0, 1);
        b.SetVoxel(0, 0, 0, 1);
        CHECK(b.ChunkCount() == 2);        // either side of a chunk boundary
        b.SetVoxel(16, 0, 0, 1);
        CHECK(b.ChunkCount() == 3);
    }

    // 5. Return value
    {
        World w;
        CHECK(w.SetVoxel(1, 2, 3, 5));     // new value
        CHECK(!w.SetVoxel(1, 2, 3, 5));    // same value
        CHECK(w.SetVoxel(1, 2, 3, 6));     // different value
        CHECK(w.SetVoxel(1, 2, 3, AIR));   // removal
        CHECK(w.GetVoxel(1, 2, 3) == AIR);
    }

    // 6. Neighbours across a boundary are independent
    {
        World w;
        w.SetVoxel(-1, 0, 0, 3);
        CHECK(w.GetVoxel(-1, 0, 0) == 3);
        CHECK(w.GetVoxel(0, 0, 0) == AIR);
        CHECK(w.GetVoxel(15, 0, 0) == AIR);
        CHECK(w.GetVoxel(-17, 0, 0) == AIR);
        CHECK(w.GetVoxel(-2, 0, 0) == AIR);
    }

    // 7. No mirroring through the origin
    {
        World w;
        w.SetVoxel(-1, -1, -1, 5);
        CHECK(w.GetVoxel(-1, -1, -1) == 5);
        CHECK(w.GetVoxel(1, 1, 1) == AIR);
        CHECK(w.GetVoxel(0, 0, 0) == AIR);
    }

    // 8. Dirty list
    {
        World w;
        CHECK(w.DirtyChunks().empty());
        w.SetVoxel(3, 3, 3, 1);
        const auto dirty = w.DirtyChunks();
        CHECK(dirty.size() == 1);
        CHECK(dirty[0] == (ChunkCoord{0, 0, 0}));
        Chunk* chunk = w.GetChunk(ChunkCoord{0, 0, 0});
        CHECK(chunk != nullptr);
        chunk->ClearDirty();
        CHECK(w.DirtyChunks().empty());
        CHECK(w.GetChunk(ChunkCoord{5, 5, 5}) == nullptr);
    }

    // 9. Brute force over a cube that spans chunks -2 .. 1 on every axis
    {
        World w;
        auto value = [](int x, int y, int z) {
            return static_cast<VoxelId>(((x + 20) * 7 + (y + 20) * 13 + (z + 20) * 31) % 250 + 1);
        };
        for (int z = -20; z <= 20; ++z)
        for (int y = -20; y <= 20; ++y)
        for (int x = -20; x <= 20; ++x)
            w.SetVoxel(x, y, z, value(x, y, z));

        for (int z = -20; z <= 20; ++z)
        for (int y = -20; y <= 20; ++y)
        for (int x = -20; x <= 20; ++x)
            CHECK(w.GetVoxel(x, y, z) == value(x, y, z));

        CHECK(w.ChunkCount() == 64);       // 4 chunks per axis: 4 * 4 * 4
    }
}

void RunVoxelTests() {
    TestTable();
    TestRoundTrip();
    TestAxesIndependent();
    TestChunkOrigin();
    TestChunk();
    TestWorld();
    std::printf("Voxel tests passed\n");
}