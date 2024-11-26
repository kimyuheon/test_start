//
// (C) Copyright 1998-2006 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "Utils.h"




resbuf* assoc(resbuf *ebuf, int restype)
{
   while((ebuf != NULL) && (ebuf->restype != restype))
      ebuf = ebuf->rbnext;
   return ebuf;
}

/****************************************************************************
**
**	Utils::objIdToEname
**
**	**jma
**
*************************************/

Acad::ErrorStatus
Utils::objIdToEname(const AcDbObjectId& objId, ads_name& ent)
{
    Acad::ErrorStatus es = acdbGetAdsName(ent, objId);
    ASSERT(es == Acad::eOk);
    if (es == Acad::eOk)
        Utils::rxErrorMsg(es);

    return es;
}

/****************************************************************************
**
**	Utils::enameToObjId
**
**	**jma
**
*************************************/

Acad::ErrorStatus
Utils::enameToObjId(ads_name ent, AcDbObjectId& objId)
{
    Acad::ErrorStatus es = acdbGetObjectId(objId, ent);
    ASSERT(es == Acad::eOk);
    if (es != Acad::eOk)
        Utils::rxErrorMsg(es);
    
    return es;
}

/***********************************************************************
**
**	Utils::handleStrToObjId
**		Convert a handle string to an objectId.
**
**	**jma
**
****************************************/

Acad::ErrorStatus
Utils::handleStrToObjId(AcDbDatabase* db, LPCTSTR handleStr, AcDbObjectId& objId, bool speak)
{
    ASSERT(handleStr != NULL);
	ASSERT(db != NULL);

    AcDbHandle objHandle(handleStr);
    Acad::ErrorStatus es;

    es = db->getAcDbObjectId(objId, false, objHandle);
    if (es != Acad::eOk) {
        if (speak) {
            acutPrintf(_T("\nERROR: Could not translate handle to objectId (%s)"), Utils::rxErrorStr(es));
        }
        return Acad::eInvalidInput;
    }
    else
        return Acad::eOk;
}

/***********************************************************************
**
**	Utils::objIdToHandleStr
**
**	**jma
**
****************************************/

LPCTSTR
Utils::objIdToHandleStr(const AcDbObjectId& objId, CString& str)
{
    AcDbObject* obj;
    Acad::ErrorStatus es;
    es = acdbOpenAcDbObject(obj, objId, AcDb::kForRead);
    if (es != Acad::eOk)
        str = ACDB_NULL_HANDLE;
    else {
        Utils::objToHandleStr(obj, str);
        obj->close();
    }
    return str;
}

LPCTSTR
Utils::objToHandleStr(const AcDbObject* obj, CString& str)
{
    ASSERT(obj != NULL);

    AcDbHandle handle;

    obj->getAcDbHandle(handle);
    TCHAR tmpStr[256];
    handle.getIntoAsciiBuffer(tmpStr);
    str = tmpStr;
    return str;
}

/****************************************************************************
**
**  ArxUtils::rxErrorMsg
**
**  **jma
**
*************************************/

void
Utils::rxErrorMsg(Acad::ErrorStatus msgId)
{
    if (msgId != Acad::eOk)
        acutPrintf(_T("\nARX ERROR: %s"), acadErrorStatusText(msgId));
}

/****************************************************************************
**
**  ArxUtils::rxErrorAlert
**
**  **jma
**
*************************************/

void
Utils::rxErrorAlert(Acad::ErrorStatus msgId)
{
    CString str;

    if (msgId != Acad::eOk){
        str.Format(_T("ARX ERROR: %s"), acadErrorStatusText(msgId));
       
    }
}

/****************************************************************************
**
**  ArxUtils::rxErrorStr
**
**  **jma
**
*************************************/

const TCHAR*
Utils::rxErrorStr(Acad::ErrorStatus msgId)
{
    return acadErrorStatusText(msgId);
}

/****************************************************************************
**
**  ArxUtils::stubMsg
**
**  **jma
**
*************************************/

void
Utils::stubMsg(LPCTSTR msg)
{
    acutPrintf(_T("\nNot Implemented Yet: %s"), msg);
}


/****************************************************************************
**
**  ArxUtils::addToCurrentSpace
**		for convienence sake since this is usually called with the current db
**
**  **jma
**
*************************************/

Acad::ErrorStatus
Utils::addToCurrentSpace(AcDbEntity* newEnt)
{
	return addToCurrentSpace(newEnt, acdbHostApplicationServices()->workingDatabase());
}

/****************************************************************************
**
**  ArxUtils::addToCurrentSpace
**      add a new entity to the current space block record.
**
**  **jma
**
*************************************/

Acad::ErrorStatus
Utils::addToCurrentSpace(AcDbEntity* newEnt, AcDbDatabase* db)
{
    ASSERT(newEnt != NULL);
	ASSERT(db != NULL);

    AcDbBlockTableRecord* blkRec = openCurrentSpaceBlock(AcDb::kForWrite, db);

    ASSERT(blkRec != NULL);

    if (blkRec == NULL)
        return Acad::eInvalidInput;

        // append new entity to current space block
    Acad::ErrorStatus es = blkRec->appendAcDbEntity(newEnt);
    if (es != Acad::eOk) {
        acutPrintf(_T("\nERROR: could not add entity to current space (%s)"), Utils::rxErrorStr(es));
    }

    blkRec->close();
    return es;
}





/****************************************************************************
**
**  ArxUtils::addToCurrentSpaceAndClose
**      add a new entity to the current space block record.
**
**  NOTE:  this function accepts responsibility for the new entity.
**  If appending to the blkRec fails, it will delete it and if successful
**  it will close it.
**
**  **jma
**
*************************************/

Acad::ErrorStatus
Utils::addToCurrentSpaceAndClose(AcDbEntity* newEnt, AcDbDatabase* db)
{
    Acad::ErrorStatus es;

    es = addToCurrentSpace(newEnt, db);
    if (es == Acad::eOk)
        newEnt->close();
    else
        delete newEnt;

    return es;
}

/****************************************************************************
**
**  ArxUtils::addToCurrentSpaceAndClose
**		for convienence sake since this is usually called with the current db
**
**  **jma
**
*************************************/

Acad::ErrorStatus
Utils::addToCurrentSpaceAndClose(AcDbEntity* newEnt)
{
	return addToCurrentSpaceAndClose(newEnt, acdbHostApplicationServices()->workingDatabase());
}

/****************************************************************************
**
**  ArxUtils::isOnLockedLayer
**
**  **jma
**
*************************************/

bool
Utils::isOnLockedLayer(AcDbObjectId& id, bool printMsg)
{
    AcDbEntity* ent;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(ent, id, AcDb::kForRead);
    if (es == Acad::eOk) {
        bool result = isOnLockedLayer(ent, printMsg);
        ent->close();
        return result;
    }

    return true;  // by default we "lock" data
}

/****************************************************************************
**
**  ArxUtils::isOnLockedLayer
**
**  **jma
**
*************************************/

