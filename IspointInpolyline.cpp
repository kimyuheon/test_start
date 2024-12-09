#include <dbents.h>
#include <acutads.h>
#include <geassign.h>
#include <cmath>

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
    // 중심과 반지름 계산
    double theta = 4.0 * atan(bulge);
    double chordLength = start.distanceTo(end);
    double radius = std::abs(chordLength / (2.0 * sin(theta / 2.0)));
    AcGeVector2d chordVec = end - start;
    AcGeVector2d orthoVec(-chordVec.y, chordVec.x);
    orthoVec.normalize();
    AcGePoint2d midPoint = (start + end) / 2.0;
    AcGePoint2d center = midPoint + orthoVec * (radius * (bulge > 0 ? 1 : -1));

    // 점이 아크 경로에 있는지 확인
    double distToCenter = p.distanceTo(center);
    if (std::abs(distToCenter - radius) > 1e-9) {
        return false; // 점이 아크의 원에 있지 않음
    }

    // 점이 아크 범위 내에 있는지 확인
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

// Ray-Casting Algorithm으로 점이 다각형 내부에 있는지 확인
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

        if (std::abs(bulge) > 1e-9) {
            // 아크 처리
            if (isPointOnArc(point, v1, v2, bulge)) {
                return true; // 점이 아크 위에 있음
            }
        } else {
            // 선분 처리
            if (isPointOnLineSegment(point, v1, v2)) {
                return true; // 점이 선분 위에 있음
            }
        }

        // Ray-Casting Algorithm
        if (((v1.y > point.y) != (v2.y > point.y)) && 
            (point.x < (v2.x - v1.x) * (point.y - v1.y) / (v2.y - v1.y) + v1.x)) {
            inside = !inside;
        }
    }

    return inside;
}

// 명령어 함수
void checkPointInPolylineCommand() {
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

    // 사용자에게 좌표 입력 요청
    ads_point userPoint;
    if (acedGetPoint(nullptr, _T("\nSpecify a point to check: "), userPoint) != RTNORM) {
        acutPrintf(_T("Invalid point.\n"));
        polyline->close();
        return;
    }

    // 3D 좌표를 2D로 변환
    AcGePoint2d point(userPoint[X], userPoint[Y]);

    // 점이 폴리선 내부에 있는지 확인
    if (isPointInsidePolyline(point, polyline)) {
        acutPrintf(_T("The point is inside the polyline.\n"));
    } else {
        acutPrintf(_T("The point is outside the polyline.\n"));
    }

    // 객체 닫기
    polyline->close();
}

// 명령어 등록
void initApp() {
    acedRegCmds->addCommand(_T("POINT_INSIDE_ARC_POLYLINE"), _T("CheckPointInArcPolyline"), _T("CheckPointInArcPolyline"), ACRX_CMD_MODAL, checkPointInPolylineCommand);
}

void unloadApp() {
    acedRegCmds->removeGroup(_T("POINT_INSIDE_ARC_POLYLINE"));
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
