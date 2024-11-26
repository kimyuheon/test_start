#include "StdAfx.h"
#include <limits>
#include "TTte.h"
#include "CadText2Excel.h"
#include <vector>

// CTE
// 캐드 표를 엑셀로
CTTte::CTTte(void){
}

CTTte::~CTTte(void){
}

// XDCT
void CTTte::setXData(AcDbEntity* pObj, const LPCTSTR lpszBookSheetName, const Adesk::Int32 row, const Adesk::Int16 col){
	if(!pObj)
		return;

	ArxAppXdata xdct(_T("CT_TBL"), pObj->database());
	xdct.setValues(lpszBookSheetName, 1000);
	xdct.setValues(row, 1071);
	xdct.setValues(col, 1070);
	xdct.setXdata(pObj);
}

void CTTte::setXData(AcDbObjectId objId, const LPCTSTR lpszBookSheetName, const Adesk::Int32 row, const Adesk::Int16 col){
	AcDbEntityPointer pEnt(objId, AcDb::kForWrite);
	if(pEnt.openStatus() == Acad::eOk){
		setXData(pEnt, lpszBookSheetName, row, col);
		pEnt->close();
	}
}

bool CTTte::getXData(AcDbEntity* obj, CString& booksheetName, Adesk::Int32& row, Adesk::Int16& col){
	CString s;
	struct resbuf *head = obj->xData(_T("CT_TBL"));
	if(!head)
		return false;

	struct resbuf *rb = head;
	int dur = 0;
	while(rb){
		switch(dur++){
		case 1:
			booksheetName=rb->resval.rstring;
			break;
		case 2:
			row=rb->resval.rlong;
			break;
		case 3:
			col=rb->resval.rint;
			break;
		}
		rb = rb->rbnext;
	}
	if(head)
		acutRelRb(head);
	return true;
}

bool CTTte::getXData(AcDbObjectId objId, CString& booksheetName, Adesk::Int32& row, Adesk::Int16& col){
	bool rc = false;
	AcDbEntityPointer pEnt(objId, AcDb::kForWrite);
	if(pEnt.openStatus() == Acad::eOk){
		rc = getXData(pEnt, booksheetName, row, col);
		pEnt->close();
	}
	return rc;
}

int CTTte::GetCurrentExcelVersion(){
	HRESULT hr = S_OK;
	CLSID clsID;
	LPOLESTR buf;

	hr = ::CLSIDFromProgID(L"Excel.Application", &clsID);
	if(FAILED(hr))
		return -1;
	hr = ::ProgIDFromCLSID(clsID, &buf);
	if(FAILED(hr))
		return -1;

	CString strVersion = buf;	
	::CoTaskMemFree(buf);
	int nVersion = _ttoi(strVersion.Mid(strVersion.ReverseFind(_T('.'))+1));
	return nVersion;
}

AcDbObjectId CTTte::GetMaxY(AcDbObjectIdArray& ids, double& maxY){
	maxY = -DBL_MAX;
	AcDbObjectId idTop=AcDbObjectId::kNull;

	for(int i = 0; i < ids.length(); i++){
		AcDbEntityPointer pEnt(ids[i], AcDb::kForRead);
		if(Acad::eOk != pEnt.openStatus())
			continue;
		
		if(pEnt->isKindOf(AcDbLine::desc())){
			AcDbLine *pLine = AcDbLine::cast(pEnt);
			AcGePoint3d ptS;
			pLine->getStartPoint(ptS);
			if(ptS.y>maxY){
				maxY = ptS.y;
				idTop = pEnt->objectId();
			}
		}
	}
	return idTop;
}

AcDbObjectId CTTte::GetMinX(AcDbObjectIdArray& ids, double& minX){
	minX = DBL_MAX;
	AcDbObjectId idLeft=AcDbObjectId::kNull;
	for(int i = 0; i < ids.length(); i++){
		AcDbEntityPointer pEnt(ids[i], AcDb::kForRead);
		if(Acad::eOk != pEnt.openStatus())
			continue;		
	
		if(pEnt->isKindOf(AcDbLine::desc())){
			AcDbLine *pLine = AcDbLine::cast(pEnt);
			AcGePoint3d ptS;			
			pLine->getStartPoint(ptS);
			if(ptS.x<minX){
				minX=ptS.x;
				idLeft=pEnt->objectId();
			}
		}
	}
	return idLeft;
}

