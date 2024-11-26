#include "stdafx.h"
#include "FEL.h"
#include "afxdialogex.h"

// CFEL 대화 상자입니다.
IMPLEMENT_DYNAMIC(CFEL, CAcUiDialog)
	CFEL::CFEL(CWnd* pParent)
	: CAcUiDialog(CFEL::IDD, pParent), m_TextHeight_Str(_T(""))
	, m_DotCntStr(_T("")), m_PreFaceStr(_T("")),  m_ScaleStr(_T("")), m_LengthStr(_T("")){
}

CFEL::~CFEL(){
}

void CFEL::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADLINE, m_btn1);
	DDX_Control(pDX, IDC_RADARROW, m_btn2);
	DDX_Text(pDX, IDC_EDTSCALE, m_ScaleStr);
	DDX_Text(pDX, IDC_EDTPOINT, m_DotCntStr);
	DDX_Text(pDX, IDC_EDTLENGTH, m_LengthStr);
	DDX_Control(pDX, IDC_CBXLAYER, m_CbxLayer);
	DDX_Text(pDX, IDC_EDTHEADER, m_PreFaceStr);
	DDX_Text(pDX, IDC_EDTHEIGHT, m_TextHeight_Str);
	DDX_Control(pDX, IDC_EDTHEIGHT, m_TextHeight_Ctrl);
}

BEGIN_MESSAGE_MAP(CFEL, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CFEL::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFEL::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_EDTSCALE, &CFEL::OnEnUpdateEdtscale)
	ON_EN_UPDATE(IDC_EDTPOINT, &CFEL::OnEnUpdateEdtpoint)
	ON_BN_CLICKED(IDC_RADLINE, &CFEL::OnBnClickedRadline)
	ON_BN_CLICKED(IDC_RADARROW, &CFEL::OnBnClickedRadarrow)
	ON_EN_UPDATE(IDC_EDTLENGTH, &CFEL::OnEnUpdateEdtlength)
	ON_EN_UPDATE(IDC_EDTHEIGHT, &CFEL::OnEnUpdateEdtheight)
	ON_EN_UPDATE(IDC_EDTHEADER, &CFEL::OnEnUpdateEdtpreface)
	ON_BN_CLICKED(ID_FEL_HELP, &CFEL::OnBnClickedFelHelp)
END_MESSAGE_MAP()

// CFEL 메시지 처리기입니다.
BOOL CFEL::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_bSelect.AutoLoad();

	//모드 1.가로선  2.지시선	
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0 )
		OnBnClickedRadline();
	else if(tempStr.CompareNoCase(_T("2")) == 0)
		OnBnClickedRadarrow();
	
	//가로선길이
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("Length"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTLENGTH, _T("100"));
	else
		SetDlgItemText(IDC_EDTLENGTH, tempStr);

	//축척
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("Scale"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTSCALE, _T("1000"));
	else
		SetDlgItemText(IDC_EDTSCALE, tempStr);

	//소수점
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("DotCnt"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTPOINT, _T("3"));
	else
		SetDlgItemText(IDC_EDTPOINT, tempStr);

	//머리말
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("PreFace"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTHEADER, _T("EL="));
	else
		SetDlgItemText(IDC_EDTHEADER, tempStr);

	//글자높이
	tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("TextHeight"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_EDTHEIGHT, _T("0.300"));
	else
		SetDlgItemText(IDC_EDTHEIGHT, tempStr);

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
		int len = wcslen(name);
		if (len > 0)
			m_CbxLayer.AddString(name);
	}
	delete pLayerTblIter;
	delete pLayerTbl;

	//변수 및 리소스 초기화
	m_CbxLayer.SetCurSel(0);
	return TRUE;
}

