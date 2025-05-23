#include "StdAfx.h"
#include "WKUtils.h"

CWKUtils::CWKUtils(void){
}

CWKUtils::~CWKUtils(void){
}

// 사용자에게서 포인트를 입력 받는다.
BOOL CWKUtils::GetPoint(AcGePoint3d& point){
	CString strMsg(_T("\n포인트선택:"));

	if(acedGetPoint(NULL, strMsg, asDblArray(point)) == RTNORM){
		return TRUE;
	}
	else{
		acutPrintf(_T("*취소*"));
		return FALSE;
	}
}

// 사용자에게 Entity을 선택 받는다.
AcDbObjectId CWKUtils::GetLayer(CString& LayerName, CString msg){
	AcDbObjectId Layerid = NULL;
	ads_name entName;
	ads_point pt;
	ACHAR* LName = NULL;

	if(acedEntSel(msg, entName, pt) != RTNORM){
		acutPrintf(_T("*취소*"));
		return Layerid;
	}
	struct resbuf *rbEnt; //엔티티 데이터 버퍼의 결과를 저장
	// 버퍼의 결과로부터 얻어진 실체 데이터를 저장 entName
	rbEnt = acdbEntGet(entName);
	AcDbObjectId id;
	acdbGetObjectId(id,entName);

	AcDbEntityPointer pEnt(id, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){
		LName = pEnt->layer();
		Layerid = pEnt->layerId();
		LayerName = LName;
		pEnt->close();
	}
	return Layerid;
}

// 사용자에게 Entities을 선택 받는다.
AcDbObjectIdArray CWKUtils::GetLayers(CString msg){
	AcDbObjectIdArray layerIds;
	ads_name entName, entNames;
	ads_point pt;
	ads_name ename, sname;
	ACHAR* LName=NULL;

	if(acedSSGet(_T("I"), NULL, NULL, NULL, entNames) != RTNORM)
	{	
		if(acedSSGet(NULL, NULL, NULL, NULL, entNames) != RTNORM)
		{
			acutPrintf(_T("*취소*"));
			return layerIds;
		}
	}

	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	acedSSLength(entNames, &len);

	for(int i = 0; i < len; i++){		
		acedSSName(entNames, i, entName);

		struct resbuf *rbEnt; //엔티티 데이터 버퍼의 결과를 저장
		// 버퍼의 결과로부터 얻어진 실체 데이터를 저장 entName
		rbEnt = acdbEntGet(entName);
		AcDbObjectId id;
		acdbGetObjectId(id, entName);

		AcDbEntityPointer pEnt(id, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			LName = pEnt->layer();
			layerIds.append(pEnt->layerId());
			pEnt->close();
		}
	}
	acedSSFree(entNames);
	return layerIds;
}

//사용자에게 Xref 를 선택 받는다.``
BOOL CWKUtils::GetXrefEntity(CString msg,AcDbObjectId& ObjId,CString& Xrefname){
	BOOL bRC = FALSE;
	AcDbObjectId Layerid =NULL;
	ads_name entName;
	ads_point pt;
	ACHAR* LName=NULL;

	if(acedEntSel(msg, entName, pt) != RTNORM){
		acutPrintf(_T("*취소*"));
		return FALSE;
	}
	struct resbuf *rbEnt; //엔티티 데이터 버퍼의 결과를 저장
	// 버퍼의 결과로부터 얻어진 실체 데이터를 저장 entName
	rbEnt = acdbEntGet(entName);
	AcDbObjectId id;
	acdbGetObjectId(id, entName);

	AcDbEntityPointer pEnt(id, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){		
		AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
		if(pSelBlockRef){
			AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
			if(pBTR.openStatus() == Acad::eOk){
				if(pBTR->isFromExternalReference()){
					pBTR->getName(LName);
					Xrefname = LName;
					ObjId = id;
					bRC = TRUE;
				}
				pBTR->close();
			}
		}
		pEnt->close();
	}
	return bRC;
}

//블록이지만 외부 참조인지 여부
BOOL CWKUtils::IsXref(AcDbObjectId& ObjId){
	BOOL bRC = FALSE;

	AcDbEntityPointer pEnt(ObjId, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){
		AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
		if(pSelBlockRef){
			AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
			if(pBTR.openStatus() == Acad::eOk){
				if(pBTR->isFromExternalReference()){
					bRC = TRUE;
				}
				pBTR->close() ;
			}//end-if
		}
		pEnt->close();
	}
	return bRC;
}

void CWKUtils::fLayerInformation(){
	//get the layers
	AcDbLayerTable *pLayT = NULL;
	AcDbLayerTableRecord *pLayTR = NULL;
	AcDbLayerTableIterator *pLayIter = NULL;
	Acad::ErrorStatus mEs;
	ACHAR *pName;

	if(Acad::eOk != acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayT,AcDb::kForRead)){
		acutPrintf(L"\nUnable to get the LayerTable.");
		return; 
	}

	mEs = pLayT->newIterator(pLayIter);
	pLayT->close();

	if(Acad::eOk != mEs){
		acutPrintf(L"\nUnable to get the Layer Iterator.");
		return; 
	}

	while(!pLayIter->done()){
		mEs = pLayIter->getRecord(pLayTR,AcDb::kForRead);
		if(Acad::eOk == mEs){
			//get the name
			pLayTR->getName(pName);			
			if(true == pLayTR->isDependent())
				acutPrintf(L"\nLayer : %s is Xref dependent.",pName);
			else
				acutPrintf(L"\nLayer : %s is not Xref dependent.",pName);
		}

		pLayTR->close();
		pLayIter->step();
	}

	delete pLayIter;//delete the AcDbLayerTableIterator 
}

void CWKUtils::RenameLayer(CString SourceName , CString TargetName){
	ACHAR reply[133];
	Acad::ErrorStatus es;
	AcDbDatabase *dwg = acdbHostApplicationServices()->workingDatabase();
	if(dwg != NULL){
		AcDbLayerTableRecordPointer layerTableRec(SourceName, dwg, AcDb::kForRead);
		if(layerTableRec.openStatus() == Acad::eOk){
			es = layerTableRec->upgradeOpen();
			es = layerTableRec->setName(TargetName);
			if(es == Acad::eOk){
			}
			else{
			}
		}
		else{
			acutPrintf (_T("Layer %s doesn't exist..."), reply);
		}
	}
}


//도면층 속성 변경하기
BOOL CWKUtils::SetlayerLock(AcDbDatabase* db, CString& Lyername, bool bisLock){
	BOOL bRC = FALSE;
	Acad::ErrorStatus es;
	AcDbLayerTable* pLayerTbl = NULL;
	db->getLayerTable(pLayerTbl, AcDb::kForWrite);
	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);

	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for(; !pLayerIterator->done(); pLayerIterator->step()){
		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		if(Lyername.CompareNoCase(pLName) == 0){
			pLayerTblRcd->upgradeOpen();
			pLayerTblRcd->setIsLocked(bisLock);
			pLayerTblRcd->downgradeOpen();
			bRC = TRUE;
			break;
		}
		pLayerTblRcd->close();
		acutDelString(pLName);
	}
	delete pLayerIterator;
	pLayerTbl->close();
	return bRC;
}

//도면층 컬러 가져오기
int CWKUtils::GetlayerColor(AcDbObjectId LyId){
	int colorindex = -1;
	AcDbLayerTableRecordPointer layerTableRec(LyId,  AcDb::kForRead);
	if(layerTableRec.openStatus() == Acad::eOk){
		AcCmColor color = layerTableRec->color();
		colorindex = color.colorIndex();
	}
	return colorindex;
}

//도면층 컬러 가져오기
BOOL CWKUtils::SetlayerColor(int ColorIndex,AcDbObjectId LyId){
	BOOL bRC = FALSE;
	AcDbLayerTableRecordPointer layerTableRec(LyId,  AcDb::kForWrite);
	if(layerTableRec.openStatus() == Acad::eOk){
		AcCmColor color;
		color.setColorIndex(ColorIndex);
		layerTableRec->setColor(color);
		layerTableRec->close();
		bRC =TRUE;
	}		
	return bRC;
}

//도면층 생성
BOOL CWKUtils::createLayer(CString name, int ColorIndex, AcDbObjectId& LyId){
	BOOL BRc = FALSE;
	AcDbLayerTable *pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTbl, AcDb::kForWrite);

	if(!pLayerTbl->has(name)){
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(name);
		AcCmColor color;
		color.setColorIndex(ColorIndex); 
		pLayerTblRcd->setColor(color);
		pLayerTbl->add(pLayerTblRcd);
		LyId = pLayerTblRcd->id();
		pLayerTblRcd->close();
		BRc = TRUE;
	}
	pLayerTbl->close();
	return BRc;
}

