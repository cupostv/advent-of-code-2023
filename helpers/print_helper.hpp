#pragma once

#include <ostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>

template < class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& vec) {
    os << "vec[";
    for (auto &v : vec)
        os << " " << v;
    os << " ]";
    return os;
}

template < class T >
std::ostream& operator << (std::ostream& os, const std::list<T>& vec) {
    os << "list[";
    for (auto &v : vec)
        os << " " << v;
    os << " ]";
    return os;
}

template < class T >
std::ostream& operator << (std::ostream& os, const std::queue<T>& vec) {
    os << "queue[";
    for (auto &v : vec)
        os << " " << v;
    os << " ]";
    return os;
}

template < class K, class V >
std::ostream& operator << (std::ostream& os, const std::unordered_map<K, V>& map) {
    os << "uo_map[";
    for (auto &[key, value] : map)
        os << " { " << key << " = " << value << " } ";
    os << " ]";
    return os;
}

template < class K, class V >
std::ostream& operator << (std::ostream& os, const std::map<K, V>& map) {
    os << "map[";
    for (auto &[key, value] : map)
        os << " { " << key << " = " << value << " } ";
    os << " ]";
    return os;
}

namespace helper {


template < class T >
void dumpRow(const std::vector<T>& vec) {
    for (auto &v : vec) {
        std::cout << v << std::endl;
    }
}

}