void CFEL::OnBnClickedOk(){
	UpdateData();
	CAcUiDialog::OnOK();
#ifdef _DONOT
	CAcUiDialog::OnOK();
	CString modeStr;
	if(!(lineStat == 1 || lineStat == 2))
		lineStat = 1;

	modeStr.Format(_T("%d"), lineStat);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("JOBMODE"),modeStr);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("Scale"),m_ScaleStr);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("Length"),m_LengthStr);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("DotCnt"),m_DotCntStr);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("PreFace"),m_PreFaceStr);
	iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("TextHeight"),m_TextHeight_Str);

	CString tempStr = iniFile.readINI(_T("config.ini"), _T("FEL"), _T("ELValue"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		m_ELValue_str = _T("0.000");
	else
		m_ELValue_str = tempStr;

	double m_ELValue = _tstof(tempStr);
	AcDbLayerTable* pLayerTable;
	AcCmColor color;
	color.setColorIndex(2);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return;
	pDb->getLayerTable(pLayerTable, AcDb::kForWrite);
	if(pLayerTable->has(m_LayerStr) != Adesk::kTrue){
		AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(m_LayerStr);
		pLayerTableRecord->setColor(color);

		AcDbLinetypeTable* pLinetypeTable;
		pDb->getLinetypeTable(pLinetypeTable,AcDb::kForRead);

		AcDbObjectId linetypeId;
		pLinetypeTable->getAt(_T("BYLAYER"),linetypeId);
		pLayerTableRecord->setLinetypeObjectId(linetypeId);
		pLayerTable->add(pLayerTableRecord);
		pLayerTable->close();
		pLayerTableRecord->close();
	}
	else
		pLayerTable->close();


	int stat;
	TCHAR kWord [133];
	ads_name ens;
	AcGePoint3d ELpt;
	CString inString, EntStr, Scale_Print, Scale_Print2, RTERROR_str, S_ScaleStr, m_EL, strMsg;
	double m_Scale = _tstof(m_ScaleStr);
	bool b_TF = true;
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	while(b_TF){
		acedInitGet(0, _T("c C 0 1 2 3 4 5 6 7 8 9 10"));
		strMsg.LoadStringW(IDS_FEL_BASEPT_SEL); // "기준점 EL 선택 또는 값 입력(%.3f)<축척 변경(C)>: "
		EntStr.Format(strMsg, m_ELValue);
		strMsg.LoadStringW(IDS_SET_CMD_SCALE); // "\n< 현재설정 : 축척=%0.f >"
		Scale_Print.Format(strMsg, m_Scale);
		strMsg.LoadStringW(IDS_SET_SCALE); // "< 현재설정 : 축척=%0.f >"
		Scale_Print2.Format(strMsg, m_Scale);
		acutPrintf(Scale_Print);
		stat = acedEntSel(EntStr, ens, asDblArray(ELpt));
		if(stat == RTCAN || stat == RTNONE)
			b_TF = false;
		else if(stat == RTKWORD){
			resbuf zValueStr;
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			if(RTERROR_str.CompareNoCase(_T("")) == 0)
				break;
			else if(RTERROR_str.CompareNoCase(_T("C")) == 0 || RTERROR_str.CompareNoCase(_T("c")) == 0){
				strMsg.LoadStringW(IDS_FEL_INPUT_SCALE); // "\n축척 입력<%.0f> : "
				S_ScaleStr.Format(strMsg, m_Scale);
				if(acedGetReal(S_ScaleStr, &m_Scale) == RTNORM){
					m_ScaleStr.Format(_T("%0.f"), m_Scale);
					iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("Scale"), m_ScaleStr);
				}	
			}
			else{
				if(RTERROR_str.GetLength() > 0){
					m_ELValue_str.Format(_T("%s"), RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
					break;
				}
				else{
					m_ELValue_str.Format(_T("%f"), m_ELValue);
					break;
				}
			}
		}
		else if(stat == RTERROR){
			resbuf zValueStr, zValueStr2;
			strMsg.LoadStringW(IDS_CHECK_STRING); //"**선택한 객체는 문자가 아닙니다.**"
			ads_getvar(_T("LASTPROMPT"), &zValueStr);
			RTERROR_str.Format(_T("%s"), zValueStr.resval.rstring);
			RTERROR_str.Replace(EntStr, _T(""));
			ads_getvar(_T("ERRNO"), &zValueStr2);
			if(zValueStr2.resval.rint == 52)
				break;
			if(RTERROR_str.CompareNoCase(Scale_Print2) == 0)
				continue;
			else if(RTERROR_str.CompareNoCase(strMsg) == 0)
				continue;
			else{
				if(RTERROR_str.GetLength() > 0){
					m_ELValue_str.Format(_T("%s"), RTERROR_str);
					iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
					break;
				}
				else{
					m_ELValue_str.Format(_T("%f"), m_ELValue);
					break;
				}
			}
		}
		else if(stat == RTNORM){
			AcGePoint3d insertPoint;
			AcDbObjectId objId;
			acdbGetObjectId(objId, ens);
			AcDbEntityPointer pEnt(objId, AcDb::kForRead);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText* pText = AcDbText::cast(pEnt);
					inString.Format(_T("%s"), pText->textString());
					inString = CheckInputNumber(inString);
					pText->close();
				}
				else{
					strMsg.LoadStringW(IDS_CHECK_STRING_ENTER); // "\n**선택한 객체는 문자가 아닙니다.**\n"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			acedSSFree(ens);
			m_ELValue_str = inString;
			iniFile.writeINI(_T("config.ini"), _T("FEL"), _T("ELValue"), m_ELValue_str);
			b_TF = false;
		}
	}
	AcGePoint3d STpt, spt, ept;
	double StrNum = _tstof(m_ELValue_str);
	CString getptStr;
	strMsg.LoadStringW(IDS_FEL_BASEPT_SEL); // "\n기준점(%.3f)선택 : "
	getptStr.Format(strMsg, StrNum);
	if(acedGetPoint(NULL, getptStr, asDblArray(STpt)) != RTNORM)
		return;

	bool b_EL = true;
	bool H_Lead = true;
	double EL_Value, H_Scale, H_Height, H_Length, H_DotNum;
	H_Scale = _tstof(m_ScaleStr);			//축척
	H_Length = _tstof(m_LengthStr);			//길이
	H_DotNum = _tstof(m_DotCntStr);			//소수점
	H_Height = _tstof(m_TextHeight_Str);	//문자높이

	while(b_EL){
		strMsg.LoadStringW(IDS_FEL_EL_INPUT); // "\n표시할 EL 입력 : "
		if(acedGetReal(strMsg, &EL_Value) != RTNORM)
			b_EL = false;
		else{
			if(lineStat == 1){
				AcGePoint3d Npt, pPt;
				AcGePoint3dArray ptArray;
				pPt.y = STpt.y + (EL_Value - StrNum) * 1000 / H_Scale;
				pPt.x = STpt.x;
				ads_polar(asDblArray(pPt), 0.0, 1000 / H_Scale * (H_Length / 2), asDblArray(spt));
				ads_polar(asDblArray(pPt), ads_angle(asDblArray(spt), asDblArray(pPt)), 1000 / H_Scale * (H_Length / 2), asDblArray(ept));
				ptArray.append(spt); ptArray.append(ept);
				bool m_pLine = CreatePLine(ptArray, 0, _T("BYBLOCK"), 1, m_LayerStr);
				ptArray.removeAll();
				if(m_pLine == false)
					b_EL = false;

				CString HTxtStr,last;
				int H_DecimalNum = H_DotNum;
				last.Format(_T("%s%%.%df"), m_PreFaceStr, H_DecimalNum);
				HTxtStr.Format(last,EL_Value/* * m_Scale / 1000*/);
				Npt.y = STpt.y + 1000 / H_Scale * (EL_Value - StrNum + 0.1);
				Npt.x = STpt.x;

				bool m_Text = CreateText(Npt, 1000 / H_Scale*H_Height, 0.0, HTxtStr, m_LayerStr, 1);
				if(m_Text == false)
					b_EL = false;
			}
			else if(lineStat == 2){
				AcGePoint3d Lpt, ArrowPt1, ArrowPt2, LinePt1, LinePt2;
				AcGePoint3dArray LptArray;
				double H_DotLength = 0;

				CString LTxtstr,Ltxt;
				int H_DecimalNum = H_DotNum;
				Ltxt.Format(_T("%s%%.%df"), m_PreFaceStr, H_DecimalNum);
				LTxtstr.Format(Ltxt,EL_Value);
				Lpt.y = STpt.y + 1000 / H_Scale * (EL_Value - StrNum + 1.1) + 1000 / H_Scale * (H_Length - 1);
				Lpt.x = STpt.x;

				bool m_LText = CreateText(Lpt, 1000 / H_Scale*H_Height, 0.0, LTxtstr, m_LayerStr, 2);
				if(m_LText == false)
					b_EL = false;

				Lpt.y = STpt.y + (EL_Value - StrNum) * 1000 / H_Scale;
				Lpt.x = STpt.x;
				acutPolar(asDblArray(Lpt), DTR(90.0), 1000 / H_Scale * 0.3, asDblArray(ArrowPt1));
				acutPolar(asDblArray(ArrowPt1), DTR(180.0), 1000 / H_Scale * 0.1, asDblArray(ArrowPt2));
				bool m_Solid = CreateSolid(Lpt, ArrowPt1, ArrowPt2, m_LayerStr);
				if(m_Solid == false)
					b_EL = false;

				Lpt.y = STpt.y + (EL_Value - StrNum) * 1000 / H_Scale;
				Lpt.x = STpt.x;
				if(H_DotNum == 0)
					H_DotLength = (LTxtstr.GetLength() * 0.2331 + 0.2) * 1000 / H_Scale;
				else if(H_DotNum > 0)
					H_DotLength = ((LTxtstr.GetLength() - 1) * 0.2331 + 0.1164 + 0.2) * 1000 / H_Scale;
				
				acutPolar(asDblArray(Lpt), DTR(90.0), 1000 / H_Scale * H_Length, asDblArray(LinePt1));
				acutPolar(asDblArray(LinePt1), 0.0, H_DotLength , asDblArray(LinePt2));
				LptArray.append(Lpt); LptArray.append(LinePt1); LptArray.append(LinePt2);
				bool m_LPline = CreatePLine(LptArray, 0, _T("BYBLOCK"), 1, m_LayerStr);
				LptArray.removeAll();
				if(m_LPline == false)
					b_EL = false;
			}
		}
	}
	CompleteEditorCommand();
#endif
}

void CFEL::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

void CFEL::OnBnClickedRadline(){
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	CString strMsg;
	strMsg.LoadStringW(IDS_LEN_HOR_LINE); // "가로선 길이"
	SetDlgItemText(IDC_STATICRADTEXT, strMsg);
	Length = _ttoi(m_LengthStr);
	SetDlgItemInt(IDC_EDTLENGTH, Length);
	lineStat = 1;
	m_btn1.SetCheck(BST_CHECKED);
	m_btn2.SetCheck(BST_UNCHECKED);
}

void CFEL::OnBnClickedRadarrow(){
	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);
	CString strMsg;
	strMsg.LoadStringW(IDS_LEN_LEAD_LINE); // "지시선 길이"
	SetDlgItemText(IDC_STATICRADTEXT, strMsg);
	Length = _ttoi(m_LengthStr);
	lineStat = 2;
	m_btn1.SetCheck(BST_UNCHECKED);
	m_btn2.SetCheck(BST_CHECKED);
}

