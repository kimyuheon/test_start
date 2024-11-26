// GEL.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "GEL.h"
#include "afxdialogex.h"

// CGEL 대화 상자입니다.
IMPLEMENT_DYNAMIC(CGEL, CAcUiDialog)
	CGEL::CGEL(CWnd* pParent)
	: CAcUiDialog(CGEL::IDD, pParent), m_TextHeight_Str(_T(""))
	, m_dotCnt_Str(_T("")){
}

CGEL::~CGEL(){
}

void CGEL::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBXLAYER, m_CbxLayer);
	DDX_Control(pDX, IDC_BLAYERSELECT, m_SelectBn);
	DDX_Control(pDX, IDC_EDTTEXTHEIGHT, m_TextHeight_Ctrl);
	DDX_Text(pDX, IDC_EDTTEXTHEIGHT, m_TextHeight_Str);
	DDX_Text(pDX, IDC_EDTDOTNUM, m_dotCnt_Str);
}

BEGIN_MESSAGE_MAP(CGEL, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CGEL::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CGEL::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CGEL::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHKADDPOINT, &CGEL::OnBnClickedChkaddpoint)
	ON_BN_CLICKED(IDC_CHKDRAWARROW, &CGEL::OnBnClickedChkdrawarrow)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINDOT, &CGEL::OnDeltaposSpindot)
	ON_BN_CLICKED(IDC_CHKEXTEND, &CGEL::OnBnClickedChkextend)
	ON_BN_CLICKED(IDC_BLAYERSELECT, &CGEL::OnBnClickedBlayerselect)
	ON_BN_CLICKED(IDC_BCOLOR, &CGEL::OnBnClickedBcolor)
	ON_CBN_SELCHANGE(IDC_CBXLAYER, &CGEL::OnCbnSelchangeCbxlayer)
	ON_WM_DRAWITEM()
	ON_EN_UPDATE(IDC_EDTTEXTHEIGHT, &CGEL::OnEnUpdateEdttextheight)
END_MESSAGE_MAP()

// CGEL 메시지 처리기입니다.
//초기화
BOOL CGEL::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
    SetIcon(hIcon, FALSE);

	CString pointCreateStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"));
	if(pointCreateStr.CompareNoCase(_T("")) == 0 || pointCreateStr.CompareNoCase(_T("FALSE")) == 0)	CheckDlgButton(IDC_CHKADDPOINT, FALSE);
	else	CheckDlgButton(IDC_CHKADDPOINT, TRUE);
		
	CString drawLeaderLineStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"));
	if(drawLeaderLineStr.CompareNoCase(_T("")) == 0 || drawLeaderLineStr.CompareNoCase(_T("FALSE")) == 0)	CheckDlgButton(IDC_CHKDRAWARROW, FALSE);
	else	CheckDlgButton(IDC_CHKDRAWARROW, TRUE);

	CString extendStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("EXTEND"));
	if(extendStr.CompareNoCase(_T("")) == 0 || extendStr.CompareNoCase(_T("FALSE")) == 0)	CheckDlgButton(IDC_CHKEXTEND, FALSE);
	else	CheckDlgButton(IDC_CHKEXTEND, TRUE);

	CString scaleStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("SCALE"));
	if(scaleStr.CompareNoCase(_T("")) == 0){
		scale = 1000;
		scaleStr.Format(_T("%d"), scale);
	}
	SetDlgItemText(IDC_EDTSCALE, scaleStr);

	CString markText = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELMARK"));
	if(markText.CompareNoCase(_T("")) == 0)	markText = _T("EL=");
	SetDlgItemText(IDC_EDTELSTR, markText);

	CString dotCntStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("DOTPOSITION"));
	if(dotCntStr.CompareNoCase(_T("")) == 0){
		dotCnt = 4;				
		dotCntStr.Format(_T("%d"), dotCnt);
	}
	else	dotCnt = _ttoi(dotCntStr);
	SetDlgItemText(IDC_EDTDOTNUM, dotCntStr);
		
	CString textSizeStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("TEXTSIZE"));
	if(textSizeStr.CompareNoCase(_T("")) == 0){
		textSize = 2.5;
		textSizeStr.Format(_T("%g"), textSize);
	}
	SetDlgItemText(IDC_EDTTEXTHEIGHT, textSizeStr);

	CString spValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"));
	if(spValue.CompareNoCase(_T("")) == 0)	spValue = _T("0");
	SetDlgItemText(IDC_EDTSTARTVALUE, spValue);

	CString epValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"));
	if(epValue.CompareNoCase(_T("")) == 0)	epValue = _T("100");
	SetDlgItemText(IDC_EDTENDVALUE, epValue);

	CString markExtText = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("EXTENDEMARK"));
	if(markExtText.CompareNoCase(_T("")) == 0)	markExtText = _T("L=");
	SetDlgItemText(IDC_EDTEXTENDSTR, markExtText);

	CString extendeDistStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"));
	if(extendeDistStr.CompareNoCase(_T("")) == 0 || extendeDistStr.CompareNoCase(_T("3D")) == 0)	CheckDlgButton(IDC_RADIO1, TRUE);
	else	CheckDlgButton(IDC_RADIO2, TRUE);
	
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
		m_CbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	m_CbxLayer.SetCurSel(0);

	CString curLayerStr;
	m_CbxLayer.GetLBString(m_CbxLayer.GetCurSel(), curLayerStr);
	LayerChangeGetColor(curLayerStr);
	OnCbnSelchangeCbxlayer();
	m_SelectBn.AutoLoad();

	OnBnClickedChkaddpoint();
	OnBnClickedChkextend();

	return TRUE;
}

