// DSLO.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "DFB.h"
#include "afxdialogex.h"

// CDFB 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDFB, CAcUiDialog)
	CDFB::CDFB(CWnd* pParent)
	: CAcUiDialog(CDFB::IDD, pParent), 
	m_nEditBaseDraw(1)
	, m_nEditSubDraw(2)
{
}

CDFB::~CDFB(){

	
}

void CDFB::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBXLAYER, m_cbxLayer);
	DDX_Control(pDX, IDC_BLAYERSELECT, m_selectBn);
	DDX_Control(pDX, IDC_EDIT_1, m_EditBaseDraw_Ctrl);
	DDX_Text(pDX, IDC_EDIT_1, m_nEditBaseDraw);
	DDX_Text(pDX, IDC_EDIT_2, m_nEditSubDraw);
}

BEGIN_MESSAGE_MAP(CDFB, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CDFB::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDFB::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_1, &CDFB::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BCOLOR, &CDFB::OnBnClickedBcolor)
	ON_WM_DRAWITEM()
	ON_EN_UPDATE(IDC_EDIT_1, &CDFB::OnEnUpdateEdit1)
	ON_CBN_SELCHANGE(IDC_CBXLAYER, &CDFB::OnCbnSelchangeCbxlayer)
	ON_BN_CLICKED(IDC_BLAYERSELECT, &CDFB::OnBnClickedBlayerselect)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_3, &CDFB::OnDeltaposSpin3)
	ON_BN_CLICKED(ID_DFB_HELP, &CDFB::OnBnClickedDfbHelp)
END_MESSAGE_MAP()

// CDFB 메시지 처리기입니다.

BOOL CDFB::OnInitDialog(){
	CAcUiDialog::OnInitDialog();

	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	jobStat = 1;
	CString baseShowSpaceStr = iniFile.readINI(_T("config.ini"), _T("DFB"), _T("BASESHOWCOUNT"));
	if(baseShowSpaceStr.CompareNoCase(_T("")) == 0)	baseShowSpaceStr = _T("1");
	SetDlgItemText(IDC_EDIT_1, baseShowSpaceStr);
	CString subShowCnteStr = iniFile.readINI(_T("config.ini"), _T("DFB"), _T("SUBSHOWCOUNT"));
	if(subShowCnteStr.CompareNoCase(_T("")) == 0)	subShowCnteStr = _T("2");
	SetDlgItemText(IDC_EDIT_2, subShowCnteStr);
	
	AcDbObjectId layer_id;
	if(!FindLayer(_T("CIVIL-DFB-Fill"),layer_id))
	{
		createLayer(_T("CIVIL-DFB-Fill"),1,layer_id);
	}

	//도면층 불러오기
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		m_cbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

 	m_cbxLayer.SetCurSel(m_cbxLayer.FindString(0,_T("CIVIL-DFB-Fill")));
	OnCbnSelchangeCbxlayer();
	m_selectBn.AutoLoad();
	return TRUE;
}