bool CTTte::GetHSubLine(int& row2, AcDbLine* pLineV1, AcGePoint3d& ptInt){
	bool bRC = false;
	for(int r1 = row2; r1 < m_tableH.size(); r1++){
		for(int r2 = 0; r2 < m_tableH[r1].length(); r2++){
			AcDbObjectId idHor = m_tableH[r1][r2];
			AcDbEntityPointer pLineHor1(idHor, AcDb::kForRead);
			if(pLineHor1.openStatus() == Acad::eOk){
				 AcDbLine *pLineH1 = AcDbLine::cast(pLineHor1);
				 if(pLineH1 != NULL && pLineV1 != NULL){
					 if(uge.IntersLine(pLineH1, pLineV1, true, ptInt) == true){
						 AcGePoint3d ptChk = ptInt - AcGeVector3d::kXAxis * 1.0;
						 AcGePoint3d ptS, ptE;
						 pLineH1->getStartPoint(ptS);
						 pLineH1->getEndPoint(ptE);
						 AcGeLineSeg3d lseg1(ptS, ptE);
						 ArxUtils::PyoSi(ptChk);
						 if(lseg1.isOn(ptChk)==Adesk::kTrue){
							 row2 = r1;
							 bRC = true;
							 return bRC;
						 }
					 }
				 }
			}
		}
	}
	return bRC;
}

void ads_regen();
bool CTTte::GetVSubLine(const int& row1, const int& col1, AcDbLine* pLineH0, int& row2, int&col2){
	bool bRC = false;
	if(pLineH0 == NULL)
		return bRC;

	for(int c1 = col1; c1 < m_tableV.size(); c1++){
		for(int c2 = 0; c2 < m_tableV[c1].length(); c2++){
			AcDbObjectId idVer = m_tableV[c1][c2];
			AcDbEntityPointer pLineVer1(idVer, AcDb::kForRead);
			if(pLineVer1.openStatus() == Acad::eOk){
				 AcDbLine* pLineV1 = AcDbLine::cast(pLineVer1);
				 int nRow2 = row1;
				 AcGePoint3d ptInt;
				 if(pLineV1 != NULL){
					 AcGePoint3d pt;
					 if(uge.IntersLine(pLineH0, pLineV1, true, pt) == true){
						 if(GetHSubLine(nRow2, pLineV1, ptInt) == true){
							 row2 = nRow2;
							 col2 = c1;
							 bRC = true;
							 return bRC;
						 }
					 }
				 }
			}
		}
	}
	return bRC;
}

// 그리드 영역 경계점(우측하단)을 찾는다.
bool CTTte::GetRightDown(const int& row1, const int& col1, int& row2, int& col2){
	bool bRC = false;

	AcDbEntityPointer pLineHor0(m_tableH[row1 - 1][0], AcDb::kForRead);
	AcDbLine *pLineH0 = NULL;
	AcDbLine *pLineH1 = NULL;
	
	int r1 = row1 - 1;
	int r2len = m_tableH[r1].length();
	for(int r2 = 0; r2 < r2len; r2++){
		AcDbObjectId idHor = m_tableH[r1][r2];
		AcDbEntityPointer pLineHor0(idHor, AcDb::kForRead);
		if(pLineHor0.openStatus() == Acad::eOk){
			pLineH0 = AcDbLine::cast(pLineHor0);
			if(GetVSubLine(row1, col1, pLineH0, row2, col2) == true){
				bRC = true;
				return bRC;
			}
		}
	}
	return bRC;
}

double CTTte::GetMinTextHgt(){
	double dblMinTextHgt = DBL_MAX;
	if(m_idsText.length() < 1)
		return -1.0;
	for(int i = 0; i < m_idsText.length(); i++){
		AcDbEntityPointer pEnt(m_idsText[i], AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			AcDbText* pText = AcDbText::cast(pEnt);
			AcGePoint3d pt;
			if(pText != NULL){
				if(pText->height() < dblMinTextHgt)
					dblMinTextHgt = pText->height();
			}
			else{
				AcDbMText* pMText = AcDbMText::cast(pEnt);
				if(pMText != NULL){
					if(pMText->textHeight() < dblMinTextHgt)
						dblMinTextHgt = pMText->textHeight();
				}
			}
		}
	}
	return dblMinTextHgt;
}