//두 점 선택
void CGEL::OnBnClickedOk(){
	UpdateData(FALSE);
	CAcUiDialog::OnOK();
	if(!CheckInsertValue())
		return;

	ads_name firsten, seconden;
	ads_point sp, ep;
	AcGePoint3d textStartPosition;

	bool selectionCheck = false;
	if(ads_entsel(_T("\n시작점: "), firsten, sp) == RTNORM){
		if(ads_entsel(_T("\n끝점: "), seconden, ep) == RTNORM)
			selectionCheck = true;
		else
			selectionCheck = false;
	}
	if(!selectionCheck)
		return;

	if(IsDlgButtonChecked(IDC_CHKADDPOINT))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"), _T("FALSE"));

	if(IsDlgButtonChecked(IDC_CHKDRAWARROW))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"), _T("FALSE"));
	
	if(IsDlgButtonChecked(IDC_CHKEXTEND))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTEND"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTEND"), _T("FALSE"));
	
	CString iniStr;
	GetDlgItemText(IDC_EDTSCALE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("SCALE"), iniStr);
	
	GetDlgItemText(IDC_EDTELSTR, markText);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELMARK"), markText);
	
	m_dotCnt_Str.Format(_T("%d"), dotCnt);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DOTPOSITION"), m_dotCnt_Str);
	
	GetDlgItemText(IDC_EDTTEXTHEIGHT, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("TEXTSIZE"), iniStr);
	
	GetDlgItemText(IDC_EDTSTARTVALUE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"), iniStr);
	
	GetDlgItemText(IDC_EDTENDVALUE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"), iniStr);

	GetDlgItemText(IDC_EDTEXTENDSTR, markExtText);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEMARK"), markExtText);

	if(IsDlgButtonChecked(IDC_RADIO1))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"), _T("3D"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"), _T("2D"));
	
	//가상선 그리기
	AcDbLine* pVirtualLine = new AcDbLine(asPnt3d(sp), asPnt3d(ep));
	pVirtualLine->setColorIndex(4);

	//3D 임시점
	AcGePoint3d tempStartP, tempEndP;
	tempStartP = pVirtualLine->startPoint();
	tempEndP = pVirtualLine->endPoint();
	tempStartP.z = _ttof(spValue);
	tempEndP.z = _ttof(epValue);

	pVirtualLine->setDatabaseDefaults();
	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);	//현재 작업창을 쓰자
	pVirtualLine->setDatabaseDefaults();
	blkRec->appendAcDbEntity(pVirtualLine);
	vLineId = pVirtualLine->id();
	pVirtualLine->close();

	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), selectCurLayerName);
	textSize = _ttof(m_TextHeight_Str);

	//작업 시작
	bool loopJob = true;
	ads_point selectPoint;
	while(loopJob){
		double baseDist = asPnt3d(sp).distanceTo(asPnt3d(ep));
		if(ads_getpoint(NULL, _T("\nEL을 구할 점 지정: "), selectPoint) == RTNORM){
			if(chkAddPoint == true || IsDlgButtonChecked(IDC_CHKADDPOINT)){
				AcDbPoint* pPoint = new AcDbPoint(asPnt3d(selectPoint));
				pPoint->setDatabaseDefaults();
				blkRec->appendAcDbEntity(pPoint);
				pPoint->close();
			}
			//지시전 유무
			if(IsDlgButtonChecked(IDC_CHKDRAWARROW)){
			}
			else
				textStartPosition = asPnt3d(selectPoint);
			//텍스트 표기
			double pickDist = asPnt3d(sp).distanceTo(asPnt3d(selectPoint));
			double distPer = pickDist / baseDist * 100;
			double insDist = (_ttof(epValue) - _ttof(spValue)) * (distPer * 0.01) + _ttof(spValue);
			
			CString insText;
			insText.Format(_T("%g"), insDist);
			AcDbText* pPicText = new AcDbText;
			pPicText->setTextString(insText);
			pPicText->setHeight(textSize);
			pPicText->setLayer(selectCurLayerName);
			pPicText->setPosition(textStartPosition);
			pPicText->setDatabaseDefaults();
			blkRec->appendAcDbEntity(pPicText);
			pPicText->close();

			//연장표기 방법 선택
			if(IsDlgButtonChecked(IDC_CHKEXTEND)){
				AcDbText* pAddDist = new AcDbText;
				if(IsDlgButtonChecked(IDC_RADIO1)){
					double temp3DP = tempStartP.distanceTo(asPnt3d(selectPoint));
				}
				insText.Format(_T("%g"), pickDist);
				pAddDist->setTextString(insText);
				pAddDist->setHeight(textSize);
				pAddDist->setLayer(selectCurLayerName);
				pAddDist->setVerticalMode(AcDb::kTextTop);
				pAddDist->setHorizontalMode(AcDb::kTextLeft); 
				pAddDist->setAlignmentPoint(textStartPosition);
				pAddDist->setDatabaseDefaults();
				blkRec->appendAcDbEntity(pAddDist);
				pAddDist->close();
			}
		}
		else
			loopJob = false;
		if(!loopJob)
			break;
	}
	blkRec->close();
}