void CDFB::OnBnClickedOk(){

	UpdateData();
	if(m_nEditBaseDraw == 0)
		return;

	UpdateData(FALSE);
	OnOK();
	CString layerStr;
	m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), layerStr);
	AcDbObjectIdArray ids;
	AcGePoint3d base_pt,sp, ep;
	ads_name endEn;
	AcDbObjectId endLineId;
	bool bisFindMidLine = false;
	CString curLayer, strMsg;
	m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), curLayer);
	setLayer = (LPCTSTR)curLayer;
	bool selectionCheck = false;
	
	BeginEditorCommand();
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	strMsg.LoadStringW(IDS_SLOPE_START); // "\n사면 시작점 선택: "
	if(acedGetPoint(NULL, strMsg, asDblArray(base_pt)) == RTNORM){
		while (true){
			ids.removeAll();
			strMsg.LoadStringW(IDS_BERM_MSG);// "\n** 소단선 선택(소단선을 순서대로 선택 후, 모두 선택하면 엔터입력.소단이 없어도 엔터입력) **"
			acutPrintf(strMsg);
			SSGET(ids, _T("ARC,LWPOLYLINE"));
			if(ids.length() == 0 || ids.length() % 2 == 0) break;
			if(ids.length() ==1){
				strMsg.LoadStringW(IDS_BERM_CHECK); // "하나의 소단은 두개의 선으로 이루어져야 합니다."
				AfxMessageBox(strMsg);
				strMsg.LoadStringW(IDS_BERM_RESTART); // "\n** 소단선만 처음부터 다시 선택하세요 **"
				acutPrintf(strMsg);
			}

		}
		if(ids.length() >1)	bisFindMidLine = true;
		strMsg.LoadStringW(IDS_SLOPE_END); // "\n사면 끝선 선택: "
		if(ads_entsel(strMsg, endEn, asDblArray(ep)) == RTNORM) selectionCheck = true;
	}
	CompleteEditorCommand();
	if(!selectionCheck)	return;
	if(acdbGetObjectId(endLineId, endEn) == Acad::eOk){
		AcDbEntityPointer pEnt(endLineId, AcDb::kForRead);
		if(pEnt.openStatus() == Acad::eOk){
			if(pEnt->isKindOf(AcDbCurve::desc())){
				AcDbCurve * endLine = AcDbCurve::cast(pEnt);

				AcGePoint3d deadLine_sp,deadLine_ep;
				endLine->getEndPoint(deadLine_ep);
				endLine->getStartPoint(deadLine_sp);

				double total_dist=0 ;
				endLine->getDistAtPoint(deadLine_ep,total_dist);

				double dblMajorLineOffset = total_dist / (m_nEditBaseDraw+1);
				double dblMinorLineOffset = dblMajorLineOffset / (m_nEditSubDraw+1);
				for(int i = 0  ; i < m_nEditBaseDraw+1; i++){
					AcGePoint3d taget_pt;
					endLine->getPointAtDist(dblMajorLineOffset*(i+1),taget_pt);
					AcDbLine* line = new AcDbLine(base_pt,taget_pt);
					if(bisFindMidLine){
						for(int z = 0; z < ids.length(); z+=2){
							AcDbEntityPointer pEnt1(ids[z], AcDb::kForRead);
							if(pEnt1.openStatus() == Acad::eOk){
								AcDbCurve * endLine = AcDbCurve::cast(pEnt1);
								AcGePoint3dArray pts;
								if(endLine->intersectWith(line,kExtendThis ,pts) ==Acad::eOk){
									if(i < m_nEditBaseDraw){
										if(z ==0)CreateAcDbLine(base_pt,pts.first(),setLayer,m_curColorIndex);
										else{
											AcDbEntityPointer pEnt3(ids[z-1], AcDb::kForRead);
											if(pEnt3.openStatus() == Acad::eOk){
												AcDbCurve * endLine1 = AcDbCurve::cast(pEnt3);
												AcGePoint3dArray pts1;
												if(endLine1->intersectWith(line,kExtendThis ,pts1) ==Acad::eOk){
													CreateAcDbLine(pts.first(),pts1.first(),setLayer,m_curColorIndex);
												}
												pEnt3->close();
											}
										}
									}
								}
								endLine->close();
								pEnt1->close();
							}
						}
						AcDbEntityPointer pEnt2(ids.last(), AcDb::kForRead);
						if(pEnt2.openStatus() == Acad::eOk){
							AcDbCurve * endLine = AcDbCurve::cast(pEnt2);
							AcGePoint3dArray pts;
							if(endLine->intersectWith(line,kExtendThis ,pts) ==Acad::eOk){
								if(i < m_nEditBaseDraw)	CreateAcDbLine(pts.first(),taget_pt,setLayer,m_curColorIndex);
							}
							endLine->close();
							pEnt2->close();
						}
					}
					else {if(i < m_nEditBaseDraw)CreateAcDbLine(base_pt,taget_pt,setLayer,m_curColorIndex);}

					double curDist = dblMajorLineOffset*(i);
					for(int j = 0 ; j < m_nEditSubDraw; j++){
						AcGePoint3d taget_pt;
						endLine->getPointAtDist(dblMinorLineOffset*(j+1) +curDist,taget_pt);
						AcDbLine* line1 = new AcDbLine(base_pt,taget_pt);
						if(bisFindMidLine){
							for(int z = 0; z < ids.length(); z+=2){
								AcDbEntityPointer pEnt1(ids[z], AcDb::kForRead);
								if(pEnt1.openStatus() == Acad::eOk){
									AcDbCurve * endLine = AcDbCurve::cast(pEnt1);
									AcGePoint3dArray pts;
									if(endLine->intersectWith(line1,kExtendThis ,pts) ==Acad::eOk){
										AcGeVector3d vec = (taget_pt - base_pt).normalize() * (pts.first().distanceTo(base_pt)*0.5);
										AcGePoint3d result_pt = base_pt + vec ;
										if(z ==0){ --z; CreateAcDbLine(base_pt,result_pt,setLayer,m_curColorIndex);}
 										else{
 											AcDbEntityPointer pEnt3(ids[z+1], AcDb::kForRead);
 											if(pEnt3.openStatus() == Acad::eOk){
 												AcDbCurve * endLine1 = AcDbCurve::cast(pEnt3);
 												AcGePoint3dArray pts1;
 												if(endLine1->intersectWith(line1,kExtendThis ,pts1) ==Acad::eOk){
 													AcGeVector3d vec = (pts1.first() - base_pt).normalize() * (pts.first().distanceTo(pts1.first())*0.5);
 													AcGePoint3d result_pt = pts.first() + vec ;
 													CreateAcDbLine(pts.first(),result_pt,setLayer,m_curColorIndex);
 												}
 												pEnt3->close();
 											}
 										}
									}
									endLine->close();
									pEnt1->close();
								}
						
							}
							AcDbEntityPointer pEnt2(ids.last(), AcDb::kForRead);
							if(pEnt2.openStatus() == Acad::eOk){
								AcDbCurve * endLine = AcDbCurve::cast(pEnt2);
								AcGePoint3dArray pts;
								if(endLine->intersectWith(line1,kExtendThis ,pts) ==Acad::eOk){
									AcGeVector3d vec = (taget_pt - base_pt).normalize() * (pts.first().distanceTo(taget_pt)*0.5);
									AcGePoint3d result_pt = pts.first() + vec ;
									CreateAcDbLine(pts.first(),result_pt,setLayer,m_curColorIndex);
								}
								endLine->close();
								pEnt2->close();
							}
						}
						else{
							AcGeVector3d vec = (taget_pt - base_pt).normalize() * (taget_pt.distanceTo(base_pt)*0.5);
							AcGePoint3d result_pt = base_pt + vec ;
							CreateAcDbLine(base_pt,result_pt,setLayer,m_curColorIndex);
						}
						line1->close();
						delete(line1);
						line1 = NULL;
					}
					line->close();
					delete(line);
					line = NULL;
				}
				if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()){
					CreateAcDbLine(base_pt,deadLine_ep,setLayer,m_curColorIndex);
					CreateAcDbLine(base_pt,deadLine_sp,setLayer,m_curColorIndex);
				}
			}
			pEnt->close();
		}
	}
}