bool CTTte::GetTextInRange(const int& row1, const int& col1, const int& row2, const int& col2, CString& str){
	bool bRC = false;
	AcDbEntityPointer pLineHor0(m_tableH[row1 - 1][0], AcDb::kForRead);
	AcDbEntityPointer pLineHor1(m_tableH[row2][0], AcDb::kForRead);
	AcDbEntityPointer pLineVer0(m_tableV[col1 - 1][0], AcDb::kForRead);
	AcDbEntityPointer pLineVer1(m_tableV[col2][0], AcDb::kForRead);
	
	AcDbLine *lh0 = NULL;
	AcDbLine *lh1 = NULL;
	AcDbLine *lv0 = NULL;
	AcDbLine *lv1 = NULL;
	if(pLineHor0.openStatus()==Acad::eOk){
		lh0=AcDbLine::cast(pLineHor0);
	}
	if(pLineHor1.openStatus()==Acad::eOk){
		lh1=AcDbLine::cast(pLineHor1);
	}
	if(pLineVer0.openStatus()==Acad::eOk){
		lv0=AcDbLine::cast(pLineVer0);
	}
	if(pLineVer1.openStatus()==Acad::eOk){
		lv1=AcDbLine::cast(pLineVer1);
	}

	AcGePoint3d pt0, pt1, pt2, pt3;
	uge.IntersLine(lh0, lv0, false, pt0);	// Left Top
	uge.IntersLine(lv0, lh1, false, pt1);
	uge.IntersLine(lh1, lv1, false, pt2);	// Right Bottom
	uge.IntersLine(lh0, lv1, false, pt3);

	AcGePoint3dArray pl;
	pl.append(pt0);
	pl.append(pt1);
	pl.append(pt2);
	pl.append(pt3);
	pl.append(pt0);
	
	for(int i=0;i<m_idsText.length();i++)
	{
		AcDbEntityPointer pEnt(m_idsText[i], AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk){
			AcDbText* pText=AcDbText::cast(pEnt);
			AcGePoint3d pt;
			if(pText!=NULL){
				pt=pText->alignmentPoint();
				if(pt0.x<=pt.x && pt2.x>=pt.x && pt0.y>=pt.y && pt2.y<=pt.y)
				{
					str=pText->textString();
					if(this->m_bSyncDwgExcel) {
						if(pEnt->upgradeOpen()==Acad::eOk){
							this->setXData(pEnt, _T("Book1|Sheet1"), row1, col1);
						}
					}
					bRC=true;
					break;
				}
			}else{
				AcDbMText* pText=AcDbMText::cast(pEnt);


				AcGePoint3dArray pts;
				pText->getBoundingPoints(pts);
				pt=uge.midpoint(pts[0], pts[1]);
				ArxUtils::PyoSi(pt);

				/*AcDbExtents bounds;
				pText->bounds(bounds);
				pt=uge.midpoint(bounds.maxPoint(), bounds.minPoint());*/


			

				//if(uge.isInside(pl, pText->location())==true)
				if(pt0.x<=pt.x && pt2.x>=pt.x && pt0.y>=pt.y && pt2.y<=pt.y)
				{
					
					str=pText->text();
					if(this->m_bSyncDwgExcel){
						if(pEnt->upgradeOpen()==Acad::eOk){
							this->setXData(pEnt, _T("Book1|Sheet1"), row1, col1);
						}
					}
					bRC=true;
					break;
				}
			}
		}
	}
			
	return bRC;
}


void CTTte::UpdateXData(LPCTSTR lpszBook, LPCTSTR lpszSheet)
{
	CString strBookSheet, strBookSheetNew;
	Adesk::Int32 row1;
	Adesk::Int16 col1;

	strBookSheetNew.Format(_T("%s|%s"), lpszBook, lpszSheet);

	for(int i=0;i<m_idsText.length();i++)
	{
		AcDbEntityPointer pEnt(m_idsText[i], AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk)
		{
			AcDbText* pText=AcDbText::cast(pEnt);
			AcGePoint3d pt;
			if(pText!=NULL){
				
				if(pEnt->upgradeOpen()==Acad::eOk){
					this->getXData(pEnt, strBookSheet, row1, col1);
					row1+=(this->m_excel.m_nRowStart-1);
					col1+=(this->m_excel.m_nColStart-1);
					this->setXData(pEnt, strBookSheetNew, row1, col1);
				}					
	
			
			}else{
				AcDbMText* pText=AcDbMText::cast(pEnt);
				if(pText!=NULL)
				{
					if(pEnt->upgradeOpen()==Acad::eOk){
						this->getXData(pEnt, strBookSheet, row1, col1);
						row1+=(this->m_excel.m_nRowStart-1);
						col1+=(this->m_excel.m_nColStart-1);
						this->setXData(pEnt, strBookSheetNew, row1, col1);
					}
				}					
			}
		}	
	}
}


