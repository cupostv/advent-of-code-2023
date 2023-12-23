#include "../helpers/includes.hpp"

#define INPUT "input.txt"

struct Point3D {
  int64_t z;
  int64_t x;
  int64_t y;

  auto operator<=>(const Point3D&) const = default;

  friend std::ostream& operator << (std::ostream& os, const Point3D& p) {
      os << "(" << p.x << "," << p.y << "," << p.z << ")";
      return os;
  }

  friend std::istream& operator >> (std::istream& is, Point3D& p) {
      is >> p.x;
      is.ignore(2, ',');
      is >> p.y;
      is.ignore(2, ',');
      is >> p.z;
      return is;
  }
};

// Start from lowest z
// Maybe use raycasting and cast the ray from each x or y down
// and check intersections. Take maximum intersection and translate brick to
// that Z position
struct Brick {
  Point3D p1;
  Point3D p2;

  int32_t id;

  std::pair<bool, Point3D> intersects(const Brick &b) const;

  void translateZ(int64_t z) {
    auto zDiff = p2.z - p1.z;
    assert(p2.z >= p1.z);
    p1.z = z;
    p2.z = z + zDiff;
  }

  auto operator<=>(const Brick&) const = default;

  friend std::ostream& operator << (std::ostream& os, const Brick& p) {
      os << p.id << "[" << p.p1 << "," << p.p2 << "]";
      return os;
  }
};

struct Ray2D {
  helper::Point p;

  // Check if ray intersects brick
  std::pair<bool, Point3D> intersects(const Brick &b) const {
    // Ray has x y and infinite z
    if ((p.x >= b.p1.x && p.x <= b.p2.x) &&
        (p.y >= b.p1.y && p.y <= b.p2.y)) return {true, {std::max(b.p1.z, b.p2.z), p.x, p.y}};
    return {false, {}};
  }
};

std::pair<bool, Point3D> Brick::intersects(const Brick &b) const {
  // cast all rays to find intersection point
  // one ray can only intersect with one point in a brick
  // we are safe to stop once we find first point
  auto startX = std::min(p1.x, p2.x);
  auto stopX = std::max(p1.x, p2.x);
  auto startY = std::min(p1.y, p2.y);
  auto stopY = std::max(p1.y, p2.y);
  for (auto x = startX; x <= stopX; x++) {
    for (auto y = startY; y <= stopY; y++) {
      auto ray = Ray2D{{x, y}};
      auto [intersects, point] = ray.intersects(b);
      if (intersects) return {intersects, point};
    }
  }
  return {false, {}};
}

struct FallingBricks {
  // first vecotr are all bricks with lowest Z
  // Last vector are all bricks with highest Z
  std::vector<Brick> bricks;

  void sort() {
    // Bricks that have different p2.z will be at the end of one z value
    std::sort(bricks.begin(), bricks.end(), [](const auto &b1, const auto &b2) {
      if (b1.p1.z < b2.p1.z) return true;
      if (b1.p2.z < b2.p2.z) return true;
      return false;
    });
  }

  using Supporting = std::unordered_map<int32_t, std::vector<int32_t>>;
  using SupportedBy = std::unordered_map<int32_t, std::set<int32_t>>;

  auto squeezeAndGetSupport() {
    sort();

    // squeeze bricks to the lowest Z and stack them
    // max brick size == 4
    auto lowestZ = bricks[0].p1.z;

    Supporting supports;

    for (int32_t i = 1; i < bricks.size(); i++) {
      int64_t maxZ = -1;
      std::vector<int32_t> maxSupports;
      for (int32_t j = i - 1; j >= 0; j--) {
        if (bricks[i].p1.z == bricks[j].p1.z) continue;
        if (bricks[i].p1.z < bricks[j].p2.z) continue;
        auto [intersect, point] = bricks[i].intersects(bricks[j]);
        if (intersect) {
          // TODO: break when tracking backwards once we
          if(point.z > maxZ) {
            maxSupports = {bricks[j].id};
          }
          else if (point.z == maxZ) {
            maxSupports.push_back(bricks[j].id);
          }
          maxZ = std::max(maxZ, point.z);
          assert(maxZ < bricks[i].p2.z);
        }
      }
      if (maxZ == -1) {
        bricks[i].translateZ(lowestZ);
      } else {
        bricks[i].translateZ(maxZ + 1);
        for (auto max : maxSupports) {
          supports[max].push_back(bricks[i].id);
        }
      }
    }

    return supports;
  }

  int64_t getDisintegratedCount() {
    auto supports = std::move(squeezeAndGetSupport());

    SupportedBy supportedBy;
    for (auto [key, value] : supports) {
      for (auto b : value) {
        supportedBy[b].insert(key);
      }
    }

    int64_t disintegrated = 0;
    for (auto brick : bricks) {
      // Does brick support someone?
      if (supports[brick.id].size() == 0) {
        disintegrated++;
        continue;
      }

      // Does someone else support bricks supported by this brick?
      bool someoneElse = true;
      for (auto b : supports[brick.id]) {
        if (helper::find(supportedBy[b], brick.id) && supportedBy[b].size() == 1)  {
          someoneElse = false;
          break;
        }
      }

      // If yes, disintegrate current brick
      if (someoneElse) {
        disintegrated++;
      }
    }
    return disintegrated;
  }
};

int32_t main() {

    std::ifstream input;
    input.open(INPUT);

    if (!input) return EXIT_FAILURE;

    FallingBricks falling;
    int32_t i = 1;

    while (!input.eof()) {

        std::string inputRow;

        std::getline(input, inputRow);
        if (inputRow.empty()) break;
        std::stringstream inputStream(inputRow);
        Point3D p1, p2;

        std::string point;
        std::getline(inputStream, point, '~');
        std::stringstream pointStream(point);
        pointStream >> p1;

        std::getline(inputStream, point);
        pointStream.str(point);
        pointStream.clear();

        pointStream >> p2;

        auto count = 0;
        if (p1.x != p2.x) count++;
        if (p1.y != p2.y) count++;
        if (p1.z != p2.z) count++;

        assert(p1.x <= p2.x);
        assert(p1.y <= p2.y);
        assert(p1.z <= p2.z);

        assert (count <= 1);

        falling.bricks.push_back(Brick{p1, p2, i++});
    }

    std::cout << falling.getDisintegratedCount() << std::endl;

    return EXIT_SUCCESS;
}