Acad::ErrorStatus CWKUtils::postToDatabase (AcDbEntity *pEnt, AcDbObjectId& EntID)
{
	LockDocManager EnableModifyAcDbEntityAttribute;
	//AcDbObjectId idObj;
	Acad::ErrorStatus es;
	AcDbBlockTable *pBlockTable;
	AcDbBlockTableRecord *pSpaceRecord;
	AcDbDatabase *pDb =	acdbHostApplicationServices()->workingDatabase();

	es = pDb->getBlockTable(pBlockTable,AcDb::kForRead);

	if (es == Acad::eOk )
	{
		es = pBlockTable->getAt(ACDB_MODEL_SPACE,
			pSpaceRecord, AcDb::kForWrite);

		if ( es == Acad::eOk )
		{
			es = pSpaceRecord->appendAcDbEntity (EntID, pEnt);
			pSpaceRecord->close ();
			pEnt->close();
		}
		pBlockTable->close ();

	}
	return es;
}


void CWKUtils::Transform(AcGePoint3d& pt, AcGeVector3d vec)
{
	LockDocManager EnableModifyAcDbEntityAttribute;		// 객체 속성 수정이 가능하도록 한다
	AcGeMatrix3d mat;
	mat.setToTranslation(vec);
	pt.transformBy(mat);
}


void CWKUtils::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z );		
}

void CWKUtils::Polar(const AcGePoint3d &base, const AcGeVector3d &vector, double dist, AcGePoint3d &result)
{
	LockDocManager EnableModifyAcDbEntityAttribute;		// 객체 속성 수정이 가능하도록 한다
	AcGeVector3d v=vector;
	result=base;
	v.normalize();
	Transform(result, v*dist);
}

void CWKUtils::Polar(const AcGePoint3d &base, const AcGePoint3d &toPnt, double dist, AcGePoint3d &result)
{
	LockDocManager EnableModifyAcDbEntityAttribute;		// 객체 속성 수정이 가능하도록 한다
	AcGeVector3d v;
	v=toPnt-base;
	Polar(base, v, dist, result);
}


double CWKUtils::RTD(const double rads)
{
	return rads * (180.0 / PI);
}


double CWKUtils::DTR(const double degrees)
{
	return degrees * (PI / 180.0);
}
//사용법 createTextStyle(_T("SIMSUN.TTC",_T(""),"KFW");
AcDbObjectId CWKUtils::CreateTextStyle(CString fontName,CString bigFontName,CString textStyleName)
{
	LockDocManager EnableModifyAcDbEntityAttribute;		// 객체 속성 수정이 가능하도록 한다
	AcGiTextStyle *TextStyle=new AcGiTextStyle
		(fontName,
		bigFontName,
		0,										//textSize
		1.0,									//scale
		0,										//obliqueAngle
		0,										//Input tracking percent
		Adesk::kFalse,							//isBackward
		Adesk::kFalse,							//isUpsideDown
		Adesk::kFalse,							// isVertical
		Adesk::kFalse,							// isOverlined
		Adesk::kFalse,							//isUnderlined
		Adesk::kFalse,
		textStyleName);							//字体名
#if defined _R21 || _R22|| _R23|| _R23D1
	TextStyle->setFont(fontName, false, false, Charset::kUnicodeCharset,Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kDefault, Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kDoNotCare);
#else
	TextStyle->setFont(fontName, false, false, Charset::kUnicodeCharset, Autodesk::AutoCAD::PAL::FontUtils::FontPitch::kDefault, Autodesk::AutoCAD::PAL::FontUtils::FontFamily::kDoNotCare);
#endif // _R21


	AcDbObjectId textStyleId;
	toAcDbTextStyle(*TextStyle,textStyleId);
	return textStyleId;
}

BOOL CWKUtils::GetTextStyle(CString& name, AcDbObjectId& Style)
{
	LockDocManager EnableModifyAcDbEntityAttribute;		// 객체 속성 수정이 가능하도록 한다
	BOOL bRC = FALSE;
	// We need to get the BlockTable in order to retrieve all the blocks
	AcDbDatabase* pDwg=acdbHostApplicationServices()->workingDatabase();
	AcDbTextStyleTable* pTable;
	if (pDwg->getTextStyleTable (pTable, AcDb::kForRead) != Acad::eOk) {
		acutPrintf(L"\nCouldn't get the Text style table");
		return bRC;
	}

	// Iterate block table
	AcDbTextStyleTableIterator *piter =NULL ;
	if (pTable->newIterator(piter) == Acad::eOk) {
		AcDbTextStyleTableRecord *pRec;
		while (!piter->done()) {
			piter->getRecord(pRec, AcDb::kForRead) ;
			ACHAR *name1 ;
			pRec->getName(name1) ;
			pRec->close() ;
			CString strLayerName(name1);
			/*delete name1;*/
			if(strLayerName.CompareNoCase(name) ==0)
			{
				Style = pRec->id();
				bRC = TRUE;
				break;


			}

			piter->step () ;
		}
		delete piter ;
	}

	// end
	pTable->close () ;
	return bRC;
}
//도면층의 이름으로 도면층 가져오기 
BOOL CWKUtils::FindLayer(CString name, AcDbObjectId& LyId){
	BOOL bRC = FALSE;
	AcDbLayerTable* pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);
	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for(; !pLayerIterator->done(); pLayerIterator->step()){
		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		if(name.CompareNoCase(pLName) == 0){
			LyId = pLayerTblRcd->id();
			bRC = TRUE;
		}
		pLayerTblRcd->close();
		acutDelString(pLName);
	}
	delete pLayerIterator;
	pLayerTbl->close();
	return bRC;
}

//모든 도면층 모으기
void CWKUtils::iterateLayers(AcDbObjectIdArray& layerarr){
	layerarr.removeAll();
	AcDbLayerTable* pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);
	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for(; !pLayerIterator->done(); pLayerIterator->step()){
		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		layerarr.append(pLayerTblRcd->id());
		pLayerTblRcd->close();
		acutDelString(pLName);
	}
	delete pLayerIterator;
	pLayerTbl->close();
}

//현재 도면에 올라몬  도면층 모으기 Xref 제외
void CWKUtils::OnlyCurDwgiterateLayers(AcDbObjectIdArray& layerarr){
	layerarr.removeAll();
	AcDbLayerTable* pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);
	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for(; !pLayerIterator->done(); pLayerIterator->step()){
		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		if(false == pLayerTblRcd->isDependent()){
			layerarr.append(pLayerTblRcd->id());
		}		
		pLayerTblRcd->close();
		acutDelString(pLName);
	}
	delete pLayerIterator;
	pLayerTbl->close();
}

//모든 도면층 모으기
void CWKUtils::XRefiterateLayers(AcDbObjectIdArray& layerarr, CString XRefName){
	layerarr.removeAll();
	AcDbLayerTable* pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);
	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for(; !pLayerIterator->done(); pLayerIterator->step()){
		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		CString name(pLName);
		if(name.Find(XRefName) > -1){
			layerarr.append(pLayerTblRcd->id());
		}
		pLayerTblRcd->close();
		acutDelString(pLName);
	}
	delete pLayerIterator;
	pLayerTbl->close();
}


//모든 도면층 모으기
void CWKUtils::GetAllLayers(CStringArray& lst){
	lst.RemoveAll();
	AcDbLayerTable* pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);
	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for(; !pLayerIterator->done(); pLayerIterator->step()){
		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		lst.Add(pLName);
		pLayerTblRcd->close();
		acutDelString(pLName);
	}
	delete pLayerIterator;
	pLayerTbl->close();
}


//블록의 이름 가져오기
CString CWKUtils::getBlockReferenceName(AcDbBlockReference* pBlkRef){
	ACHAR* szBlkName;
	CString sBlkRefName = _T("");
	AcDbBlockTableRecord *pBlkRec = NULL;
	if(Acad::eOk == acdbOpenObject(pBlkRec, pBlkRef->blockTableRecord(), AcDb::kForRead)){
		pBlkRec->getName(szBlkName);
		pBlkRec->close();
		sBlkRefName.Format(_T("%s"), szBlkName);
	}
	return sBlkRefName;
}