bool
Utils::isOnLockedLayer(AcDbEntity* ent, bool printMsg)
{
    AcDbObject* obj;
    AcDbLayerTableRecord* layer;
    bool isLocked = false;
    Acad::ErrorStatus es;

    es = acdbOpenAcDbObject(obj, ent->layerId(), AcDb::kForRead);
    if (es == Acad::eOk) {
        layer = AcDbLayerTableRecord::cast(obj);
        if (layer)
            isLocked = layer->isLocked();
        else {
            ASSERT(0);
        }
        obj->close();
    }
    else {
        ASSERT(0);
        Utils::rxErrorMsg(es);
    }

    if (isLocked && printMsg) {
        acutPrintf(_T("\n선택한 객체가 잠긴 레이어 상태입니다."));
    }

    return isLocked;
}


void Utils::setLayerLock(AcDbObjectId& idLayer, bool islocked)
{
    AcDbLayerTableRecordPointer ptLayer(idLayer, AcDb::kForRead); 
    if (ptLayer.openStatus() == Acad::eOk) {
       ptLayer->setIsLocked(islocked);
    }    
}


/****************************************************************************
**
**	Utils::cloneAndXformObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
Utils::cloneAndXformObjects(AcDbDatabase* db, const AcDbObjectId& entToClone,
                            const AcDbObjectId& ownerBlockId,
                            const AcGeMatrix3d& xformMat, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbObjectIdArray objIdList;
    objIdList.append(entToClone);

    return cloneAndXformObjects(db, objIdList, ownerBlockId, xformMat, debugSpeak);
}

/****************************************************************************
**
**	Utils::cloneAndXformObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
Utils::cloneAndXformObjects(AcDbDatabase* db, const AcDbObjectIdArray& entsToClone,
                        const AcDbObjectId& ownerBlockId,
                        const AcGeMatrix3d& xformMat, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbIdMapping idMap;
    Acad::ErrorStatus es = db->deepCloneObjects(
                const_cast<AcDbObjectIdArray&>(entsToClone),
                const_cast<AcDbObjectId&>(ownerBlockId), idMap);

    if (es != Acad::eOk) {
        Utils::rxErrorMsg(es);
        return es;
    }

    AcDbEntity* clonedEnt;
    AcDbIdPair idPair;
    AcDbIdMappingIter iter(idMap);
    for (iter.start(); !iter.done(); iter.next()) {
        if (!iter.getMap(idPair))
            return Acad::eInvalidInput;

        if (idPair.isCloned()) {
            es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
            if (es == Acad::eOk) {
                if (idPair.isPrimary()) {
                    if (debugSpeak)
                        acutPrintf(_T("\nCloned And Transformed: %s"), Utils::objToClassStr(clonedEnt));

                    clonedEnt->transformBy(xformMat);
                }
                else if (debugSpeak)
                    acutPrintf(_T("\nCloned: %s"), Utils::objToClassStr(clonedEnt));

                clonedEnt->close();
            }
            else
                Utils::rxErrorMsg(es);
        }
    }
    return Acad::eOk;
}

LPCTSTR
Utils::objToClassStr(const AcRxObject* obj)
{
    ASSERT(obj != NULL);

    AcRxClass* rxClass = obj->isA();
    if (rxClass == NULL) {
        ASSERT(0);
        return _T("*Unknown*");
    }

    return obj->isA()->name();
}

AcDbBlockTableRecord*
Utils::openCurrentSpaceBlock(AcDb::OpenMode mode, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcDbBlockTableRecord* blkRec;

    Acad::ErrorStatus es = acdbOpenObject(blkRec, db->currentSpaceId(), mode);
    if (es != Acad::eOk)
        return NULL;
    else
        return blkRec;
}

/****************************************************************************
**
**	Utils::cloneObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
Utils::cloneObjects(AcDbDatabase* db, const AcDbObjectId& entToClone,
                        const AcDbObjectId& ownerBlockId, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbObjectIdArray objIdList;
    objIdList.append(entToClone);

    return cloneObjects(db, objIdList, ownerBlockId, debugSpeak);
}

/****************************************************************************
**
**	Utils::cloneObjects
**
**	**jma
**
*************************************/

Acad::ErrorStatus
Utils::cloneObjects(AcDbDatabase* db, const AcDbObjectIdArray& entsToClone,
                        const AcDbObjectId& ownerBlockId, bool debugSpeak)
{
	ASSERT(db != NULL);

    AcDbIdMapping idMap;
    Acad::ErrorStatus es = db->deepCloneObjects(
                const_cast<AcDbObjectIdArray&>(entsToClone),
                const_cast<AcDbObjectId&>(ownerBlockId), idMap);

    if (es != Acad::eOk) {
        Utils::rxErrorMsg(es);
        return es;
    }

    if (debugSpeak) {
        AcDbEntity* clonedEnt;
        AcDbIdPair idPair;
        AcDbIdMappingIter iter(idMap);
        for (iter.start(); !iter.done(); iter.next()) {
            if (!iter.getMap(idPair))
                return Acad::eInvalidInput;

            if (idPair.isCloned()) {
                es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
                if (es == Acad::eOk) {
                    acutPrintf(_T("\nCloned: %s"), Utils::objToClassStr(clonedEnt));
                    clonedEnt->close();
                }
                else
                    Utils::rxErrorMsg(es);
            }
        }
    }
    return Acad::eOk;
}

/****************************************************************************
**
**	Utils::plineHasWidth
**
**	**jma
**
*************************************/

bool
Utils::plineHasWidth(const AcDb2dPolyline* pline)
{
    ASSERT(pline != NULL);

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return false;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
                if ((vertex->startWidth() != 0.0) || (vertex->endWidth() != 0.0)) {
                    vertex->close();
                    delete vertexIter;
                    return true;        // bail out, we found what we needed
                }
            }
            vertex->close();
        }
    }
    delete vertexIter;
    return false;
}

/****************************************************************************
**
**	Utils::plineHasWidth
**
**	**jma
**
*************************************/

bool
Utils::plineHasWidth(const AcDbPolyline* pline)
{
    double startWidth, endWidth;
    Acad::ErrorStatus es;

    for (unsigned int i=0; i<pline->numVerts(); i++) {
        es = pline->getWidthsAt(i, startWidth, endWidth);
        if (es == Acad::eOk) {
            if ((startWidth > 0.0) || (endWidth > 0.0)) {
                return true;
            }
        }
    }
    return false;
}

/****************************************************************************
**
**  Utils::plineHasZeroLengthSegments
**
**  **jma
**
*************************************/

bool
Utils::plineHasZeroLengthSegments(const AcDbPolyline* pline)
{
    AcDbPolyline::SegType segType;

    int numSegs = Utils::numSegments(pline);

    for (int i=0; i<numSegs; i++) {
        segType = pline->segType(i);
        if ((segType == AcDbPolyline::kCoincident) ||
            (segType == AcDbPolyline::kPoint) ||
            (segType == AcDbPolyline::kEmpty))
            return true;
    }

    return false;
}

/****************************************************************************
**
**	Utils::numSegments
**		get the number of segments in a polyline.
**
**	**jma
**
*************************************/

int
Utils::numSegments(const AcDbPolyline* pline)
{
    ASSERT(pline != NULL);

    int numSegs;
    if (pline->isClosed())
        numSegs = pline->numVerts();
    else
        numSegs = pline->numVerts() - 1;

    if (numSegs < 0)
        numSegs = 0;

    return numSegs;
}

/****************************************************************************
**
**	Utils::collectVertices
**
**	**jma
**
*************************************/

