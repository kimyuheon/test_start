#include "dbents.h"
#include "gepnt3d.h"
#include "geassign.h"
#include "gearc3d.h"

// 교차점 계산 함수
AcGePoint3dArray getIntersectionPoints(AcDbPolyline* pPolyline, AcDbEntity* pTrimBoundary)
{
    AcGePoint3dArray intersectionPoints;

    // 교차점 계산
    if (pPolyline->intersectWith(pTrimBoundary, AcDb::kOnBothOperands, intersectionPoints) == Acad::eOk) {
        acutPrintf(_T("Found %d intersection points.\n"), intersectionPoints.length());
    } else {
        acutPrintf(_T("No intersection points found.\n"));
    }

    return intersectionPoints;
}

// 호(Arc) 세그먼트를 분할하는 함수
void splitArcSegment(AcGeCircArc3d& arc, const AcGePoint3d& splitPoint, AcDbPolyline* pNewPolyline)
{
    // 시작점과 끝점
    AcGePoint3d startPoint = arc.startPoint();
    AcGePoint3d endPoint = arc.endPoint();

    // 분할된 호 생성
    AcGeCircArc3d arcSegment1(startPoint, arc.center(), splitPoint, arc.normal());
    AcGeCircArc3d arcSegment2(splitPoint, arc.center(), endPoint, arc.normal());

    // 분할된 호를 Polyline에 추가 (2D 변환)
    AcGePoint2d start2D(arcSegment1.startPoint().x, arcSegment1.startPoint().y);
    AcGePoint2d split2D(splitPoint.x, splitPoint.y);
    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), start2D);
    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), split2D);

    AcGePoint2d split2D2(splitPoint.x, splitPoint.y);
    AcGePoint2d end2D(endPoint.x, endPoint.y);
    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), split2D2);
    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), end2D);
}

// 폴리선을 교차점 기준으로 Trim
AcDbPolyline* trimPolyline(AcDbPolyline* pPolyline, const AcGePoint3dArray& intersectionPoints)
{
    if (!pPolyline || intersectionPoints.length() < 2) {
        acutPrintf(_T("Invalid polyline or insufficient intersection points.\n"));
        return nullptr;
    }

    // 새로운 폴리선 생성
    AcDbPolyline* pTrimmedPolyline = new AcDbPolyline();

    // 기존 폴리선의 세그먼트를 반복 처리
    int vertexIndex = 0;
    for (int i = 0; i < pPolyline->numVerts() - 1; ++i) {
        if (pPolyline->segType(i) == AcDbPolyline::kArc) {
            // 호(Arc) 세그먼트 처리
            AcGeCircArc3d arc;
            if (pPolyline->getArcSegAt(i, arc) == Acad::eOk) {
                for (int j = 0; j < intersectionPoints.length(); ++j) {
                    if (arc.isOn(intersectionPoints[j])) {
                        splitArcSegment(arc, intersectionPoints[j], pTrimmedPolyline);
                    }
                }
            }
        } else {
            // 직선 세그먼트 처리
            AcGeLineSeg3d lineSegment;
            if (pPolyline->getLineSegAt(i, lineSegment) == Acad::eOk) {
                for (int j = 0; j < intersectionPoints.length(); ++j) {
                    if (lineSegment.isOn(intersectionPoints[j])) {
                        AcGePoint3d startPoint = (j == 0) ? lineSegment.startPoint() : intersectionPoints[j - 1];
                        AcGePoint3d endPoint = intersectionPoints[j];

                        if (lineSegment.isOn(startPoint) && lineSegment.isOn(endPoint)) {
                            AcGePoint2d start2D(startPoint.x, startPoint.y);
                            AcGePoint2d end2D(endPoint.x, endPoint.y);

                            pTrimmedPolyline->addVertexAt(vertexIndex++, start2D);
                            pTrimmedPolyline->addVertexAt(vertexIndex++, end2D);
                        }
                    }
                }
            }
        }
    }

    return pTrimmedPolyline;
}

// Trim 테스트 명령
void TestTrimPolyline()
{
    AcDbPolyline* pPolyline = nullptr;
    AcDbEntity* pTrimBoundary = nullptr;

    // 여기서 pPolyline과 pTrimBoundary는 이미 생성된 객체를 가정합니다.
    // pPolyline = ...;  // 기존 폴리선 객체 가져오기
    // pTrimBoundary = ...;  // 트림 경계 객체 가져오기

    if (!pPolyline || !pTrimBoundary) {
        acutPrintf(_T("Invalid polyline or trim boundary.\n"));
        return;
    }

    // 교차점 계산
    AcGePoint3dArray intersectionPoints = getIntersectionPoints(pPolyline, pTrimBoundary);

    // 폴리선 Trim
    AcDbPolyline* pTrimmedPolyline = trimPolyline(pPolyline, intersectionPoints);
    if (pTrimmedPolyline) {
        // 트림된 폴리선을 AutoCAD에 추가
        AcDbBlockTable* pBlockTable;
        acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

        AcDbBlockTableRecord* pBlockTableRecord;
        pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

        pBlockTableRecord->appendAcDbEntity(pTrimmedPolyline);

        pBlockTableRecord->close();
        pBlockTable->close();

        pTrimmedPolyline->close();

        acutPrintf(_T("Trimmed polyline added to the model space.\n"));
    }
}