//텍스트 스타일 ID 얻기
AcDbObjectId CWKUtils::GetTextStyleId(CString sTextStyleName){
	AcDbTextStyleTable *pTable;
	AcDbObjectId textStyleId = AcDbObjectId::kNull;
	Acad::ErrorStatus es = Acad::eOk;
	if((es = acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTable, AcDb::kForRead)) == Acad::eOk){
		if((es = pTable->getAt(sTextStyleName, textStyleId, Adesk::kFalse)) != Acad::eOk){
			textStyleId = AcDbObjectId::kNull;
		}
		pTable->close();
	}
	return textStyleId;
}



void CWKUtils::GetTextStyleName(AcDbObjectId idText, CString& strStyleName){
	AcDbEntityPointer pEnt(idText, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){
		
		AcDbText* pText = AcDbText::cast(pEnt);
		if(pText){
			AcGiTextStyle giTextStyle;
			fromAcDbTextStyle(giTextStyle, pText->textStyle());
			strStyleName = giTextStyle.styleName();
			pText->close();
		}
		pEnt->close();
	}

}

//텍스트의 너비 얻어오기
double CWKUtils::GetTextWidth(CString sText, CString sTextStyle, double dTextHeight){
	AcDbObjectId idTextStyle = GetTextStyleId(sTextStyle);
	AcGiTextStyle giTextStyle;
	fromAcDbTextStyle(giTextStyle, idTextStyle);
	giTextStyle.setTextSize(dTextHeight);
	AcGePoint2d ptExt = giTextStyle.extents(sText.GetBuffer(), Adesk::kFalse, sText.GetLength(), Adesk::kFalse);
	return ptExt.x;
}

double CWKUtils::GetTextWidth(AcDbObjectId idText){
	ads_point ptLowerLeft, ptUpperRight;
	ads_name ent;
	struct resbuf * textent;
	acdbGetAdsName(ent, idText);
	textent = acdbEntGet(ent);
	acedTextBox(textent, ptLowerLeft, ptUpperRight);
	return ptUpperRight[X] - ptLowerLeft[X];
}

//텍스트 박스 얻기
AcGePoint3dArray CWKUtils::GetTextBox(AcDbObjectId idText){
	AcGePoint3dArray arTextBox;
	ads_point ptLowerLeft, ptUpperRight;
	struct resbuf* textent;
	ads_name ent;
	acdbGetAdsName(ent, idText);
	textent = acdbEntGet(ent);
	acedTextBox(textent, ptLowerLeft, ptUpperRight);
	arTextBox.append(AcGePoint3d(ptLowerLeft[X], ptLowerLeft[Y], 0.0));
	arTextBox.append(AcGePoint3d(ptUpperRight[X], ptUpperRight[Y], 0.0));
	return arTextBox;
}

//블록 내의 텍스트 박스만 모은다.
AcGePoint3dArray CWKUtils::GetAttributeBlkRefVisibleBox(AcDbBlockReference* pBlockRef){
	AcGePoint3dArray arTextBox, arSingleTextBox;
	AcDbObjectId idBlkDef;
	AcDbBlockTableRecord* pBTR;
	AcDbAttribute* pAttr;
	AcDbObjectIterator* pAttrIterator;
	AcGePoint3d ptInsert, ptTemp;
	double dRotation;

	idBlkDef = pBlockRef->blockTableRecord();
	acdbOpenObject((AcDbObject *&)pBTR, idBlkDef, AcDb::kForRead);
	if(pBTR->hasAttributeDefinitions()){
		pAttrIterator = pBlockRef->attributeIterator();
		dRotation = pBlockRef->rotation();
		for(; !pAttrIterator->done(); pAttrIterator->step()){
			AcDbObjectId attrObjId;
			attrObjId = pAttrIterator->objectId();
			pBlockRef->openAttribute((AcDbAttribute*&)pAttr, attrObjId, AcDb::kForRead);
			if((pAttr->isInvisible() == Adesk::kFalse) && (wcscmp(pAttr->textString(), _T("")) > 0)){
				ptInsert = pAttr->position();
				ptInsert.rotateBy(-dRotation, AcGeVector3d::kZAxis, pBlockRef->position());
				arSingleTextBox.setLogicalLength(0);
				arSingleTextBox = GetTextBox(pAttr->objectId());
				for(int i = 0; i < arSingleTextBox.length(); i ++){
					ptTemp = arSingleTextBox.at(i);
					ptTemp += ptInsert.asVector();
					arSingleTextBox.removeAt(i);
					arSingleTextBox.insertAt(i, ptTemp);
				}
				arTextBox.append(arSingleTextBox);
			}
			pAttr->close();
		}
		delete pAttrIterator;
	}
	pBTR->close();

	AcGePoint3d ptLL, ptUR;
	ptLL = arTextBox.at(0);
	if(arTextBox.length() > 0){
		for(int i = 1; i < arTextBox.length(); i ++){
			ptTemp = arTextBox.at(i);
			if(ptTemp.y < ptLL.y)
				ptLL.y = ptTemp.y;
		}
		for(int i = 1; i < arTextBox.length(); i ++){
			ptTemp = arTextBox.at(i);
			if(ptTemp.x < ptLL.x)
				ptLL.x = ptTemp.x;
		}
	}
	ptUR = arTextBox.at(0);
	if(arTextBox.length() > 0){
		for(int i = 1; i < arTextBox.length(); i ++){
			ptTemp = arTextBox.at(i);
			if(ptTemp.y > ptUR.y)
				ptUR.y = ptTemp.y;
		}
		for(int i = 1; i < arTextBox.length(); i ++){
			ptTemp = arTextBox.at(i);
			if(ptTemp.x > ptUR.x)
				ptUR.x = ptTemp.x;
		}
	}

	ptLL.rotateBy(dRotation, AcGeVector3d::kZAxis, pBlockRef->position());
	ptUR.rotateBy(dRotation, AcGeVector3d::kZAxis, pBlockRef->position());
	arTextBox.setLogicalLength(0);
	arTextBox.append(ptLL);
	arTextBox.append(ptUR);
	return arTextBox;
}

void CWKUtils::GetBlockReferenceAttributeList(AcDbObjectId blkid, CArray<AttBlock,AttBlock>& sAttributeTag)
{

	sAttributeTag.RemoveAll();


	AcDbEntityPointer pEnt(blkid, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){		
		AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pEnt);
		if(pBlockRef){
			AcDbObjectId idBlkDef;
			AcDbBlockTableRecord* pBTR;
			AcDbAttribute* pAttr;
			AcDbObjectIterator* pAttrIterator;
			idBlkDef = pBlockRef->blockTableRecord();
			acdbOpenObject((AcDbObject*&)pBTR, idBlkDef, AcDb::kForRead);
			if(pBTR->hasAttributeDefinitions()){
				pAttrIterator = pBlockRef->attributeIterator();
				for(; !pAttrIterator->done(); pAttrIterator->step()){
					AcDbObjectId attrObjId;
					attrObjId = pAttrIterator->objectId();
					pBlockRef->openAttribute((AcDbAttribute*&)pAttr, attrObjId, AcDb::kForRead);

					AttBlock att;
					att.AttributeTag = pAttr->tag();
					att.strValue = pAttr->textString();
					sAttributeTag.Add(att);
					pAttr->close();
				}
				delete pAttrIterator;
			}
			pBTR->close();
		}
		pEnt->close();
	}

}

void CWKUtils::GetBlockReferenceAttributeList(AcDbObjectId blkid, AttBlockArray& sAttributeTag)
{

	sAttributeTag.removeAll();


	AcDbEntityPointer pEnt(blkid, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){		
		AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pEnt);
		if(pBlockRef){
			AcDbObjectId idBlkDef;
			AcDbBlockTableRecord* pBTR;
			AcDbAttribute* pAttr;
			AcDbObjectIterator* pAttrIterator;
			idBlkDef = pBlockRef->blockTableRecord();
			acdbOpenObject((AcDbObject*&)pBTR, idBlkDef, AcDb::kForRead);
			if(pBTR->hasAttributeDefinitions()){
				pAttrIterator = pBlockRef->attributeIterator();
				for(; !pAttrIterator->done(); pAttrIterator->step()){
					AcDbObjectId attrObjId;
					attrObjId = pAttrIterator->objectId();
					pBlockRef->openAttribute((AcDbAttribute*&)pAttr, attrObjId, AcDb::kForRead);

					AttBlock att;
					att.AttributeTag = pAttr->tag();
					att.strValue = pAttr->textString();
					
					sAttributeTag.append(att);
					pAttr->close();
				}
				delete pAttrIterator;
			}
			pBTR->close();
			pBlockRef->close();
		}
		pEnt->close();
	}

}


