#pragma once

// Minimal Siv3D stubs for testing purposes
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <chrono>
#include <cmath>

namespace s3d {

// Basic types
using String = std::u32string;
using FilePath = std::string;

// Math types
struct Point {
    int x = 0, y = 0;
    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}
};

struct Vec2 {
    double x = 0.0, y = 0.0;
    Vec2() = default;
    Vec2(double x_, double y_) : x(x_), y(y_) {}
    
    double distanceFrom(const Vec2& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

struct RectF {
    double x = 0.0, y = 0.0, w = 0.0, h = 0.0;
    RectF() = default;
    RectF(double x_, double y_, double w_, double h_) : x(x_), y(y_), w(w_), h(h_) {}
    
    bool contains(const Vec2& point) const {
        return point.x >= x && point.x <= x + w &&
               point.y >= y && point.y <= y + h;
    }
};

struct ColorF {
    double r = 1.0, g = 1.0, b = 1.0, a = 1.0;
    ColorF() = default;
    ColorF(double r_, double g_, double b_, double a_ = 1.0) : r(r_), g(g_), b(b_), a(a_) {}
};

// Noise
class PerlinNoise {
public:
    PerlinNoise(int seed = 0) {}
    double noise2D_01(double x, double y) const { return 0.5; } // Stub
};

// JSON
class JSON {
public:
    JSON() = default;
    JSON(const JSON&) = default;
    JSON& operator=(const JSON&) = default;
    
    static JSON array() { return JSON{}; }
    static JSON Load(const FilePath& path) { return JSON{}; }
    
    bool save(const FilePath& path) const { return true; }
    bool contains(const String& key) const { return false; }
    
    JSON& operator[](const String& key) { return *this; }
    const JSON& operator[](const String& key) const { return *this; }
    JSON& operator[](size_t index) { return *this; }
    const JSON& operator[](size_t index) const { return *this; }
    
    size_t size() const { return 0; }
    void push_back(const JSON& value) {}
    
    template<typename T>
    T get() const { return T{}; }
    
    String getString() const { return U""; }
    bool is_string() const { return false; }
    bool is_array() const { return false; }
    bool is_object() const { return false; }
    
    // Iterator support
    struct iterator {
        iterator& operator++() { return *this; }
        bool operator!=(const iterator&) const { return false; }
        std::pair<std::string, JSON> operator*() const { return {"", JSON{}}; }
    };
    
    iterator begin() const { return iterator{}; }
    iterator end() const { return iterator{}; }
    
    std::vector<std::pair<std::string, JSON>> items() const { return {}; }
    
    explicit operator bool() const { return true; }
};

// Filesystem
namespace FileSystem {
    inline bool IsDirectory(const FilePath& path) { return false; }
    inline bool CreateDirectories(const FilePath& path) { return true; }
    inline std::vector<FilePath> DirectoryContents(const FilePath& path) { return {}; }
}

// String utilities
inline String ToString(int value) {
    std::string str = std::to_string(value);
    return String(str.begin(), str.end());
}

} // namespace s3d