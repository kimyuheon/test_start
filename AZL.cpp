#include "StdAfx.h"
#include "AZL.h"

CAZL::CAZL(void){
}

CAZL::~CAZL(void){
}

void CAZL::MainDraw(){
	AcGePoint3d spt, ept;
	TCHAR Text[30];
	CString Az_ValueStr;//방위각
	double Az_Value, Az_Dist, Az_x,Az_y;
	resbuf Old_DotStyle, New_DotStyle;
	CString Ext_Azimuth, strMsg;
	AcDbObjectId DotObjId, LineObjId;
	ads_name ads_Dot, ads_Line;

	strMsg.LoadStringW(IDS_ZVL_START_SET); // "\n시작 점 지정 : "
	if(acedGetPoint(0, strMsg, asDblArray(spt)) != RTNORM)
		return;

	ads_getvar(_T("PDMODE"), &Old_DotStyle);
	DotObjId = CreatePoint(spt);//점 생성ddddddd
	acdbGetAdsName(ads_Dot, DotObjId);
	New_DotStyle.resval.rint = 3;
	ads_setvar(_T("PDMODE"), &New_DotStyle);
	while(true){
		strMsg.LoadStringW(IDS_CIVIL_ANG_INPUT); // "\n방위각 입력 : "
		int stat = acedGetString(1, strMsg, Text);
		if(stat == RTCAN || stat == RTNONE){
			ads_setvar(_T("PDMODE"), &Old_DotStyle);
			acdbEntDel(ads_Dot);
			return;
		}
		else if(stat == RTERROR){
			continue;
		}
		else if(stat == RTNORM){
			Ext_Azimuth = CheckInputNumber((LPCTSTR)Text);
			ExtractAzimuth(Ext_Azimuth);//DstrRe = 도 MstrRe = 분 SstrRe = 초
			if(MstrRe == _T(""))
				MstrRe = _T("0");
			if(SstrRe == _T(""))
				SstrRe = _T("0");

			Az_Value = (_tstof(DstrRe) + (_tstof(MstrRe) / 60) + (_tstof(SstrRe) / 60 / 60)) / 180 * PI;//더블형 방위각
			Az_y = cos(Az_Value) * 10000;//위거(Y)	가상선Y
			Az_x = sin(Az_Value) * 10000;//경거(X)	가상선X
			ept.x = spt.x + Az_x;
			ept.y = spt.y + Az_y;
			LineObjId = CreateLine(spt,ept);
			acdbGetAdsName(ads_Line, LineObjId);
			highlight(LineObjId);
			break;
		}
		else{
			ads_setvar(_T("PDMODE"), &Old_DotStyle);
			acdbEntDel(ads_Dot);
			return;
		}
	}
	strMsg.LoadStringW(IDS_CIVIL_DIST_INPUT); // "\n거리 입력 : "
	if(acedGetReal(strMsg, &Az_Dist) != RTNORM){
		ads_setvar(_T("PDMODE"), &Old_DotStyle);
		acdbEntDel(ads_Dot);
		acdbEntDel(ads_Line);
		return;
	}
	Az_y = cos(Az_Value) * Az_Dist;	//위거(Y)	가상선Y
	Az_x = sin(Az_Value) * Az_Dist;	//경거(X)	가상선X
	ept.x = spt.x + Az_x;
	ept.y = spt.y + Az_y;
	CreateLine(spt, ept);
	//점, 가상선 지우기
	acdbEntDel(ads_Dot);
	acdbEntDel(ads_Line);
	ads_setvar(_T("PDMODE"), &Old_DotStyle);//종료시 기존시스템변수 리턴
}

AcDbObjectId CAZL::CreatePoint(AcGePoint3d pos){
	double ofsetX,ofsetY;
	AcDbObjectId pPointObjId;
	AcDbPoint* pPoint = new AcDbPoint;
	pPoint->setPosition(AcGePoint3d(pos.x, pos.y, 0));
	pPoint->setColorIndex(2);
	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
	pPoint->setDatabaseDefaults();
	blkRec->appendAcDbEntity(pPointObjId,pPoint);
	pPoint->close();
	blkRec->close();
	return pPointObjId;
}

//숫자, 점, (-) 추출
CString CAZL::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	CString getstr;
	for(int i = 0; i < str.GetLength(); i++){
		getstr = str.Mid(i,1);
		getAtChar = (TCHAR*)(LPCTSTR)getstr;
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 45 || ascii == 46)){
			str.Remove(*getAtChar);
		}
	}
	return str;
}

//방위각(도,분,초)추출
CString CAZL::ExtractAzimuth(CString str){
	TCHAR* getAtChar;
	CString strNum;
	int i = 0, j = 0, k = 0, l = 0;
	for(i; i < str.GetLength(); i++){
		strNum = str.Mid(i,1);	//도 추출
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 45)){
			if(ascii == 45){	//(-)일떄 
				if(i == 0){
					DstrRe.Append(getAtChar);
					continue;
				}
				else
					break;
			}
			DstrRe.Append(getAtChar);
		}
	}
	j = i+1;
	for(j; j < str.GetLength(); j++){
		strNum = str.Mid(j,1);	//분 추출
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 45)){
			if(ascii == 45){//(-)일떄 
				break;
			}
			MstrRe.Append(getAtChar);
		}
	}
	k = j+1;
	for(k; k < str.GetLength(); k++){
		strNum = str.Mid(k,1);	//초 추출
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46 || ascii == 45)){
			if(ascii == 45){//(-)일떄 
				break;
			}
			SstrRe.Append(getAtChar);
		}
	}
	return DstrRe;
}

//라인 생성
AcDbObjectId CAZL::CreateLine(AcGePoint3d spt, AcGePoint3d ept){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return NULL;

	AcDbLine* pLine = new AcDbLine(spt,ept);
	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pLine);
			pBTR->close();
			pLine->close();
		}
		else
			return NULL;
		pBT->close();
	}
	else
		return NULL;
	pLine->close();
	return pLineObjId;
}

void CAZL::highlight(AcDbObjectId objId){
	AcDbEntity* pEnt;
	if(acdbOpenObject(pEnt,objId,AcDb::kForRead)==Acad::eWasErased)
		return;
	pEnt->draw();
	pEnt->highlight();
	pEnt->close();
}