void CWKUtils::SetBlockReferenceAttribute(AcDbObjectId blkid, CString sAttributeTag, CString strValue)
{

	AcDbEntityPointer pEnt(blkid, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){		
		AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pEnt);
		if(pBlockRef){
			AcDbObjectId idBlkDef;
			AcDbBlockTableRecord* pBTR;
			AcDbAttribute* pAttr;
			AcDbObjectIterator* pAttrIterator;
			idBlkDef = pBlockRef->blockTableRecord();
			acdbOpenObject((AcDbObject*&)pBTR, idBlkDef, AcDb::kForRead);
			if(pBTR->hasAttributeDefinitions()){
				pAttrIterator = pBlockRef->attributeIterator();
				for(; !pAttrIterator->done(); pAttrIterator->step()){
					AcDbObjectId attrObjId;
					attrObjId = pAttrIterator->objectId();
					pBlockRef->openAttribute((AcDbAttribute*&)pAttr, attrObjId, AcDb::kForRead);

					if(sAttributeTag.CompareNoCase(pAttr->tag())==0)
					{
						pAttr->upgradeOpen();
						pAttr->setTextString(strValue);
						pAttr->downgradeOpen();

					}
					pAttr->close();
				}
				delete pAttrIterator;
			}
			pBTR->close();
		}
		pEnt->close();
	}

}


//블록에서 특정 태그만 뽑아 오기
AcGePoint3dArray CWKUtils::GetBlockReferenceAttributeTextBox(AcDbBlockReference *pBlockRef, CString sAttributeTag){
	AcGePoint3dArray arTextBox;
	AcDbObjectId idBlkDef;
	AcDbBlockTableRecord* pBTR;
	AcDbAttribute* pAttr;
	AcDbObjectIterator* pAttrIterator;
	idBlkDef = pBlockRef->blockTableRecord();
	acdbOpenObject((AcDbObject*&)pBTR, idBlkDef, AcDb::kForRead);
	if(pBTR->hasAttributeDefinitions()){
		pAttrIterator = pBlockRef->attributeIterator();
		for(; !pAttrIterator->done(); pAttrIterator->step()){
			AcDbObjectId attrObjId;
			attrObjId = pAttrIterator->objectId();
			pBlockRef->openAttribute((AcDbAttribute*&)pAttr, attrObjId, AcDb::kForRead);
			if(!sAttributeTag.CompareNoCase(pAttr->tag())){
				arTextBox = GetTextBox(pAttr->objectId());
				pAttr->close();
				break;
			}
			pAttr->close();
		}
		delete pAttrIterator;
	}
	pBTR->close();

	AcGePoint3d ptTemp, ptInsert = pAttr->position();
	double dRotation = pBlockRef->rotation();
	AcGeVector3d vec;
	for(int i = 0; i < arTextBox.length(); i ++){
		ptTemp = arTextBox.at(i);
		vec = AcGeVector3d(ptTemp.x, ptTemp.y, 0.0);
		vec.rotateBy(dRotation, AcGeVector3d::kZAxis);
		vec += ptInsert.asVector();
		arTextBox.removeAt(i);
		arTextBox.insertAt(i, AcGePoint3d(vec.x, vec.y, 0.0));
	}
	return arTextBox;
}

//현제 도면에 엔티티 삽입하기
Acad::ErrorStatus CWKUtils::AddEntityToModelSpace(AcDbObjectId &objId, AcDbEntity* pEntity, bool bCloseEntity){
	Acad::ErrorStatus es = Acad::eOk;
	AcApDocument *pDoc = acDocManager->curDocument();
	es = acDocManager->lockDocument(pDoc, AcAp::kWrite);
	AcDbBlockTable* pBlockTable;
	AcDbBlockTableRecord* pSpaceRecord;

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite);
	es = pSpaceRecord->appendAcDbEntity(objId, pEntity);

	pBlockTable->close();
	pSpaceRecord->close();
	if(bCloseEntity)
		pEntity->close();

	es = acDocManager->unlockDocument(pDoc);
	return es;
}

void CWKUtils::getLayerName(AcDbObjectId idLayer, CString& strLayerName){
	AcDbLayerTableRecordPointer layerTableRec(idLayer, AcDb::kForRead);
	if(layerTableRec.openStatus() == Acad::eOk){
		ACHAR* pLayerName;
		layerTableRec->getName(pLayerName);
		strLayerName=pLayerName;
		delete[] pLayerName;
		layerTableRec->close();	
	}
}

void CWKUtils::setLayerName(AcDbObjectId idLayer, CString& strLayerName){
	AcDbLayerTableRecordPointer layerTableRec(idLayer, AcDb::kForRead);
	if(layerTableRec.openStatus() == Acad::eOk){
		ACHAR* pLayerName;
		layerTableRec->getName(pLayerName);
		strLayerName=pLayerName;
		delete[] pLayerName;
		layerTableRec->close();	
	}
}


void CWKUtils::GetAllEntity(CString LayerName, AcDbObjectIdArray& entities)
{
	//LockDocManager EnableModifyAcDbEntityAttribute;		// 객체 속성 수정이 가능하도록 한다
	// 레코드 전체 조회 
	// 주어진 클래스 유형의 객체 아이디 목록을 만든다.

	//CString strName = _T("kdcsSeismicSupport");

	entities.removeAll();
	AcDbObjectIdArray ids;

	AcDbBlockTable* pTable;  
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pTable, AcDb::kForRead);
	if(pTable == NULL) return;  
	AcDbBlockTableIterator* pTableIter;
	for( pTable->newIterator(pTableIter); !pTableIter->done(); pTableIter->step())
	{    
		//PumpAcadMessages();
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
			//PumpAcadMessages();
			AcDbEntity*pEnt;     
			pRecordIter->getEntity(pEnt, AcDb::kForRead); 
			if(pEnt != NULL ) 
			{ 
				if(LayerName.CompareNoCase(pEnt->layer()) ==0)
					entities.append(pEnt->objectId());
				pEnt->close();     
			}     
		}    
		delete pRecordIter;    
		pRecord->close();  

	}   
	delete pTableIter; 
	pTable->close();


}


//도면층에 걸린 모든 객체를 삭제하자
long CWKUtils::MoveAllEntityInLayer(CString &strSoureLyName, AcDbObjectId& TargetLyId, AcDbDatabase* pDB){  
	ASSERT(lpLayerName != NULL);
	if(strSoureLyName.CompareNoCase(_T("")) == 0)
		return Acad::eInvalidInput;

	SetlayerLock(pDB,strSoureLyName,false);
	AcDbObjectIdArray IdArray;

	GetAllEntity(strSoureLyName,IdArray);
	if(IdArray.length() <= 0)
		return Acad::eInvalidInput;

	int nLenNum=IdArray.logicalLength();
	if(nLenNum <= 0)
		return 0;

	AcDbObjectId eId;
	AcDbEntity *pE;

	for(int i = 0; i < nLenNum; i++){
		eId = IdArray.at(i);
		Acad::ErrorStatus es =   acdbOpenObject(pE, eId, AcDb::kForWrite);
		if(Acad::eOk == es){
		es =	pE->setLayer(TargetLyId);
			pE->close();
		}
	}
	IdArray.setLogicalLength(0);
	return 1;
}

//도면층에 걸린 모든 객체를 삭제하자
long CWKUtils::DeleteAllEntityInLayer(CString &strLyName,AcDbDatabase* pDB){
	ASSERT(lpLayerName != NULL);
	if(strLyName.CompareNoCase(_T("")) == 0)
		return Acad::eInvalidInput;

	AcDbObjectIdArray IdArray;
	if(SelectEntityInLayer(IdArray,strLyName) <= 0)
		return Acad::eInvalidInput;

	int nLenNum = IdArray.logicalLength();
	if(nLenNum <= 0)
		return 0;

	AcDbObjectId eId;
	AcDbEntity *pE;

	for(int i = 0; i < nLenNum; i++){
		eId = IdArray.at(i);
		if(Acad::eOk == acdbOpenObject(pE, eId, AcDb::kForWrite)){
			pE->erase(); pE->close();
		}
	}
	IdArray.setLogicalLength(0);
	return 1;
}