//가로선 길이
void CFEL::OnEnUpdateEdtlength(){
	UpdateData(TRUE);
	Length = _ttoi(m_LengthStr);
	UpdateData(FALSE);
}

//레이어
void CFEL::OnEnUpdateEdtlayer(){
	UpdateData(TRUE);
	m_LayerStr;
	UpdateData(FALSE);
}

//축척
void CFEL::OnEnUpdateEdtscale(){
	UpdateData(TRUE);
	Scale = _ttoi(m_ScaleStr);
	UpdateData(FALSE);
}

//소수점
void CFEL::OnEnUpdateEdtpoint(){
	UpdateData(TRUE);
	DotCnt = _ttoi(m_DotCntStr);
	UpdateData(FALSE);
	if(DotCnt < 0 )
		m_DotCntStr = _T("0");
	else if(DotCnt > 6)
		m_DotCntStr = _T("6");

	UpdateData(TRUE);
}

//머리말
void CFEL::OnEnUpdateEdtpreface(){
	UpdateData(TRUE);
	m_PreFaceStr;
	UpdateData(FALSE);
}

//문자높이
void CFEL::OnEnUpdateEdtheight(){
	UpdateData(TRUE);
	m_TextHeight_Str = CheckInputNumber(m_TextHeight_Str);
	UpdateData(FALSE);

	m_TextHeight_Ctrl.SetSel(0, -1);
	m_TextHeight_Ctrl.SetSel(-1, -1);
	UpdateData(TRUE);
}