double CDFB::GetRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

void CDFB::OnBnClickedCancel(){
	OnCancel();
}

void CDFB::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult){

	UpdateData();
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta <0)
	{
		m_nEditSubDraw++;
	}
	else
	{
		m_nEditSubDraw--;
	}
	*pResult = 0;
	UpdateData(FALSE);
}

void CDFB::OnBnClickedBcolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();
	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();
	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//색상 변경
void CDFB::SelectColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0)
		colorIndexStr.Format(_T("By Block"));
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0)
		colorIndexStr.Format(_T("By Layer"));

	btnAfterClr = acedGetRGB(colorIndex);
	m_curColorIndex = colorIndex;

	Invalidate();
}

void CDFB::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BCOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct -> hDC);					//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;					//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255,255,255),RGB(0,0,0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);				//버튼색상
		UINT state = lpDrawItemStruct-> itemState;			//버튼상태구하기
		if((state &ODS_SELECTED))
			dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		else
			dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);		

		TCHAR buffer[MAX_PATH];								//버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer,MAX_PATH);						//버퍼초기화
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();                                                  //버튼의 dc 풀어주기
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDFB::OnEnUpdateEdit1(){
	UpdateData(TRUE);
//	m_EditBaseDraw_Str = CheckInputNumber(m_EditBaseDraw_Str);
	UpdateData(FALSE);
	m_EditBaseDraw_Ctrl.SetSel(0, -1);
	m_EditBaseDraw_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//숫자 및 점(.) 입력 체크
CString CDFB::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))
			str.Remove(*getAtChar);
	}
	return str;
}

