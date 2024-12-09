#include <dbents.h>
#include <acutads.h>
#include <geassign.h>
#include <cmath>
#include <vector>

// 점이 선분 위에 있는지 확인
bool isPointOnLineSegment(const AcGePoint2d& p, const AcGePoint2d& a, const AcGePoint2d& b) {
    if (p.x < std::min(a.x, b.x) || p.x > std::max(a.x, b.x) ||
        p.y < std::min(a.y, b.y) || p.y > std::max(a.y, b.y)) {
        return false;
    }

    double cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    return std::abs(cross) < 1e-9;
}

// 점이 아크 위에 있는지 확인
bool isPointOnArc(const AcGePoint2d& p, const AcGePoint2d& start, const AcGePoint2d& end, double bulge) {
    // 아크의 중심과 반지름 계산
    double theta = 4.0 * atan(bulge);
    double chordLength = start.distanceTo(end);
    double radius = std::abs(chordLength / (2.0 * sin(theta / 2.0)));
    AcGeVector2d chordVec = end - start;
    AcGeVector2d orthoVec(-chordVec.y, chordVec.x);
    orthoVec.normalize();
    AcGePoint2d midPoint = (start + end) / 2.0;
    AcGePoint2d center = midPoint + orthoVec * (radius * (bulge > 0 ? 1 : -1));

    // 점이 아크의 원 위에 있는지 확인
    double distToCenter = p.distanceTo(center);
    if (std::abs(distToCenter - radius) > 1e-9) {
        return false; // 점이 아크의 원 위에 있지 않음
    }

    // 점이 아크의 범위 내에 있는지 확인
    AcGeVector2d vecStart = start - center;
    AcGeVector2d vecEnd = end - center;
    AcGeVector2d vecP = p - center;

    double angleStart = atan2(vecStart.y, vecStart.x);
    double angleEnd = atan2(vecEnd.y, vecEnd.x);
    double angleP = atan2(vecP.y, vecP.x);

    if (bulge < 0) {
        std::swap(angleStart, angleEnd);
    }

    if (angleStart < 0) angleStart += 2 * M_PI;
    if (angleEnd < 0) angleEnd += 2 * M_PI;
    if (angleP < 0) angleP += 2 * M_PI;

    if (angleStart > angleEnd) {
        return angleP >= angleStart || angleP <= angleEnd;
    } else {
        return angleP >= angleStart && angleP <= angleEnd;
    }
}

// 점이 선분 또는 아크 위에 있는지 확인
bool isPointOnPolylineEdge(const AcGePoint2d& point, const AcGePoint2d& v1, const AcGePoint2d& v2, double bulge) {
    if (std::abs(bulge) > 1e-9) {
        // 아크 처리
        return isPointOnArc(point, v1, v2, bulge);
    } else {
        // 선분 처리
        return isPointOnLineSegment(point, v1, v2);
    }
}

// Ray-Casting Algorithm으로 점이 다각형 내부에 있는지 확인 (곡선 포함)
bool isPointInsidePolyline(const AcGePoint2d& point, const AcDbPolyline* polyline) {
    if (!polyline || !polyline->isClosed()) {
        acutPrintf(_T("The polyline is not closed.\n"));
        return false;
    }

    int numVerts = polyline->numVerts();
    bool inside = false;

    for (int i = 0; i < numVerts; ++i) {
        AcGePoint2d v1, v2;
        polyline->getPointAt(i, v1);
        polyline->getPointAt((i + 1) % numVerts, v2);

        // 불지 값 가져오기
        double bulge = polyline->bulgeAt(i);

        // Ray-Casting Algorithm (선분 또는 아크)
        if (((v1.y > point.y) != (v2.y > point.y)) &&
            (point.x < (v2.x - v1.x) * (point.y - v1.y) / (v2.y - v1.y) + v1.x)) {
            inside = !inside;
        }

        // 점이 선분 또는 아크 위에 있는지 확인
        if (isPointOnPolylineEdge(point, v1, v2, bulge)) {
            return true; // 점이 폴리선의 경계 위에 있음
        }
    }

    return inside;
}

// 폴리선 내부의 모든 점 탐색
std::vector<AcGePoint2d> detectPointsInsidePolyline(const AcDbPolyline* polyline, double step = 1.0) {
    std::vector<AcGePoint2d> points;

    if (!polyline || !polyline->isClosed()) {
        acutPrintf(_T("The polyline is not closed.\n"));
        return points;
    }

    // 폴리선의 경계 영역 가져오기
    AcGePoint2d minPoint, maxPoint;
    polyline->getPointAt(0, minPoint);
    maxPoint = minPoint;

    for (int i = 1; i < polyline->numVerts(); ++i) {
        AcGePoint2d point;
        polyline->getPointAt(i, point);
        minPoint.x = std::min(minPoint.x, point.x);
        minPoint.y = std::min(minPoint.y, point.y);
        maxPoint.x = std::max(maxPoint.x, point.x);
        maxPoint.y = std::max(maxPoint.y, point.y);
    }

    // 경계 영역 내에서 점 탐색
    for (double x = minPoint.x; x <= maxPoint.x; x += step) {
        for (double y = minPoint.y; y <= maxPoint.y; y += step) {
            AcGePoint2d testPoint(x, y);

            // 점이 폴리선 내부에 있는지 확인
            if (isPointInsidePolyline(testPoint, polyline)) {
                points.push_back(testPoint);
            }
        }
    }

    return points;
}

// 명령어 함수
void detectPolylineRegionCommand() {
    AcDbObjectId objId;
    ads_name en;

    // 사용자에게 폴리선 선택 요청
    if (acedEntSel(_T("\nSelect a closed polyline: "), en, nullptr) != RTNORM) {
        acutPrintf(_T("No object selected.\n"));
        return;
    }

    // 선택된 엔티티의 객체 ID 가져오기
    if (acdbGetObjectId(objId, en) != Acad::eOk) {
        acutPrintf(_T("Failed to get object ID.\n"));
        return;
    }

    AcDbPolyline* polyline = nullptr;

    // 객체 열기
    if (acdbOpenObject(polyline, objId, AcDb::kForRead) != Acad::eOk) {
        acutPrintf(_T("The selected object is not a polyline.\n"));
        return;
    }

    // 폴리선 내부 점 탐색
    double step = 1.0; // 점 탐색 간격
    std::vector<AcGePoint2d> points = detectPointsInsidePolyline(polyline, step);

    acutPrintf(_T("Detected %d points inside the polyline.\n"), points.size());

    // 내부 점 출력 (샘플)
    for (size_t i = 0; i < points.size() && i < 10; ++i) {
        acutPrintf(_T("Point: (%.2f, %.2f)\n"), points[i].x, points[i].y);
    }

    // 객체 닫기
    polyline->close();
}

// 명령어 등록
void initApp() {
    acedRegCmds->addCommand(_T("POLYLINE_REGION_DETECT"), _T("DetectPolylineRegion"), _T("DetectPolylineRegion"), ACRX_CMD_MODAL, detectPolylineRegionCommand);
}

void unloadApp() {
    acedRegCmds->removeGroup(_T("POLYLINE_REGION_DETECT"));
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt) {
    switch (msg) {
        case AcRx::kInitAppMsg:
            acrxUnlockApplication(pkt);
            acrxRegisterAppMDIAware(pkt);
            initApp();
            break;
        case AcRx::kUnloadAppMsg:
            unloadApp();
            break;
    }
    return AcRx::kRetOK;
}