//숫자 및 점(.) 입력 체크
CString CFEL::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	CString strNum, strRe = _T("");
	int k = 0;
	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i, 1);
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if (((ascii >= 48 && ascii <= 57) || ascii == 46)) {
			strRe.Append(getAtChar);
		}
	}

	if (strRe.GetAt(0) == '.') // 맨앞에 .이면 삭제
		strRe = strRe.Mid(1, strRe.GetLength() - 1);

	return strRe;
} 

//숫자 점 체크
bool CFEL::CheckCStringDigit(CString str){
	char atStr;
	int dotflag = 0;
	for(int i = 0 ; i < str.GetLength(); i++){
		if(dotflag == 2)
			return false;

		atStr = str.GetAt(i);
		if(atStr >= '0' && atStr <= '9')
			continue;
		else
			return false;

		if(atStr == '.')
			dotflag++;
	}
	return true;
}

//폴리선 생성
bool CFEL::CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId plineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)
		return false;

	AcDbPolyline* pPLine = new AcDbPolyline();
	for(int i = 0; i < ptArray.length(); i++){
		pPLine->addVertexAt(i, AcGePoint2d(ptArray[i].x, ptArray[i].y));
	}

	pPLine->setConstantWidth(LineWidth);
	pPLine->setLinetype(LineType);
	pPLine->setLinetypeScale(LineTypeScale);
	pPLine->setLayer(Layer);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		// [kyh] 현재 작업창
		if (acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite) == Acad::eOk) {
		//if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(plineObjId, pPLine);
			pBTR->close();
			pPLine->close();
		}
		else
			return false;
		pBT->close();
	}
	else
		return false;

	pPLine->close();
	return true;
}