//도면층에 걸린 모든 객체를 삭제하자
long CWKUtils::DeleteAllEntityInLayer(AcDbObjectId &LyId, AcDbDatabase* pDB){  
	CString strLyName = _T("");
	getLayerName(LyId,strLyName);
	if(strLyName.CompareNoCase(_T("")) == 0)
		return 0;
	ASSERT(lpLayerName != NULL);
	if(strLyName.CompareNoCase(_T("")) == 0)
		return Acad::eInvalidInput;

	AcDbObjectIdArray IdArray;
	if(SelectEntityInLayer(IdArray, strLyName) <= 0)
		return Acad::eInvalidInput;

	int nLenNum = IdArray.logicalLength();
	if(nLenNum <= 0)
		return 0;

	AcDbObjectId eId;
	AcDbEntity *pE;

	for(int i = 0; i < nLenNum; i++){
		eId=IdArray.at(i);
		if(Acad::eOk == acdbOpenObject(pE, eId, AcDb::kForWrite)){
			pE->erase();
			pE->close();
		}
	}
	IdArray.setLogicalLength(0);
	return 1;
}

long CWKUtils::SelectEntityInLayer(AcDbObjectIdArray& IdArray, CString& strLyName){
	IdArray.setLogicalLength(0);
	int nlen = wcslen(strLyName) + 1;
	ads_name ents;
	struct resbuf *rb;
	rb = acutNewRb(AcDb::kDxfLayerName);
	rb->restype = AcDb::kDxfLayerName;
	rb->resval.rstring = (ACHAR*)malloc(nlen * sizeof(ACHAR));
	wcscpy(rb->resval.rstring, strLyName);
	rb->rbnext = NULL;
	acedSSGet(_T("X"), NULL, NULL, rb, ents);
	// [+] SUP-29742
	// long	entNums = 0;
	// [-] END
	Adesk::Int32 entNums = 0;
	acedSSLength(ents, &entNums);
	if(entNums == 0)
		return 0;
	else{
		for(long a = 0; a < entNums; a++){
			AcDbObjectId objId;
			ads_name ent;
			acedSSName(ents, a, ent);
			acdbGetObjectId(objId, ent);
			IdArray.append(objId);
		}
	}
	acedSSFree(ents);
	acutRelRb(rb);
	return 1;
}

//특정 조건에 도면층를 가져온다.
//sourcearr= 현재 도면의 전체 도면층
//targetarr = 조건에 맞는 도면층만 모아서 보내줄 배열
//bIsFrozen = 동결된 도면층를 모을것인가?
//bIsOff = 꺼진 도면층를 모을것인가?
void CWKUtils::LayerFilter(AcDbObjectIdArray& sourcearr, AcDbObjectIdArray& targetarr, BOOL bIsFrozen, BOOL bIsOff){
	Acad::ErrorStatus es;
	targetarr.removeAll();
	for(int i = 0; i < sourcearr.length(); i++){
		AcDbLayerTableRecordPointer layerTableRec(sourcearr[i], AcDb::kForRead);
		if(layerTableRec.openStatus() == Acad::eOk){
			if(bIsFrozen){
				if(layerTableRec->isFrozen()){
					targetarr.append(layerTableRec->id());
				}
			}
			if(bIsOff){
				if(layerTableRec->isOff()){
					targetarr.append(layerTableRec->id());
				}
			}
		}
	}
}

void CWKUtils::LayerFilterDelete(void){
	AcDbDatabase* pDb = new AcDbDatabase(Adesk::kFalse);
	pDb->readDwgFile(_T("c:\\drawing1.dwg"));
	pDb->closeInput(true);
	AcDbLayerTablePointer pLyrTbl(pDb->layerTableId(), AcDb::kForRead);
	if(pLyrTbl.openStatus() == Acad::eOk){
		AcDbObjectId pExtDicId = pLyrTbl->extensionDictionary();
		if(pExtDicId.isValid()){
			AcDbDictionaryPointer pDict(pExtDicId, AcDb::kForRead);
			if(Acad::eOk == pDict.openStatus()){
				AcDbObjectId pObjId;
				pDict->getAt(_T("ACAD_LAYERFILTERS"), pObjId);
				if(pObjId.isValid()){
					AcDbDictionaryPointer pFiltDict(pObjId, AcDb::kForRead);
					if(Acad::eOk == pFiltDict.openStatus()){
						AcDbObjectId filtObjId;
						AcDbDictionaryIterator *pItr = pFiltDict->newIterator();
						if(NULL != pItr){
							for(; !pItr->done(); pItr->next()){
								AcDbObjectPointer<AcDbXrecord> pLyrFltr(pItr->objectId(), AcDb::kForWrite);
								if(Acad::eOk == pLyrFltr.openStatus()){
									pLyrFltr->erase();
								}
							}
							delete pItr;
						}
					}
				}    
				pDict->getAt(_T("ACLYDICTIONARY"), pObjId);
				if(pObjId.isValid()){
					AcDbDictionaryPointer pFiltDict(pObjId, AcDb::kForRead);
					if(Acad::eOk == pFiltDict.openStatus()){
						AcDbObjectId filtObjId;
						AcDbDictionaryIterator* pItr = pFiltDict->newIterator();
						if(NULL != pItr){
							for(; !pItr->done(); pItr->next()){
								AcDbObjectPointer<AcDbXrecord> pLyrFltr(pItr->objectId(), AcDb::kForWrite);
								if(Acad::eOk == pLyrFltr.openStatus()){
									pLyrFltr->erase();
								}
							}
							delete pItr;
						}
					}
				}
			}
		}
	}
	Acad::ErrorStatus es = pDb->saveAs(_T("c:\\drawing1.dwg"));
	delete pDb; 
}

void CWKUtils::layersReferencedPurge(){
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	AcDbLayerTablePointer layerTable(db, AcDb::kForRead);
	AcDbLayerTableIterator* layerTableIterator;
	layerTable->newIterator(layerTableIterator);

	AcDbObjectId id;
	AcDbObjectIdArray layersNotReferenced;
	AcDbObjectIdArray layersReferenced;
	for(; !layerTableIterator->done (); layerTableIterator->step()){
		layerTableIterator->getRecordId(id);
		layersNotReferenced.append(id);
		layersReferenced.append(id);
	}
	delete layerTableIterator;

	if(db->purge(layersNotReferenced) != Acad::eOk){
		acutPrintf (_T("\nPurge failed!"));
		return;
	}
	acutPrintf (_T("\nLayers not used"));
	for(int i = 0; i < layersNotReferenced.length(); ++i){
		id = layersNotReferenced[i];
		AcDbLayerTableRecordPointer layerTableRecord(id, AcDb::kForRead);
		if(layerTableRecord.openStatus() == Acad::eOk){
			TCHAR* name;
			layerTableRecord->getName(name);
			acutPrintf(_T("\nLayername : %s"), name);
		}
	}
	for(int i = 0; i < layersNotReferenced.length(); ++i)
		layersReferenced.remove(layersNotReferenced[i]);

	acutPrintf(_T("\nLayers used"));
	for(int i = 0; i < layersReferenced.length(); ++i){
		id = layersNotReferenced[i];
		AcDbLayerTableRecordPointer layerTableRecord(id, AcDb::kForRead);
		if(layerTableRecord.openStatus() == Acad::eOk){
			TCHAR* name;
			layerTableRecord->getName(name);
			acutPrintf(_T("\nLayername : %s"), name);
		}
	}
}

BOOL CWKUtils::PurgeLayer(AcDbObjectIdArray& ids){
	BOOL bRC = FALSE;
	for(int i = 0; i < ids.length(); i++){
		AcDbDatabase *dwg = acdbHostApplicationServices()->workingDatabase();
		AcDbLayerTableRecordPointer pE(ids[i], AcDb::kForWrite);
		if(pE.openStatus() == Acad::eOk){
			pE->erase();
			pE->close();
			bRC = TRUE;
		}
	}
	return bRC;
}

//선택한 객체의 도면층만 복원을 클릭 했을때 사용자에게 외부참조된 폼속의 객체의 도면층와 폼의 경로를 얻어오자
BOOL CWKUtils::GetXrefObjectLayerAndXrefPath(CString& LayerName,CString& XrefPath){
	BOOL bRC = FALSE;
	ads_point ptres;
	ads_point xformres[4];
	struct resbuf* refstkres;
	ads_name ent;
	if(acedNEntSel(NULL, ent, ptres, xformres, &refstkres) != RTNORM)
		return FALSE;

	AcDbObjectId mObjId;
	acdbGetObjectId(mObjId, ent);
	AcDbEntity* pEnt = NULL;
	if(Acad::eOk == acdbOpenObject(pEnt, mObjId, AcDb::kForRead)){
		LayerName = pEnt->layer();
		pEnt->close();
	}

	//객체의 부모객체인 부모블록의 경로를 가져온다.
	if(NULL != refstkres){
		ACHAR* Name;
		int nBlkRefCol;
		AcDbObjectId id;
		acdbGetObjectId(id, refstkres->resval.rlname);
		if(Acad::eOk != acdbOpenObject(pEnt, id, AcDb::kForRead))
			return FALSE;

		if(pEnt->isA() == AcDbBlockReference::desc()){
			AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
			if(pSelBlockRef){
				AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
				if(pBTR.openStatus() == Acad::eOk){
					if(pBTR->isFromExternalReference()){
						pBTR->pathName(Name);
						XrefPath = Name;
						bRC = TRUE;
					}
					pBTR->close();
				}
			}
		} 
		pEnt->close();
	}
	if(NULL != refstkres)
		acutRelRb(refstkres);
	return bRC;
}