void CTTte::GetHorLines(AcDbObjectIdArray& idsLineH)
{
	// 수평선들을 행으로 정렬
	double maxY=-DBL_MAX;

	for(int i=0;i<m_tableH.size();i++){
		m_tableH[i].removeAll();
		m_tableH[i].setLogicalLength(0);
	}
	m_tableH.clear();

	while(idsLineH.length())
	{
		AcDbObjectId idMaxY=this->GetMaxY(idsLineH, maxY);
		AcDbObjectIdArray ids;
		for(int i=0;i<idsLineH.length();i++)
		{
			AcDbObjectId id=idsLineH[i];
			AcDbEntityPointer pEnt(id, AcDb::kForRead);
			if(Acad::eOk != pEnt.openStatus()) 
			{
				continue;
			}			
			
			AcDbLine *pLine = AcDbLine::cast(pEnt);
			if(pLine!=NULL){
				AcGePoint3d ptS;			
				pLine->getStartPoint(ptS);
				if(fabs(ptS.y - maxY)<m_gTol) {
					ids.append(pLine->objectId());
				}
			}
		}
		for(int i=0;i<ids.length();i++) idsLineH.remove(ids[i]);
		this->m_tableH.push_back(ids);
	}
}


void CTTte::GetVerLines(AcDbObjectIdArray& idsLineV)
{
	// 수직선들을 열로 정렬
	double minX=0;
	for(int i=0;i<m_tableV.size();i++){
		m_tableV[i].removeAll();
		m_tableV[i].setLogicalLength(0);
	}
	m_tableV.clear();
	while(idsLineV.length())
	{
		AcDbObjectId idMinX=this->GetMinX(idsLineV , minX);
		AcDbObjectIdArray ids;
		for(int i=0;i<idsLineV.length();i++)
		{
			AcDbObjectId id=idsLineV[i];
			AcDbEntityPointer pEnt(id, AcDb::kForRead);
			if(Acad::eOk != pEnt.openStatus()) 
			{
				continue;
			}			
			
			AcDbLine *pLine = AcDbLine::cast(pEnt);
			if(pLine!=NULL){
				AcGePoint3d ptS;			
				pLine->getStartPoint(ptS);
				if(fabs(ptS.x-minX)<m_gTol) {
					ids.append(pLine->objectId());
				}
			}
		}
		for(int i=0;i<ids.length();i++) idsLineV.remove(ids[i]);
		
		m_tableV.push_back(ids);
	}//end-while
}


void CTTte::PrintText(AcDbObjectIdArray& ids)
{
	for(int i=0;i<ids.length();i++)
	{
		AcDbEntityPointer pEnt2(ids[i], AcDb::kForRead);
		if(pEnt2.openStatus()!=Acad::eOk) continue;
		if(pEnt2->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(pEnt2);
			ads_printf(_T("%s "), pText->textString());		
		}else if(pEnt2->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt2);
			ads_printf(_T("%s "), pMText->contents());
		}
		
	}
	ads_printf(_T("\n"));
}

void CTTte::SortY(AcDbObjectIdArray& ids, bool bAscending)
{

	//PrintText(ids);

	AcDbObjectIdArray idsSortedText;
	AcDbObjectId idMin;
	while(ids.length())
	{
		idMin=ids.first();
		AcGePoint3d ptMin;
		AcDbEntityPointer pEnt1(idMin, AcDb::kForRead);
		if(pEnt1.openStatus()!=Acad::eOk) continue;
		if(pEnt1->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(pEnt1);
			ptMin=pText->alignmentPoint();			
		}else if(pEnt1->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt1);
			ptMin=pMText->location();
		}

		//ArxUtils::PyoSi(ptMin);
		
		for(int i=1;i<ids.length();i++)
		{
			AcGePoint3d pt2;
			AcDbEntityPointer pEnt2(ids[i], AcDb::kForRead);
			if(pEnt2.openStatus()!=Acad::eOk) continue;
			if(pEnt2->isKindOf(AcDbText::desc()))
			{
				AcDbText* pText=AcDbText::cast(pEnt2);
				pt2=pText->alignmentPoint();			
			}else if(pEnt2->isKindOf(AcDbMText::desc()))
			{
				AcDbMText* pMText=AcDbMText::cast(pEnt2);
				pt2=pMText->location();
			}
			if(ptMin.y>pt2.y){
				ptMin=pt2;
				idMin=pEnt2->objectId();
			}else if(fabs(ptMin.y-pt2.y)<0.00000001){		// y값이 같은 경우
				if(bAscending){
					if(ptMin.x>pt2.x)
					{
						ptMin=pt2;
						idMin=pEnt2->objectId();
					}
				}else{
					if(ptMin.x<pt2.x)
					{
						ptMin=pt2;
						idMin=pEnt2->objectId();
					}
				}
			}
		}
		idsSortedText.append(idMin);		
		ids.remove(idMin);
	}
	


	if(bAscending==false) idsSortedText.reverse();
	ids.removeAll();
	ids.setLogicalLength(0);
	if(idsSortedText.length()>0) ids.append(idsSortedText);

	//PrintText(ids);

}

 


