#include <iostream>
#include <vector>
#include <cmath>

// 2D Point 정의
struct Point {
    double x, y, z;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

// Delaunay 삼각형 구조체
struct Triangle {
    Point a, b, c;
    Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}
};

// 삼각형 내부 선형 보간
double interpolateInTriangle(const Point &p, const Triangle &tri) {
    double x = p.x, y = p.y;

    double x1 = tri.a.x, y1 = tri.a.y, z1 = tri.a.z;
    double x2 = tri.b.x, y2 = tri.b.y, z2 = tri.b.z;
    double x3 = tri.c.x, y3 = tri.c.y, z3 = tri.c.z;

    double detT = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    double w1 = ((x2 - x) * (y3 - y) - (x3 - x) * (y2 - y)) / detT;
    double w2 = ((x3 - x) * (y1 - y) - (x1 - x) * (y3 - y)) / detT;
    double w3 = 1 - w1 - w2;

    return w1 * z1 + w2 * z2 + w3 * z3;
}

int main() {
    // 주어진 (x, y, z) 점들
    std::vector<Point> points = {
        Point(0, 0, 10), Point(1, 0, 20), Point(0, 1, 30), Point(1, 1, 40)
    };

    // 하나의 삼각형 정의 (단순한 예제)
    Triangle tri(points[0], points[1], points[2]);

    // 보간할 (x, y) 좌표
    std::vector<Point> targets = {
        Point(0.5, 0.5, 0), Point(0.2, 0.2, 0), Point(0.7, 0.3, 0)
    };

    for (const auto &p : targets) {
        double z = interpolateInTriangle(p, tri);
        std::cout << "Z at (" << p.x << ", " << p.y << ") = " << z << std::endl;
    }

    return 0;
}