CString CWKUtils::XrefGetPath(AcDbObjectId id){
	ACHAR* path = NULL;

	AcDbEntityPointer pEnt(id, AcDb::kForRead);
	if(pEnt.openStatus() == Acad::eOk){
		AcDbBlockReference* pSelBlockRef = AcDbBlockReference::cast(pEnt);
		if(pSelBlockRef){
			AcDbBlockTableRecordPointer pBTR(pSelBlockRef->blockTableRecord(), AcDb::kForRead);
			if(pBTR.openStatus() == Acad::eOk){
				if(pBTR->isFromExternalReference()){
					pBTR->pathName(path);
				}
				pBTR->close() ;
			}
		}
		pEnt->close();
	}
	return path;
}

void CWKUtils::GetColorPicker(int& newColorIndex){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	int nCurColor = oldColor.colorIndex();
	int nNewColor = oldColor.colorIndex();

	acedSetColorDialog(nNewColor, Adesk :: kTrue, nCurColor); 
	newColorIndex = nNewColor;
}

void CWKUtils::GetTrueColorPicker(int& newColorIndex, AcDbObjectId LyId){
	AcCmColor NewColor;
	AcCmColor OldColor;

	if(LyId == NULL)
		LyId = acdbHostApplicationServices()->workingDatabase()->clayer();

	AcDbLayerTableRecordPointer ptLayer(LyId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	ptLayer->close();

	acedSetColorDialogTrueColor(NewColor, Adesk :: kTrue, OldColor, AcCm::kTrueColorTab); 
	newColorIndex = NewColor.colorIndex();
}

CString CWKUtils::GetLineTypeName(AcDbObjectId& linetype_id){
	CString LIneTypeName;
	AcDbObject* pObj2;
	if(Acad::eOk == acdbOpenAcDbObject(pObj2,linetype_id, AcDb::kForRead)){
		AcDbLinetypeTableRecord* pLinetype = AcDbLinetypeTableRecord::cast(pObj2);
		if(NULL != pLinetype){
			ACHAR* linetypeName;
			pLinetype->getName(linetypeName);
			LIneTypeName = linetypeName;
			delete[] linetypeName;
			pObj2->close();
		}
	}
	return LIneTypeName;
}

//모든 라인 타입을 가져온다.
BOOL CWKUtils::GetAllLineType(AcDbObjectIdArray& layerarr){
	layerarr.removeAll();
	AcDbLinetypeTable* pLinetypeTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);

	AcDbLinetypeTableIterator* pLinetypeIterator;
	pLinetypeTbl->newIterator(pLinetypeIterator);

	AcDbLinetypeTableRecord* pLinetypeTblRcd;
	TCHAR* pLName;
	for(; !pLinetypeIterator->done(); pLinetypeIterator->step()){
		pLinetypeIterator->getRecord(pLinetypeTblRcd, AcDb::kForRead);
		pLinetypeTblRcd->getName(pLName);
		layerarr.append(pLinetypeTblRcd->id());
		pLinetypeTblRcd->close();
		acutDelString(pLName);
	}
	delete pLinetypeIterator;
	pLinetypeTbl->close();
	return TRUE;
}

//외부 파일에서 모든 라인 타입을 가져온다.
BOOL CWKUtils::GetAllLineType(AcDbObjectIdArray& layerarr, CString Path){
	Acad::ErrorStatus es;
	AcDbDatabase db(Adesk::kFalse);
	es = db.readDwgFile(Path, AcDbDatabase::kForReadAndAllShare);

	if(Acad::eOk == es){
		layerarr.removeAll();
		AcDbLinetypeTable* pLinetypeTbl = NULL;
		db.getLinetypeTable(pLinetypeTbl, AcDb::kForRead);
		AcDbLinetypeTableIterator* pLinetypeIterator;
		pLinetypeTbl->newIterator(pLinetypeIterator);
		AcDbLinetypeTableRecord* pLinetypeTblRcd;
		TCHAR* pLName;
		for(; !pLinetypeIterator->done(); pLinetypeIterator->step()){
			pLinetypeIterator->getRecord(pLinetypeTblRcd, AcDb::kForRead);
			pLinetypeTblRcd->getName(pLName);
			layerarr.append(pLinetypeTblRcd->id());
			pLinetypeTblRcd->close();
			acutDelString(pLName);
		}
		delete pLinetypeIterator;
		pLinetypeTbl->close();
	}
	return TRUE;
}

BOOL CWKUtils::GetAllLineType(AcDbDatabase* db, AcDbObjectIdArray& layerarr, CString Path){
	Acad::ErrorStatus es;
	layerarr.removeAll();
	AcDbLinetypeTable* pLinetypeTbl = NULL;
	db->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);
	AcDbLinetypeTableIterator* pLinetypeIterator;
	pLinetypeTbl->newIterator(pLinetypeIterator);
	AcDbLinetypeTableRecord* pLinetypeTblRcd;
	TCHAR* pLName;
	for(; !pLinetypeIterator->done(); pLinetypeIterator->step()){
		pLinetypeIterator->getRecord(pLinetypeTblRcd, AcDb::kForRead);
		pLinetypeTblRcd->getName(pLName);
		layerarr.append(pLinetypeTblRcd->id());
		pLinetypeTblRcd->close();
		acutDelString(pLName);
	}
	delete pLinetypeIterator;
	pLinetypeTbl->close();
	return TRUE;
}