void CTTte::SortX(AcDbObjectIdArray& ids, bool bAscending)
{	
	AcDbObjectIdArray idsSortedText;
	AcDbObjectId idMin;
	while(ids.length())
	{
		idMin=ids.first();
		

		AcGePoint3d ptMin;
		AcDbEntityPointer pEnt1(idMin, AcDb::kForRead);
		if(pEnt1.openStatus()!=Acad::eOk) continue;
		if(pEnt1->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(pEnt1);
			ptMin=pText->alignmentPoint();			
		}else if(pEnt1->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt1);
			ptMin=pMText->location();
		}
		
		for(int i=1;i<ids.length();i++)
		{
		
			AcGePoint3d pt2;
			AcDbEntityPointer pEnt2(ids[i], AcDb::kForRead);
			if(pEnt2.openStatus()!=Acad::eOk) continue;
			if(pEnt2->isKindOf(AcDbText::desc()))
			{
				AcDbText* pText=AcDbText::cast(pEnt2);
				pt2=pText->alignmentPoint();			
			}else if(pEnt2->isKindOf(AcDbMText::desc()))
			{
				AcDbMText* pMText=AcDbMText::cast(pEnt2);
				pt2=pMText->location();
			}
			if(ptMin.x>pt2.x){
				ptMin=pt2;
				idMin=pEnt2->objectId();
			}else if(fabs(ptMin.x-pt2.x)<0.00000001){		// x값이 같은 경우
				if(bAscending){
					if(ptMin.y<pt2.y)
					{
						ptMin=pt2;
						idMin=pEnt2->objectId();
					}
				}
				else{
					if(ptMin.y>pt2.y)
					{
						ptMin=pt2;
						idMin=pEnt2->objectId();
					}
				}
			}
		}
		idsSortedText.append(idMin);	
		ids.remove(idMin);
	}


	if(bAscending==false) idsSortedText.reverse();
	ids.removeAll();
	ids.setLogicalLength(0);
	if(idsSortedText.length()>0) ids.append(idsSortedText);

}



void CTTte::SortByString(AcDbObjectIdArray& ids, bool bAscending)
{
	AcDbObjectIdArray idsSortedText;
	AcDbObjectId idMin;
	while(ids.length())
	{
		idMin=ids[0];
		

		CString  strMin;
		AcDbEntityPointer pEnt1(idMin, AcDb::kForRead);
		if(pEnt1.openStatus()!=Acad::eOk) continue;
		if(pEnt1->isKindOf(AcDbText::desc()))
		{
			AcDbText* pText=AcDbText::cast(pEnt1);
			strMin=pText->textString();
		}else if(pEnt1->isKindOf(AcDbMText::desc()))
		{
			AcDbMText* pMText=AcDbMText::cast(pEnt1);
			strMin=pMText->contents();
		}
		
		for(int i=1;i<ids.length();i++)
		{
			CString str2;
			AcDbEntityPointer pEnt2(ids[i], AcDb::kForRead);
			if(pEnt2.openStatus()!=Acad::eOk) continue;
			if(pEnt2->isKindOf(AcDbText::desc()))
			{
				AcDbText* pText=AcDbText::cast(pEnt2);
				str2=pText->textString();		
			}else if(pEnt2->isKindOf(AcDbMText::desc()))
			{
				AcDbMText* pMText=AcDbMText::cast(pEnt2);
				str2=pMText->contents();
			}
			if(strMin.Compare(str2)>0){
				strMin=str2;
				idMin=pEnt2->objectId();
			}
		}
		idsSortedText.append(idMin);		
		ids.remove(idMin);
	}


	if(bAscending==false) idsSortedText.reverse();
	ids.removeAll();
	ids.setLogicalLength(0);
	if(idsSortedText.length()>0) ids.append(idsSortedText);
}


