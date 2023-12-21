#pragma once

namespace helper {

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct PointHash {
public:
  std::size_t operator()(const Point &p) const
  {
    std::size_t seed = 0;
    hash_combine(seed, p.x);
    hash_combine(seed, p.y);

    return seed;
  }
};

struct PairHash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    std::size_t seed = 0;
    hash_combine(seed, x.first);
    hash_combine(seed, x.second);

    return seed;
  }
};

}