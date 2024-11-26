#include "StdAfx.h"
#include "Utils.h"
#include "EntMake.h"


EntMake::EntMake(void)
{
	pDb=curDoc()->database();
	m_strLinetype=_T("CONTINUOUS");
	m_ltscl=1.0;
	m_textsize=2.5;
	m_LW=AcDb::kLnWtByLayer;
	_tcscpy_s(m_BlockName,  sizeof(m_BlockName), ACDB_MODEL_SPACE);
}


EntMake::~EntMake(void)
{
}


void EntMake::GetProperty(AcDbEntity* pEnt)
{
	m_strLayer=pEnt->layer();
	m_strLinetype=pEnt->linetype();
	m_ltscl=pEnt->linetypeScale();
	m_LW=pEnt->lineWeight();
}

void EntMake::SetProperty(AcDbEntity* pEnt)
{
	pEnt->setLayer(m_strLayer);
	pEnt->setLinetype(m_strLinetype);
	pEnt->setLinetypeScale(m_ltscl);
	pEnt->setLineWeight(m_LW);
}


void EntMake::setLayerAndColor(LPCTSTR lpszLayer, int colorindex)
{
	m_strLayer=lpszLayer;
	m_colorindex=colorindex;
}


Acad::ErrorStatus 
EntMake::appendToBlock(AcDbEntity* pEnt, AcDbObjectId& idEnt, const TCHAR *pcBlockName, AcDbDatabase *pDb)
{
	Acad::ErrorStatus		es;
	AcDbBlockTable*			pBlockTable = NULL;
	AcDbBlockTableRecord*	pBlockTableRecord = NULL;

	// Default to current database
	if(pEnt==NULL) return Acad::eNullObjectPointer;
	if(!pDb){
		pDb = acdbHostApplicationServices()->workingDatabase();
		if (pDb == NULL)
			return  Acad::eNoDatabase;
	}//end-if

	// Get block table
	if((es = pDb->getBlockTable( pBlockTable, AcDb::kForRead )) != Acad::eOk)
		return es;

    // Find block record
	if((es = pBlockTable->getAt(pcBlockName, pBlockTableRecord, AcDb::kForWrite )) != Acad::eOk)
	{
		pBlockTable->close();
		return es;
	}
    
	// append new entity to block:
    es = pBlockTableRecord->appendAcDbEntity( idEnt, pEnt );
	    
	pBlockTableRecord->close();
	pBlockTable->close();

	return es;
}

AcDbObjectId 
EntMake::makePolyline(AcGePoint3dArray& vertices)
{
	int ptCount;
	AcDbObjectId id;

	ptCount=vertices.length();
	if(ptCount==0) return NULL;
    AcDbPolyline* pline = new AcDbPolyline(ptCount); // optimized to know how many verts
	if (!pline)
	{
		acedAlert(_T("폴리선을 생성할 메모리가 충분하지 않습니다!"));
	}else{
	    Acad::ErrorStatus es;
        // append all the vertices (as 2D)
		AcGePoint2d pt;
		AcGePlane pln;
		for (int i=0;i<ptCount;i++) {
			pt = vertices.at(i).convert2d(pln);
			es = pline->addVertexAt(i, pt);
			if (es != Acad::eOk) {
				delete pline;
				Utils::rxErrorMsg(es);
				return NULL;
			}//end-if
		}
	    pline->setElevation(0.0);
		SetProperty(pline);
		if(appendToBlock(pline, id, m_BlockName, pDb)==Acad::eOk)
			pline->close();
		else 
			delete pline;
	}//end-if-else
	return id;
}



AcDbObjectId
EntMake::make2dPolyline(AcDb::Poly2dType type, AcGePoint3dArray& vertices, double elevation,	Adesk::Boolean closed, double defStartWidth, double defEndWidth, AcGeDoubleArray* bulges)
{

	AcDb2dPolyline* pline=new AcDb2dPolyline(type, vertices, elevation, closed, defStartWidth, defEndWidth, bulges);
	AcDbObjectId id;

	if (!pline)
	{
		acedAlert(_T("폴리선을 생성할 메모리가 충분하지 않습니다!"));
	}else{
		SetProperty(pline);
		if(appendToBlock(pline, id, m_BlockName, pDb)==Acad::eOk)
			pline->close();
		else 
			delete pline;
	}//end-if-else
	return id;
}


AcDbObjectId
EntMake::makeText(AcGePoint3d ptPos, double hgt, double rot, LPCTSTR textString, AcDb::TextHorzMode horzMode, AcDb::TextVertMode vertMode, AcGeVector3d norm)
{
	AcDbObjectId id;
    AcDbText* text = new AcDbText;
	text->setDatabaseDefaults();
	text->setColorIndex(m_colorindex);
	text->setLayer(m_strLayer);

	text->setTextStyle(m_textstyle);  
	AcGiTextStyle textstyle;

	if(fromAcDbTextStyle(textstyle, m_textstyle)==Acad::eOk)
	{
	
		text->setWidthFactor(textstyle.xScale());
		text->setOblique(textstyle.obliquingAngle());		
	}


	text->setNormal(norm);
    text->setHeight(hgt);
    text->setRotation(rot);
    text->setTextString(textString);
	text->setHorizontalMode(horzMode);
	text->setVerticalMode(vertMode);
	text->setAlignmentPoint(ptPos);
	//text->setPosition(ptPos);



    Utils::transformToWcs(text, acdbHostApplicationServices()->workingDatabase());

	SetProperty(text);
	if(appendToBlock(text, id, m_BlockName, pDb)==Acad::eOk)
		text->close();
	else 
		delete text;

   return id;
}

AcDbObjectId
EntMake::makeLine(const AcGePoint3d& pt1, const AcGePoint3d& pt2)
{
	AcDbObjectId id;
    AcDbLine* line = new AcDbLine;
    line->setDatabaseDefaults();
    line->setStartPoint(pt1);
    line->setEndPoint(pt2);

     Utils::transformToWcs(line, acdbHostApplicationServices()->workingDatabase());

	SetProperty(line);
	if(appendToBlock(line, id, m_BlockName, pDb)==Acad::eOk)
		line->close();
	else 
		delete line;

	return id;
}