void
Utils::collectVertices(const AcDb2dPolyline* pline, AcGePoint3dArray& pts,
            AcGeDoubleArray& bulges, bool asWcsPts)
{
    ASSERT(pline != NULL);
    ASSERT(pts.isEmpty() && bulges.isEmpty());

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
                if (asWcsPts)
                    pts.append(pline->vertexPosition(*vertex));        // returns WCS
                else
                    pts.append(vertex->position());                    // returns ECS
                bulges.append(vertex->bulge());
            }
            vertex->close();
        }
    }
    delete vertexIter;

    ASSERT(pts.isEmpty() == Adesk::kFalse);

    if (pline->isClosed()) {
        AcGePoint3d tmpPt = pts[0];        // used to be a bug in dynamic arrays (not sure if its still there??)
        pts.append(tmpPt);
        bulges.append(0.0);
    }
}

/****************************************************************************
**
**	Utils::collectVertices
**
**	**csa
**
*************************************/

void
Utils::collectVertices(const AcDb2dPolyline* pline, AcGePoint3dArray& pts,
                        AcDbIntArray& types, AcGeDoubleArray& bulges,
                        AcGeDoubleArray& startWidths, 
                        AcGeDoubleArray& endWidths, bool& hasWidth)
{
    ASSERT(pline != NULL);
    ASSERT(pts.isEmpty() && bulges.isEmpty());

    hasWidth = false;

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
                pts.append(pline->vertexPosition(*vertex));        // returns WCS
                bulges.append(vertex->bulge());
                startWidths.append(vertex->startWidth());
                endWidths.append(vertex->endWidth());

                if (vertex->startWidth() || vertex->endWidth())
                    hasWidth = true;
                types.append(vertex->vertexType());
            }
            vertex->close();
        }
    }
    delete vertexIter;

    ASSERT(pts.isEmpty() == false);

    if (pline->isClosed()) {
        AcGePoint3d tmpPt = pts[0];        // used to be a bug in dynamic arrays (not sure if its still there??)
        pts.append(tmpPt);

        bulges.append(0.0);

        int tmpType = types[0];
        types.append(tmpType);

        double tmpWidth = startWidths[0];
        startWidths.append(tmpWidth);

        tmpWidth = endWidths[0];
        endWidths.append(tmpWidth);
    }
}

/****************************************************************************
**
**	Utils::collectVertices
**
**	**jma
**
*************************************/

void
Utils::collectVertices(const AcDb3dPolyline* pline, AcGePoint3dArray& pts)
{
    ASSERT(pline != NULL);
    ASSERT(pts.isEmpty());

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb3dPolylineVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
        if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k3dControlVertex)
                pts.append(vertex->position());
            vertex->close();
        }
    }
    delete vertexIter;

    ASSERT(pts.isEmpty() == false);

    if (pline->isClosed()) {
        AcGePoint3d tmpPt = pts[0];        // used to be a bug in dynamic arrays (not sure if its still there??)
        pts.append(tmpPt);
    }
}

/****************************************************************************
**
**	Utils::collectVertices
**
**	**jma
**
*************************************/

void
Utils::collectVertices(const AcDbPolyFaceMesh* pface, AcDbObjectIdArray& vfaces, AcGePoint3dArray& pts)
{
    AcDbObjectIterator*  vertexIter = pface->vertexIterator();
    if (vertexIter == NULL)
        return;

    AcDbFaceRecord* vface;
    AcDbPolyFaceMeshVertex* pfaceVertex;
    AcDbObject* obj;

        // walk through and seperate vfaces and vertices into two
        // seperate arrays
    Acad::ErrorStatus es;
    for (; !vertexIter->done(); vertexIter->step()) {
        es = acdbOpenObject(obj, vertexIter->objectId(), AcDb::kForRead);
        if (es == Acad::eOk) {
            if ((vface = AcDbFaceRecord::cast(obj)) != NULL)
                vfaces.append(obj->objectId());
            else if ((pfaceVertex = AcDbPolyFaceMeshVertex::cast(obj)) != NULL)
                pts.append(pfaceVertex->position());
            else {
                ASSERT(0);
            }
            obj->close();
        }
        else
            Utils::rxErrorMsg(es);
    }
    delete vertexIter;
}

/****************************************************************************
**
**	Utils::collectVertices
**		grab the vertices for a polygon mesh and insert close vertices if
**	appropriate.  The new mSize and nSize counts will be returned in mSize/nSize
**
**	**jma
**
*************************************/

bool
Utils::collectVertices(const AcDbPolygonMesh* pmesh, AcGePoint3dArray& pts,
                            int& mSize, int& nSize)
{
    AcDbObjectIterator*  vertexIter = pmesh->vertexIterator();
    if (vertexIter == NULL)
        return false;

    Acad::ErrorStatus es;
    AcDbPolygonMeshVertex* pVertex;
    AcGePoint3d tmpPt;

    if (pmesh->polyMeshType() == AcDb::kSimpleMesh) {
        mSize = pmesh->mSize();
        nSize = pmesh->nSize();
    }
    else {
        mSize = pmesh->mSurfaceDensity();
        nSize = pmesh->nSurfaceDensity();
    }

    int nCount = 0;
    int totalCount = 0;
    for (; !vertexIter->done(); vertexIter->step()) {
        es = acdbOpenObject(pVertex, vertexIter->objectId(), AcDb::kForRead);
        if (es != Acad::eOk) {
            Utils::rxErrorMsg(es);
            delete vertexIter;
            return false;
        }
        pts.append(pVertex->position());
        nCount++;
        totalCount++;
        if (nCount == nSize) {
            if (pmesh->isNClosed()) {
                tmpPt = pts[totalCount - nSize];        // work around bug by making temp copy (not sure if still a bug??)
                pts.append(tmpPt);
                totalCount++;
            }
            nCount = 0;
        }
    }
    delete vertexIter;

    if (pmesh->isNClosed())
        nSize++;
    if (pmesh->isMClosed()) {
        mSize++;
        for (int i=0; i<nSize; i++) {
            tmpPt = pts[i];        // see note above
            pts.append(tmpPt);    
        }
    }

    return true;
}

/****************************************************************************
**
**	Utils::isPaperSpace
**
**	**jma
**
*************************************/

bool
Utils::isPaperSpace(AcDbDatabase* db)
{
	if (db->tilemode() == 1)
		return false;

	if (db->paperSpaceVportId() == acedGetCurViewportObjectId())
		return true;
	else
		return false;
}

/****************************************************************************
**
**	Utils::transformToWcs
**
**	**jma
**
*************************************/

void
Utils::transformToWcs(AcDbEntity* ent, AcDbDatabase* db)
{
    ASSERT(ent->isWriteEnabled());
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getUcsToWcsMatrix(m, db);
    Acad::ErrorStatus es = ent->transformBy(m);

    ASSERT(es == Acad::eOk);
}

/****************************************************************************
**
**	Utils::transformToUcs
**
**	**jma
**
*************************************/

void
Utils::transformToUcs(AcDbEntity* ent, AcDbDatabase* db)
{
    ASSERT(ent->isWriteEnabled());
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getWcsToUcsMatrix(m, db);
    Acad::ErrorStatus es = ent->transformBy(m);

    ASSERT(es == Acad::eOk);
}

/****************************************************************************
**
**	Utils::getUcsXAxis
**		returns current UCS X Axis, even if currently the paperSpace viewport
**
**	**jma
**
*************************************/

