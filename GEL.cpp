// GEL.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "GEL.h"
#include "afxdialogex.h"

extern CGEL* pGEL;

// CGEL 대화 상자입니다.
IMPLEMENT_DYNAMIC(CGEL, CAcUiDialog)

CGEL::CGEL(CWnd* pParent)
	: CAcUiDialog(CGEL::IDD, pParent), m_TextHeight_Str(_T(""))
	, m_dotCnt_Str(_T("")) {
}

CGEL::~CGEL() {
}

void CGEL::DoDataExchange(CDataExchange* pDX) {
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBXLAYER, m_CbxLayer);
	DDX_Control(pDX, IDC_BLAYERSELECT, m_SelectBn);
	DDX_Control(pDX, IDC_EDTTEXTHEIGHT, m_TextHeight_Ctrl);
	DDX_Text(pDX, IDC_EDTTEXTHEIGHT, m_TextHeight_Str);
	DDX_Text(pDX, IDC_EDTDOTNUM, m_dotCnt_Str);
	DDX_Text(pDX, IDC_EDTSCALE, m_Scale_Str);
}

BEGIN_MESSAGE_MAP(CGEL, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
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
	ON_EN_UPDATE(IDC_EDTSCALE, &CGEL::OnEnUpdateEdtscale)
	ON_BN_CLICKED(ID_GEL_HELP, &CGEL::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_CHKPTZVALUE, &CGEL::OnBnClickedChkptzvalue)
	ON_BN_CLICKED(IDC_CHK_EL_CONTINUE, &CGEL::OnBnClickedChkElContinue)
END_MESSAGE_MAP()

// CGEL 메시지 처리기입니다.
// 
LRESULT CGEL::onAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

//초기화
BOOL CGEL::OnInitDialog() {
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	CString pointCreateStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"));
	if (pointCreateStr.CompareNoCase(_T("")) == 0 || pointCreateStr.CompareNoCase(_T("FALSE")) == 0)	CheckDlgButton(IDC_CHKADDPOINT, FALSE);
	else	CheckDlgButton(IDC_CHKADDPOINT, TRUE);

	CString pointZvalueStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("POINTZVALUE"));
	if (pointZvalueStr.CompareNoCase(_T("")) == 0 || pointZvalueStr.CompareNoCase(_T("FALSE")) == 0) {
		CheckDlgButton(IDC_CHKPTZVALUE, FALSE);
	} else {
		if (pointCreateStr.CompareNoCase(_T("")) == 0 || pointCreateStr.CompareNoCase(_T("FALSE")) == 0)
			CheckDlgButton(IDC_CHKADDPOINT, FALSE);
		else 
			CheckDlgButton(IDC_CHKADDPOINT, TRUE);

		CheckDlgButton(IDC_CHKPTZVALUE, TRUE);
	}

	CString ELContinueStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELCONTINUE"));
	if (ELContinueStr.CompareNoCase(_T("")) == 0 || ELContinueStr.CompareNoCase(_T("TRUE")) == 0) {
		CheckDlgButton(IDC_CHK_EL_CONTINUE, TRUE);
	} else {
		CheckDlgButton(IDC_CHK_EL_CONTINUE, FALSE);
	}

	CString drawLeaderLineStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"));
	if (drawLeaderLineStr.CompareNoCase(_T("")) == 0 || drawLeaderLineStr.CompareNoCase(_T("FALSE")) == 0)	CheckDlgButton(IDC_CHKDRAWARROW, FALSE);
	else	CheckDlgButton(IDC_CHKDRAWARROW, TRUE);

	CString extendStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("EXTEND"));
	if (extendStr.CompareNoCase(_T("")) == 0 || extendStr.CompareNoCase(_T("FALSE")) == 0)	CheckDlgButton(IDC_CHKEXTEND, FALSE);
	else	CheckDlgButton(IDC_CHKEXTEND, TRUE);

	CString scaleStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("SCALE"));
	if (scaleStr.CompareNoCase(_T("")) == 0) {
		scale = 1000;
		scaleStr.Format(_T("%d"), scale);
	}
	SetDlgItemText(IDC_EDTSCALE, scaleStr);

	// EL 표시 문자 "" 일 시 유지
	CString markText = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELMARK"));
	//if (markText.CompareNoCase(_T("")) == 0)	markText = _T("EL=");
	SetDlgItemText(IDC_EDTELSTR, markText);

	CString dotCntStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("DOTPOSITION"));
	if (dotCntStr.CompareNoCase(_T("")) == 0) {
		dotCnt = 4;
		dotCntStr.Format(_T("%d"), dotCnt);
	}
	else	dotCnt = _ttoi(dotCntStr);
	SetDlgItemText(IDC_EDTDOTNUM, dotCntStr);

	CString textSizeStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("TEXTSIZE"));
	if (textSizeStr.CompareNoCase(_T("")) == 0) {
		textSize = 2.5;
		textSizeStr.Format(_T("%g"), textSize);
	}
	SetDlgItemText(IDC_EDTTEXTHEIGHT, textSizeStr);

	/*	김성곤 대리 수정
	CString spValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"));
	if(spValue.CompareNoCase(_T("")) == 0)	spValue = _T("0");
	SetDlgItemText(IDC_EDTSTARTVALUE, spValue);

	CString epValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"));
	if(epValue.CompareNoCase(_T("")) == 0)	epValue = _T("100");
	SetDlgItemText(IDC_EDTENDVALUE, epValue);
	*/

	CString markExtText = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("EXTENDEMARK"));
	if (markExtText.CompareNoCase(_T("")) == 0)	markExtText = _T("L=");
	SetDlgItemText(IDC_EDTEXTENDSTR, markExtText);

	CString extendeDistStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"));
	if (extendeDistStr.CompareNoCase(_T("")) == 0 || extendeDistStr.CompareNoCase(_T("3D")) == 0)	CheckDlgButton(IDC_RADIO1, TRUE);
	else	CheckDlgButton(IDC_RADIO2, TRUE);

	CString LayerStr = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("LAYER"));
	if (LayerStr.CompareNoCase(_T("")) == 0) {
		LayerStr = "0";
	}
	//도면층 불러오기
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for (pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()) {
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		pLayerTblRcd->getName(name);
		m_CbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;
	int index = m_CbxLayer.SelectString(-1, LayerStr);
	if (index != -1)
	{
		m_CbxLayer.SetCurSel(index);
	}
	else
	{
		m_CbxLayer.SetCurSel(0);
	}

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
void CGEL::OnBnClickedOk()
{
	UpdateData(FALSE);
	//CAcUiDialog::OnOK();
	//CAcUiDialog::ShowWindow(TRUE);
	//if(!CheckInsertValue())
	//	return;
	m_selected = 0;
	acDocManagerPtr()->lockDocument(curDoc());

	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(FALSE);

	CString spValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"));
	if (spValue.CompareNoCase(_T("")) == 0)	spValue = _T("0");

	CString epValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"));
	if (epValue.CompareNoCase(_T("")) == 0)	epValue = _T("100");

	ads_name firsten, seconden;
	ads_point sp, ep, Esp, Eep;
	AcGePoint3d textStartPosition;
	ads_point sp_xformres[4], ep_xformres[4];

	struct resbuf* sp_refstkres, * ep_refstkres;

	CString old_spValue = spValue;
	CString old_epValue = epValue;
	CString midValue, strMsg, promptMsg;

	struct resbuf fromrb, toWCS, toDCS;
	fromrb.restype = RTSHORT;
	fromrb.resval.rint = 1; // UCS
	toWCS.restype = RTSHORT;
	toWCS.resval.rint = 0; // WCS 
	toDCS.restype = RTSHORT;
	toDCS.resval.rint = 2; // DCS

	bool selectionCheck = false;
	acedGetAcadDwgView()->SetFocus();
	
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	
	strMsg.LoadStringW(IDS_GEL_START_SEL); // "\n시작점 선택 :"
	if (ads_getpoint(NULL, strMsg, sp) == RTNORM)
	{
		strMsg.LoadStringW(IDS_GEL_START_EL_SEL); // "\n시작점 EL 선택 또는 EL값 입력<"
		if (ads_nentsel(strMsg + spValue + _T(">: "), firsten, Esp, sp_xformres, &sp_refstkres) == RTNORM)
		{
			AcDbObjectId objId;
			acdbGetObjectId(objId, firsten);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);

			if (pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText* pText = AcDbText::cast(pEnt);
				spValue = pText->textString();
				spValue = CheckInputNumber(spValue);
				pText->close();
			}

			else if (pEnt->isKindOf(AcDbMText::desc()))
			{
				AcDbMText* pMText = AcDbMText::cast(pEnt);
				spValue = pMText->contents();
				spValue = CheckInputNumber(spValue);
				pMText->close();
			}
		}
		else
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || GetAsyncKeyState(VK_ESCAPE) & 0x8001)
			{
				((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
				acDocManagerPtr()->unlockDocument(curDoc());
				return;
			}
			promptMsg = strMsg + spValue + _T(">: ");

			resbuf zValueStr;
			ads_getvar(_T("LastPrompt"), &zValueStr);
			spValue = zValueStr.resval.rstring;
			promptMsg.Replace(L"\n", L"");
			//spValue.Replace(promptMsg, L"");
			spValue = CheckCStringNumber(spValue);
			if (spValue == " " || spValue == "")
			{
				spValue = old_spValue;
			}
		}

		strMsg.LoadStringW(IDS_GEL_END_SEL); // "\n끝점 선택 : "
		if (ads_getpoint(sp, strMsg, ep) == RTNORM)
		{
			strMsg.LoadStringW(IDS_GEL_END_EL_SEL); // "\n끝점 EL 선택 또는 EL값 입력<"
			if (ads_nentsel(strMsg + epValue + _T(">: "), seconden, Eep, ep_xformres, &ep_refstkres) == RTNORM)
			{
				AcDbObjectId objId1;
				acdbGetObjectId(objId1, seconden);
				AcDbEntityPointer pEnt1(objId1, AcDb::kForRead);

				if (pEnt1->isKindOf(AcDbText::desc()))
				{
					AcDbText* pText1 = AcDbText::cast(pEnt1);
					epValue = pText1->textString();
					epValue = CheckInputNumber(epValue);
					pText1->close();
				}

				else if (pEnt1->isKindOf(AcDbMText::desc()))
				{
					AcDbMText* pMText1 = AcDbMText::cast(pEnt1);
					epValue = pMText1->contents();
					epValue = CheckInputNumber(epValue);
					pMText1->close();
				}
			}
			else
			{
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || GetAsyncKeyState(VK_ESCAPE) & 0x8001)
				{ 
					((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
					acDocManagerPtr()->unlockDocument(curDoc());
					return;
				}
				promptMsg = strMsg + epValue + _T(">: ");

				int len, findlen;
				resbuf zValueStr;
				ads_getvar(_T("LastPrompt"), &zValueStr);
				epValue = zValueStr.resval.rstring;
				promptMsg.Replace(L"\n", L"");
				//epValue.Replace(promptMsg, L"");
				epValue = CheckCStringNumber(epValue);
				if (epValue == " " || epValue == "")
				{
					epValue = old_epValue;
				}
			}
			selectionCheck = true;
		}
		else
		{ 
			((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			selectionCheck = false;
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
		selectionCheck = false;
	}
	/*
	if(ads_entsel(_T("\n시작점: "), firsten, sp) == RTNORM){
		if(ads_entsel(_T("\n끝점: "), seconden, ep) == RTNORM)
			selectionCheck = true;
		else
			selectionCheck = false;
	}
	*/
	if (!selectionCheck){
		acDocManagerPtr()->unlockDocument(curDoc());
		return;
	}
	if (IsDlgButtonChecked(IDC_CHKADDPOINT))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHKPTZVALUE))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("POINTZVALUE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("POINTZVALUE"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHK_EL_CONTINUE))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELCONTINUE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELCONTINUE"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHKDRAWARROW))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHKEXTEND))
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

	//GetDlgItemText(IDC_EDTSTARTVALUE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"), spValue);

	//GetDlgItemText(IDC_EDTENDVALUE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"), epValue);

	GetDlgItemText(IDC_EDTEXTENDSTR, markExtText);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEMARK"), markExtText);

	if (IsDlgButtonChecked(IDC_RADIO1))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"), _T("3D"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"), _T("2D"));

	CString iniLay;
	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), iniLay);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("LAYER"), iniLay);

	// 현재 문자스타일 ID 얻는 방법//
	CString CurTextStyle;
	resbuf zValueStr;
	ads_getvar(_T("TEXTSTYLE"), &zValueStr);
	CurTextStyle.Format(_T("%s"), zValueStr.resval.rstring);

	AcDbTextStyleTable* pTextStyleTable;
	AcDbObjectId CurTextStyleId;
	acdbCurDwg()->getTextStyleTable(pTextStyleTable, AcDb::kForRead);
	pTextStyleTable->getAt(CurTextStyle, CurTextStyleId);
	pTextStyleTable->close();

	//가상선 그리기
	//[김의헌] AcGiTransientManager 함수 사용하여 다시 구연
	//작업 시작
	/*auto pVirtualLine = new AcDbLine();
	pVirtualLine->setColorIndex(4);
	pVirtualLine->setDatabaseDefaults();*/

	AcGePoint3d tempStartP, tempEndP;
	acedTrans(sp, &fromrb, &toWCS, FALSE, sp);
	acedTrans(ep, &fromrb, &toWCS, FALSE, ep);
	tempStartP = asPnt3d(sp);
	tempEndP = asPnt3d(ep);
	//pVirtualLine->setStartPoint(AcGePoint3d(tempStartP));
	//pVirtualLine->setEndPoint(AcGePoint3d(tempEndP));

	// 객체 스냅 문제로 실제 라인 그리기로 변경
	//AcGiTransientManager* pTransientManager = acgiGetTransientManager();
	//struct resbuf res;
	//acedGetVar(L"CVPORT", &res);
	//pTransientManager->addTransient(pVirtualLine, kAcGiMain, 0, (AcDbIntArray)res.resval.rint);

	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);	//현재 작업창을 쓰자

	AcDbLine* pVirtualLine = new AcDbLine(asPnt3d(sp), asPnt3d(ep));
	pVirtualLine->setColorIndex(4);
	pVirtualLine->setDatabaseDefaults();
	blkRec->appendAcDbEntity(pVirtualLine);
	vLineId = pVirtualLine->id();
	pVirtualLine->close();
	//blkRec->close();
	/*
	//acedGrDraw(sp,ep,4,0); 오스냅 관련하여 실제 라인을 생성하기로 변경
	AcDbLine* pVirtualLine = new AcDbLine(asPnt3d(sp), asPnt3d(ep));
	pVirtualLine->setColorIndex(4);
	pVirtualLine->setDatabaseDefaults();
	//3D 임시점 (김성곤 대리 수정)
	AcGePoint3d tempStartP, tempEndP;
	//tempStartP = pVirtualLine->startPoint();
	//tempEndP = pVirtualLine->endPoint();
	tempStartP = asPnt3d(sp);
	tempEndP = asPnt3d(ep);
	tempStartP.z = _ttof(spValue);
	tempEndP.z = _ttof(epValue);

	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);	//현재 작업창을 쓰자

	blkRec->appendAcDbEntity(pVirtualLine);
	vLineId = pVirtualLine->id();
	pVirtualLine->close();
	//blkRec->close();
	*/

	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), selectCurLayerName);
	textSize = _ttof(m_TextHeight_Str);
	scale = _ttof(m_Scale_Str);

	//AcDbBlockTableRecord* blkRec2 = new AcDbBlockTableRecord;	
	//acdbOpenObject(blkRec2, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);

	loopJob = true;
	ads_point selectPoint, tmpPoint;
	ACHAR* tmp;
	while (loopJob)
	{
		double baseDist = asPnt3d(sp).distanceTo(asPnt3d(ep));
		strMsg.LoadStringW(IDS_GEL_EL_PICK); // "\nEL을 구할 점 지정: "
		tmpPoint[0] = selectPoint[0];
		tmpPoint[1] = selectPoint[1];
		tmpPoint[2] = selectPoint[2];
		int state = ads_getpoint(NULL, strMsg, selectPoint);
		if (state == RTNORM) {
			double NumMidValue = 0.0;
			bool bNumMidValue = false;
			resbuf zValueStr;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			midValue = zValueStr.resval.rstring;
			m_before_cmd = midValue;
			strMsg.Replace(L"\n", L"");
			int itemp = midValue.Find(strMsg);
			if (midValue.Find(strMsg) == -1) { // 좌표 클릭
				midValue = "";
			} else if (m_cur_cmd == m_before_cmd) { // 좌표 클릭
				midValue = "";
			} else if (m_before_cmd.GetLength() > m_cur_cmd.GetLength()) {
				midValue = CheckCStringNumber(midValue);
			} else { // 입력값 추출
				midValue = CheckCStringNumber(midValue);
			}
			m_cur_cmd = m_before_cmd;

			acedTrans(selectPoint, &fromrb, &toWCS, FALSE, selectPoint);

			if (midValue == "")	{
				textStartPosition = asPnt3d(selectPoint);
			} else {
				bNumMidValue = true;
				NumMidValue = _wtof(midValue);
			}
			double pickDist;
			double distPer;
			double insDist;
			AcGePoint3d MidPoint, tmp3Point;
			ads_point tmp_point;
			// 김성곤 대리 수정 (z값이 다를경우 값이 비정상적으로 출력되는 현상에 대한 수정) z값은 모두 0으로 처리			
			selectPoint[2] = 0.0;
			sp[2] = 0.0;
			ep[2] = 0.0;
			// 끝
			if (!bNumMidValue)	{
				//텍스트 표기
				pickDist = asPnt3d(sp).distanceTo(asPnt3d(selectPoint));
				distPer = pickDist / baseDist * 100;
				insDist = ((_ttof(epValue) - _ttof(spValue)) * (distPer * 0.01) + _ttof(spValue)) /** scale / 1000*/;
				//selectPoint[2] = insDist;
				tmp3Point = asPnt3d(selectPoint);
			} else {
				double totdist;
				insDist = NumMidValue;
				AcDbLine* tmpLine = new AcDbLine;
				tmpLine->setStartPoint(asPnt3d(sp));
				tmpLine->setEndPoint(asPnt3d(ep));
				tmpLine->getDistAtPoint(asPnt3d(ep), totdist);
				if ((insDist - _ttof(spValue)) >= 0) {
					pickDist = totdist * (insDist - _ttof(spValue)) / (_ttof(epValue) - _ttof(spValue));
					tmpLine->getPointAtDist(pickDist, MidPoint);
				} else {
					pickDist = totdist * abs((insDist - _ttof(spValue))) / (_ttof(epValue) - _ttof(spValue));
					tmpLine->getPointAtDist(-pickDist, MidPoint);
				}
				MidPoint.z = insDist;
				tmp3Point = MidPoint;
			}

			CString insText;

			insText = ReturnNumber(insDist); // 김성곤 대리 수정
			// POINT 생성
			if (IsDlgButtonChecked(IDC_CHKADDPOINT)) {
				if (IsDlgButtonChecked(IDC_CHKPTZVALUE)) {
					selectPoint[2] = insDist;
				}
				AcDbPoint* pPoint = new AcDbPoint(asPnt3d(selectPoint));
				pPoint->setDatabaseDefaults();
				blkRec->appendAcDbEntity(pPoint);
				pPoint->close();
			}
			//insText.Format(_T("%g"), insDist); // 김성곤 대리 수정
			AcDbText* pPicText = new AcDbText;
			pPicText->setTextString(insText);
			pPicText->setHeight(textSize);
			pPicText->setLayer(selectCurLayerName);
			pPicText->setTextStyle(CurTextStyleId);
			pPicText->setColorIndex(m_curColorIndex);

			// 지시선 표기
			if (IsDlgButtonChecked(IDC_CHKDRAWARROW)) {
				AcGePoint3d left_point3d, right_point3d, Max_TextPoint, Min_TextPoint, new_textpoint;
				AcDbExtents textbox;
				if (NumMidValue == 0.0) {
					//pPicText->setPosition(textStartPosition);
					pPicText->getGeomExtents(textbox);
					Max_TextPoint = textbox.maxPoint();
					Min_TextPoint = textbox.minPoint();
					double pDist = sqrt(2 * pow(textSize * 2, 2)); // 지시선 왼쪽 point
					double width = Max_TextPoint.x - Min_TextPoint.x; // 지시선 오른쪽 point					
					ads_polar(asDblArray(textStartPosition), DTR(45.0), pDist, asDblArray(left_point3d));
					ads_polar(asDblArray(left_point3d), DTR(0), width * 0.25 + width, asDblArray(right_point3d));
					new_textpoint.x = left_point3d.x;
					new_textpoint.y = left_point3d.y + textSize / 2;
					new_textpoint.z = textStartPosition.z;

					AcGePoint2d base_point2d, left_point2d, right_point2d;
					base_point2d.x = textStartPosition.x;
					base_point2d.y = textStartPosition.y;
					left_point2d.x = left_point3d.x;
					left_point2d.y = left_point3d.y;
					right_point2d.x = right_point3d.x;
					right_point2d.y = right_point3d.y;

					AcDbPolyline* ArrPline = new AcDbPolyline;
					ArrPline->addVertexAt(0, base_point2d);
					ArrPline->addVertexAt(1, left_point2d);
					ArrPline->addVertexAt(2, right_point2d);
					ArrPline->setLayer(selectCurLayerName);
					ArrPline->setDatabaseDefaults();
					blkRec->appendAcDbEntity(ArrPline);
					ArrPline->close();

					pPicText->setPosition(new_textpoint);
				} else {
					//pPicText->setPosition(MidPoint);
					pPicText->getGeomExtents(textbox);
					Max_TextPoint = textbox.maxPoint();
					Min_TextPoint = textbox.minPoint();
					double pDist = sqrt(2 * pow(textSize * 2, 2)); // 지시선 왼쪽 point
					double width = Max_TextPoint.x - Min_TextPoint.x; // 지시선 오른쪽 point					
					ads_polar(asDblArray(MidPoint), DTR(45.0), pDist, asDblArray(left_point3d));
					ads_polar(asDblArray(left_point3d), DTR(0), width * 0.25 + width, asDblArray(right_point3d));
					new_textpoint.x = left_point3d.x;
					new_textpoint.y = left_point3d.y + textSize / 2;
					new_textpoint.z = MidPoint.z;

					AcGePoint2d base_point2d, left_point2d, right_point2d;
					base_point2d.x = MidPoint.x;
					base_point2d.y = MidPoint.y;
					left_point2d.x = left_point3d.x;
					left_point2d.y = left_point3d.y;
					right_point2d.x = right_point3d.x;
					right_point2d.y = right_point3d.y;

					AcDbPolyline* ArrPline = new AcDbPolyline;
					ArrPline->addVertexAt(0, base_point2d);
					ArrPline->addVertexAt(1, left_point2d);
					ArrPline->addVertexAt(2, right_point2d);
					ArrPline->setLayer(selectCurLayerName);
					ArrPline->setDatabaseDefaults();
					blkRec->appendAcDbEntity(ArrPline);
					ArrPline->close();

					pPicText->setPosition(new_textpoint);
				}
			} else {
				if (!bNumMidValue) {
					pPicText->setPosition(textStartPosition);
				} else {
					pPicText->setPosition(MidPoint);
				}
			}

			pPicText->setDatabaseDefaults();
			blkRec->appendAcDbEntity(pPicText);
			pPicText->close();

			//연장표기 방법 선택
			if (IsDlgButtonChecked(IDC_CHKEXTEND)) {
				AcDbText* pAddDist = new AcDbText;
				if (IsDlgButtonChecked(IDC_RADIO1)) { // 3d 거리
					double temp3DP = tempStartP.distanceTo(tmp3Point);
					double tmpDist = tempStartP.distanceTo(tempEndP);
					temp3DP = sqrt(pow(temp3DP * (double)scale / 1000, 2) + pow(_ttof(insText) - _ttof(spValue), 2));
					insText = LReturnNumber(temp3DP);
				} else { // 2d 거리				
					pickDist = pickDist * (double)scale / 1000;
					insText = LReturnNumber(pickDist); // 김성곤 대리 수정
				}
				//insText.Format(_T("%g"), pickDist); // 김성곤 대리 수정
				pAddDist->setTextString(insText);
				pAddDist->setDatabaseDefaults();
				pAddDist->setHeight(textSize);
				pAddDist->setLayer(selectCurLayerName);
				pAddDist->setTextStyle(CurTextStyleId);
				pAddDist->setVerticalMode(AcDb::kTextTop);
				pAddDist->setHorizontalMode(AcDb::kTextLeft);
				pAddDist->setColorIndex(m_curColorIndex);
				if (NumMidValue == 0.0)	{
					AcGePoint3d textint;
					ads_polar(asDblArray(textStartPosition), DTR(-90.0), (double)textSize, asDblArray(textint));
					pAddDist->setAlignmentPoint(textint);
				} else {
					AcGePoint3d textint;
					ads_polar(asDblArray(MidPoint), DTR(-90.0), (double)textSize, asDblArray(textint));
					pAddDist->setAlignmentPoint(textint);
				}
				blkRec->appendAcDbEntity(pAddDist);
				pAddDist->close();
			}
		} else {
			AcDbEntity* tmpEnt;
			acdbOpenAcDbEntity(tmpEnt, vLineId, AcDb::kForWrite);
			tmpEnt->erase();
			tmpEnt->close();
			//pTransientManager->eraseTransient(pVirtualLine, (AcDbIntArray)res.resval.rint);
			((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			loopJob = false;
		}
		if (!IsDlgButtonChecked(IDC_CHK_EL_CONTINUE)) {
			AcDbEntity* tmpEnt;
			acdbOpenAcDbEntity(tmpEnt, vLineId, AcDb::kForWrite);
			tmpEnt->erase();
			tmpEnt->close();
			((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			loopJob = false;
		}
		if (!loopJob) {
			break;
		}
	}
	blkRec->close();
	acDocManagerPtr()->unlockDocument(curDoc());

	if (!IsDlgButtonChecked(IDC_CHK_EL_CONTINUE)) {
		acutPrintf(L"\n");
		
		acedGetAcadDwgView()->SetFocus();
		acTransactionManagerPtr()->flushGraphics();
		acedUpdateDisplay();
		this->SetFocus();
		((CButton*)GetDlgItem(IDOK))->SetFocus();
		GotoDlgCtrl(GetDlgItem(IDOK));
	}
	//CompleteEditorCommand();
}

void CGEL::test()
{
	if (!IsDlgButtonChecked(IDC_CHK_EL_CONTINUE)) {
		acutPrintf(L"\n");

		acedGetAcadDwgView()->SetFocus();
		acTransactionManagerPtr()->flushGraphics();
		acedUpdateDisplay();
		this->SetFocus();
		((CButton*)GetDlgItem(IDOK))->SetFocus();
		GotoDlgCtrl(GetDlgItem(IDOK));
	}
}

//폴리선 선택
void CGEL::OnBnClickedButton1() {
	UpdateData(FALSE);
	//CAcUiDialog::OnOK();
	//if(!CheckInsertValue())
	//	return;
	m_selected = 1;
	acDocManagerPtr()->lockDocument(curDoc());

	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(FALSE);

	CString spValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"));
	if (spValue.CompareNoCase(_T("")) == 0)	spValue = _T("0");

	CString epValue = iniFile.readINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"));
	if (epValue.CompareNoCase(_T("")) == 0)	epValue = _T("100");

	ads_name en;
	ads_point pt;
	AcGePoint3d Pointsp, Pointep, NVertex;
	AcGePoint3dArray SelArr;
	double dist1, dist2, Totaldist, selpt;
	int iVertex;

	ads_name firsten, seconden;
	ads_point sp, ep, Esp, Eep;
	AcGePoint3d textStartPosition;
	ads_point sp_xformres[4], ep_xformres[4];

	struct resbuf* sp_refstkres, * ep_refstkres;

	CString old_spValue = spValue;
	CString old_epValue = epValue;
	CString midValue, strMsg, promptMsg;

	AcDbEntity* pEnt;
	AcDbObjectId objId;
	acdbGetObjectId(objId, en);
	AcDbPolyline* pPline;
	Acad::ErrorStatus es;

	struct resbuf fromrb, toWCS, toDCS;
	fromrb.restype = RTSHORT;
	fromrb.resval.rint = 1; // UCS
	toWCS.restype = RTSHORT;
	toWCS.resval.rint = 0; // WCS 
	toDCS.restype = RTSHORT;
	toDCS.resval.rint = 2; // DCS

	bool selectionCheck = false;
	acedGetAcadDwgView()->SetFocus();
	//BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	strMsg.LoadStringW(IDS_CAL_BASELINE_SEL); // "\nEL 계산 기준 선 선택: "
	if (ads_entsel(strMsg, en, pt) == RTNORM)
	{
		if (acdbGetObjectId(objId, en) != Acad::eOk)
			return;
		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead) == Acad::eOk)
		{
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				pPline = AcDbPolyline::cast(pEnt);
				iVertex = pPline->numVerts();
				for (unsigned int i = 0; i < iVertex; i++)
				{
					pPline->getPointAt(i, NVertex);
					SelArr.append(NVertex);
				}
				selectionCheck = true;

				strMsg.LoadStringW(IDS_GEL_START_SEL); // "\n시작점 선택 :"
				if (ads_getpoint(NULL, strMsg, sp) == RTNORM)
				{
					acedTrans(sp, &fromrb, &toWCS, FALSE, sp);
					es = pPline->getDistAtPoint(asPnt3d(sp), dist1);
					if (es != Acad::eOk) {
						AcGePoint3d tmp;
						pPline->getClosestPointTo(asPnt3d(sp), tmp);
						pPline->getDistAtPoint(tmp, dist1);
					}
					strMsg.LoadStringW(IDS_GEL_START_EL_SEL); // "\n시작점 EL 선택 또는 EL값 입력<"
					if (ads_nentsel(strMsg + spValue + _T(">: "), firsten, Esp, sp_xformres, &sp_refstkres) == RTNORM)
					{
						AcDbObjectId objId;
						acdbGetObjectId(objId, firsten);
						AcDbEntityPointer pEnt(objId, AcDb::kForRead);

						if (pEnt->isKindOf(AcDbText::desc()))
						{
							AcDbText* pText = AcDbText::cast(pEnt);
							spValue = pText->textString();
							spValue = CheckInputNumber(spValue);
							pText->close();
						}

						else if (pEnt->isKindOf(AcDbMText::desc()))
						{
							AcDbMText* pMText = AcDbMText::cast(pEnt);
							spValue = pMText->contents();
							spValue = CheckInputNumber(spValue);
							pMText->close();
						}
					}
					else
					{
						if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || GetAsyncKeyState(VK_ESCAPE) & 0x8001)
						{
							((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
							((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
							((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
							((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
							pPline->close();
							acDocManagerPtr()->unlockDocument(curDoc());
							return;
						}
						promptMsg = strMsg + spValue + _T(">: ");

						resbuf zValueStr;
						ads_getvar(_T("LastPrompt"), &zValueStr);
						spValue = zValueStr.resval.rstring;
						promptMsg.Replace(L"\n", L"");
						//spValue.Replace(promptMsg, L"");
						spValue = CheckCStringNumber(spValue);
						if (spValue == " " || spValue == "")
						{
							spValue = old_spValue;
						}
					}

					strMsg.LoadStringW(IDS_GEL_END_SEL); // "\n끝점 선택 : "
					if (ads_getpoint(sp, strMsg, ep) == RTNORM)
					{
						acedTrans(ep, &fromrb, &toWCS, FALSE, ep);
						es = pPline->getDistAtPoint(asPnt3d(ep), dist2);
						if (es != Acad::eOk) {
							AcGePoint3d tmp;
							pPline->getClosestPointTo(asPnt3d(ep), tmp);
							pPline->getDistAtPoint(tmp, dist2);
						}
						strMsg.LoadStringW(IDS_GEL_END_EL_SEL); // "\n끝점 EL 선택 또는 EL값 입력<"
						if (ads_nentsel(strMsg + epValue + _T(">: "), seconden, Eep, ep_xformres, &ep_refstkres) == RTNORM)
						{
							AcDbObjectId objId1;
							acdbGetObjectId(objId1, seconden);
							AcDbEntityPointer pEnt1(objId1, AcDb::kForRead);

							if (pEnt1->isKindOf(AcDbText::desc()))
							{
								AcDbText* pText1 = AcDbText::cast(pEnt1);
								epValue = pText1->textString();
								epValue = CheckInputNumber(epValue);
								pText1->close();
							}

							else if (pEnt1->isKindOf(AcDbMText::desc()))
							{
								AcDbMText* pMText1 = AcDbMText::cast(pEnt1);
								epValue = pMText1->contents();
								epValue = CheckInputNumber(epValue);
								pMText1->close();
							}
						}
						else
						{
							if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || GetAsyncKeyState(VK_ESCAPE) & 0x8001)
							{
								((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
								((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
								((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
								((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
								acDocManagerPtr()->unlockDocument(curDoc());
								pPline->close();
								return;
							}
							promptMsg = strMsg + epValue + _T(">: ");

							int len, findlen;
							resbuf zValueStr;
							ads_getvar(_T("LastPrompt"), &zValueStr);
							epValue = zValueStr.resval.rstring;
							promptMsg.Replace(L"\n", L"");
							//epValue.Replace(promptMsg, L"");
							epValue = CheckCStringNumber(epValue);
							if (epValue == " " || epValue == "")
							{
								epValue = old_epValue;
							}
						}
						selectionCheck = true;
					}
					else
					{
						((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
						((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
						((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
						((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
						selectionCheck = false;
					}
				}
				else
				{
					((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
					pPline->close();
					acDocManagerPtr()->unlockDocument(curDoc());
					return;
				}
			}
			pEnt->close();
		}
	}

	if (!selectionCheck) {
		strMsg.LoadStringW(IDS_CHECK_PLINE); // "\n폴리선 객체가 아닙니다"
		acutPrintf(strMsg);
		//pGEL = NULL;
		//CancelEditorCommand();
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
		acDocManagerPtr()->unlockDocument(curDoc());

		return;
	}

	// ini 파일 적용
	if (IsDlgButtonChecked(IDC_CHKADDPOINT))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("CREATEPOINT"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHKPTZVALUE))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("POINTZVALUE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("POINTZVALUE"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHK_EL_CONTINUE))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELCONTINUE"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELCONTINUE"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHKDRAWARROW))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("DRAWLEADER"), _T("FALSE"));

	if (IsDlgButtonChecked(IDC_CHKEXTEND))
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

	//GetDlgItemText(IDC_EDTSTARTVALUE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELSTARTNUM"), spValue);

	//GetDlgItemText(IDC_EDTENDVALUE, iniStr);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("ELENDNUM"), epValue);

	GetDlgItemText(IDC_EDTEXTENDSTR, markExtText);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEMARK"), markExtText);

	if (IsDlgButtonChecked(IDC_RADIO1))
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"), _T("3D"));
	else
		iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("EXTENDEDIST"), _T("2D"));

	CString iniLay;
	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), iniLay);
	iniFile.writeINI(_T("config.ini"), _T("GEL"), _T("LAYER"), iniLay);

	// 현재 문자스타일 ID 얻는 방법//
	CString CurTextStyle;
	resbuf zValueStr;
	ads_getvar(_T("TEXTSTYLE"), &zValueStr);
	CurTextStyle.Format(_T("%s"), zValueStr.resval.rstring);

	AcDbTextStyleTable* pTextStyleTable;
	AcDbObjectId CurTextStyleId;
	acdbCurDwg()->getTextStyleTable(pTextStyleTable, AcDb::kForRead);
	pTextStyleTable->getAt(CurTextStyle, CurTextStyleId);
	pTextStyleTable->close();

	// [+] 가상선 그리기
	auto pVirtualPLine = new AcDbPolyline();
	pVirtualPLine->setColorIndex(4);
	pVirtualPLine->setDatabaseDefaults();

	AcGePoint3d tempStartP, tempEndP;
	tempStartP = asPnt3d(sp);
	tempEndP = asPnt3d(ep);

	for (unsigned int i = 0; i < iVertex; i++)
	{
		//pVirtualPLine->getPointAt(i, SelArr[i]);
		pVirtualPLine->addVertexAt(i, AcGePoint2d(SelArr[i].x, SelArr[i].y));
	}

	AcGiTransientManager* pTransientManager = acgiGetTransientManager();
	struct resbuf res;
	acedGetVar(L"CVPORT", &res);
	pTransientManager->addTransient(pVirtualPLine, kAcGiMain, 0, (AcDbIntArray)res.resval.rint);
	// [-] 가상선 그리기

	AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);	//현재 작업창을 쓰자

	// 도면층
	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), selectCurLayerName);
	textSize = _ttof(m_TextHeight_Str);
	scale = _ttof(m_Scale_Str);

	// 작업 시작
	bool loopJob = true;
	ads_point selectPoint;
	AcGePoint3d perpt;
	ACHAR* tmp;

	while (loopJob)
	{
		//double baseDist = asPnt3d(sp).distanceTo(asPnt3d(ep));
		double baseDist = dist2 - dist1;

		strMsg.LoadStringW(IDS_GEL_EL_PICK);
		if (ads_getpoint(NULL, strMsg, selectPoint) == RTNORM)
		{
			double NumMidValue = 0.0;
			bool bNumMidValue = false;
			resbuf zValueStr;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			midValue = zValueStr.resval.rstring;
			m_before_cmd = midValue;
			strMsg.Replace(L"\n", L"");
			int itemp = midValue.Find(strMsg);
			if (midValue.Find(strMsg) == -1) { // 좌표 클릭
				midValue = "";
			} else if (m_cur_cmd == m_before_cmd) { // 좌표 클릭
				midValue = "";
			} else if (m_before_cmd.GetLength() > m_cur_cmd.GetLength()) {
				midValue = CheckCStringNumber(midValue);
			} else { // 입력값 추출
				midValue = CheckCStringNumber(midValue);
			}
			m_cur_cmd = m_before_cmd;
			acedTrans(selectPoint, &fromrb, &toWCS, FALSE, selectPoint);

			pPline->getClosestPointTo(asPnt3d(selectPoint), perpt);
			//acutPrintf(_T("perpt : X = %f, Y = %f, Z = %f\n"), perpt.x, perpt.y, perpt.z);

			pPline->getDistAtPoint(perpt, selpt);

			if (midValue == "") {
				textStartPosition = asPnt3d(selectPoint);
			} else {
				bNumMidValue = true;
				NumMidValue = _wtof(midValue);
			}

			double pickDist;
			double distPer;
			double insDist;
			AcGePoint3d MidPoint, tmp3Point;
			ads_point tmp_point;

			selectPoint[2] = 0.0;
			sp[2] = 0.0;
			ep[2] = 0.0;

			if (!bNumMidValue)
			{
				//텍스트 표기
				pickDist = selpt - dist1;
				distPer = pickDist / baseDist * 100;
				//scale 연산은 연장 표시에만 적용
				insDist = ((_ttof(epValue) - _ttof(spValue)) * (distPer * 0.01) + _ttof(spValue))/* * scale / 1000*/;
				//selectPoint[2] = insDist;
				tmp3Point = asPnt3d(selectPoint);
			}
			else
			{
				double totdist;
				insDist = NumMidValue;
				AcDbLine* tmpLine = new AcDbLine;
				tmpLine->setStartPoint(asPnt3d(sp));
				tmpLine->setEndPoint(asPnt3d(ep));
				tmpLine->getDistAtPoint(asPnt3d(ep), totdist);
				if ((insDist - _ttof(spValue)) >= 0)
				{
					pickDist = totdist * (insDist - _ttof(spValue)) / (_ttof(epValue) - _ttof(spValue));
					tmpLine->getPointAtDist(pickDist, MidPoint);
				}
				else
				{
					pickDist = totdist * abs((insDist - _ttof(spValue))) / (_ttof(epValue) - _ttof(spValue));
					tmpLine->getPointAtDist(-pickDist, MidPoint);
				}
				MidPoint.z = insDist;
				tmp3Point = MidPoint;
			}

			CString insText;

			insText = ReturnNumber(insDist);
			// POINT 생성
			if (IsDlgButtonChecked(IDC_CHKADDPOINT))
			{
				if (IsDlgButtonChecked(IDC_CHKPTZVALUE)) {
					selectPoint[2] = insDist;
				}
				AcDbPoint* pPoint = new AcDbPoint(asPnt3d(selectPoint));
				pPoint->setDatabaseDefaults();
				blkRec->appendAcDbEntity(pPoint);
				pPoint->close();
			}
			AcDbText* pPicText = new AcDbText;
			//pPicText->setRotation(0);
			pPicText->setTextString(insText);
			pPicText->setHeight(textSize);
			pPicText->setLayer(selectCurLayerName);
			pPicText->setTextStyle(CurTextStyleId);
			pPicText->setColorIndex(m_curColorIndex);

			// 지시선 표기
			if (IsDlgButtonChecked(IDC_CHKDRAWARROW))
			{
				AcGePoint3d left_point3d, right_point3d, Max_TextPoint, Min_TextPoint, new_textpoint;
				AcDbExtents textbox;
				if (NumMidValue == 0.0)
				{
					pPicText->getGeomExtents(textbox);
					Max_TextPoint = textbox.maxPoint();
					Min_TextPoint = textbox.minPoint();
					double pDist = sqrt(2 * pow(textSize * 2, 2)); // 지시선 왼쪽 point
					double width = Max_TextPoint.x - Min_TextPoint.x; // 지시선 오른쪽 point					
					ads_polar(asDblArray(textStartPosition), DTR(45.0), pDist, asDblArray(left_point3d));
					ads_polar(asDblArray(left_point3d), DTR(0), width * 0.25 + width, asDblArray(right_point3d));
					new_textpoint.x = left_point3d.x;
					new_textpoint.y = left_point3d.y + textSize / 2;
					new_textpoint.z = textStartPosition.z;

					AcGePoint2d base_point2d, left_point2d, right_point2d;
					base_point2d.x = textStartPosition.x;
					base_point2d.y = textStartPosition.y;
					left_point2d.x = left_point3d.x;
					left_point2d.y = left_point3d.y;
					right_point2d.x = right_point3d.x;
					right_point2d.y = right_point3d.y;

					AcDbPolyline* ArrPline = new AcDbPolyline;
					ArrPline->addVertexAt(0, base_point2d);
					ArrPline->addVertexAt(1, left_point2d);
					ArrPline->addVertexAt(2, right_point2d);
					ArrPline->setLayer(selectCurLayerName);
					ArrPline->setDatabaseDefaults();
					blkRec->appendAcDbEntity(ArrPline);
					ArrPline->close();

					pPicText->setPosition(new_textpoint);
				}
				else
				{
					//pPicText->setPosition(MidPoint);
					pPicText->getGeomExtents(textbox);
					Max_TextPoint = textbox.maxPoint();
					Min_TextPoint = textbox.minPoint();
					double pDist = sqrt(2 * pow(textSize * 2, 2)); // 지시선 왼쪽 point
					double width = Max_TextPoint.x - Min_TextPoint.x; // 지시선 오른쪽 point					
					ads_polar(asDblArray(MidPoint), DTR(45.0), pDist, asDblArray(left_point3d));
					ads_polar(asDblArray(left_point3d), DTR(0), width * 0.25 + width, asDblArray(right_point3d));
					new_textpoint.x = left_point3d.x;
					new_textpoint.y = left_point3d.y + textSize / 2;
					new_textpoint.z = MidPoint.z;

					AcGePoint2d base_point2d, left_point2d, right_point2d;
					base_point2d.x = MidPoint.x;
					base_point2d.y = MidPoint.y;
					left_point2d.x = left_point3d.x;
					left_point2d.y = left_point3d.y;
					right_point2d.x = right_point3d.x;
					right_point2d.y = right_point3d.y;

					AcDbPolyline* ArrPline = new AcDbPolyline;
					ArrPline->addVertexAt(0, base_point2d);
					ArrPline->addVertexAt(1, left_point2d);
					ArrPline->addVertexAt(2, right_point2d);
					ArrPline->setLayer(selectCurLayerName);
					ArrPline->setDatabaseDefaults();
					blkRec->appendAcDbEntity(ArrPline);
					ArrPline->close();

					pPicText->setPosition(new_textpoint);
				}
			}
			else
			{
				if (!bNumMidValue)
				{
					pPicText->setPosition(textStartPosition);
				}
				else
				{
					pPicText->setPosition(MidPoint);
				}
			}

			pPicText->setDatabaseDefaults();
			blkRec->appendAcDbEntity(pPicText);
			pPicText->close();


			//연장표기 방법 선택
			if (IsDlgButtonChecked(IDC_CHKEXTEND))
			{
				AcDbText* pAddDist = new AcDbText;
				if (IsDlgButtonChecked(IDC_RADIO1)) // 3d 거리
				{
					double temp3DP = tempStartP.distanceTo(tmp3Point);
					temp3DP = sqrt(pow(temp3DP * pow((double)scale / 1000, 2), 2) + pow(_ttof(insText) - _ttof(spValue), 2));
					insText = LReturnNumber(temp3DP);
				}
				else // 2d 거리
				{
					pickDist = pickDist * (double)scale / 1000;
					insText = LReturnNumber(pickDist); // 김성곤 대리 수정
				}
				//insText.Format(_T("%g"), pickDist); // 김성곤 대리 수정
				pAddDist->setTextString(insText);
				pAddDist->setDatabaseDefaults();
				pAddDist->setHeight(textSize);
				pAddDist->setLayer(selectCurLayerName);
				pAddDist->setTextStyle(CurTextStyleId);
				pAddDist->setVerticalMode(AcDb::kTextTop);
				pAddDist->setHorizontalMode(AcDb::kTextLeft);
				pAddDist->setColorIndex(m_curColorIndex);
				if (NumMidValue == 0.0)
				{
					AcGePoint3d textint;
					ads_polar(asDblArray(textStartPosition), DTR(-90.0), (double)textSize, asDblArray(textint));
					pAddDist->setAlignmentPoint(textint);
				}
				else
				{
					AcGePoint3d textint;
					ads_polar(asDblArray(MidPoint), DTR(-90.0), (double)textSize, asDblArray(textint));
					pAddDist->setAlignmentPoint(MidPoint);
				}
				blkRec->appendAcDbEntity(pAddDist);
				pAddDist->close();
			}
		}
		else
		{
			pTransientManager->eraseTransient(pVirtualPLine, (AcDbIntArray)res.resval.rint);
			((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			loopJob = false;
		}
		if (!IsDlgButtonChecked(IDC_CHK_EL_CONTINUE)) {
			pTransientManager->eraseTransient(pVirtualPLine, (AcDbIntArray)res.resval.rint);
			((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(ID_GEL_HELP))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			loopJob = false;
		}
		if (!loopJob)
			break;
	}
	pPline->close();
	blkRec->close();
	acDocManagerPtr()->unlockDocument(curDoc());
	if (!IsDlgButtonChecked(IDC_CHK_EL_CONTINUE)) {
		acutPrintf(L"\n");
		
		acedGetAcadDwgView()->SetFocus();
		acTransactionManagerPtr()->flushGraphics();
		acedUpdateDisplay();
		this->SetFocus();
		((CButton*)GetDlgItem(IDC_BUTTON1))->SetFocus();
		GotoDlgCtrl(GetDlgItem(IDC_BUTTON1));
	}
	//CompleteEditorCommand();
}

//종료
void CGEL::OnBnClickedCancel() {
	DestroyWindow();
	//CAcUiDialog::OnCancel();
}

BOOL CGEL::CheckInsertValue(void) {
	GetDlgItemText(IDC_EDTSTARTVALUE, spValue);
	GetDlgItemText(IDC_EDTENDVALUE, epValue);

	if (spValue.IsEmpty() || epValue.IsEmpty())
		return false;
	else if (spValue.CompareNoCase(epValue) == 0)
		return false;
	else
		return true;
}

void CGEL::OnBnClickedChkdrawarrow() {
	chkAddArrow = true;
}

void CGEL::OnBnClickedChkaddpoint() {
	if (((CButton*)GetDlgItem(IDC_CHKADDPOINT))->GetCheck()) {
		chkAddPoint = true;
		((CButton*)GetDlgItem(IDC_CHKPTZVALUE))->EnableWindow(true);
	} else {
		chkAddPoint = false;
		((CButton*)GetDlgItem(IDC_CHKPTZVALUE))->EnableWindow(false);
	}
	
}

void CGEL::OnBnClickedChkptzvalue() {
	if (((CButton*)GetDlgItem(IDC_CHKPTZVALUE))->GetCheck()) {
		chkAddPtZvalue = true;
	} else {
		chkAddPtZvalue = false;
	}
}

void CGEL::OnDeltaposSpindot(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0) {
		if (dotCnt >= 6)
			return;
		dotCnt++;
	}
	else {
		if (dotCnt <= 0)
			return;
		dotCnt--;
	}

	SetDlgItemInt(IDC_EDTDOTNUM, dotCnt);
	*pResult = 0;
}

void CGEL::OnBnClickedChkextend() {
	if (IsDlgButtonChecked(IDC_CHKEXTEND)) {
		GetDlgItem(IDC_EDTEXTENDSTR)->EnableWindow(true);
		GetDlgItem(IDC_RADIO1)->EnableWindow(true);
		GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	} else {
		GetDlgItem(IDC_EDTEXTENDSTR)->EnableWindow(false);
		GetDlgItem(IDC_RADIO1)->EnableWindow(false);
		GetDlgItem(IDC_RADIO2)->EnableWindow(false);
	}
}

void CGEL::OnBnClickedBlayerselect() {
	ads_name en;
	ads_point pt;

	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strMsg;
	strMsg.LoadStringW(IDS_GET_LAYER_ENTITY); // "\n도면층을 가져올 객체 선택: "
	if (acedEntSel(strMsg, en, pt) == RTNORM) {
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
		for (pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()) {
			pLayerTblIter->getRecordId(LayerId);
			AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
			TCHAR* name;
			pLayerTblRcd->getName(name);
			if (0 == _tcscmp(selectLayerName, name)) {
				m_CbxLayer.SetCurSel(cbxIndex);
				LayerChangeGetColor(selectLayerName);
				break;
			}
			cbxIndex++;
		}
		delete pLayerTblIter;
		delete pLayerTbl;
		OnCbnSelchangeCbxlayer();
	} else { 
		CancelEditorCommand();
		this->ShowWindow(SW_SHOW);
		this->EnableWindow(TRUE);
	}
}

void CGEL::OnBnClickedBcolor() {
	AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();
	AcDbLayerTableRecordPointer ptLayer(layerId, AcDb::kForRead);
	AcCmColor oldColor = ptLayer->color();

	int nCurColor = oldColor.colorIndex();
	int selectColor = oldColor.colorIndex();

	acedSetColorDialog(selectColor, Adesk::kTrue, nCurColor);
	SelectColorChange(selectColor);
}

//색상 변경
void CGEL::SelectColorChange(int colorIndex) {
	CString colorIndexStr, strMSg;
	colorIndexStr.Format(_T("%d"), colorIndex);
	if (colorIndexStr.CompareNoCase(_T("0")) == 0) {
		strMSg.LoadStringW(IDS_BYBLOCK); // "블록별"
		colorIndexStr.Format(strMSg);
	} else if (colorIndexStr.CompareNoCase(_T("256")) == 0) {
		strMSg.LoadStringW(IDS_BYLAYER); // "도면층별"
		colorIndexStr.Format(strMSg);
	}

	btnAfterClr = acedGetRGB(colorIndex);
	m_curColorIndex = colorIndex;

	Invalidate();
}

void CGEL::OnCbnSelchangeCbxlayer() {
	int curLayer = m_CbxLayer.GetCurSel();
	m_CbxLayer.GetLBText(m_CbxLayer.GetCurSel(), selectCurLayerName);
	LayerChangeGetColor(selectCurLayerName);
}

//도면층별 색상 가져오기
void CGEL::LayerChangeGetColor(CString layerName) {
	AcDbLayerTable* pLayerTbl = NULL;
	AcDbLayerTablePointer pLayerPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbLayerTableIterator* pLayerTblIter;
	pLayerPoint->newIterator(pLayerTblIter);

	AcDbObjectId LayerId;
	for (pLayerTblIter->start(); !pLayerTblIter->done(); pLayerTblIter->step()) {
		pLayerTblIter->getRecordId(LayerId);
		AcDbLayerTableRecordPointer pLayerTblRcd(LayerId, AcDb::kForRead);
		TCHAR* name;
		AcCmColor color;
		int colorIndex;
		pLayerTblRcd->getName(name);
		color = pLayerTblRcd->color();
		colorIndex = color.colorIndex();
		if (layerName.CompareNoCase(name) == 0) {
			SelectColorChange(colorIndex);
		}
	}
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CGEL::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
	if ((nIDCtl == IDC_BCOLOR)) {
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);					//버튼의 dc구하기
		rect = lpDrawItemStruct->rcItem;					//버튼영역 구하기
		dc.Draw3dRect(&rect, RGB(255, 255, 255), RGB(0, 0, 0));	//버튼의 외각선 그리기
		dc.FillSolidRect(&rect, btnAfterClr);				//버튼색상
		UINT state = lpDrawItemStruct->itemState;			//버튼상태구하기
		if ((state & ODS_SELECTED))	dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else						dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

		TCHAR buffer[MAX_PATH];								//버튼의 text를 얻기위한 임시버퍼
		ZeroMemory(buffer, MAX_PATH);						//버퍼초기화
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH); //버튼의 text얻기
		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //버튼의 text넣기
		dc.Detach();                                                  //버튼의 dc 풀어주기
	}
	CAcUiDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
// 문자높이
void CGEL::OnEnUpdateEdttextheight() {
	UpdateData(TRUE);
	m_TextHeight_Str = CheckInputNumber(m_TextHeight_Str);
	UpdateData(FALSE);

	m_TextHeight_Ctrl.SetSel(0, -1);
	m_TextHeight_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

// 스케일
void CGEL::OnEnUpdateEdtscale()
{
	UpdateData(TRUE);
	m_Scale_Str;
	UpdateData(FALSE);
}
//소수점 계산 EL=
CString CGEL::ReturnNumber(double value) {
	CString result = _T("");
	GetDlgItemText(IDC_EDTELSTR, markText);
	switch (dotCnt) {
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
// 연장표시 L=
CString CGEL::LReturnNumber(double value) {
	CString result = _T("");
	GetDlgItemText(IDC_EDTEXTENDSTR, markExtText);
	switch (dotCnt) {
	case 0:
		result.Format(_T("%s%.0lf"), markExtText, value);
		break;
	case 1:
		result.Format(_T("%s%.1lf"), markExtText, value);
		break;
	case 2:
		result.Format(_T("%s%.2lf"), markExtText, value);
		break;
	case 3:
		result.Format(_T("%s%.3lf"), markExtText, value);
		break;
	case 4:
		result.Format(_T("%s%.4lf"), markExtText, value);
		break;
	case 5:
		result.Format(_T("%s%.5lf"), markExtText, value);
		break;
	case 6:
		result.Format(_T("%s%.6lf"), markExtText, value);
		break;
	default:
		return result;
		break;
	}
	return result;
}

//숫자 및 점(.) 입력 체크
CString CGEL::CheckInputNumber(CString str) {
	TCHAR* getAtChar;
	CString new_str;
	int isNum = -1;
	for (int i = 0; i < str.GetLength(); i++) {
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i, 1);
		int ascii = __toascii(*getAtChar);
		if ((ascii >= 48 && ascii <= 57) || ascii == 45 || ascii == 46) {
			//str.Remove(*getAtChar);
			if (isNum == -1 || isNum == 1) {
				isNum = 1;
				new_str.AppendChar(*getAtChar);
			} else {
				if (new_str.IsEmpty()) {
					isNum = 1;
					new_str.AppendChar(*getAtChar);
				} else if (isNum == 2) {
					// 뒤에 조건에 없는 문자열이 있다면 new_str 비우기
					new_str.Empty();
					isNum = 1;
					new_str.AppendChar(*getAtChar);
					continue;
				} else break;
			}
		} else {
			isNum = 2;
		}
	}
	return new_str;
}

//숫자 점 체크
bool CGEL::CheckCStringDigit(CString str) {
	char atStr;
	int dotflag = 0;
	for (int i = 0; i < str.GetLength(); i++) {
		if (dotflag == 2) {
			return false;
		}
		atStr = str.GetAt(i);
		if (atStr >= '0' && atStr <= '9') {

		}
		else {
			return false;
		}
		if (atStr == '.')	dotflag++;
	}
	return true;
}

//숫자 체크
CString CGEL::CheckCStringNumber(CString str)
{
	char atStr;
	int dotflag = 0, isNum = -1;
	CString new_text;
	AfxExtractSubString(new_text, str, 1, ':');

	CString new_text1 = "";
	for (int i = 0; i < new_text.GetLength(); i++) {
		atStr = new_text.GetAt(i);
		if ((atStr >= '0' && atStr <= '9') || atStr == '.' || atStr == '-') {
			if (isNum == -1 || isNum == 1) {
				isNum = 1;
				new_text1 += atStr;
			} else {
				if (new_text1.IsEmpty()) {
					isNum = 1;
					new_text1 += atStr;
				} else break; 
			}
		} else {
			isNum = 2;
		}
	}

	return new_text1;
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CGEL::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}



void CGEL::OnBnClickedHelp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	long lResult;
	HKEY hKey;
	DWORD dwType;
	DWORD dwBytes = 200;
	TCHAR buffer[100];
	//CString HelpPath;
	CString Real_Path;
	CString Help_Path;
	HMODULE hinst;

	// open Regstry Key
	// _2021 AND _2021M IS SAME
	/*
#ifdef _ZRX_KR
	#ifdef _2022
		lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\ZDREAM\\2022"), 0, KEY_ALL_ACCESS, &hKey);
	#elif _2021
		lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\ZDREAM\\2021"), 0, KEY_ALL_ACCESS, &hKey);
	#elif _2021M
		lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\ZDREAM\\2021"), 0, KEY_ALL_ACCESS, &hKey);
	#elif _2020
		lResult = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\ZDREAM2020"), 0, KEY_ALL_ACCESS, &hKey);
	#endif
#endif

	lResult = RegQueryValueEx(hKey, _T("Path"), 0, NULL, (LPBYTE)buffer, &dwBytes);
	HelpPath = CString(buffer);
	*/
	Help_Path = g_modulePath;
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

	Help_Path.Format(_T("%s\\%s::/1. html/GEL.html"), Help_Path, helpFile);

	//Help_Path.Format(_T("%s\\ZDREAM.chm::/GEL.htm#GEL"), Help_Path);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}

BOOL CGEL::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	pGEL = NULL;

	return CAcUiDialog::DestroyWindow();
}

void CGEL::PostNcDestroy()
{
	delete this;
	pGEL = NULL;
	//CAcUiDialog::PostNcDestroy();
}


void CGEL::OnBnClickedChkElContinue()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_selected) { // 폴리선 선택일 시
		//this->SetFocus();
		//((CButton*)GetDlgItem(IDC_BUTTON1))->SetFocus();
		GotoDlgCtrl(GetDlgItem(IDC_BUTTON1));
		//this->Invalidate();
	} 
}