//텍스트생성 mode = 1 (가로선) mode = 2 (지시선)
bool CFEL::CreateText(AcGePoint3d pt, double Height, double Rotate, CString TxtStr, CString Layer, int mode){
	Acad::ErrorStatus es;
	AcDbObjectId textStyleId, pTextObjId;
	double XScale = 0, obliquingAngle = 0;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbText* pText = new AcDbText();
	textStyleId = pDb->textstyle();
	pText->setTextStyle(textStyleId);
	AcDbTextStyleTableRecord *pTSTR = NULL;
	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR;
	acdbOpenObject(pTSTR, textStyleId, AcDb::kForRead);
	if(pTSTR){
		XScale=pTSTR->xScale();
		obliquingAngle = pTSTR->obliquingAngle();
		pTSTR->close();
		pText->setWidthFactor(XScale);
	}

	pText->setOblique(obliquingAngle);
	pText->setLayer(Layer);
	pText->setPosition(pt);
	if(mode == 1){
		es = pText->setVerticalMode(AcDb::kTextBottom);
		es = pText->setHorizontalMode(AcDb::kTextCenter);
	}
	else{
		es = pText->setVerticalMode(AcDb::kTextBase);
		es = pText->setHorizontalMode(AcDb::kTextLeft);
	}
	pText->setAlignmentPoint(pt);
	pText->setHeight(Height);
	pText->setRotation(Rotate);
	pText->setTextString((LPCTSTR)TxtStr);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		// [kyh] 현재 작업창
		if (acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite) == Acad::eOk) {
		//if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pTextObjId, pText);
			pBTR->close();
			pText->close();
		}
		pBT->close();
	}
	return true;
}

//솔리드 생성
bool CFEL::CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer){
	Acad::ErrorStatus es;
	AcDbObjectId SolidId;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	if(pDb == NULL)
		return false;

	AcDbBlockTable* pBT;
	AcDbBlockTableRecord* pBTR = NULL;

	AcDbSolid* Arrow = new AcDbSolid(Solpt1, Solpt2, Solpt3,Solpt3);
	Arrow->setLayer(Layer);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if (acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(SolidId, Arrow);
			pBTR->close();
			Arrow->close();
		}
		/*if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(SolidId, Arrow);
			pBTR->close();
			Arrow->close();
		}*/
		pBT->close();
	}
	return true;
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CFEL::DTR(double angle){
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CFEL::RTD(double radian){	
	return (radian * 180.0 / PI);
}

void CFEL::pyosi(ads_point pt1){	
	AcGePoint3d _pt1;
	ads_arxpt(_pt1, pt1);
	if(_pt1.x>ErrVal || _pt1.y>ErrVal || _pt1.z>ErrVal){
		//	ads_printf(_T("\n pyosi ERROR!! _pt1.x=%.1f y=%.1f z=%.1f "), _pt1.x, _pt1.y, _pt1.z);
	}
	struct resbuf *ent = NULL, *sys1 = NULL, *sys2 = NULL;
	ads_point pt2, pt3;
	ads_real ang[] = {0.25, 0.75};
	ads_real yr1 = 0.0, vs = 0.0 ,ss = 0.0;
	short pb = 0, dur = 0;

	sys1 = ads_newrb(RTREAL);
	sys2 = ads_newrb(RT3DPOINT);
	ads_getvar(_T("VIEWSIZE"), sys1);
	ads_getvar(_T("SCREENSIZE"), sys2);
	vs = sys1->resval.rreal;
	ss = sys2->resval.rpoint[Y];
	ads_relrb(sys1);
	ads_relrb(sys2);
	yr1 = vs / ss * 6;

	for(dur = 0; dur < 2; dur++){
		ads_polar(pt1, ang[dur] * PI, yr1, pt2);
		ads_polar(pt1, (ang[dur] + 1.0) * PI, yr1, pt3);
		ads_grdraw(pt2, pt3, 1, 0);
	}
}

// ADS 좌표를 ARX 좌표로 변환
void CFEL::ads_arxpt(AcGePoint3d& ps,ads_point pe){
	ps.set(pe[X], pe[Y], pe[Z]);
}

void CFEL::OnBnClickedFelHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/FEL.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
