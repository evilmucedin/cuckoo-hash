#include <cstdlib>
#include <iostream>
#include <unordered_set>

#include "CuckooHash.h"

using namespace std;

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

int main() {
    using CH = CuckooHash<uint64_t, uint64_t, Hash0, Hash1>;

    using Data = CH::Vector;

    // static constexpr size_t N = 76207733;
    static constexpr size_t N = 76207;

    Data data(N);
    unordered_set<uint64_t> used;
    for (size_t i = 0; i < data.size(); ++i) {
        uint64_t key = rand();
        while (1 == used.count(key)) {
            key = rand();
        }
        used.insert(key);
        data[i] = std::make_pair(key, rand());
    }

    CH ch(data, -1);
    for (size_t i = 0; i < data.size(); ++i) {
        uint64_t dummy;
        if (!ch.lookup(data[i].first, &dummy)) {
            throw std::runtime_error("!!!");
            if (dummy != data[i].second) {
                throw std::runtime_error("???");
            }
        }
    }

    return 0;
}
