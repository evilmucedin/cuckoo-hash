#include <unordered_set>

#include <gtest/gtest.h>

#include "CuckooHash.h"

using namespace std;

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

using CH = CuckooHash<uint64_t, int, Hash0, Hash1, Hash2>;
using Data = CH::Vector;
};

TEST(CuckooHashTest, empty) {
    CH h;
    int dummy;
    ASSERT_FALSE(h.lookup(1, &dummy));
}

TEST(CuckooHashTest, allSizes) {
    static const vector<size_t> sizes = {0, 1, 2, 5, 10, 1000, 1000000};

    for (auto size: sizes) {
        Data data(size);
        unordered_set<uint64_t> used;
        for (size_t i = 0; i < data.size(); ++i) {
            uint64_t key = rand();
            while (1 == used.count(key)) {
                key = rand();
            }
            used.insert(key);
            data[i] = std::make_pair(key, key);
        }

        CH ch(data, -1);
        for (size_t i = 0; i < data.size(); ++i) {
            int value;
            ASSERT_TRUE(ch.lookup(data[i].first, &value));
            ASSERT_EQ(data[i].second, value);
        }

        for (size_t i = 0; i < size; ++i) {
            uint64_t key = rand();
            int dummy;
            ASSERT_EQ((int)ch.lookup(key, &dummy), used.count(key));
        }

        int count = 0;
        for (const auto& item: ch.getData()) {
            if (ch.hasValue(item)) {
                ++count;
            }
        }

        ASSERT_EQ(count, size);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
