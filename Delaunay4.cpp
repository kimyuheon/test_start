#include <acge.h> // ObjectARX의 AeGePoint3d 사용
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// 삼각형 구조체 정의
struct Triangle {
    AeGePoint3d a, b, c;
    Triangle(AeGePoint3d pa, AeGePoint3d pb, AeGePoint3d pc) : a(pa), b(pb), c(pc) {}
};

// K-D 트리 노드 정의
struct KDTreeNode {
    AeGePoint3d point;
    int left;
    int right;
    KDTreeNode(AeGePoint3d p) : point(p), left(-1), right(-1) {}
};

// 유틸리티 함수
double distance(AeGePoint3d p1, AeGePoint3d p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// 외접원 내부 점 검사
bool isInsideCircumcircle(AeGePoint3d p, Triangle tri) {
    double ax = tri.a.x - p.x;
    double ay = tri.a.y - p.y;
    double bx = tri.b.x - p.x;
    double by = tri.b.y - p.y;
    double cx = tri.c.x - p.x;
    double cy = tri.c.y - p.y;

    double det = (ax * ax + ay * ay) * (bx * cy - cx * by) -
                 (bx * bx + by * by) * (ax * cy - cx * ay) +
                 (cx * cx + cy * cy) * (ax * by - bx * ay);

    return det > 0;
}

// 삼각형 내부 점 검사
bool isInsideTriangle(AeGePoint3d p, Triangle tri) {
    double detT = (tri.b.x - tri.a.x) * (tri.c.y - tri.a.y) -
                  (tri.c.x - tri.a.x) * (tri.b.y - tri.a.y);

    double alpha = ((tri.b.x - p.x) * (tri.c.y - p.y) -
                    (tri.c.x - p.x) * (tri.b.y - p.y)) / detT;
    double beta = ((tri.c.x - p.x) * (tri.a.y - p.y) -
                   (tri.a.x - p.x) * (tri.c.y - p.y)) / detT;
    double gamma = 1.0 - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

// 선형 보간
double interpolate(AeGePoint3d p, Triangle tri) {
    double detT = (tri.b.x - tri.a.x) * (tri.c.y - tri.a.y) -
                  (tri.c.x - tri.a.x) * (tri.b.y - tri.a.y);

    double alpha = ((tri.b.x - p.x) * (tri.c.y - p.y) -
                    (tri.c.x - p.x) * (tri.b.y - p.y)) / detT;
    double beta = ((tri.c.x - p.x) * (tri.a.y - p.y) -
                   (tri.a.x - p.x) * (tri.c.y - p.y)) / detT;
    double gamma = 1.0 - alpha - beta;

    return alpha * tri.a.z + beta * tri.b.z + gamma * tri.c.z;
}

// Delaunay 삼각분할
std::vector<Triangle> delaunayTriangulation(std::vector<AeGePoint3d> points) {
    std::vector<Triangle> triangles;

    // Super Triangle
    AeGePoint3d superA(-1000, -1000, 0);
    AeGePoint3d superB(1000, -1000, 0);
    AeGePoint3d superC(0, 1000, 0);
    triangles.push_back(Triangle(superA, superB, superC));

    // Bowyer-Watson 알고리즘
    for (int i = 0; i < points.size(); ++i) {
        std::vector<Triangle> badTriangles;
        std::vector<std::pair<AeGePoint3d, AeGePoint3d>> polygon;

        // 외접원에 포함된 삼각형 찾기
        for (int j = 0; j < triangles.size(); ++j) {
            if (isInsideCircumcircle(points[i], triangles[j])) {
                badTriangles.push_back(triangles[j]);
                polygon.push_back(std::make_pair(triangles[j].a, triangles[j].b));
                polygon.push_back(std::make_pair(triangles[j].b, triangles[j].c));
                polygon.push_back(std::make_pair(triangles[j].c, triangles[j].a));
            }
        }

        // 중복 간선 제거
        for (int j = 0; j < polygon.size(); ++j) {
            for (int k = j + 1; k < polygon.size(); ++k) {
                if ((polygon[j].first.x == polygon[k].second.x &&
                     polygon[j].first.y == polygon[k].second.y &&
                     polygon[j].second.x == polygon[k].first.x &&
                     polygon[j].second.y == polygon[k].first.y)) {
                    polygon.erase(polygon.begin() + k);
                    polygon.erase(polygon.begin() + j);
                    j--;
                    break;
                }
            }
        }

        // 새로운 삼각형 추가
        for (int j = 0; j < polygon.size(); ++j) {
            triangles.push_back(Triangle(polygon[j].first, polygon[j].second, points[i]));
        }
    }

    // Super Triangle에 의존한 삼각형 제거
    std::vector<Triangle> result;
    for (int i = 0; i < triangles.size(); ++i) {
        if (triangles[i].a.x < -999 || triangles[i].b.x > 999 || triangles[i].c.y > 999) {
            continue;
        }
        result.push_back(triangles[i]);
    }

    return result;
}

// K-D 트리 생성
int buildKDTree(std::vector<KDTreeNode> &tree, int left, int right, int depth) {
    if (left > right) return -1;

    int axis = depth % 2; // 0: x축, 1: y축
    int mid = (left + right) / 2;

    std::nth_element(tree.begin() + left, tree.begin() + mid, tree.begin() + right + 1,
                     [axis](KDTreeNode a, KDTreeNode b) {
                         return axis == 0 ? a.point.x < b.point.x : a.point.y < b.point.y;
                     });

    tree[mid].left = buildKDTree(tree, left, mid - 1, depth + 1);
    tree[mid].right = buildKDTree(tree, mid + 1, right, depth + 1);

    return mid;
}

// K-D 트리에서 삼각형 검색
int searchKDTree(std::vector<KDTreeNode> tree, AeGePoint3d target, int root, int depth) {
    if (root == -1) return -1;

    int axis = depth % 2;
    double diff = (axis == 0) ? (target.x - tree[root].point.x) : (target.y - tree[root].point.y);

    int next = (diff < 0) ? tree[root].left : tree[root].right;
    int best = searchKDTree(tree, target, next, depth + 1);

    return best == -1 ? root : best;
}

int main() {
    // 입력 데이터
    std::vector<AeGePoint3d> points;
    points.push_back(AeGePoint3d(0, 0, 10));
    points.push_back(AeGePoint3d(1, 0, 20));
    points.push_back(AeGePoint3d(0, 1, 30));
    points.push_back(AeGePoint3d(1, 1, 40));
    points.push_back(AeGePoint3d(0.5, 0.5, 25));

    // Delaunay 삼각분할 수행
    std::vector<Triangle> triangles = delaunayTriangulation(points);

    // K-D 트리 구성
    std::vector<KDTreeNode> kdTreeNodes;
    for (int i = 0; i < triangles.size(); ++i) {
        AeGePoint3d centroid((triangles[i].a.x + triangles[i].b.x + triangles[i].c.x) / 3,
                             (triangles[i].a.y + triangles[i].b.y + triangles[i].c.y) / 3, 0);
        kdTreeNodes.push_back(KDTreeNode(centroid));
    }
    int root = buildKDTree(kdTreeNodes, 0, kdTreeNodes.size() - 1, 0);

    // 보간 대상 좌표
    std::vector<AeGePoint3d> targets;
    targets.push_back(AeGePoint3d(0.25, 0.25));
    targets.push_back(AeGePoint3d(0.75, 0.75));
    targets.push_back(AeGePoint3d(0.1, 0.9));

    // 보간 수행
    for (int i = 0; i < targets.size(); ++i) {
        int nearestTriIdx = searchKDTree(kdTreeNodes, targets[i], root, 0);
        if (nearestTriIdx != -1 && isInsideTriangle(targets[i], triangles[nearestTriIdx])) {
            double z = interpolate(targets[i], triangles[nearestTriIdx]);
            std::cout << "Z at (" << targets[i].x << ", " << targets[i].y << ") = " << z << std::endl;
        } else {
            std::cout << "Target (" << targets[i].x << ", " << targets[i].y << ") is outside triangulated area.\n";
        }
    }

    return 0;
}
