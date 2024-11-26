#include "StdAfx.h"
#include "TextProcess.h"

CTextProcess::CTextProcess(void)
{
}

CTextProcess::~CTextProcess(void)
{
}


AcDbObjectId CTextProcess::insertTextCMD(AcGePoint3d insPt, double textHeight,CString StyleName,double widthFactor,double angle,CString Lyname,int colorindex,CString strText,AcDb::TextVertMode& TextVPos,AcDb::TextHorzMode& TextHPos)
{





	CWKUtils utils;
	AcDbObjectId Layid;
	if(!utils.FindLayer(Lyname,Layid))
	{
		utils.createLayer(Lyname,0,Layid);
	}
	AcDbObjectId StyleId;
	if(!utils.GetTextStyle(StyleName,StyleId))
	{
		StyleId = utils.CreateTextStyle(_T("굴림체"),_T(""),StyleName);
	}

	if(StyleId==NULL)return NULL;

	

	LockDocManager EnableModifyAcDbEntityAttribute;
	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	AcDbText* pText = new AcDbText;

	pText->setLayer(Lyname);
	pText->setTextStyle(StyleId);
	pText->setRotation(angle);
	pText->setHeight(textHeight);
	pText->setWidthFactor(widthFactor);
	pText->setColorIndex(colorindex);
	
	pText->setPosition(insPt);
	pText->setTextString(strText);
	//pText->setVerticalMode(TextPos);
	pText->setHorizontalMode(TextHPos);
	pText->setVerticalMode(TextVPos);
	pText->setAlignmentPoint(insPt);
	AcDbObjectId TextId = NULL;
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);	//현재 작업창을 쓰자
	pText->setDatabaseDefaults();
	blkRec->appendAcDbEntity(TextId,pText);
	pText->close();
	blkRec->close();
	return TextId;
	
}