//폴리선 선택
void CGEL::OnBnClickedButton1(){
	if(!CheckInsertValue()){
		return;
	}
	ads_name en;
	ads_point pt;
	AcGePoint3d sp, ep;

	BeginEditorCommand();
	bool selectionCheck = false;
	if(ads_entsel(_T("\n폴리선 선택: "), en, pt) == RTNORM){
		AcDbEntity* pEnt;
		AcDbObjectId objId;
		acdbGetObjectId(objId, en);
		if(acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) == Acad::eOk){
			if(pEnt->isKindOf(AcDbPolyline::desc())){
				AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
				pPline->getStartPoint(sp);
				pPline->getEndPoint(ep);
				selectionCheck = true;


				pPline->close();
			}
		}
		pEnt->close();
	}	
	if(!selectionCheck){
		acutPrintf(_T("\n폴리선 객체가 아닙니다"));
		CancelEditorCommand();
		return;
	}
	CompleteEditorCommand();
}

//종료
void CGEL::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

BOOL CGEL::CheckInsertValue(void){
	GetDlgItemText(IDC_EDTSTARTVALUE, spValue);
	GetDlgItemText(IDC_EDTENDVALUE, epValue);

	if(spValue.IsEmpty() || epValue.IsEmpty())
		return false;
	else if(spValue.CompareNoCase(epValue) == 0)
		return false;
	else
		return true;
}

void CGEL::OnBnClickedChkdrawarrow(){
	chkAddArrow = true;
}

void CGEL::OnBnClickedChkaddpoint(){
	chkAddPoint = true;
}

void CGEL::OnDeltaposSpindot(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta < 0){
		if(dotCnt >= 6)
			return;
		dotCnt++;
	}
	else{
		if(dotCnt <= 0)
			return;
		dotCnt--;
	}

	SetDlgItemInt(IDC_EDTDOTNUM, dotCnt);
	*pResult = 0;
}

void CGEL::OnBnClickedChkextend(){
	if(IsDlgButtonChecked(IDC_CHKEXTEND)){
		GetDlgItem(IDC_EDTEXTENDSTR)->EnableWindow(true);
		GetDlgItem(IDC_RADIO1)->EnableWindow(true);
		GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	}
	else{
		GetDlgItem(IDC_EDTEXTENDSTR)->EnableWindow(false);
		GetDlgItem(IDC_RADIO1)->EnableWindow(false);
		GetDlgItem(IDC_RADIO2)->EnableWindow(false);
	}
}

