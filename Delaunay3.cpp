#include <iostream>
#include <vector>
#include <cmath>
#include <set>

// 점 정의
struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

// 삼각형 정의
struct Triangle {
    Point a, b, c;
    Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}
};

// 외접원 계산 함수
bool isInsideCircumcircle(const Point &p, const Triangle &t) {
    double ax = t.a.x - p.x, ay = t.a.y - p.y;
    double bx = t.b.x - p.x, by = t.b.y - p.y;
    double cx = t.c.x - p.x, cy = t.c.y - p.y;

    double det = (ax * ax + ay * ay) * (bx * cy - cx * by)
               - (bx * bx + by * by) * (ax * cy - cx * ay)
               + (cx * cx + cy * cy) * (ax * by - bx * ay);

    return det > 0;
}

// Bowyer-Watson 알고리즘을 사용한 Delaunay 삼각분할
std::vector<Triangle> delaunayTriangulation(const std::vector<Point> &points) {
    // Super Triangle (모든 점을 포함하는 매우 큰 삼각형)
    Point p1(-1000, -1000), p2(1000, -1000), p3(0, 1000);
    std::vector<Triangle> triangles = {Triangle(p1, p2, p3)};

    for (const auto &point : points) {
        std::vector<Triangle> badTriangles;

        // 외접원에 포함되는 삼각형 찾기
        for (const auto &t : triangles) {
            if (isInsideCircumcircle(point, t)) {
                badTriangles.push_back(t);
            }
        }

        // 경계 간선(홀 포인트) 찾기
        std::set<std::pair<Point, Point>> polygon;
        for (const auto &t : badTriangles) {
            auto edges = std::vector<std::pair<Point, Point>>{
                {t.a, t.b}, {t.b, t.c}, {t.c, t.a}};
            for (const auto &e : edges) {
                auto reverseEdge = std::make_pair(e.second, e.first);
                if (polygon.find(reverseEdge) != polygon.end()) {
                    polygon.erase(reverseEdge);
                } else {
                    polygon.insert(e);
                }
            }
        }

        // Bad Triangles 삭제
        triangles.erase(
            std::remove_if(triangles.begin(), triangles.end(),
                           [&badTriangles](const Triangle &t) {
                               return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
                           }),
            triangles.end());

        // 새 삼각형 생성
        for (const auto &e : polygon) {
            triangles.emplace_back(e.first, e.second, point);
        }
    }

    // Super Triangle에 의존한 삼각형 제거
    triangles.erase(
        std::remove_if(triangles.begin(), triangles.end(),
                       [&p1, &p2, &p3](const Triangle &t) {
                           return t.a == p1 || t.a == p2 || t.a == p3 ||
                                  t.b == p1 || t.b == p2 || t.b == p3 ||
                                  t.c == p1 || t.c == p2 || t.c == p3;
                       }),
        triangles.end());

    return triangles;
}

// 보간 함수
double interpolate(const Point &p, const Triangle &t) {
    double x = p.x, y = p.y;

    double x1 = t.a.x, y1 = t.a.y, z1 = t.a.z;
    double x2 = t.b.x, y2 = t.b.y, z2 = t.b.z;
    double x3 = t.c.x, y3 = t.c.y, z3 = t.c.z;

    double det = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    double w1 = ((x2 - x) * (y3 - y) - (x3 - x) * (y2 - y)) / det;
    double w2 = ((x3 - x) * (y1 - y) - (x1 - x) * (y3 - y)) / det;
    double w3 = 1 - w1 - w2;

    return w1 * z1 + w2 * z2 + w3 * z3;
}

int main() {
    // 입력 데이터
    std::vector<Point> points = {
        {0, 0, 10}, {1, 0, 20}, {0, 1, 30}, {1, 1, 40}, {0.5, 0.5, 25}
    };

    // Delaunay 삼각분할 수행
    auto triangles = delaunayTriangulation(points);

    // 보간 대상
    std::vector<Point> targets = {
        {0.25, 0.25}, {0.75, 0.75}, {0.1, 0.9}
    };

    for (const auto &p : targets) {
        for (const auto &t : triangles) {
            // 삼각형 내부에 포함되는지 확인 (barycentric coordinates 사용)
            double detT = (t.b.x - t.a.x) * (t.c.y - t.a.y) - (t.c.x - t.a.x) * (t.b.y - t.a.y);
            double alpha = ((t.b.x - p.x) * (t.c.y - p.y) - (t.c.x - p.x) * (t.b.y - p.y)) / detT;
            double beta = ((t.c.x - p.x) * (t.a.y - p.y) - (t.a.x - p.x) * (t.c.y - p.y)) / detT;
            double gamma = 1 - alpha - beta;

            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                double z = interpolate(p, t);
                std::cout << "Z at (" << p.x << ", " << p.y << ") = " << z << std::endl;
                break;
            }
        }
    }

    return 0;
}
