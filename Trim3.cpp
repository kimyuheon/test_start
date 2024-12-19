#include "dbents.h"
#include "geassign.h"
#include "gepnt3d.h"
#include "gearc3d.h"
#include <cmath>

// 중심과 반지름 계산 함수
void calculateArcCenterAndRadius(const AcGePoint2d& start, const AcGePoint2d& end, double bulge, AcGePoint2d& center, double& radius)
{
    double theta = atan(bulge) * 4.0; // 중심각
    double chordLength = start.distanceTo(end);
    radius = chordLength / (2 * sin(theta / 2));

    // 호의 중심 계산
    AcGeVector2d chordVector = end - start;
    AcGeVector2d normalVector(-chordVector.y, chordVector.x);
    normalVector.normalize();

    double sagitta = radius * (1 - cos(theta / 2));
    center = start + chordVector / 2 + normalVector * (bulge > 0 ? sagitta : -sagitta);
}

// 새로운 `bulge` 계산 함수
double calculateNewBulge(double startAngle, double endAngle)
{
    double theta = endAngle - startAngle;
    return tan(theta / 4.0);
}

// 호의 중간 부분을 처리하여 새로운 세그먼트를 추가
void trimArcSegment(AcGePoint2d start, AcGePoint2d end, double bulge, AcGePoint2d trimStart, AcGePoint2d trimEnd, AcDbPolyline* pNewPolyline)
{
    AcGePoint2d center;
    double radius;
    calculateArcCenterAndRadius(start, end, bulge, center, radius);

    // 시작 각도와 끝 각도 계산
    double startAngle = atan2(start.y - center.y, start.x - center.x);
    double endAngle = atan2(end.y - center.y, end.x - center.x);

    // #include "dbents.h"
#include "geassign.h"
#include "gepnt3d.h"
#include "gearc3d.h"
#include <cmath>

// 중심과 반지름 계산 함수
void calculateArcCenterAndRadius(const AcGePoint2d& start, const AcGePoint2d& end, double bulge, AcGePoint2d& center, double& radius)
{
    double theta = atan(bulge) * 4.0; // 중심각
    double chordLength = start.distanceTo(end);
    radius = chordLength / (2 * sin(theta / 2));

    // 호의 중심 계산
    AcGeVector2d chordVector = end - start;
    AcGeVector2d normalVector(-chordVector.y, chordVector.x);
    normalVector.normalize();

    double sagitta = radius * (1 - cos(theta / 2));
    center = start + chordVector / 2 + normalVector * (bulge > 0 ? sagitta : -sagitta);
}

// 새로운 `bulge` 계산 함수
double calculateNewBulge(double startAngle, double endAngle)
{
    double theta = endAngle - startAngle;
    return tan(theta / 4.0);
}

// 호의 중간 부분을 처리하여 새로운 세그먼트를 추가
void trimArcSegment(AcGePoint2d start, AcGePoint2d end, double bulge, AcGePoint2d trimStart, AcGePoint2d trimEnd, AcDbPolyline* pNewPolyline)
{
    AcGePoint2d center;
    double radius;
    calculateArcCenterAndRadius(start, end, bulge, center, radius);

    // 시작 각도와 끝 각도 계산
    double startAngle = atan2(start.y - center.y, start.x - center.x);
    double endAngle = atan2(end.y - center.y, end.x - center.x);

    // Trim 지점의 각도 계산
    double trimStartAngle = atan2(trimStart.y - center.y, trimStart.x - center.x);
    double trimEndAngle = atan2(trimEnd.y - center.y, trimEnd.x - center.x);

    // 새로운 `bulge` 값 계산
    double newBulge = calculateNewBulge(trimStartAngle, trimEndAngle);

    // 새로운 호 세그먼트 추가
    int index = pNewPolyline->numVerts();
    pNewPolyline->addVertexAt(index, trimStart, newBulge);
    pNewPolyline->addVertexAt(index + 1, trimEnd);
} 지점의 각도 계산
    double trimStartAngle = atan2(trimStart.y - center.y, trimStart.x - center.x);
    double trimEndAngle = atan2(trimEnd.y - center.y, trimEnd.x - center.x);

    // 새로운 `bulge` 값 계산
    double newBulge = calculateNewBulge(trimStartAngle, trimEndAngle);

    // 새로운 호 세그먼트 추가
    int index = pNewPolyline->numVerts();
    pNewPolyline->addVertexAt(index, trimStart, newBulge);
    pNewPolyline->addVertexAt(index + 1, trimEnd);
}

AcDbPolyline* trimPolylineUsingBulge(AcDbPolyline* pPolyline, const AcGePoint3d& startPoint, const AcGePoint3d& endPoint)
{
    if (!pPolyline) {
        acutPrintf(_T("Invalid polyline.\n"));
        return nullptr;
    }

    AcDbPolyline* pNewPolyline = new AcDbPolyline();

    bool started = false;

    for (int i = 0; i < pPolyline->numVerts() - 1; ++i) {
        // 시작점과 끝점 가져오기
        AcGePoint2d start2D, end2D;
        pPolyline->getPointAt(i, start2D);
        pPolyline->getPointAt(i + 1, end2D);

        // bulge 값 가져오기
        double bulge = pPolyline->getBulgeAt(i);

        // 2D 좌표를 3D 좌표로 변환
        AcGePoint3d segmentStart(start2D.x, start2D.y, 0.0);
        AcGePoint3d segmentEnd(end2D.x, end2D.y, 0.0);

        // Trim 작업 시작 여부 확인
        if (!started && segmentStart.isEqualTo(startPoint)) {
            started = true;
        }

        // Trim 작업 수행
        if (started) {
            if (segmentEnd.isEqualTo(endPoint)) {
                if (bulge != 0.0) {
                    // 호 세그먼트 Trim
                    trimArcSegment(start2D, end2D, bulge, AcGePoint2d(startPoint.x, startPoint.y), AcGePoint2d(endPoint.x, endPoint.y), pNewPolyline);
                } else {
                    // 직선 세그먼트 Trim
                    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), start2D);
                    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), end2D);
                }
                break; // Trim 완료
            }

            // 현재 세그먼트를 새로운 폴리선에 추가
            if (bulge != 0.0) {
                // 호 세그먼트 추가
                trimArcSegment(start2D, end2D, bulge, start2D, end2D, pNewPolyline);
            } else {
                // 직선 세그먼트 추가
                pNewPolyline->addVertexAt(pNewPolyline->numVerts(), start2D);
                pNewPolyline->addVertexAt(pNewPolyline->numVerts(), end2D);
            }
        }
    }

    return pNewPolyline;
}