BOOL CWKUtils::FileOpen(CString& PathName, CString& FileName){
	BOOL bRC = FALSE;
	CString szFilter = _T("도면층 설정 파일(*.lsd)|*.lsd|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	dlg.m_ofn.lpstrTitle = _T("도면층 상태 가져오기");

	if(dlg.DoModal() == IDOK){
		PathName = dlg.GetPathName(); // 전체 경로를 포함한 파일 이름
		FileName = dlg.GetFileName(); // 파일 이름
		bRC = TRUE;
	}

	return bRC;
}

BOOL CWKUtils::FileSave(CString DefaultFileName, CString& PathName, CString& FileName){
	BOOL bRC = FALSE;
	CString szFilter = _T("도면층 설정 파일(*.lsd)|*.lsd|");
	CFileDialog dlg(FALSE, _T("lsd"), DefaultFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	dlg.m_ofn.lpstrTitle =_T("도면층 상태 내보내기");

	if(dlg.DoModal() == IDOK){
		PathName = dlg.GetPathName(); // 전체 경로를 포함한 파일 이름
		FileName = dlg.GetFileName(); // 파일 이름
		bRC = TRUE;
	}
	return bRC;
}

BOOL CWKUtils::GetFolderPath(CString& strPath){
	BOOL bRC = FALSE;
	ITEMIDLIST* pidlBrowse;
	WCHAR pszPathname[MAX_PATH];

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = NULL;
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0x00, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select folder");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 사용자정의 함수에 넘겨질 인자로 사용자가 설정하고자 하는 경로를 설정한다.
	// 예를들어 초기폴더경로를 C드라이브로 설정하는 경우
	CString strInitPath = _T("C:\\");
	BrInfo.lParam = (LPARAM)strInitPath.GetBuffer();

	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if(pidlBrowse != NULL){
		SHGetPathFromIDList(pidlBrowse, pszPathname);
		strPath = pszPathname;
		bRC = TRUE;
	}
	return bRC;
}

//라인 웨이트 를 스트링으로 변경 한다
LPCTSTR CWKUtils::lineWeightTypeToStr(AcDb::LineWeight type, CString& str){
	if(type == AcDb::kLnWt000)	str = _T("0.00 mm");
	else if(type == AcDb::kLnWt005)	str = _T("0.05 mm");
	else if(type == AcDb::kLnWt009)	str = _T("0.09 mm");
	else if(type == AcDb::kLnWt013)	str = _T("0.13 mm");
	else if(type == AcDb::kLnWt015)	str = _T("0.15 mm");
	else if(type == AcDb::kLnWt018)	str = _T("0.18 mm");
	else if(type == AcDb::kLnWt020)	str = _T("0.20 mm");
	else if(type == AcDb::kLnWt025)	str = _T("0.25 mm");
	else if(type == AcDb::kLnWt030)	str = _T("0.30 mm");
	else if(type == AcDb::kLnWt035)	str = _T("0.35 mm");
	else if(type == AcDb::kLnWt040)	str = _T("0.40 mm");
	else if(type == AcDb::kLnWt050)	str = _T("0.50 mm");
	else if(type == AcDb::kLnWt053)	str = _T("0.53 mm");
	else if(type == AcDb::kLnWt060)	str = _T("0.60 mm");
	else if(type == AcDb::kLnWt070)	str = _T("0.70 mm");
	else if(type == AcDb::kLnWt080)	str = _T("0.80 mm");
	else if(type == AcDb::kLnWt090)	str = _T("0.90 mm");
	else if(type == AcDb::kLnWt100)	str = _T("1.00 mm");
	else if(type == AcDb::kLnWt106)	str = _T("1.06 mm");
	else if(type == AcDb::kLnWt120)	str = _T("1.20 mm");
	else if(type == AcDb::kLnWt140)	str = _T("1.40 mm");
	else if(type == AcDb::kLnWt158)	str = _T("1.58 mm");
	else if(type == AcDb::kLnWt200)	str = _T("2.00 mm");
	else if(type == AcDb::kLnWt211)	str = _T("2.11 mm");
	else if(type == AcDb::kLnWtByLayer)	str = _T("ByLayer");
	else if(type == AcDb::kLnWtByBlock)	str = _T("ByBlock");
	else if(type == AcDb::kLnWtByLwDefault)	str = _T("기본값");
	else{
		ASSERT(0);
		str = _T("*Unknown*");
	}
	return str;
}

int CWKUtils::StrTolineWeightType(CString str, AcDb::LineWeight& type){
	if(str.CompareNoCase(_T("0.00 mm")) == 0)	type = AcDb::kLnWt000;
	else if(str.CompareNoCase(_T("0.05 mm")) == 0)	type = AcDb::kLnWt005; 
	else if(str.CompareNoCase(_T("0.09 mm")) == 0)	type = AcDb::kLnWt009; 
	else if(str.CompareNoCase(_T("0.13 mm")) == 0)	type = AcDb::kLnWt013; 
	else if(str.CompareNoCase(_T("0.15 mm")) == 0)	type = AcDb::kLnWt015; 
	else if(str.CompareNoCase(_T("0.18 mm")) == 0)	type = AcDb::kLnWt018; 
	else if(str.CompareNoCase(_T("0.20 mm")) == 0)	type = AcDb::kLnWt020; 
	else if(str.CompareNoCase(_T("0.25 mm")) == 0)	type = AcDb::kLnWt025; 
	else if(str.CompareNoCase(_T("0.30 mm")) == 0)	type = AcDb::kLnWt030; 
	else if(str.CompareNoCase(_T("0.35 mm")) == 0)	type = AcDb::kLnWt035; 
	else if(str.CompareNoCase(_T("0.40 mm")) == 0)	type = AcDb::kLnWt040; 
	else if(str.CompareNoCase(_T("0.50 mm")) == 0)	type = AcDb::kLnWt050; 
	else if(str.CompareNoCase(_T("0.53 mm")) == 0)	type = AcDb::kLnWt053; 
	else if(str.CompareNoCase(_T("0.60 mm")) == 0)	type = AcDb::kLnWt060; 
	else if(str.CompareNoCase(_T("0.70 mm")) == 0)	type = AcDb::kLnWt070; 
	else if(str.CompareNoCase(_T("0.80 mm")) == 0)	type = AcDb::kLnWt080; 
	else if(str.CompareNoCase(_T("0.90 mm")) == 0)	type = AcDb::kLnWt090; 
	else if(str.CompareNoCase(_T("1.00 mm")) == 0)	type = AcDb::kLnWt100; 
	else if(str.CompareNoCase(_T("1.06 mm")) == 0)	type = AcDb::kLnWt106; 
	else if(str.CompareNoCase(_T("1.20 mm")) == 0)	type = AcDb::kLnWt120; 
	else if(str.CompareNoCase(_T("1.40 mm")) == 0)	type = AcDb::kLnWt140; 
	else if(str.CompareNoCase(_T("1.58 mm")) == 0)	type = AcDb::kLnWt158; 
	else if(str.CompareNoCase(_T("2.00 mm")) == 0)	type = AcDb::kLnWt200; 
	else if(str.CompareNoCase(_T("2.11 mm")) == 0)	type = AcDb::kLnWt211; 
	else if(str.CompareNoCase(_T("ByLayer")) == 0)	type = AcDb::kLnWtByLayer; 
	else if(str.CompareNoCase(_T("ByBlock")) == 0)	type = AcDb::kLnWtByBlock; 
	else if(str.CompareNoCase(_T("기본값")) == 0)	type = AcDb::kLnWtByLwDefault; 
	return type;
}

//대표적인 색상의 이름을 가져온다.
LPCTSTR CWKUtils::RGBToColorName(int r, int g, int b, CString& str){
	if(r == 255 && g == 0 && b == 0)	str = _T("빨간색");
	else if(r == 255 && g == 255 && b == 0)	str = _T("노란색");
	else if(r == 0 && g == 255 && b == 0)	str = _T("초록색");
	else if(r == 0 && g == 255 && b == 255)	str = _T("하늘색");
	else if(r == 0 && g == 0 && b == 255)	str = _T("파란색");
	else if(r == 255 && g == 0 && b == 255)	str = _T("선홍색");
	else if(r == 255 && g == 255 && b == 255)	str = _T("흰색");
	else	str = _T("null");
	return str;
}

void CWKUtils::explodeBlockReference(AcDbObjectId BlockId, BOOL bisoriginlayer, CString LayerName, BOOL bisNewlayer, BOOL bisbylayer, BOOL bisbylock, BOOL bisorigincolor){
	AcDbEntity* pEnt;
	if(Acad::eOk != acdbOpenAcDbEntity(pEnt, BlockId, AcDb::kForWrite)){
		acutPrintf(L"\nCannot open selected entity.");
		return;
	}

	AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pEnt);
	if(pBlockRef != NULL){ 
		AcDbVoidPtrArray entitySet;
		Acad::ErrorStatus es = pBlockRef->explode(entitySet);
		pBlockRef->erase();
		pBlockRef->close();
		assert(es == Acad::eOk);
		AcDbBlockTable* pBT;
		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBT, AcDb::kForRead);
		AcDbBlockTableRecord* pBTR;
		pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite);
		pBT->close();

		for(long i = 0l; i < entitySet.length(); ++i){
			AcDbEntity* pNewEnt = (AcDbEntity*)entitySet.at(i);
			if(bisoriginlayer){
				ACHAR* asdf;
				asdf = pNewEnt->layer();
				CString adfadsfasdf = asdf;
			}
			else if(bisoriginlayer == FALSE)
				pNewEnt->setLayer(LayerName);			

			if(bisbylayer)
				pNewEnt->setColorIndex(256);
			else if(bisbylock)
				pNewEnt->setColorIndex(0);
			else if(bisorigincolor){
				int color = pNewEnt->colorIndex();
				int a = color;
			}

			pBTR->appendAcDbEntity(pNewEnt);
			pNewEnt->close();
		}
		pBTR->close();
	}
	else{
		acutPrintf(L"Selected entity is not an INSERT.");
		return;
	}
}

BOOL CWKUtils::BlockSSGET(AcDbObjectIdArray& ids,CString Msg){
	BOOL bSuccess = FALSE;
	struct resbuf* filter = acutBuildList(RTDXF0, _T("INSERT"), RTNONE);
	ads_name ss;
	int ssss = acedSSGet(NULL, NULL, NULL ,filter, ss);
	if(RTNORM == ssss){
		ids.removeAll();
		ss2Array(ss, ids);
		bSuccess = TRUE;
	}
	else{
		bSuccess = FALSE;
	}
	return bSuccess;
}

