#include <gtest/gtest.h>

#include "CuckooHash.h"

namespace {
struct Hash0 {
    uint64_t operator()(uint64_t value) const {
        return ((value << 7) + 11) ^ (value >> 57) ^ value;
    }
};

struct Hash1 {
    uint64_t operator()(uint64_t value) const {
        return ((value << 13) + 23) ^ (value >> 51) ^ value;
    }
};

struct Hash2 {
    uint64_t operator()(uint64_t value) const {
        return ((value << 17) + 37) ^ (value >> 47) ^ value;
    }
};
};

using CH = CuckooHash<uint64_t, int, Hash0, Hash1, Hash2>;

TEST(empty, CuckooHashTest) {
    CH h;
    int dummy;
    ASSERT_FALSE(h.lookup(1, &dummy));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