void CGEL::OnBnClickedBlayerselect(){
	ads_name en;
	ads_point pt;

	BeginEditorCommand();
	if(acedEntSel(_T("\n도면층을 가져올 객체 선택: "), en, pt) == RTNORM){
		CompleteEditorCommand();

		AcDbObjectId entityId, selectId;
		AcDbEntity* eObj;
		entityId.setFromOldId(en[0]);
		acdbOpenObject(eObj, entityId, AcDb::kForRead);

		selectId = eObj->layerId();
		AcDbLayerTableRecordPointer pSelectLayer(selectId, AcDb::kForRead);

		TCHAR* selectLayerName;
		pSelectLayer->getName(selectLayerName);

		//도면층 불러오기
		AcDbLayerTable* pLayerTbl = NULL;
		AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
		AcDbLayerTableIterator* pLayerTblIter;
		pLayerPoint->newIterator(pLayerTblIter);

		int cbxIndex = 0;
		AcDbObjectId LayerId;
		for(pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()){
			pLayerTblIter->getRecordId(LayerId);
			AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
			TCHAR* name;
			pLayerTblRcd->getName(name);
			if(0 == _tcscmp(selectLayerName, name)){
				m_CbxLayer.SetCurSel(cbxIndex);
				LayerChangeGetColor(selectLayerName);
				break;
			}
			cbxIndex++;
		}
		delete pLayerTblIter;
		delete pLayerTbl;
		OnCbnSelchangeCbxlayer();
	}
	else	CancelEditorCommand();
}

void CGEL::OnBnClickedBcolor(){
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();

	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();

	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//색상 변경
void CGEL::SelectColorChange(int colorIndex){
	CString colorIndexStr;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if(colorIndexStr.CompareNoCase(_T("0")) == 0)			colorIndexStr.Format(_T("블록별"));
	else if(colorIndexStr.CompareNoCase(_T("256")) == 0)	colorIndexStr.Format(_T("도면층별"));

	btnAfterClr = acedGetRGB(colorIndex);
	m_curColorIndex = colorIndex;

	Invalidate();
}

void CGEL::OnCbnSelchangeCbxlayer(){
	int curLayer = m_CbxLayer.GetCurSel();
	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), selectCurLayerName);
	LayerChangeGetColor(selectCurLayerName);
}

//도면층별 색상 가져오기
void CGEL::LayerChangeGetColor(CString layerName){
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
		if(layerName.CompareNoCase(name) == 0){
			SelectColorChange(colorIndex);
		}
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CGEL::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
	if((nIDCtl == IDC_BCOLOR)){
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct -> hDC);					//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;					//버튼영역 구하기
		dc.Draw3dRect(&rect,RGB(255,255,255),RGB(0,0,0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);				//버튼색상
		UINT state = lpDrawItemStruct-> itemState;			//버튼상태구하기
		if((state &ODS_SELECTED))	dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		else						dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);

		TCHAR buffer[MAX_PATH];								//버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer,MAX_PATH);						//버퍼초기화
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();                                                  //버튼의 dc 풀어주기
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGEL::OnEnUpdateEdttextheight(){
	UpdateData(TRUE);
	m_TextHeight_Str = CheckInputNumber(m_TextHeight_Str);
	UpdateData(FALSE);

	m_TextHeight_Ctrl.SetSel(0, -1);
	m_TextHeight_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//소수점 계산
CString CGEL::ReturnNumber(double value){
	CString result = _T("");
	GetDlgItemText(IDC_EDTELSTR, markText);
	switch(dotCnt){
	case 0:
		result.Format(_T("%s%.0lf"), markText, value);
		break;
	case 1:
		result.Format(_T("%s%.1lf"), markText, value);
		break;
	case 2:
		result.Format(_T("%s%.2lf"), markText, value);
		break;
	case 3:
		result.Format(_T("%s%.3lf"), markText, value);
		break;
	case 4:
		result.Format(_T("%s%.4lf"), markText, value);
		break;
	case 5:
		result.Format(_T("%s%.5lf"), markText, value);
		break;
	case 6:
		result.Format(_T("%s%.6lf"), markText, value);
		break;
	default:
		return result;
		break;
	}
	//AfxMessageBox(result);
	return result;
}

//숫자 및 점(.) 입력 체크
CString CGEL::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46)){
			str.Remove(*getAtChar);
		}
	}
	return str;
} 

//숫자 점 체크
bool CGEL::CheckCStringDigit(CString str){
	char atStr;
	int dotflag = 0;
	for(int i = 0 ; i < str.GetLength(); i++){
		if(dotflag == 2){
			return false;
		}
		atStr = str.GetAt(i);
		if(atStr >= '0' && atStr <= '9'){

		}
		else{
			return false;
		}
		if(atStr == '.')	dotflag++;
	}
	return true;
}