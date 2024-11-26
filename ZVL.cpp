#include "stdafx.h"
#include "ZVL.h"
#include "afxdialogex.h"

// CZVL 대화 상자입니다.
IMPLEMENT_DYNAMIC(CZVL, CAcUiDialog)
	CZVL::CZVL(CWnd* pParent)
	: CAcUiDialog(CZVL::IDD, pParent){
}

CZVL::~CZVL(){
}

void CZVL::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZVL, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CZVL::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CZVL::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADLINE, &CZVL::OnBnClickedRadline)
	ON_BN_CLICKED(IDC_RADPLINE, &CZVL::OnBnClickedRadpline)
	ON_BN_CLICKED(IDC_RAD3DPLINE, &CZVL::OnBnClickedRad3Dpline)
	ON_BN_CLICKED(ID_ZVL_HELP, &CZVL::OnBnClickedZvlHelp)
END_MESSAGE_MAP()

BOOL CZVL::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	CString jobStr = iniFile.readINI(_T("config.ini"), _T("ZVL"), _T("LINETYPE"));
	if(jobStr.CompareNoCase(_T("")) == 0 || jobStr.CompareNoCase(_T("LINE")) == 0){
		CheckDlgButton(IDC_RADLINE, TRUE);		
		m_Mode = _T("LINE");
	}
	else if(jobStr.CompareNoCase(_T("POLYLINE")) == 0){
		CheckDlgButton(IDC_RADPLINE, TRUE);		
		m_Mode = _T("POLYLINE");
	}
	else if (jobStr.CompareNoCase(_T("3DPOLYLINE")) == 0) {
		CheckDlgButton(IDC_RAD3DPLINE, TRUE);
		m_Mode = _T("3DPOLYLINE");
	}
	else
		CheckDlgButton(IDC_RADLINE, TRUE);

	return TRUE;
}