AcGeVector3d
Utils::getUcsXAxis(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		return xDir;
	}
	else {
		ASSERT(0);
		return AcGeVector3d::kXAxis;
	}
}

/****************************************************************************
**
**	Utils::getUcsYAxis
**		returns current UCS Y Axis, even if currently the paperSpace viewport
**
**	**jma
**
*************************************/

AcGeVector3d
Utils::getUcsYAxis(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		return yDir;
	}
	else {
		ASSERT(0);
		return AcGeVector3d::kYAxis;
	}
}

/****************************************************************************
**
**	Utils::getUcsZAxis
**
**	**jma
**
*************************************/

AcGeVector3d
Utils::getUcsZAxis(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		return zDir;
	}
	else {
		ASSERT(0);
		return AcGeVector3d::kZAxis;
	}
}

/****************************************************************************
**
**	Utils::getUcsPlane
**
**	**jma
**
*************************************/

AcGePlane
Utils::getUcsPlane(AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d m;

	if (acdbUcsMatrix(m, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		m.getCoordSystem(origin, xDir, yDir, zDir);
		AcGePlane ucsPlane(origin, xDir, yDir);
		return ucsPlane;
	}
	else {
		ASSERT(0);
        AcGePlane ucsPlane(AcGePoint3d::kOrigin, AcGeVector3d::kIdentity);
        return ucsPlane;
	}
}

/****************************************************************************
**
**    Utils::getUcsToWcsMatrix
**
**    **jma
**
*************************************/

void
Utils::getUcsToWcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db)
{
	ASSERT(db != NULL);

	if (!acdbUcsMatrix(m, db)) {
		m.setToIdentity();
		ASSERT(0);
	}
}

/****************************************************************************
**
**	Utils::getWcsToUcsMatrix
**
**	**jma
**
*************************************/

void
Utils::getWcsToUcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db)
{
    getUcsToWcsMatrix(m, db);
    m.invert();
}

/****************************************************************************
**
**	Utils::getUcsToWcsOriginMatrix
**		move an object form one coordinate system to another.
**
**	NOTE:    The basePt must be passed in as a WCS point!
**
**	**jma
**
*************************************/

void
Utils::getUcsToWcsOriginMatrix(AcGeMatrix3d& m,
                        const AcGePoint3d& wcsBasePt, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d tmpMat;

	if (acdbUcsMatrix(tmpMat, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		tmpMat.getCoordSystem(origin, xDir, yDir, zDir);
		origin += wcsBasePt.asVector();

	    m.setToAlignCoordSys(origin, xDir, yDir, zDir,
            AcGePoint3d::kOrigin, AcGeVector3d::kXAxis,
            AcGeVector3d::kYAxis, AcGeVector3d::kZAxis);
	}
	else {
		ASSERT(0);
        m = AcGeMatrix3d::kIdentity;
	}
}

/****************************************************************************
**
**	Utils::getEcsPlane
**		This is the ECS plane for an AutoCAD entity. It can then be used to
**	calculate angles relative to the ECS plane, for instance, angles of 
**	AcDbArc, AcDbBlockReference
**
**	**jma
**
*************************************/

void
Utils::getEcsPlane(const AcGeVector3d& entNormal, AcGePlane& ecsPlane)
{
    AcGeMatrix3d ecsMat;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, entNormal, ecsMat);

    AcGePoint3d origin;
    AcGeVector3d xAxis, yAxis, zAxis;
    ecsMat.getCoordSystem(origin, xAxis, yAxis, zAxis);
    ecsPlane.set(origin, xAxis, yAxis);
}

/****************************************************************************
**
**	Utils::getEcsPlane
**	same as above, but also return the XAxis of ECS, and use the origin
**	passed in for the plane
**
**	**jma
**
*************************************/

void
Utils::getEcsPlane(const AcGePoint3d& origin,
                        const AcGeVector3d& entNormal, AcGePlane& ecsPlane,
                        AcGeVector3d& ecsXAxis)
{
    AcGeMatrix3d ecsMat;
    getEcsToWcsMatrix(origin, entNormal, ecsMat);

    AcGePoint3d tmpOrigin;
    AcGeVector3d yAxis, zAxis;
    ecsMat.getCoordSystem(tmpOrigin, ecsXAxis, yAxis, zAxis);
    ecsPlane.set(tmpOrigin, ecsXAxis, yAxis);
}

/****************************************************************************
**
**	Utils::getEcsXAxis
**
**	**jma
**
*************************************/

void
Utils::getEcsXAxis(const AcGeVector3d& ecsZAxis, AcGeVector3d& xAxis)
{
    AcGeMatrix3d arbMat;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, ecsZAxis, arbMat);

    AcGePoint3d origin;
    AcGeVector3d yAxis, zAxis;

    arbMat.getCoordSystem(origin, xAxis, yAxis, zAxis);    // return xAxis;
}

/****************************************************************************
**
**	Utils::getEcsToWcsMatrix
**		run AutoCAD's arbitrary matrix algorithm for ECS entities
**
**	**jma
**
*************************************/

void
Utils::getEcsToWcsMatrix(const AcGePoint3d& origin,
                    const AcGeVector3d& zAxis, AcGeMatrix3d& mat)
{
    const double kArbBound = 0.015625;         //  1/64th

        // short circuit if in WCS already
    if (zAxis == AcGeVector3d::kZAxis) {
        mat.setToIdentity();
        return;
    }

    AcGeVector3d xAxis, yAxis;

    ASSERT(zAxis.isUnitLength());

    if ((fabs(zAxis.x) < kArbBound) && (fabs(zAxis.y) < kArbBound))
        xAxis = AcGeVector3d::kYAxis.crossProduct(zAxis);
    else
        xAxis = AcGeVector3d::kZAxis.crossProduct(zAxis);

    xAxis.normalize();
    yAxis = zAxis.crossProduct(xAxis);
    yAxis.normalize();

    mat.setCoordSystem(AcGePoint3d::kOrigin, xAxis, yAxis, zAxis);
}

/****************************************************************************
**
**	Utils::wcsToUcs
**
**	**jma
**
*************************************/

AcGePoint3d
Utils::wcsToUcs(const AcGePoint3d& pt)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getWcsToUcsMatrix(m, db);
    return m * pt;
}

/****************************************************************************
**
**	Utils::wcsToUcs
**
**	**jma
**
*************************************/

AcGeVector3d
Utils::wcsToUcs(const AcGeVector3d& vec)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getWcsToUcsMatrix(m, db);

    AcGeVector3d newv = vec;
    newv.transformBy(m);

    return newv;
}

/****************************************************************************
**
**	Utils::wcsToUcs
**
**	**jma
**
*************************************/

void
Utils::wcsToUcs(AcGePoint3dArray& ptArray)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getWcsToUcsMatrix(m, db);

    int len = ptArray.length();
    for (int i=0; i<len; i++)
        ptArray[i] = m * ptArray[i];
}

/****************************************************************************
**
**	Utils::ucsToWcs
**
**	**jma
**
*************************************/

AcGePoint3d
Utils::ucsToWcs(const AcGePoint3d& pt)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getUcsToWcsMatrix(m, db);
    return m * pt;
}

/****************************************************************************
**
**	Utils::ucsToWcs
**
**	**jma
**
*************************************/

