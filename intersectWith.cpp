#include "dbents.h"
#include "geassign.h"
#include "gepnt3d.h"
#include "gepttol.h"

// 공차에 따른 점 비교
bool isPointCloseEnough(const AcGePoint3d& p1, const AcGePoint3d& p2, double tolerance)
{
    return p1.distanceTo(p2) <= tolerance;
}

// 세그먼트 단위로 교차 계산 및 정확도 향상
void calculateAccurateIntersections(AcDbPolyline* pPolyline1, AcDbPolyline* pPolyline2, double tolerance)
{
    if (!pPolyline1 || !pPolyline2) {
        acutPrintf(_T("Invalid polylines.\n"));
        return;
    }

    AcGePoint3dArray intersectionPoints; // 최종 교차점 저장

    // 첫 번째 Polyline의 각 세그먼트와 두 번째 Polyline 전체 교차 확인
    for (int i = 0; i < pPolyline1->numVerts() - 1; ++i) {
        AcGeLineSeg3d lineSegment;
        if (pPolyline1->getLineSegAt(i, lineSegment) == Acad::eOk) {
            AcGePoint3dArray tempPoints;

            // 두 번째 Polyline과 교차 확인
            if (pPolyline2->intersectWith(lineSegment, AcDb::kOnBothOperands, tempPoints) == Acad::eOk) {
                for (int j = 0; j < tempPoints.length(); ++j) {
                    const AcGePoint3d& point = tempPoints[j];

                    // 공차를 기준으로 중복 제거
                    bool isUnique = true;
                    for (int k = 0; k < intersectionPoints.length(); ++k) {
                        if (isPointCloseEnough(intersectionPoints[k], point, tolerance)) {
                            isUnique = false;
                            break;
                        }
                    }

                    if (isUnique) {
                        intersectionPoints.append(point);
                    }
                }
            }
        }
    }

    // 교차점 결과 출력
    acutPrintf(_T("Accurate intersection points found: %d\n"), intersectionPoints.length());
    for (int i = 0; i < intersectionPoints.length(); ++i) {
        acutPrintf(_T("Point %d: %.6f, %.6f, %.6f\n"),
                   i + 1, intersectionPoints[i].x, intersectionPoints[i].y, intersectionPoints[i].z);
    }
}