void CWKUtils::ss2Array(ads_name ss, AcDbObjectIdArray& ids){
	// [+] SUP-29742
	// long	sslen;
	// [-] END
	Adesk::Int32 len;
	int	ret;
	AcDbObjectId objId;
	ads_name ent;

	acedSSLength(ss, &len);
	for(long i = 0L; i < len; i++){
		ret = acedSSName(ss, i, ent);
		ASSERT(ret == RTNORM);
		if(enameToObjId(ent, objId) == TRUE){
			ids.append(objId);
		}
	}
	acedSSFree(ss);
}

void CWKUtils::ss2Array_all(ads_name ss, AcDbObjectIdArray& ids){
	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	int	ret;
	AcDbObjectId objId;
	ads_name ent;

	acedSSLength(ss, &len);
	for(long i = 0L; i < len; i++){
		ret = acedSSName(ss, i, ent);
		ASSERT(ret == RTNORM);
			Acad::ErrorStatus es = acdbGetObjectId(objId, ent);
			if(es==eOk)	ids.append(objId);
	}
	
	acedSSFree(ss);
}

BOOL CWKUtils::enameToObjId(ads_name ent, AcDbObjectId& objId){
	BOOL bRC = FALSE;
	Acad::ErrorStatus es = acdbGetObjectId(objId, ent);
	if(es == Acad::eOk){
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isA() == AcDbBlockReference::desc()){
				AcDbBlockReference* pSelBlockRef=AcDbBlockReference::cast(pEnt);
				if(pSelBlockRef){
					AcDbObjectId blockid = pSelBlockRef->blockTableRecord();
					AcDbBlockTableRecordPointer pBTR(blockid, AcDb::kForRead);
					if(pBTR.openStatus() == Acad::eOk){
						ACHAR* name;
						pBTR->getName(name);
						bRC = TRUE;
						pBTR->close();
					}
				}
			}
			pEnt->close();
		}
	}
	return bRC;
}

void CWKUtils::ASDKLayerFilterDelete_DeleteLayerFilters(void){
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbLayerTablePointer pLyrTbl(pDb->layerTableId(), AcDb::kForRead);
	if(pLyrTbl.openStatus() == Acad::eOk){
		AcDbObjectId pExtDicId = pLyrTbl->extensionDictionary();
		if(pExtDicId.isValid()){
			AcDbDictionaryPointer pDict(pExtDicId,AcDb::kForRead);
			if(Acad::eOk == pDict.openStatus()){
				AcDbObjectId pObjId;
				pDict->getAt(_T("ACAD_LAYERFILTERS"), pObjId);
				if(pObjId.isValid()){
					AcDbDictionaryPointer pFiltDict(pObjId, AcDb::kForRead);
					if(Acad::eOk == pFiltDict.openStatus()){
						AcDbObjectId filtObjId;
						AcDbDictionaryIterator *pItr = pFiltDict->newIterator();
						if(NULL != pItr){
							for(; !pItr->done(); pItr->next()){
								AcDbObjectPointer<AcDbXrecord> pLyrFltr(pItr->objectId(),AcDb::kForWrite);
								if(Acad::eOk == pLyrFltr.openStatus()){
									pLyrFltr->erase();
								}
							}
							delete pItr;
						}
					}
				}
				pDict->getAt(_T("ACLYDICTIONARY"), pObjId);
				if(pObjId.isValid()){
					AcDbDictionaryPointer pFiltDict(pObjId, AcDb::kForRead);
					if(Acad::eOk == pFiltDict.openStatus()){
						AcDbObjectId filtObjId;
						AcDbDictionaryIterator* pItr = pFiltDict->newIterator();
						if(NULL != pItr){
							for(; !pItr->done(); pItr->next()){
								AcDbObjectPointer<AcDbXrecord> pLyrFltr(pItr->objectId(), AcDb::kForWrite);
								if(Acad::eOk == pLyrFltr.openStatus()){
									pLyrFltr->erase();
								}
							}
							delete pItr;
						}
					}
				}
			}
		}
	}
}

//출력 여부 지정
BOOL CWKUtils::SetlayerPlottable(AcDbObjectId& LyId, bool bisPlottable){
	BOOL bRC = FALSE;
	AcDbLayerTableRecordPointer layerTableRec(LyId, AcDb::kForWrite);
	if(layerTableRec.openStatus() == Acad::eOk){
		layerTableRec->setIsPlottable(bisPlottable);
		layerTableRec->close();
		bRC = TRUE;
	}
	return bRC;
}

BOOL CWKUtils::getTextStyleList(CStringArray& lst)
{
	lst.RemoveAll();
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	AcDbTextStyleTable* pStyleTbl = NULL;
	AcDbTextStyleTablePointer pStylePoint(pDB, AcDb::kForRead);
	AcDbTextStyleTableIterator* pStyleTblIter;
	pStylePoint->newIterator(pStyleTblIter);

	AcDbObjectId StyleId;
	for(pStyleTblIter->start(); !pStyleTblIter->done(); pStyleTblIter->step()){
		pStyleTblIter->getRecordId(StyleId);
		AcDbTextStyleTableRecordPointer pStyleTblRcd(StyleId, AcDb::kForRead);
		TCHAR* name;
		pStyleTblRcd->getName(name);
		lst.Add(name);
	}
	delete pStyleTblIter;
	delete pStyleTbl;
	return lst.GetCount() >0 ? TRUE : FALSE;
}


BOOL CWKUtils::SSGET(AcDbObjectIdArray& ids,CString strfilter){
	BOOL bSuccess = FALSE;

	struct resbuf * filter =acutBuildList(RTDXF0,/*ACRX_T(strfilter)*/strfilter,RTNONE);
	ads_name ss;
	int ssss = acedSSGet(NULL, NULL, NULL ,filter, ss);
	if(RTNORM == ssss){				//드래그로 객체를 가져온다.
		ids.removeAll();			//블록 배열 초기화
		ss2Array_all(ss, ids);			//사용자가 지정한 블록 가져오기
		bSuccess = TRUE;
	}
	else bSuccess = FALSE;
	return bSuccess;
}

BOOL CWKUtils::SSGET(AcDbObjectIdArray& ids,AcGePoint3d p1,AcGePoint3d p2,CString strfilter){
	BOOL bSuccess = FALSE;

	struct resbuf * filter =acutBuildList(RTDXF0,/*ACRX_T(strfilter)*/strfilter,RTNONE);
	ads_name ss;
	int ssss = acedSSGet(_T("C"), asDblArray(p1), asDblArray(p2) ,filter, ss);
	if(RTNORM == ssss){				//드래그로 객체를 가져온다.
		ids.removeAll();			//블록 배열 초기화
		ss2Array_all(ss, ids);			//사용자가 지정한 블록 가져오기
		bSuccess = TRUE;
	}
	else bSuccess = FALSE;
	return bSuccess;
}




// AcDbObjectId를 보내서 블록이름과 블록 삽입점을 얻어온다
bool CWKUtils::AsdkGetBlockAttribute(AcDbObjectId objId, CString& name, AcGePoint3d& pos,AcDbExtents& Extents){ // Let the user select an INSERT.

	AcDbEntity *pEnt;
	AcDbBlockReference *pInsert;
	Acad::ErrorStatus es; // Test the entity type.


	if (Acad::eOk != (es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead))) {
#ifdef _DEBUGMODE
		acutPrintf(L"\nCannot access selected entity.\n");
#endif // _DEBUGMODE
		return false;
	}
	pInsert = AcDbBlockReference::cast(pEnt);

	if (!pInsert) {
		//	acutPrintf(L"\nSelected entity is not an INSERT.\n");
		pEnt->close();
		return false;
	} // Get the objectID of the block definition.

	AcDbObjectId blockDefId = pInsert->blockTableRecord(); // Close the selected INSERT.

	pos = pInsert->position();	// insertion point
	pInsert->geomExtentsBestFit(Extents);
	pInsert->close(); // Open the block definition.
	AcDbBlockTableRecord *pBlkRecord;
	if (Acad::eOk != (es = acdbOpenObject(pBlkRecord, blockDefId,AcDb::kForRead))) {
		acutPrintf(L"\nCannot access block definition.\n");
		return false;
	} // Get the name of the block definition.
	const TCHAR* pBlkName;
	es = pBlkRecord->getName(pBlkName);		// block name(dwg file name)
	pBlkRecord->close();
	if ((Acad::eOk != es) || !pBlkName) {
		acutPrintf(L"\nCannot extract block definition name.\n");
		return false;
	}
	name.Format(_T("%s"),pBlkName);
	pEnt->close();
	return true;
	//	acutPrintf(L"\nName of block definition: '%s'\n", pBlkName);
}