void CDFB::OnCbnSelchangeCbxlayer(){
	int curLayer = m_cbxLayer.GetCurSel();
	CString layerName;
	m_cbxLayer.GetLBText(m_cbxLayer.GetCurSel(), layerName);
	LayerChangeGetColor(layerName);
}

//도면층별 색상 가져오기
void CDFB::LayerChangeGetColor(CString layerName){
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		AcCmColor color;
		int colorIndex;
		pLayerTblRcd->getName(name);
		color = pLayerTblRcd->color();
		colorIndex = color.colorIndex();
		if(layerName.CompareNoCase(name) == 0)
			SelectColorChange(colorIndex);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CDFB::OnBnClickedBlayerselect(){
	UpdateData();
	BeginEditorCommand();
	
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strLayerName, strMsg;
	AcDbObjectId LayerId;
	strMsg.LoadStringW(IDS_LAYER_SELECT); // "\n레이어를 가져올 객체 선택 :"
	LayerId = GetLayer(strLayerName, strMsg);
	if(LayerId !=NULL)
	{
		m_cbxLayer.SelectString(0,strLayerName);
		int ncolor = GetlayerColor(LayerId);
		SelectColorChange(ncolor);
	}
	
	CompleteEditorCommand();
	UpdateData(FALSE);
}

void CDFB::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData();
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMUpDown->iDelta <0)
	{
		m_nEditBaseDraw++;
	}
	else
	{
		m_nEditBaseDraw--;
	}
	*pResult = 0;
	UpdateData(FALSE);
}



//도면층 컬러 가져오기
int CDFB::GetlayerColor(AcDbObjectId LyId)
{
	int colorindex=-1;

	// try to get the layer we want to rename
	AcDbLayerTableRecordPointer layerTableRec(LyId,  AcDb::kForRead);
	// if ok
	if (layerTableRec.openStatus() == Acad::eOk)
	{

		AcCmColor color = layerTableRec->color();
		colorindex = color.colorIndex();

	}
	return colorindex;
}