void CZVL::OnBnClickedOk(){
	CAcUiDialog::OnOK();
	AcGePoint3d drawSp, drawEp;
	AcDbObjectIdArray objIds, obj3DId;

	double zValue;
	CString zValueStr = iniFile.readINI(_T("config.ini"), _T("ZVL"), _T("ZVALUE"));
	if(zValueStr.CompareNoCase(_T("")) == 0)
		zValueStr = _T("0.0000");

	zValue = _ttof(zValueStr);
	ads_point pt1, pt2, pt3;
	ads_name en;
	ads_real inputValue;
	int result, jobResult;
	bool secondary = false;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	CString strMsg, strMsg1, qtsTxt;
	strMsg.LoadStringW(IDS_ZVL_START_SET); // "\n시작점 지정: "
	result = acedGetPoint(NULL, strMsg, pt1);
	drawSp = asPnt3d(pt1);
	qtsTxt.LoadStringW(IDS_ZVL_SEL_Z_INPUT); // "기존 문자 선택 또는 Z값 입력"
	//qtsTxt = _T("기존 문자 선택 또는 Z값 입력");

	//AcDbLine* pLines = new AcDbLine();
	AcDbPolyline* pPline = new AcDbPolyline();
	AcDb3dPolyline* p3DPline = new AcDb3dPolyline();
	AcDb3dPolylineVertex* vp;
	AcGePoint3dArray pt3dArr;
	int i = 0;
	while(true){
		if(secondary){
			strMsg.LoadStringW(IDS_NEXT_PT_SET); // "\n다음점 지정: "
			result = acedGetPoint(pt1, strMsg, pt2);
			drawEp = asPnt3d(pt2);
			pt1[0] = pt2[0];
			pt1[1] = pt2[1];
			pt1[2] = pt2[2];
		}
		CString insertValue;
		if(result == RTNORM){
			CString putTxt;
			putTxt.Format(_T("%s(%s): "), qtsTxt, zValueStr);
			jobResult = acedEntSel(L"\n" + putTxt, en, pt3);
			if(jobResult == RTNORM){
				AcDbObjectId objId;
				if(acdbGetObjectId(objId, en) == Acad::eOk){
					AcDbEntityPointer pEnt(objId, AcDb::kForRead);
					if(pEnt.openStatus() == Acad::eOk){
						if(pEnt->isKindOf(AcDbText::desc())){
							AcDbText* pText = AcDbText::cast(pEnt);
							insertValue = pText->textString();
							pText->close();
						}
						else if (pEnt->isKindOf(AcDbMText::desc())) {
							AcDbMText* pMText = AcDbMText::cast(pEnt);
							insertValue = pMText->contents();
							pMText->close();
						}
					}
					pEnt->close();
				}
				zValue = _ttof(insertValue);
				zValueStr = insertValue;
			}
			else if(jobResult == RTNONE){
				zValue = _ttof(insertValue);
				zValueStr = insertValue;
			}
			else if(jobResult == RTERROR){
				resbuf inputZValueStr;
				ads_getvar(_T("LASTPROMPT"), &inputZValueStr);
				insertValue.Format(_T("%s"), inputZValueStr.resval.rstring);
				insertValue.Replace(putTxt, _T(""));

				strMsg.LoadStringW(IDS_ZVL_START_SET);	// "시작점 지정: "
				strMsg1.LoadStringW(IDS_NEXT_PT_SET);	// "다음점 지정: "

				if(insertValue.CompareNoCase(_T("")) == 0){
					zValue = _ttof(zValueStr);
				}
				else if(insertValue.CompareNoCase(strMsg) == 0)
					continue;
				else if(insertValue.CompareNoCase(strMsg1) == 0)
					continue;
				else{
					zValue = _ttof(insertValue);
					zValueStr = insertValue;
				}
			}
		}
		if(result == RTCAN || result == RTNONE)
			break;

		if(!secondary){
			if (m_Mode.CompareNoCase(_T("LINE")) == 0) {
				drawSp.z = zValue;
			}
			else if (m_Mode.CompareNoCase(_T("POLYLINE")) == 0) {
				drawSp.z = zValue;
				pPline->addVertexAt(i, AcGePoint2d(drawSp.x, drawSp.y));
				pPline->setElevation(zValue);
			}
			else if (m_Mode.CompareNoCase(_T("3DPOLYLINE")) == 0) {
				drawSp.z = zValue;
				pt3dArr.append(AcGePoint3d(drawSp.x, drawSp.y, drawSp.z));
				p3DPline = new AcDb3dPolyline(AcDb::k3dSimplePoly, pt3dArr);
			}
		}

		drawEp.z = zValue;
		if(secondary){
			if (m_Mode.CompareNoCase(_T("LINE")) == 0) {
				/*pLines->setEndPoint(AcGePoint3d(drawEp.x, drawEp.y, drawEp.z));
				AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
				acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
				pLines->setDatabaseDefaults();
				blkRec->appendAcDbEntity(pLines);
				blkRec->close();*/
			}
			else if(m_Mode.CompareNoCase(_T("POLYLINE")) == 0){
				pPline->addVertexAt(i, AcGePoint2d(drawEp.x, drawEp.y));
				pPline->setElevation(zValue);
				AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
				acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
				pPline->setDatabaseDefaults();
				blkRec->appendAcDbEntity(pPline);
				blkRec->close();
			}
			else if (m_Mode.CompareNoCase(_T("3DPOLYLINE")) == 0) {
				pt3dArr.append(AcGePoint3d(drawEp.x, drawEp.y, drawEp.z));
				p3DPline = new AcDb3dPolyline(AcDb::k3dSimplePoly, pt3dArr);
				AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
				acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
				p3DPline->setDatabaseDefaults();
				AcDbObjectId plineObjId;
				blkRec->appendAcDbEntity(plineObjId, p3DPline);
				obj3DId.append(p3DPline->objectId());
				p3DPline->close();
				blkRec->close();
			}
			AcDbLine* pLine = new AcDbLine();
			pLine->setStartPoint(drawSp);
			pLine->setEndPoint(drawEp);

			AcDbBlockTableRecord* blkRec = new AcDbBlockTableRecord;
			acdbOpenObject(blkRec, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite);
			pLine->setDatabaseDefaults();
			blkRec->appendAcDbEntity(pLine);
			objIds.append(pLine->objectId());
			pLine->close();
			blkRec->close();
			drawSp = asPnt3d(pt2);
			drawSp.z = zValue;
		}
		i++;
		secondary = true;
	}
	//pLines->close();
	p3DPline->close();
	pPline->close();
	CompleteEditorCommand();
	if(m_Mode.CompareNoCase(_T("POLYLINE")) == 0){
		for(int i = 0; i < objIds.length(); i++){
			AcDbEntityPointer pEnt(objIds.at(i), AcDb::kForRead);
			pEnt->upgradeOpen();
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					pLine->erase();
					pLine->close();
				}
			}
			pEnt->close();
		}
	}
	else if (m_Mode.CompareNoCase(_T("3DPOLYLINE")) == 0) {
		for (int i = 0; i < objIds.length(); i++) {
			AcDbEntityPointer pEnt(objIds.at(i), AcDb::kForRead);
			pEnt->upgradeOpen();
			if (pEnt.openStatus() == Acad::eOk) {
				if (pEnt->isKindOf(AcDbLine::desc())) {
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					pLine->erase();
					pLine->close();
				}
			}
			pEnt->close();
		}

		for (int i = 0; i < obj3DId.length() - 1; i++) {
			AcDbEntityPointer pEnt1(obj3DId.at(i), AcDb::kForRead);
			pEnt1->upgradeOpen();
			if (pEnt1.openStatus() == Acad::eOk) {
				if (pEnt1->isKindOf(AcDb3dPolyline::desc())) {
					AcDb3dPolyline* p3Dline = AcDb3dPolyline::cast(pEnt1);
					p3Dline->erase();
					p3Dline->close();
				}
			}
			pEnt1->close();
		}
		obj3DId.removeAll();
	}
	objIds.removeAll();

	if(IsDlgButtonChecked(IDC_RADLINE))
		iniFile.writeINI(_T("config.ini"), _T("ZVL"), _T("LINETYPE"), _T("LINE"));
	else if(IsDlgButtonChecked(IDC_RADPLINE))
		iniFile.writeINI(_T("config.ini"), _T("ZVL"), _T("LINETYPE"), _T("POLYLINE"));
	else if (IsDlgButtonChecked(IDC_RAD3DPLINE))
		iniFile.writeINI(_T("config.ini"), _T("ZVL"), _T("LINETYPE"), _T("3DPOLYLINE"));

	iniFile.writeINI(_T("config.ini"), _T("ZVL"), _T("ZVALUE"), zValueStr);	
}

void CZVL::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

void CZVL::OnBnClickedRadline(){
	m_Mode = _T("LINE");
}

void CZVL::OnBnClickedRadpline(){
	m_Mode = _T("POLYLINE");
}

void CZVL::OnBnClickedRad3Dpline() {
	m_Mode = _T("3DPOLYLINE");
}

void CZVL::OnBnClickedZvlHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/ZVL.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
