#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef K::Point_2 Point;

int main() {
    // (x, y, z) 좌표
    std::vector<std::tuple<Point, double>> points = {
        {{0, 0}, 10}, {{1, 0}, 20}, {{0, 1}, 30}, {{1, 1}, 40}, {{0.5, 0.5}, 25}
    };

    // Delaunay 삼각분할
    Delaunay dt;
    for (const auto& p : points) {
        dt.insert(std::get<0>(p));
    }

    // 보간 대상 (x, y) 좌표
    std::vector<Point> targets = {{0.25, 0.25}, {0.75, 0.75}, {0.1, 0.9}};

    for (const auto& target : targets) {
        auto face = dt.locate(target);
        if (dt.is_infinite(face)) {
            std::cout << "Point (" << target << ") is outside the triangulation." << std::endl;
            continue;
        }

        // 삼각형 꼭지점 가져오기
        auto v1 = face->vertex(0);
        auto v2 = face->vertex(1);
        auto v3 = face->vertex(2);

        double z1 = std::get<1>(points[v1->info()]);
        double z2 = std::get<1>(points[v2->info()]);
        double z3 = std::get<1>(points[v3->info()]);

        // 보간
        double areaTotal = CGAL::area(v1->point(), v2->point(), v3->point());
        double w1 = CGAL::area(target, v2->point(), v3->point()) / areaTotal;
        double w2 = CGAL::area(v1->point(), target, v3->point()) / areaTotal;
        double w3 = CGAL::area(v1->point(), v2->point(), target) / areaTotal;

        double z = w1 * z1 + w2 * z2 + w3 * z3;

        std::cout << "Z at (" << target.x() << ", " << target.y() << ") is " << z << std::endl;
    }

    return 0;
}