AcGeVector3d
Utils::ucsToWcs(const AcGeVector3d& vec)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;

    getUcsToWcsMatrix(m, db);

    AcGeVector3d newv = vec;
    newv.transformBy(m);

    return newv;
}

/****************************************************************************
**
**	Utils::ucsToWcs
**
**	**jma
**
*************************************/

void
Utils::ucsToWcs(AcGePoint3dArray& ptArray)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

    AcGeMatrix3d m;
    getUcsToWcsMatrix(m, db);

    int len = ptArray.length();
    for (int i=0; i<len; i++)
        ptArray[i] = m * ptArray[i];
}

/************************************************************************
**
**	Utils::ecsToWcs
**
**	**jma
**
***************************************/

AcGePoint3d
Utils::ecsToWcs(const AcGePoint3d& pt, const AcGeVector3d& entNormal)
{
    AcGeMatrix3d m;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, entNormal, m);

    return m * pt;
}

/************************************************************************
**
**	Utils::ecsToWcs
**
**	**jma
**
***************************************/

AcGeVector3d
Utils::ecsToWcs(const AcGeVector3d& vec, const AcGeVector3d& entNormal)
{
    AcGeMatrix3d m;
    getEcsToWcsMatrix(AcGePoint3d::kOrigin, entNormal, m);

    return m * vec;
}

/************************************************************************
**
**	Utils::dcsToUcs
**
**	**jma
**
***************************************/

AcGePoint3d
Utils::dcsToUcs(const AcGePoint2d& pt)
{
    resbuf fromRb, toRb;
    ads_point newPt;

    fromRb.restype = RTSHORT;
    fromRb.resval.rint = AcDb::kCurDisplayCS;

    toRb.restype = RTSHORT;
    toRb.resval.rint = AcDb::kUserCS;

    short result = acedTrans(asDblArray(pt), &fromRb, &toRb, FALSE, newPt);
    ASSERT(result == RTNORM);

    return asPnt3d(newPt);
}

/************************************************************************
**
**	Utils::ucsToDcs
**
**	**jma
**
***************************************/

AcGePoint2d
Utils::ucsToDcs(const AcGePoint3d& pt)
{
    resbuf fromRb, toRb;
    ads_point newPt;

    fromRb.restype = RTSHORT;
    fromRb.resval.rint = AcDb::kUserCS;

    toRb.restype = RTSHORT;
    toRb.resval.rint = AcDb::kCurDisplayCS;

    short result = acedTrans(asDblArray(pt), &fromRb, &toRb, FALSE, newPt);
    ASSERT(result == RTNORM);

    return asPnt2d(newPt);
}

/************************************************************************
**
**	Utils::getAllDatabases
**
**	**jma
**
***************************************/

void
Utils::getAllDatabases(AcArray<AcDbDatabase*>& dbPtrs)
{
	//AcDbVoidPtrArray* dbs = acdbActiveDatabaseArray();
    AcArray<AcDbDatabase*> dbs = acdbActiveDatabaseArray();

	if (dbs.length() > 0) 
    {
		dbPtrs = dbs;
	}
	else 
    {
		ASSERT(0);
	}
}

/*
Utils::getAllDatabases(AcDbVoidPtrArray& dbPtrs)
{
    AcDbVoidPtrArray* dbs = acdbActiveDatabaseArray();

    if (dbs)
    {
        dbPtrs = *dbs;
    }
    else
    {
        ASSERT(0);
    }
}
*/

/************************************************************************
**
**	Utils::getAllDocuments
**
**	**jma
**
***************************************/

void
Utils::getAllDocuments(AcDbVoidPtrArray& docPtrs)
{
    AcApDocumentIterator* iter = acDocManager->newAcApDocumentIterator();
    if (iter) {
        AcApDocument* tmpDoc;
        for (; !iter->done(); iter->step()) {
            tmpDoc = iter->document();
            if (tmpDoc) {
                docPtrs.append(tmpDoc);
            }
            else {
                ASSERT(0);
            }
        }
        delete iter;
	}
}

/****************************************************************************
**
**  Utils::verifyClassType
**
**  **jma
**
*************************************/

Acad::ErrorStatus
Utils::verifyClassType(const AcDbObject* obj, AcRxClass* classType)
{
    ASSERT(classType != NULL);

    if (obj->isAProxy()) {
            // we can't really know if this is correct or not, so just leave it
        return Acad::eOk;
    }
    else {
        if (obj->isKindOf(classType))
            return Acad::eOk;
    }

    return Acad::eWrongObjectType;
}

/****************************************************************************
**
**  Utils::verifyEntityReference
**
**  **jma
**
*************************************/

Acad::ErrorStatus
Utils::verifyEntityReference(const AcDbEntity* ent, const AcDbObjectId& idToValidate,
                 AcRxClass* classType, bool allowNull, bool allowErased)
{
    if (ent == NULL) {
        ASSERT(0);
        return Acad::eInvalidInput;
    }

        // first check for NULL
    if (idToValidate == AcDbObjectId::kNull) {
        if (allowNull)
            return Acad::eOk;
        else
            return Acad::eNullObjectId;
    }

        // now open it up and see what it is
    Acad::ErrorStatus es;
    AcDbEntity* entToValidate;
    es = acdbOpenAcDbEntity(entToValidate, idToValidate, AcDb::kForRead);
    if (es == Acad::eOk) {
            // make sure all references are from the same database
            // which should always be the case!
        if (ent->database() != entToValidate->database()) {
            entToValidate->close();
            return Acad::eWrongDatabase;
        }
            // make sure that references are from the same block
            // This can fail from a simple deepClone() to make a
            // blockTableRecord.  If the user didn't pick all the
            // referenced objects in the anchor at the time of
            // making the block, they will still point to the
            // entities in modelSpace... which will eventually crash
            // the whole thing.
        if (ent->blockId() != entToValidate->blockId()) {
            entToValidate->close();
            return Acad::eNotInBlock;
        }

            // see if they want to validate based on object type
        if (classType != NULL) {
            es = verifyClassType(entToValidate, classType);
            if (es != Acad::eOk) {
                entToValidate->close();
                return es;
            }
        }

        entToValidate->close();
        return Acad::eOk;               // passed our test    
    }
    else if (es == Acad::eWasErased) {
        if (allowErased)
            return Acad::eOk;           // passed under relaxed constraints
        else
            return Acad::eWasErased;    // failed
    }
    else {
        return es;                      // failed
    }
}




Acad::ErrorStatus Utils::convertGeCurveToPolyline(AcGeCurve2d* pCurve, AcDbPolyline*& pResultPoly)
{
	AcGeVoidPointerArray resultCurves;

	AcGeCompositeCurve2d* pResultCurve;
	AcGeCurve2d* pThisCurve;
	AcGeCircArc2d* pArc;
	AcGeLineSeg2d* pLine;
	AcGePoint2d endPt;

	int nCurves;
	double bulge, ang;

	if(pCurve->isKindOf(AcGe::kCompositeCrv2d))
	{
		pResultCurve = (AcGeCompositeCurve2d*)pCurve;
		pResultCurve->getCurveList(resultCurves );
	}
	else
	{
		resultCurves.append(pCurve);
	}

	nCurves = resultCurves.length();
	
	pResultPoly = new AcDbPolyline(nCurves);

	int i=0;
	for(i=0; i < nCurves; i++)
	{
		pThisCurve = (AcGeCurve2d*)(resultCurves[i]);

		if(pThisCurve->isKindOf(AcGe::kCircArc2d))
		{
			pArc = (AcGeCircArc2d*)pThisCurve;
			bulge = 0.0;
			ang = 0.25 * (pArc->endAng() - pArc->startAng()); 
			bulge = tan(ang);
			if(pArc->isClockWise())
			{
				bulge = -bulge;
			}

			pResultPoly->addVertexAt(i, pArc->startPoint(), bulge );
		}
		else if(pThisCurve->isKindOf( AcGe::kLineSeg2d))
		{
			pLine = (AcGeLineSeg2d*)pThisCurve;
			pResultPoly->addVertexAt(i, pLine->startPoint(), 0 );
		}
	}// for
	
	if(pThisCurve->hasEndPoint(endPt))
	{
		pResultPoly->addVertexAt(i, endPt, 0);
	}

	return Acad::eOk;
}