// 사용자에게 Entity을 선택 받는다.
AcDbObjectId CDFB::GetLayer(CString& LayerName, CString msg){
	AcDbObjectId Layerid = NULL;
	ads_name entName;
	ads_point pt;
	ACHAR* LName=NULL;
	CString strMsg;

	strMsg.LoadStringW(IDS_SEL_LAY_CANCEL); // "*취소*"
	if(acedEntSel(msg, entName, pt) != RTNORM){
		acutPrintf(strMsg);
		return Layerid;
	}
	struct resbuf *rbEnt;
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


AcDbObjectId CDFB::CreateAcDbLine(AcGePoint3d& sp, AcGePoint3d& ep, CString strLayerName,int color_index )
{
	Acad::ErrorStatus es;
	AcDbObjectId plineObjId;

	AcDbLine *pNewPline = new AcDbLine(sp,ep);
	pNewPline->setLayer(strLayerName);
	pNewPline->setColorIndex(color_index);

	if((es = postToDatabase(pNewPline,plineObjId)) != Acad::eOk){
		delete pNewPline;
		return NULL;
	}
	pNewPline->close();

	return plineObjId;
}

Acad::ErrorStatus CDFB::postToDatabase (AcDbEntity *pEnt, AcDbObjectId& EntID)
{
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



BOOL CDFB::SSGET(AcDbObjectIdArray& ids,CString strfilter){
	BOOL bSuccess = FALSE;
	struct resbuf* filter = acutBuildList(RTDXF0, strfilter, NULL);
	ads_name ent;
	CString strMsg;

	strMsg.LoadStringW(IDS_SELECT_ENTITY);
	const wchar_t* prompts[2] = { strMsg, L"", };
	//int rtcoe = acedSSGet(NULL, NULL, NULL ,filter, ent);
	int rtcoe = acedSSGet(L":$", prompts, NULL, filter, ent);
	if(RTNORM == rtcoe){
		ids.removeAll();
		ss2Array(ent, ids);
		bSuccess = TRUE;
	}
	else bSuccess = FALSE;
	return bSuccess;
}

void CDFB::ss2Array(ads_name ss, AcDbObjectIdArray& ids){
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
		if(es == Acad::eOk)
			ids.append(objId);
	}
	acedSSFree(ss);
}


//도면층 생성
BOOL CDFB::createLayer(CString name,int ColorIndex,AcDbObjectId& LyId) {

	BOOL BRc=FALSE;
	AcDbLayerTable *pLayerTbl = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForWrite);

	if (!pLayerTbl->has(name)) {

		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		pLayerTblRcd->setName(name);

		AcCmColor color;
		color.setColorIndex(ColorIndex); 
		pLayerTblRcd->setColor(color);
		pLayerTbl->add(pLayerTblRcd);
		LyId = pLayerTblRcd->id();
		pLayerTblRcd->close();
		BRc = TRUE;

	} //else acutPrintf(_T("\nLayer already exists"));
	pLayerTbl->close();

	return BRc;

}

//도면층의 이름으로 도면층 가져오기 
BOOL CDFB::FindLayer(CString name,AcDbObjectId& LyId) {

	BOOL bRC = FALSE;
	AcDbLayerTable* pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	AcDbLayerTableIterator* pLayerIterator;
	pLayerTbl->newIterator(pLayerIterator);

	AcDbLayerTableRecord* pLayerTblRcd;
	TCHAR* pLName;
	for (; !pLayerIterator->done(); pLayerIterator->step()) {

		pLayerIterator->getRecord(pLayerTblRcd, AcDb::kForRead);
		pLayerTblRcd->getName(pLName);
		if(name.CompareNoCase(pLName) ==0){
			LyId=pLayerTblRcd->id();
			bRC = TRUE;

		}
		pLayerTblRcd->close();
		acutDelString(pLName);

	}
	delete pLayerIterator;
	pLayerTbl->close();
	return bRC;

}


void CDFB::OnBnClickedDfbHelp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Help_Path = g_modulePath;
	int findNUM = Help_Path.ReverseFind('\\');
	Help_Path.Delete(findNUM, Help_Path.GetLength() - 1);

	CString cadlang;
	cadlang.Format(L"%s", g_CADLang);

	CString lang =
		ReadStringValueInRegistry(HKEY_CURRENT_USER, L"SOFTWARE\\WORKS", L"Language");

	CString helpFile = L"ZDREAM.chm";
#ifdef _ZYXCAD
	helpFile.Replace(L"ZDREAM.chm", L"WORKS.chm");
#endif
	helpFile = GetHelpPath(cadlang, lang, helpFile);

	Help_Path.Format(_T("%s\\%s::/1. html/DFB.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
