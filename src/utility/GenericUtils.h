/// @author James Holtom

#ifndef GENERIC_UTILS_H
#define GENERIC_UTILS_H

#include <functional>
#include <utility>

/// A hashing function used to hash a pair of any type(s).
struct PairHashingFunction {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const
  {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);

    return hash1 ^ hash2;
  }
};

#endif // ! GENERIC_UTILS_H