void Utils::GetTextStyleNames(CStringArray& strArray)
{
	// We need to get the BlockTable in order to retrieve all the blocks
	AcDbDatabase* pDwg=acdbHostApplicationServices()->workingDatabase();
    AcDbTextStyleTable* pTable;
    if (pDwg->getTextStyleTable (pTable, AcDb::kForRead) != Acad::eOk) {
        acutPrintf(L"\nCouldn't get the Text style table");
        return;
    }

    // Iterate block table
    AcDbTextStyleTableIterator *piter =NULL ;
    if (pTable->newIterator(piter) == Acad::eOk) {
        AcDbTextStyleTableRecord *pRec;
        while (!piter->done()) {
            piter->getRecord(pRec, AcDb::kForRead) ;
            ACHAR *name ;
            pRec->getName(name) ;
            pRec->close() ;
			CString strLayerName(name);
			strArray.Add(strLayerName);
            delete name ;
            piter->step () ;
        }
        delete piter ;
    }

    // end
    pTable->close () ;

}

bool Utils::GetTextStyleName(const AcDbObjectId idTextStyle, CString& strCurTextStyle)
{
	bool bRC=false;
	// Get the current layer pointer 
	AcDbTextStyleTableRecordPointer ptTextStyle(idTextStyle, AcDb::kForRead); 	
	if (ptTextStyle.openStatus() == Acad::eOk)
	{
		const ACHAR *name ;
		ptTextStyle->getName(name) ;
		strCurTextStyle=name;
		bRC=true;
	}
	return bRC;
}

void Utils::GetLayerName(AcDbObjectId idLayer, CString& strLayerName)
{
	// try to get the layer we want to rename    
	AcDbLayerTableRecordPointer layerTableRec(idLayer, AcDb::kForRead);    
	// if ok    
	if (layerTableRec.openStatus() == Acad::eOk)    {
		ACHAR* pLayerName;
		layerTableRec->getName(pLayerName);
		strLayerName=pLayerName;
		delete[] pLayerName;
		layerTableRec->close();	
	}

}

void Utils::GetLayerNames(CStringArray& strArray)
{
	// We need to get the BlockTable in order to retrieve all the blocks
	AcDbDatabase* pDwg=acdbHostApplicationServices()->workingDatabase();
    AcDbLayerTable* pLayerTable;
    if (pDwg->getLayerTable (pLayerTable, AcDb::kForRead) != Acad::eOk) {
        acutPrintf(L"\nCouldn't get the Layer table");
        return;
    }

    // Iterate block table
    AcDbLayerTableIterator *piter =NULL ;
    if (pLayerTable->newIterator(piter) == Acad::eOk) {
        AcDbLayerTableRecord *pRec;
	
        while (!piter->done()) {
            piter->getRecord(pRec, AcDb::kForRead) ;
            ACHAR *name ;
            pRec->getName(name) ;
            pRec->close() ;
			CString strLayerName(name);
			strArray.Add(strLayerName);
            delete name ;
            piter->step () ;
        }
        delete piter ;
    }

    // end
    pLayerTable->close () ;

}




int Utils::GetEntColor(AcDbEntity *&pEnt)
{
	int nCol = -1;
	if (NULL != pEnt)
	{
		nCol=pEnt->colorIndex();
		if(nCol==256)
		{
			//Entity color is BYLAYER, so get
			//color of entity layer
			AcDbLayerTableRecord *pLayerTableRecord=NULL;
			if (acdbOpenObject(pLayerTableRecord, pEnt->layerId(), AcDb::kForRead) == Acad::eOk)
			{
				nCol=pLayerTableRecord->color().colorIndex();
				pLayerTableRecord->close();
			}
		}
	}
	return nCol;
}

void Utils::GetLayer(LPCTSTR lpszLayer, AcDbObjectId& id)
{
	// We need to get the BlockTable in order to retrieve all the blocks
	AcDbDatabase* pDwg=acdbHostApplicationServices()->workingDatabase();
    AcDbLayerTable* pLayerTbl;
    if (pDwg->getLayerTable (pLayerTbl, AcDb::kForRead) != Acad::eOk) {
        acutPrintf(L"\n레이어 테이블을 가져올 수 없습니다.");
        return;
    }

    // layer name
    ACHAR layName[256];
	_tcscpy(layName, lpszLayer);
    if( pLayerTbl->has(layName) ) // Is there a layer with given name?
    {
		AcDbLayerTableRecord *pLayerTblRcd;
		AcCmColor color;
		if( (pLayerTbl->getAt(layName, pLayerTblRcd, AcDb::kForRead, Adesk::kFalse )) == Acad::eOk )
		{
			//color = pLayerTblRcd->color();
			id=pLayerTblRcd->objectId();
			pLayerTblRcd->close();
		} 
		else
			acutPrintf(_T("\n레이어(%s)를 열 수 없습니다.!"), layName);
    } 
    else
      acutPrintf(_T("\nLayer %s does not exist."), layName);

    pLayerTbl->close();


}

void Utils::GetColorOfLayer(const LPCTSTR lpszLayer, int& colorIndex)
{
	// We need to get the BlockTable in order to retrieve all the blocks
	AcDbDatabase* pDwg=acdbHostApplicationServices()->workingDatabase();
    AcDbLayerTable* pLayerTbl;
    if (pDwg->getLayerTable (pLayerTbl, AcDb::kForRead) != Acad::eOk) {
        acutPrintf(L"\n레이어 테이블을 가져올 수 없습니다.");
        return;
    }

    // layer name
    ACHAR layName[256];
	_tcscpy(layName, lpszLayer);
    if( pLayerTbl->has(layName) ) // Is there a layer with given name?
    {
		AcDbLayerTableRecord *pLayerTblRcd;
		AcCmColor color;
		if( (pLayerTbl->getAt(layName, pLayerTblRcd, AcDb::kForRead, Adesk::kFalse )) == Acad::eOk )
		{
			AcCmColor color = pLayerTblRcd->color();
			colorIndex = color.colorIndex();
			pLayerTblRcd->close();
		} 
		else
			acutPrintf(_T("\n레이어(%s)를 열 수 없습니다!"), layName);
    } 
    else
      acutPrintf(_T("\n레이어(%s)가 존재하지 않습니다."), layName);

    pLayerTbl->close();
}


