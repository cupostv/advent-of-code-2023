#include "../helpers/includes.hpp"

// fx(t) = x + t * vx

// We have a system with multiple unknown
// if we take 3 first points
// x, vx, t0, t1, t2
// x0 + t0 * vx0 = x + t0 * vx
// x1 + t1 * vx1 = x + t1 * vx
// x2 + t2 * vx2 = x + t2 * vx
// If we do the same for y and z
// We will get 9 equations with 9 unknown and we can calculate x, y and z
// Except it's too much to do manually
// I could use Z3 library, but instead I will just calculate result with online equation solvers

// First three points
// 189484959431670, 401088290781515, 262795384692232 @ 95, -36, 39
// 175716591307178, 163094456512341, 297355219841654 @ 160, 182, -56
// 283402568811320, 363632912812075, 314435988531407 @ -6, 37, -10

// (189484959431670 + t0 * 95 = x + t0 * vx)
// (401088290781515 + t0 * -36 = y + t0 * vy)
// (262795384692232 + t0 * 39 = z + t0 * vz)

// (175716591307178 + t1 * 160 = x + t1 * vx)
// (163094456512341 + t1 * 182 = y + t1 * vy)
// (297355219841654 + t1 * -56 = z + t1 * vz)

// (283402568811320 + t2 * -6 = x + t2 * vx)
// (363632912812075 + t2 * 37 = y + t2 * vy)
// (314435988531407 + t2 * -10 = z + t2 * vz)

// Solve t0, t1, t2, x, y, z, vx, vy, vz
// (x0 + t0 * vx0 = x + t0 * vx) => t0 * vx0 - t0 * vx = x - x0 => t0 = (x - x0) / (vx0 - vx)
// (y0 + t0 * vy0 = y + t0 * vy) => t0 = (y - y0) / (vy0 - vy)
// (z0 + t0 * vz0 = z + t0 * vz) =>
// x = x0 + t0 * vx0 - t0 * vx
// y = y0 + t0 * vy0 - t0 * vy
// z = z0 + t0 * vz0 - t0 * vz

// (x1 + t1 * vx1 = x + t1 * vx)
// (y1 + t1 * vy1 = y + t1 * vy)
// (z1 + t1 * vz1 = z + t1 * vz)

// (x2 + t2 * vx2 = x + t2 * vx)
// (y2 + t2 * vy2 = y + t2 * vy)
// (z2 + t2 * vz2 = z + t2 * vz)

#define INPUT "input.txt"

int32_t main() {

    // Thx https://quickmath.com/webMathematica3/quickmath/equations/solve/advanced.jsp#c=solve_solveequationsadvanced&v1=x%253D189484959431670%2Ba*95-a*d%250Ay%253D401088290781515%2Ba*-36-a*e%250Az%253D262795384692232%2520%2Ba*39-a*f%250Ax%253D175716591307178%2Bb*160-b*d%250Ay%253D163094456512341%2Bb*182-b*e%250Az%253D297355219841654%2Bb*-56-b*f%250Ax%253D283402568811320%2Bc*-6-c*d%250Ay%253D363632912812075%2Bc*37-c*e%250Az%253D314435988531407%2Bc*-10-c*f&v2=x%250Ay%250Az%250Aa%250Ab%250Ac%250Ad%250Ae%250Af <3
    std::cout << std::fixed << 234382970331570LL + 100887864960615LL + 231102671115832LL << " :)" << std::endl;

    return EXIT_SUCCESS;
}
