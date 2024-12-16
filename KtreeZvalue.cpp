#include "acge.h"        // Geometry library
#include "acarray.h"     // Array utility
#include <vector>
#include <cmath>         // For sqrt
#include <queue>         // For nearest neighbor search
#include <algorithm>     // For sorting

// k-d 트리 노드 정의
struct KDNode {
    AcGePoint3d point;       // 현재 점
    KDNode* left;            // 왼쪽 서브트리
    KDNode* right;           // 오른쪽 서브트리

    KDNode(const AcGePoint3d& pt) : point(pt), left(nullptr), right(nullptr) {}
};

// k-d 트리 클래스 정의
class KDTree {
private:
    KDNode* root;            // 루트 노드

    // 재귀적으로 트리를 구축
    KDNode* buildTree(std::vector<AcGePoint3d>& points, int depth) {
        if (points.empty()) return nullptr;

        // 분할 차원 결정
        int axis = depth % 3; // 0: x, 1: y, 2: z

        // 분할을 위해 정렬
        auto comparator = [axis](const AcGePoint3d& a, const AcGePoint3d& b) {
            if (axis == 0) return a.x < b.x;
            if (axis == 1) return a.y < b.y;
            return a.z < b.z;
        };
        std::sort(points.begin(), points.end(), comparator);

        // 중간 점을 선택
        size_t mid = points.size() / 2;
        AcGePoint3d median = points[mid];

        // 왼쪽과 오른쪽 서브트리
        std::vector<AcGePoint3d> leftPoints(points.begin(), points.begin() + mid);
        std::vector<AcGePoint3d> rightPoints(points.begin() + mid + 1, points.end());

        // 노드 생성 및 서브트리 연결
        KDNode* node = new KDNode(median);
        node->left = buildTree(leftPoints, depth + 1);
        node->right = buildTree(rightPoints, depth + 1);

        return node;
    }

    // 범위 검색 (재귀)
    void rangeSearch(KDNode* node, const AcGePoint3d& target, double range, int depth, std::vector<double>& result) {
        if (!node) return;

        // 거리 계산
        double distance = target.distanceTo(node->point);
        if (distance <= range) {
            result.push_back(node->point.z); // Z 좌표 추가
        }

        // 분할 차원 결정
        int axis = depth % 3;
        double delta = 0.0;
        if (axis == 0) delta = target.x - node->point.x;
        if (axis == 1) delta = target.y - node->point.y;
        if (axis == 2) delta = target.z - node->point.z;

        // 탐색할 방향 결정
        if (delta < 0) {
            rangeSearch(node->left, target, range, depth + 1, result);
            if (std::abs(delta) <= range) {
                rangeSearch(node->right, target, range, depth + 1, result);
            }
        } else {
            rangeSearch(node->right, target, range, depth + 1, result);
            if (std::abs(delta) <= range) {
                rangeSearch(node->left, target, range, depth + 1, result);
            }
        }
    }

public:
    // 생성자
    KDTree(const std::vector<AcGePoint3d>& points) {
        std::vector<AcGePoint3d> pointsCopy = points; // 원본 데이터 복사
        root = buildTree(pointsCopy, 0);
    }

    // 범위 검색
    std::vector<double> findZCoordinatesWithinDistance(const AcGePoint3d& target, double range) {
        std::vector<double> result;
        rangeSearch(root, target, range, 0, result);
        return result;
    }
};

// 테스트 함수
void testKDTree() {
    // 1. 3D 점 목록 생성
    std::vector<AcGePoint3d> points = {
        AcGePoint3d(0, 0, 10), AcGePoint3d(5, 5, 20), AcGePoint3d(10, 10, 30),
        AcGePoint3d(15, 15, 40), AcGePoint3d(20, 20, 50), AcGePoint3d(25, 25, 60)
    };

    // 2. k-d 트리 생성
    KDTree tree(points);

    // 3. 기준 점과 거리 범위 설정
    AcGePoint3d basePoint(10, 10, 0); // 기준점
    double distanceThreshold = 15.0; // 거리 범위

    // 4. 범위 검색 실행
    std::vector<double> result = tree.findZCoordinatesWithinDistance(basePoint, distanceThreshold);

    // 5. 결과 출력
    acutPrintf(_T("Z Coordinates within %.2f units of the base point:\n"), distanceThreshold);
    for (double z : result) {
        acutPrintf(_T("%.2f\n"), z);
    }
}

// 명령어 정의
void initApp() {
    // AutoCAD 명령 등록
    acedRegCmds->addCommand(
        _T("MY_COMMANDS"),
        _T("TestKDTree"),
        _T("TESTKDTREE"),
        ACRX_CMD_MODAL,
        testKDTree
    );
}

void unloadApp() {
    // 명령어 제거
    acedRegCmds->removeGroup(_T("MY_COMMANDS"));
}

// DllMain 함수
extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
    switch (msg) {
        case AcRx::kInitAppMsg:
            acrxDynamicLinker->unlockApplication(pkt);
            acrxDynamicLinker->registerAppMDIAware(pkt);
            initApp();
            break;
        case AcRx::kUnloadAppMsg:
            unloadApp();
            break;
    }
    return AcRx::kRetOK;
}