bool Utils::GetCurLayerNameAndColorIndex(CString& strCurLayer, int& colorIndex)
{
	bool bRC=false;
	AcDbDatabase* pDwg=acdbHostApplicationServices()->workingDatabase();
	// Get the ID of the current layer 
	AcDbObjectId layerId = pDwg->clayer(); 
	// Get the current layer pointer 
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead); 
	// Get the current layer color 
	AcCmColor oldColor = ptLayer->color(); 
	colorIndex = oldColor.colorIndex () ;	// color of the current layer 
	const ACHAR *name ;
    ptLayer->getName(name) ;
	strCurLayer=name;
	return bRC;
}


COLORREF Utils::GetRGBFromEntity( AcDbEntity *ent )
{

  COLORREF colorRef = RGB(255, 255, 255);
  AcCmColor Color = ent->color();
  AcCmEntityColor::ColorMethod ColorMethod = Color.colorMethod();

 
  switch(ColorMethod)
  {

	case AcCmEntityColor::kByACI:
    {          

      long acirgb, r,g,b;

#ifdef _ZCAD
      acirgb = AcCmEntityColor::lookUpRGB(Color.colorIndex());
#elif _ACAD
      acirgb = Color.getCOLORREF();
#endif
      b = ( acirgb & 0xff00L ); 
      g = ( acirgb & 0xff00L ) >> 8; 
      r = acirgb >> 16;
      colorRef = RGB( r, g, b);  
      break;
    }
  }

  return colorRef;

}


int Utils::SelColor () 
{ 
  // Get the ID of the current layer 
  AcDbObjectId layerId = acdbHostApplicationServices () -> workingDatabase () -> clayer (); 
  // Get the current layer pointer 
  AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead); 
  // Get the current layer color 
  AcCmColor oldColor = ptLayer->color (); 
  int nCurColor = oldColor.colorIndex () ;	// color of the current layer 
  int nNewColor = oldColor.colorIndex () ;	// user-selected color 


  if (acedSetColorDialog(nNewColor, Adesk::kTrue, nCurColor)) 
  { 
	return nNewColor; 
  } 
  else 
  { 
	return nCurColor; 
  } 
} 


void Utils::refreshDisplay(void)
{

	actrTransactionManager->queueForGraphicsFlush();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();

}

void Utils::CreateUCS(LPCTSTR lpszNewUCS, AcGePoint3d org, AcGeVector3d vx, AcGeVector3d vy)
{

	Acad::ErrorStatus es;
	ACHAR name[133];
	
 
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbUCSTable *pUCSTable = NULL;
	if (Acad::eOk != pDb->getUCSTable(pUCSTable, AcDb::kForWrite)) return;

	 

	// 주어진 이름으로 UCS 테이블을 구한다.
	AcDbUCSTableRecord *pUCS = NULL;
	AcDbObjectId idUCS;
	if (pUCSTable->getAt(lpszNewUCS, pUCS, AcDb::kForRead)!=Acad::eOk) 
	{
		// UCS 정의		
		AcDbUCSTableRecord *myUCS = new AcDbUCSTableRecord;
		AcGePoint3d  origin_point=org;
		AcGeVector3d UCSXaxis=vx;
		AcGeVector3d UCSYaxis=vy;

		myUCS->setOrigin(origin_point);
		myUCS->setXAxis(UCSXaxis);
		myUCS->setYAxis(UCSYaxis);


		es=myUCS->setName(lpszNewUCS);
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\nUCS 이름 설정 실패!"));
			return;
		}

		pUCSTable->add(idUCS, myUCS);
		es=pUCSTable->close();
		es= myUCS->close();

	}else{
		AcGePoint3d  origin_point=org;
		AcGeVector3d UCSXaxis=vx;
		AcGeVector3d UCSYaxis=vy;
		idUCS=pUCS->objectId();
		pUCS->setOrigin(origin_point);
		pUCS->setXAxis(UCSXaxis);
		pUCS->setYAxis(UCSYaxis);
		pUCS->close();
	}
	es=pUCSTable->close();


	//To set the current UCS, I accessed 
	// the active AcDbViewportTableRecord
	// and used setUCS to set the UCS I created as current.
	AcDbViewportTable *pVT; 
	es = acedVports2VportTableRecords();
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\nFailed to load vport info into vport table records"));
		return;
	}


	es=pDb->getViewportTable(pVT,AcDb::kForRead);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\nFailed to get vport table"));
		pVT->close();
		return;
	}


	AcDbViewportTableIterator* pIter = NULL;
	es=pVT->newIterator(pIter);
	if (es != Acad::eOk)
	{
		acutPrintf(_T("\nFailed to get vport table"));
		pVT->close();
		delete pIter;
		return;
	}



	for (pIter->start();!pIter->done();pIter->step())
	{
		AcDbViewportTableRecord* pRec;
		//it should be open for write mode
		es=pIter->getRecord(pRec,AcDb::kForWrite); 
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\nFailed to get vport table record"));
			pVT->close();
			pRec->close();
			delete pIter;
			return;
		} 


		TCHAR* name=NULL;
		es=pRec->getName(name);
		if (es != Acad::eOk)
		{
			acutPrintf(_T("\nFailed to get name from vport table"));
			pVT->close();
			pRec->close();
			delete pIter;
			return;
		} 



		if (_tcsicmp(name,_T("*ACTIVE"))==0)
		{
			es=pRec->setUcs(idUCS);
		}
		es=pRec->close();     

	}

	es=acedVportTableRecords2Vports(); //force update
	es=pVT->close();
	delete pIter;
	return ; 

 }


void Utils::SetConstantWidth(AcDb2dPolyline* pline, double constWidth)
{
	if(pline==NULL) return;

    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
		if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForWrite) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
				vertex->setStartWidth(constWidth);
				vertex->setEndWidth(constWidth);
            }
            vertex->close();
        }
    }
    delete vertexIter;
}

double Utils::GetConstantWidth(AcDb2dPolyline* pline)
{
	double constWidth=0;
	if(pline==NULL) return constWidth;
    AcDbObjectIterator* vertexIter = pline->vertexIterator();
    ASSERT(vertexIter != NULL);
    if (vertexIter == NULL)
        return constWidth;

    AcDb2dVertex* vertex;
    for (; !vertexIter->done(); vertexIter->step()) {
		if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
            if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
				double w1=vertex->startWidth();
				double w2=vertex->endWidth();
				if(fabs(w1-w2)<0.00001){
					constWidth=w1;
					vertex->close();
					break;
				}
            }
            vertex->close();
        }
    }
    delete vertexIter;
	return constWidth;
}



double Utils::getPickBox()
{
	int			iPickBox;
	double		viewsize;
	double		screensizeY;
	struct resbuf  *ent=NULL, *sys1=NULL,  *sys2=NULL, *sys3=NULL;


	sys1=ads_newrb(RTREAL);       /* sys1 must the allocated memory */
	sys2=ads_newrb(RT3DPOINT);
	sys3=ads_newrb(RTSHORT);

	ads_getvar(_T("VIEWSIZE"), sys1);     /* real */
	ads_getvar(_T("SCREENSIZE"), sys2);   /* point (real real) */
	ads_getvar(_T("PICKBOX"), sys3);   /* int */

	viewsize=sys1->resval.rreal;            /* view size */
	screensizeY=sys2->resval.rpoint[Y];        /* screensize */
	iPickBox=sys3->resval.rint;
	ads_relrb(sys1);
	ads_relrb(sys2);
	ads_relrb(sys3);


	
	return ((viewsize/screensizeY)*(double)iPickBox);  
}