bool CTTte::WriteToExcel()
{
	bool bRC=false;
	

	int nVersion = GetCurrentExcelVersion();
	CString str;
	str.Format(_T("Excel Version: %d"), nVersion);
	switch (nVersion) {
	case -1:
		AfxMessageBox(_T("System 내에 설치된 Excel을 찾을 수 없습니다.")); 
		return bRC;
	default:
		break;
	}
	
	if (!AfxOleInit()) {
        AfxMessageBox(_T("초기화 실패..")); 
        return bRC;
    }

	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------
	ArxRbList rbList;		
	resbuf* tmpRb;
	tmpRb = acutNewRb(-4);
	acutNewString(_T("<or"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	AcDbVoidPtrArray filterClasses;
	filterClasses.append(AcDbText::desc());
	filterClasses.append(AcDbMText::desc());


	const TCHAR* dxfStr;
	int len = filterClasses.length();
	for (int i=0; i<len; i++) {
		dxfStr = static_cast<AcRxClass*>(filterClasses[i])->dxfName();
		if (dxfStr) {
			tmpRb = acutNewRb(0);
			acutNewString(dxfStr, tmpRb->resval.rstring);
			rbList.addTail(tmpRb);
		}
	}

	tmpRb = acutNewRb(-4);
	acutNewString(_T("or>"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------

	
	ArxSelSet::SelSetStatus stat = ArxSelSet::kNone;
	ArxSelSet ss;

	AcDbObjectIdTable idTable;
	int nCnt=0;
	int nLoop=1;
	CString strMsg;
	do{
		strMsg.Format(_T("\n그룹-%d 선택(종료=엔터):"), nLoop);
		ads_printf(strMsg);
		stat=ss.userSelect(_T("객체 선택:"), NULL, rbList.data());
		std::vector<AcDbObjectId> objIds;
		AcDbObjectIdArray ids;
		if (stat == ArxSelSet::kSelected) { 
			// 선택한 객체		
			ss.asArray(ids);		
		
		}
		nCnt=ids.length();
		//for(int i=0;i<ids.length();i++) objIds.push_back(ids[i]);
		if(nCnt>0) idTable.push_back(ids);
		nLoop+=1;
	}while(nCnt>0);

	



	// 텍스트 정렬
	for(int i=0;i<idTable.size();i++)
	{
		
		switch(this->m_nTextSortType)
		{
		case 0:
			SortX(idTable[i]); break;
		case 1:
			SortY(idTable[i], false); break;
		case 2:
			SortY(idTable[i]); break;
		case 3:
			 break;		// 선택한 순서
		case 4:
			SortByString(idTable[i]); break;
		case 5:
			SortByString(idTable[i], false); break;
		}

		
	}



	long nRowStart;
	int  nColStart;
	short nInsType=(short)m_nExcelInsertType;
	CTTte *ptte=NULL;
	if(this->m_bSyncDwgExcel) ptte=this;
	m_excel.ExportText(_T("Book1"), idTable, nInsType, ptte);
	
	if(m_nTextProcType==1){			// 레이어변경	
		for(int i=0;i<idTable.size(); i++)
		{
			ArxUtils::SetLayer(idTable[i], m_Layer);				
		}
	}else if(m_nTextProcType==2){	// 색상변경
		for(int i=0;i<idTable.size(); i++)
		{
			ArxUtils::SetColorIndex(idTable[i], m_nColorIndex);				
		}
	}else if(m_nTextProcType==3){	// 삭제
		for(int i=0;i<idTable.size(); i++)
		{
			ArxUtils::EntDel(idTable[i]);	
		}
	}

	bRC=true;
	return bRC;
}


bool CTTte::go()
{
	boost::shared_ptr<CAcSystem> pAcSystem(new CAcSystem());	
	CCadText2Excel dlg;

	if(dlg.DoModal()==IDOK){
		m_nTextSortType=dlg.m_nTextSortType;				// 문자 정렬 방식
		m_nExcelInsertType=dlg.m_nExcelInsertType;		// 엑셀 삽입 방식
		m_nTextProcType=dlg.m_nTextProcType;				// 엑셀로 내보낸 문자 처리
		m_Layer=dlg.m_Layer;
		m_nColorIndex=dlg.m_nColorIndex;
		m_bSyncDwgExcel=dlg.m_bUpdateDwgExcel;			// 업데이트

		switch(dlg.m_nJob)
		{
		case 1:
			WriteToExcel(); break;
			break;
		case 2:
			updateDwg(); break;									// 도면수정->엑셀파일 업데이트
		case 3:
			updateExcel(); break;								// 엑셀수정->도면내용 업데이트
		case 4:
			removeXData(); break;
		}
	}
	return true;
}



// 도면의 테이블이 수정됨
bool CTTte::updateDwg()
{
	bool bRC=false;


	// 도면수정->Excel수정
	ExcelLib m_excel;
	


	if (!AfxOleInit()) {
        AfxMessageBox(_T("초기화 실패..")); 
        return bRC;
    }



	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------
	ArxRbList rbList;		
	resbuf* tmpRb;
	tmpRb = acutNewRb(-4);
	acutNewString(_T("<or"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	AcDbVoidPtrArray filterClasses;
	//filterClasses.append(AcDbLine::desc());
	//filterClasses.append(AcDbText::desc());
	filterClasses.append(AcDbMText::desc());


	const TCHAR* dxfStr;
	int len = filterClasses.length();
	for (int i=0; i<len; i++) {
		dxfStr = static_cast<AcRxClass*>(filterClasses[i])->dxfName();
		if (dxfStr) {
			tmpRb = acutNewRb(0);
			acutNewString(dxfStr, tmpRb->resval.rstring);
			rbList.addTail(tmpRb);
		}
	}

	tmpRb = acutNewRb(-4);
	acutNewString(_T("or>"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------

	ArxSelSet::SelSetStatus stat = ArxSelSet::kNone;
	ArxSelSet ss;
	//ArxRbList filterRbList;
	//stat=ss.userSelect(_T("객체 선택:"), NULL, rbList.data());
	stat=ss.allSelect(rbList.data());
	
	AcDbObjectIdArray objIds;
	if (stat == ArxSelSet::kSelected) { 
		// 선택한 객체		
		ss.asArray(objIds);	
	}
	

	int nCnt=objIds.length();
	if(nCnt==1)
	{
		if(ArxUtils::isOnLockedLayer(objIds.first(), false)){
			acutPrintf(_T("\n선택한 객체의 레이어가 잠겨있어 명령을 실행할 수 없습니다."));
			return FALSE;
		}
	}else if(nCnt<1) return FALSE; // 처리할 객체가 없음.
	

	bool bIsLocked=false;

	m_idsText.removeAll();
	for(int i=0;i<objIds.length();i++)
	{
		AcDbObjectId id=objIds[i];
		if(ArxUtils::isOnLockedLayer(id, false)) {
			bIsLocked=true;
			continue;;
		}

		AcDbEntityPointer pEnt(id, AcDb::kForRead);
		if(Acad::eOk != pEnt.openStatus()) 
		{
			continue;
		}

		
		if (pEnt->isKindOf(AcDbText::desc()) || pEnt->isKindOf(AcDbMText::desc())) {
			m_idsText.append(pEnt->objectId());
		}
		
		pEnt->close();

	}

	//------------------------------------------------------------------------------------
	// 엑셀 셀 수정
	//------------------------------------------------------------------------------------
	CString booksheetName;
	Adesk::Int32 row;
	Adesk::Int16 col;
	CString strData;

	while(m_idsText.length())
	{
		AcDbObjectId idText=m_idsText.first();
		m_idsText.removeFirst();

		AcDbEntityPointer pEnt(idText, AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk){
			AcDbText* pText=AcDbText::cast(pEnt);
			AcGePoint3d pt;
			if(pText!=NULL){
				strData=pText->textString();
			}else{
				AcDbMText* pMText=AcDbMText::cast(pEnt);
				if(pMText!=NULL){
					strData=pMText->text();
				}
			}

			this->getXData(pEnt, booksheetName, row, col);

	

			CtXRange rng;
			rng.set(col, row, col, row);
			CtXlsCell *pCell=NULL;

			pCell=this->m_excel.NewXlsCell(rng);
	
		

			if(pCell!=NULL)
			{
				pCell->m_bMerge=true;
	
				
								
				int ahor=DA_CENTER;
				int aver=DA_CENTER;

				pCell->SetData(strData, -1, ahor, aver, 0);
				
		
					
				
			}//-if	

		}
	}

	this->m_excel.Export(booksheetName, false);

	return bRC;
}




// 엑셀이 수정되었습니다.
bool CTTte::updateExcel()
{
	bool bRC=false;
	
	// 도면수정->Excel수정
	ExcelLib m_excel;
	AcDbObjectIdArray	m_idsText;		// 문자
	


	if (!AfxOleInit()) {
        AfxMessageBox(_T("초기화 실패..")); 
        return bRC;
    }

	

	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------
	ArxRbList rbList;		
	resbuf* tmpRb;
	tmpRb = acutNewRb(-4);
	acutNewString(_T("<or"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	AcDbVoidPtrArray filterClasses;
	//filterClasses.append(AcDbLine::desc());
	//filterClasses.append(AcDbText::desc());
	filterClasses.append(AcDbMText::desc());


	const TCHAR* dxfStr;
	int len = filterClasses.length();
	for (int i=0; i<len; i++) {
		dxfStr = static_cast<AcRxClass*>(filterClasses[i])->dxfName();
		if (dxfStr) {
			tmpRb = acutNewRb(0);
			acutNewString(dxfStr, tmpRb->resval.rstring);
			rbList.addTail(tmpRb);
		}
	}

	tmpRb = acutNewRb(-4);
	acutNewString(_T("or>"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------

	ArxSelSet::SelSetStatus stat = ArxSelSet::kNone;
	ArxSelSet ss;
	//ArxRbList filterRbList;
	//stat=ss.userSelect(_T("객체 선택:"), NULL, rbList.data());
	stat=ss.allSelect(rbList.data());
	
	AcDbObjectIdArray objIds;
	if (stat == ArxSelSet::kSelected) { 
		// 선택한 객체		
		ss.asArray(objIds);		
	
	}
	

	int nCnt=objIds.length();
	if(nCnt==1)
	{
		if(ArxUtils::isOnLockedLayer(objIds.first(), false)){
			acutPrintf(_T("\n선택한 객체의 레이어가 잠겨있어 명령을 실행할 수 없습니다."));
			return FALSE;
		}
	}else if(nCnt<1) return FALSE; // 처리할 객체가 없음.
	

	bool bIsLocked=false;

	m_idsText.removeAll();
	Adesk::Int32 row;
	Adesk::Int16 col;
	CString booksheetName;
	for(int i=0;i<objIds.length();i++)
	{
		AcDbObjectId id=objIds[i];
		if(ArxUtils::isOnLockedLayer(id, false)) {
			bIsLocked=true;
			continue;;
		}

		AcDbEntityPointer pEnt(id, AcDb::kForRead);
		if(Acad::eOk != pEnt.openStatus()) 
		{
			continue;
		}

		
		if (pEnt->isKindOf(AcDbText::desc()) || pEnt->isKindOf(AcDbMText::desc())) {
			this->getXData(pEnt, booksheetName, row, col);
			m_idsText.append(pEnt->objectId());
		}
		
		pEnt->close();

	}


	m_excel.ApplyToDwg(booksheetName, m_idsText);



	return bRC;
}


bool CTTte::removeXData()
{
	bool bRC=false;

	
	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------
	ArxRbList rbList;		
	resbuf* tmpRb;
	tmpRb = acutNewRb(-4);
	acutNewString(_T("<or"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	AcDbVoidPtrArray filterClasses;
	//filterClasses.append(AcDbLine::desc());
	//filterClasses.append(AcDbText::desc());
	filterClasses.append(AcDbMText::desc());


	const TCHAR* dxfStr;
	int len = filterClasses.length();
	for (int i=0; i<len; i++) {
		dxfStr = static_cast<AcRxClass*>(filterClasses[i])->dxfName();
		if (dxfStr) {
			tmpRb = acutNewRb(0);
			acutNewString(dxfStr, tmpRb->resval.rstring);
			rbList.addTail(tmpRb);
		}
	}

	tmpRb = acutNewRb(-4);
	acutNewString(_T("or>"), tmpRb->resval.rstring);
	rbList.addTail(tmpRb);

	//-------------------------------------------------------------------------------
	// 객체 선택 필터 추가
	//-------------------------------------------------------------------------------

	ArxSelSet::SelSetStatus stat = ArxSelSet::kNone;
	ArxSelSet ss;
	//ArxRbList filterRbList;
	//stat=ss.userSelect(_T("객체 선택:"), NULL, rbList.data());
	stat=ss.allSelect(rbList.data());
	
	AcDbObjectIdArray objIds;
	if (stat == ArxSelSet::kSelected) { 
		// 선택한 객체		
		ss.asArray(objIds);		
	
	}
	

	int nCnt=objIds.length();
	if(nCnt==1)
	{
		if(ArxUtils::isOnLockedLayer(objIds.first(), false)){
			acutPrintf(_T("\n선택한 객체의 레이어가 잠겨있어 명령을 실행할 수 없습니다."));
			return FALSE;
		}
	}else if(nCnt<1) return FALSE; // 처리할 객체가 없음.
	

	bool bIsLocked=false;

	m_idsText.removeAll();
	for(int i=0;i<objIds.length();i++)
	{
		AcDbObjectId id=objIds[i];
		if(ArxUtils::isOnLockedLayer(id, false)) {
			bIsLocked=true;
			continue;;
		}

		AcDbEntityPointer pEnt(id, AcDb::kForRead);
		if(Acad::eOk != pEnt.openStatus()) 
		{
			continue;
		}

		
		if (pEnt->isKindOf(AcDbText::desc()) || pEnt->isKindOf(AcDbMText::desc())) {
			m_idsText.append(pEnt->objectId());
		}
		
		pEnt->close();

	}



	// XDATA 삭제
	//----------------------------------------------------------------------
	Acad::ErrorStatus es;
	for(int i=0;i<m_idsText.length();i++)
	{
		AcDbObjectId objId=m_idsText[i];
		AcDbEntityPointer pEnt(objId, AcDb::kForRead);
		if(Acad::eOk != pEnt.openStatus()) 
		{
			continue;
		}
		resbuf *pXdata = pEnt->xData(_T("CT_TBL"));
		if( pXdata ){
			pEnt->upgradeOpen();
			pXdata->rbnext = NULL;
			if(pXdata->rbnext) acutRelRb(pXdata->rbnext);
			es=pEnt->setXData(pXdata);	
			acutRelRb(pXdata);
		}
		pEnt->close();
	}


	return bRC;
}
