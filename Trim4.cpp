AcDbPolyline* trimPolylineWithTwoSelectedPoints(AcDbPolyline* pPolyline, const AcGePoint3d& point1, const AcGePoint3d& point2)
{
    if (!pPolyline) {
        acutPrintf(_T("Invalid polyline.\n"));
        return nullptr;
    }

    AcDbPolyline* pNewPolyline = new AcDbPolyline();

    bool started = false; // Trim 작업 시작 여부
    bool finished = false; // Trim 작업 완료 여부

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
        if (!started && (segmentStart.distanceTo(point1) < 1e-6 || segmentEnd.distanceTo(point1) < 1e-6)) {
            started = true;
        }

        if (started && !finished) {
            if (bulge != 0.0) {
                // 호 세그먼트 처리
                AcGePoint2d trimStart = (segmentStart.distanceTo(point1) < 1e-6) ? AcGePoint2d(point1.x, point1.y) : start2D;
                AcGePoint2d trimEnd = (segmentEnd.distanceTo(point2) < 1e-6) ? AcGePoint2d(point2.x, point2.y) : end2D;
                trimArcSegment(start2D, end2D, bulge, trimStart, trimEnd, pNewPolyline);
            } else {
                // 직선 세그먼트 처리
                if (segmentStart.distanceTo(point1) < 1e-6 || segmentEnd.distanceTo(point2) < 1e-6) {
                    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), AcGePoint2d(segmentStart.x, segmentStart.y));
                    pNewPolyline->addVertexAt(pNewPolyline->numVerts(), AcGePoint2d(segmentEnd.x, segmentEnd.y));
                }
            }

            // Trim 작업 완료 여부 확인
            if (segmentEnd.distanceTo(point2) < 1e-6) {
                finished = true;
            }
        }
    }

    return pNewPolyline;
}