void Utils::PyoSi(const AcGePoint3d pos)
{
	double		ps=getPickBox();		// real size of pickbox in screen 
	
	AcGeVector3d v45=AcGeVector3d::kXAxis;
	AcGeVector3d v135=AcGeVector3d::kXAxis;
	v45.rotateBy(kRad45, AcGeVector3d::kZAxis);
	v135.rotateBy(kRad135, AcGeVector3d::kZAxis);

	AcGePoint3d pt0, pt1, pt2, pt3;
	pt0=pos+v135*ps;
	pt1=pos+v45*ps;
	pt2=pos-v135*ps;
	pt3=pos-v45*ps;

	acedGrDraw(asDblArray(pt0), asDblArray(pt2), 1, 0);
	acedGrDraw(asDblArray(pt1), asDblArray(pt3), 1, 0);

}

bool Utils::EntDel(AcDbEntity *pEnt)
{
    if (pEnt != NULL)
    {
		if(Utils::isOnLockedLayer(pEnt, false)==true) {		
			// 레이어가 잠겨있다면 이 작업을 포기한다.
			pEnt->close();
			return false;
		}

        if (! pEnt->isWriteEnabled())
            pEnt->upgradeOpen();		
		pEnt->erase();
		
        pEnt->close();
    }
	return true;
}

bool  Utils::EntDel(AcDbObjectId& entId)
{
	int rc=false;
	AcDbEntityPointer pEnt(entId, AcDb::kForWrite);
	if(pEnt.openStatus()==Acad::eOk){
		pEnt->erase();
		rc=true;
	}
	return rc;
}


void Utils::EntDel(AcDbObjectIdArray& ids)
{
	int l=ids.length();
	for(int i=0; i<l; i++)EntDel(ids[i]);
}


bool Utils::SetLayer(AcDbEntity *pEnt, LPCTSTR layer)
{
    if (pEnt != NULL)
    {
		if(Utils::isOnLockedLayer(pEnt, false)==true) {		
			// 레이어가 잠겨있다면 이 작업을 포기한다.
			pEnt->close();
			return false;
		}

        if (! pEnt->isWriteEnabled())
            pEnt->upgradeOpen();		
		pEnt->setLayer(layer);
		
        pEnt->close();
    }
	return true;
}

bool  Utils::SetLayer(AcDbObjectId& entId, LPCTSTR layer)
{
	int rc=false;
	AcDbEntityPointer pEnt(entId, AcDb::kForWrite);
	if(pEnt.openStatus()==Acad::eOk){
		pEnt->setLayer(layer);
		rc=true;
	}
	return rc;
}


void Utils::SetLayer(AcDbObjectIdArray& ids, LPCTSTR layer)
{
	int l=ids.length();
	for(int i=0; i<l; i++) SetLayer(ids[i], layer);
}


bool Utils::SetColorIndex(AcDbEntity *pEnt, int nColorIndex)
{
    if (pEnt != NULL)
    {
		if(Utils::isOnLockedLayer(pEnt, false)==true) {		
			// 레이어가 잠겨있다면 이 작업을 포기한다.
			pEnt->close();
			return false;
		}

        if (! pEnt->isWriteEnabled())
            pEnt->upgradeOpen();		
		pEnt->setColorIndex(nColorIndex);
		
        pEnt->close();
    }
	return true;
}

bool  Utils::SetColorIndex(AcDbObjectId& entId, int nColorIndex)
{
	int rc=false;
	AcDbEntityPointer pEnt(entId, AcDb::kForWrite);
	if(pEnt.openStatus()==Acad::eOk){
		pEnt->setColorIndex(nColorIndex);
		rc=true;
	}
	return rc;
}


void Utils::SetColorIndex(AcDbObjectIdArray& ids, int nColorIndex)
{
	int l=ids.length();
	for(int i=0; i<l; i++) SetColorIndex(ids[i], nColorIndex);
}


void Utils::MoveObject(AcDbObjectId ObjectId, AcGeVector3d MoveVector)
{
	// Open the object for write.
	AcDbEntity* pEnt;
	acdbOpenObject(pEnt, ObjectId, AcDb::kForWrite);

	AcGeMatrix3d transMatrix;
	transMatrix = transMatrix.setTranslation(MoveVector);
	pEnt->transformBy(transMatrix);

	// Close the object.
	pEnt->close();

	// Force the entity to draw.
	pEnt->draw();

	// Force the update of the display.
	acedUpdateDisplay();
}

void Utils::ss2Array(ads_name ss, AcDbObjectIdArray& ids)
{
    // [+] SUP-29742
    // long	len;
    // [-] END

	Adesk::Int32	len;
	int		        ret;
	
	ads_name ent;

	acedSSLength(ss, &len);
	for (long i=0L; i<len; i++) {
		ret = acedSSName(ss, i, ent);
		AcDbObjectId objId;
		ASSERT(ret == RTNORM);

		Acad::ErrorStatus es = acdbGetObjectId(objId, ent);
		if (es == Acad::eOk)
		{
			ids.append(objId);
		}

		/*if (acdbGetObjectId( objId,ent) == TRUE){

			ids.append(objId);
		}*/
	}//end-for
	acedSSFree(ss);
}


void Utils::GetAllEntityWithLayer(AcArray<AcRxClass*>& arrClassType, LPCTSTR layer, AcDbObjectIdArray& entities)
{
	CString strLayer(layer);
	// 레코드 전체 조회 
	// 주어진 클래스 유형의 객체 아이디 목록을 만든다.
	AcDbBlockTable* pTable;  
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pTable, AcDb::kForRead);
	if(pTable == NULL) return;  
	AcDbBlockTableIterator* pTableIter;
	for( pTable->newIterator(pTableIter); !pTableIter->done(); pTableIter->step())
	{      
		AcDbBlockTableRecord* pRecord;      
		pTableIter->getRecord(pRecord,AcDb::kForRead);   
		if(pRecord == NULL)
		{    
			acutPrintf(_T("\n블록테이블레코드(BTR)를 열 수 없습니다."));   
			continue;    
		}    
		AcDbBlockTableRecordIterator* pRecordIter; 
		for(pRecord->newIterator(pRecordIter); !pRecordIter->done(); pRecordIter->step()) 
		{
			AcDbEntity*pEnt;     
			pRecordIter->getEntity(pEnt, AcDb::kForRead); 
			if(pEnt != NULL ) 
			{ 
				CString strLayer1=pEnt->layer();
				if(arrClassType.length()>0){
					for(int i=0;i<arrClassType.length();i++){
						if( pEnt->isKindOf(arrClassType[i]))
						{      						
							if(strLayer.CompareNoCase(strLayer1)==0) {
								entities.append(pEnt->objectId() );    
							}					
						}//end-if  
					}//end-for
				}else{
					if(strLayer.CompareNoCase(strLayer1)==0) {
						entities.append(pEnt->objectId());    
					}		
				}
				pEnt->close();     
			}     
		}    
		delete pRecordIter;    
		pRecord->close();  

	}   
	delete pTableIter; 
	pTable->close();


}


// 선형보간법
double 
Utils::Interpolation(const double x1, const double y1,const double x2, const double y2, const double x)
{
	double dVal=y1+(y2-y1)/(x2-x1)*(x-x1);
	return dVal;
}










