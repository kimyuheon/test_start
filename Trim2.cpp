#include "dbents.h"
#include "geassign.h"
#include "gepnt3d.h"
#include "gearc3d.h"

// 교차점 계산 함수
AcGePoint3dArray getIntersectionPoints(AcDbPolyline* pPolyline, AcDbEntity* pBoundary)
{
    AcGePoint3dArray intersectionPoints;

    if (pPolyline->intersectWith(pBoundary, AcDb::kOnBothOperands, intersectionPoints) == Acad::eOk) {
        acutPrintf(_T("Found %d intersection points.\n"), intersectionPoints.length());
    } else {
        acutPrintf(_T("No intersection points found.\n"));
    }

    return intersectionPoints;
}

// 호 세그먼트 분할 및 추가
void addArcSegment(AcGeCircArc3d& arc, const AcGePoint3d& start, const AcGePoint3d& end, AcDbPolyline* pNewPolyline)
{
    double startAngle = arc.center().angleTo(start);
    double endAngle = arc.center().angleTo(end);

    AcGePoint2d start2D(start.x, start.y);
    AcGePoint2d end2D(end.x, end.y);

    int index = pNewPolyline->numVerts();
    pNewPolyline->addVertexAt(index, start2D);
    pNewPolyline->addVertexAt(index + 1, end2D, arc.radius(), startAngle, endAngle);
}

// 직선 세그먼트 추가
void addLineSegment(const AcGePoint3d& start, const AcGePoint3d& end, AcDbPolyline* pNewPolyline)
{
    AcGePoint2d start2D(start.x, start.y);
    AcGePoint2d end2D(end.x, end.y);

    int index = pNewPolyline->numVerts();
    pNewPolyline->addVertexAt(index, start2D);
    pNewPolyline->addVertexAt(index + 1, end2D);
}

// 폴리선을 특정 부분으로 분할 및 새로 생성
AcDbPolyline* splitPolyline(AcDbPolyline* pPolyline, const AcGePoint3dArray& intersectionPoints)
{
    if (!pPolyline || intersectionPoints.length() < 2) {
        acutPrintf(_T("Invalid polyline or insufficient intersection points.\n"));
        return nullptr;
    }

    // 새로운 폴리선 생성
    AcDbPolyline* pNewPolyline = new AcDbPolyline();

    for (int i = 0; i < pPolyline->numVerts() - 1; ++i) {
        if (pPolyline->segType(i) == AcDbPolyline::kArc) {
            // 호 세그먼트 처리
            AcGeCircArc3d arc;
            if (pPolyline->getArcSegAt(i, arc) == Acad::eOk) {
                for (int j = 0; j < intersectionPoints.length() - 1; ++j) {
                    if (arc.isOn(intersectionPoints[j]) && arc.isOn(intersectionPoints[j + 1])) {
                        addArcSegment(arc, intersectionPoints[j], intersectionPoints[j + 1], pNewPolyline);
                    }
                }
            }
        } else {
            // 직선 세그먼트 처리
            AcGeLineSeg3d line;
            if (pPolyline->getLineSegAt(i, line) == Acad::eOk) {
                for (int j = 0; j < intersectionPoints.length() - 1; ++j) {
                    if (line.isOn(intersectionPoints[j]) && line.isOn(intersectionPoints[j + 1])) {
                        addLineSegment(intersectionPoints[j], intersectionPoints[j + 1], pNewPolyline);
                    }
                }
            }
        }
    }

    return pNewPolyline;
}

// 테스트 명령어
void TestSplitPolyline()
{
    AcDbPolyline* pPolyline = nullptr;
    AcDbEntity* pBoundary = nullptr;

    // 예제: 이미 선택된 폴리선과 경계 객체를 사용
    // pPolyline = ...;  // 기존 폴리선 객체 가져오기
    // pBoundary = ...;  // 경계 객체 가져오기

    if (!pPolyline || !pBoundary) {
        acutPrintf(_T("Invalid polyline or boundary.\n"));
        return;
    }

    // 교차점 계산
    AcGePoint3dArray intersectionPoints = getIntersectionPoints(pPolyline, pBoundary);

    // 폴리선 분할
    AcDbPolyline* pNewPolyline = splitPolyline(pPolyline, intersectionPoints);
    if (pNewPolyline) {
        // 모델 공간에 새 폴리선 추가
        AcDbBlockTable* pBlockTable;
        acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

        AcDbBlockTableRecord* pBlockTableRecord;
        pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

        pBlockTableRecord->appendAcDbEntity(pNewPolyline);

        pBlockTableRecord->close();
        pBlockTable->close();

        pNewPolyline->close();

        acutPrintf(_T("New polyline added to the model space.\n"));
    }
}
