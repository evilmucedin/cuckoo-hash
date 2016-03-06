#pragma once

#include <vector>

// Cuckoo Hash implementation

template <typename K,
          typename V,
          typename Hash0,
          typename Hash1>
class CuckooHash {
 public:
  using Pair = std::pair<K, V>;
  using Vector = std::vector<Pair>;

#pragma pack(push, 1)
  struct Data {
    K key_;
    V value_;
  };
#pragma pack(pop)
  using DataVector = std::vector<Data>;

  CuckooHash() : invalidKey_(static_cast<K>(-1)) {}

  CuckooHash(const Vector& content, const K& invalidKey)
      : invalidKey_(invalidKey) {
    init(content, invalidKey);
  }

  void init(const Vector& content, const K& invalidKey) {
    invalidKey_ = invalidKey;

    size_t size = static_cast<float>(content.size())*1.2 + 1;
    while (!initWithSize(content, size)) {
        size = static_cast<float>(size*1.05) + 1;
    }
  }

  bool lookup(const K& key, V* value) const {
    size_t hash0 = hasher0_(key) % data_.size();
    if (data_[hash0].key_ == key) {
        *value = data_[hash0].value_;
        return true;
    }
    size_t hash1 = hasher1_(key) % data_.size();
    if (data_[hash1].key_ == key) {
        *value = data_[hash1].value_;
        return true;
    }
    return false;
  }

  const DataVector& getData() const { return data_; }

  bool hasValue(const Data& data) const { return data.key_ != invalidKey_; }

 private:
  bool initWithSize(const Vector& content, size_t size) {
    data_.clear();
    Data dummy;
    dummy.key_ = invalidKey_;
    data_.resize(size, dummy);

    for (const auto& item: content) {
        Data current;
        current.key_ = item.first;
        current.value_ = item.second;
        size_t j = 0;
        while (j < size) {
            size_t hash0 = hasher0_(current.key_) % data_.size();
            if (data_[hash0].key_ == invalidKey_) {
                data_[hash0] = current;
                break;
            }
            size_t hash1 = hasher1_(current.key_) % data_.size();
            if (data_[hash1].key_ == invalidKey_) {
                data_[hash1] = current;
                break;
            } else {
                std::swap(data_[hash1], current);
            }
            ++j;
        }
        if (j == size) {
            return false;
        }
    }

    return true;
  }

  DataVector data_;
  Hash0 hasher0_;
  Hash1 hasher1_;
  K invalidKey_;
};
