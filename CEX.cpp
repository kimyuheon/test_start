#include "StdAfx.h"
#include "CEX.h"


// CCEX 대화 상자입니다.
IMPLEMENT_DYNAMIC(CCEX, CAcUiDialog)
	CCEX::CCEX(CWnd* pParent)
	: CAcUiDialog(CCEX::IDD, pParent){
}

CCEX::~CCEX(){
}

void CCEX::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC1, bitmap1);	
	DDX_Control(pDX, IDC_BLOCK, CBlock);
	DDX_Control(pDX, IDC_ARC, CArc);
	DDX_Control(pDX, IDC_CHKARRAY, CChkArray);
	DDX_Control(pDX, IDC_CIRCLE, CCircle);
	DDX_Control(pDX, IDC_CONTEXT, CContext);
	DDX_Control(pDX, IDC_DONUT, CDonut);
	DDX_Control(pDX, IDC_EXCEPT, CExcept);
	DDX_Control(pDX, IDC_LINE, CLine);
	DDX_Control(pDX, IDC_PATH, CPath);
	DDX_Control(pDX, IDC_PATHINPUT, CPathInput);
	DDX_Control(pDX, IDC_POINT, CPoint);
	DDX_Control(pDX, IDC_POINTEDIT, CPointEdit);
	DDX_Control(pDX, IDC_POLYLINE, CPolyLine);
	DDX_Control(pDX, IDC_TEXT, CText);
	DDX_Control(pDX, IDC_ZAXIS, CZAxis);
}

BEGIN_MESSAGE_MAP(CCEX, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CCEX::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCEX::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_TEXTFILE, &CCEX::OnBnClickedTextfile)
	ON_BN_CLICKED(IDC_EXCEL, &CCEX::OnBnClickedExcel)
	ON_BN_CLICKED(IDC_EXCELFILE, &CCEX::OnBnClickedExcelfile)
	ON_BN_CLICKED(IDC_TEXT, &CCEX::OnBnClickedText)	
	ON_BN_CLICKED(IDC_CHKARRAY, &CCEX::OnBnClickedChkarray)
	ON_BN_CLICKED(IDC_PATH, &CCEX::OnBnClickedPath)
	ON_BN_CLICKED(IDC_SET, &CCEX::OnBnClickedSet)
	ON_BN_CLICKED(IDC_MINUS, &CCEX::OnBnClickedMinus)
	ON_BN_CLICKED(IDC_PLUS, &CCEX::OnBnClickedPlus)
	ON_BN_CLICKED(IDC_CONTEXT, &CCEX::OnBnClickedContext)
	ON_BN_CLICKED(IDC_RAD_CIVIL, &CCEX::OnBnClickedRadCivil)
	ON_BN_CLICKED(IDC_RAD_CAD, &CCEX::OnBnClickedRadCad)
	ON_BN_CLICKED(ID_CEX_HELP, &CCEX::OnBnClickedCexHelp)
END_MESSAGE_MAP()

BOOL CCEX::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	CString jobLineStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBLINE"));
	if(jobLineStr.CompareNoCase(_T("")) == 0 || jobLineStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_LINE, FALSE);
	else
		CheckDlgButton(IDC_LINE, TRUE);

	CString jobPolyLineStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBPOLYLINE"));
	if(jobPolyLineStr.CompareNoCase(_T("")) == 0 || jobPolyLineStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_POLYLINE, TRUE);
	else
		CheckDlgButton(IDC_POLYLINE, FALSE);

	CString jobTextStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBTEXT"));
	if(jobTextStr.CompareNoCase(_T("")) == 0 || jobTextStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_TEXT, FALSE);
	else
		CheckDlgButton(IDC_TEXT, TRUE);

	CString jobPointStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBPOINT"));
	if(jobPointStr.CompareNoCase(_T("")) == 0 || jobPointStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_POINT, FALSE);
	else
		CheckDlgButton(IDC_POINT, TRUE);

	CString jobCircleStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBCIRCLE"));
	if(jobCircleStr.CompareNoCase(_T("")) == 0 || jobCircleStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_CIRCLE, FALSE);
	else
		CheckDlgButton(IDC_CIRCLE, TRUE);

	CString jobBlockStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBBLOCK"));
	if(jobBlockStr.CompareNoCase(_T("")) == 0 || jobBlockStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_BLOCK, FALSE);
	else
		CheckDlgButton(IDC_BLOCK, TRUE);

	CString jobDonutStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBDONUT"));
	if(jobDonutStr.CompareNoCase(_T("")) == 0 || jobDonutStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_DONUT, FALSE);
	else
		CheckDlgButton(IDC_DONUT, TRUE);

	CString jobArcStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBARC"));
	if(jobArcStr.CompareNoCase(_T("")) == 0 || jobArcStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_ARC, FALSE);
	else
		CheckDlgButton(IDC_ARC, TRUE);

	CString coordinateStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("COORDINATE"));
	if(coordinateStr.CompareNoCase(_T("")) == 0 || coordinateStr.CompareNoCase(_T("CIVIL")) == 0){
		CheckDlgButton(IDC_RAD_CIVIL, TRUE);
		OnBnClickedRadCivil();
	}
	else{
		CheckDlgButton(IDC_RAD_CAD, TRUE);
		OnBnClickedRadCad();
	}

	CString jobModeStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("JOBMODE"));
	if(jobModeStr.CompareNoCase(_T("")) == 0 || jobModeStr.CompareNoCase(_T("EXCEL")) == 0){
		CheckDlgButton(IDC_EXCEL, TRUE);
		OnBnClickedExcel();
		CContext.EnableWindow(false);
	}
	else if(jobModeStr.CompareNoCase(_T("EXCELFILE")) == 0){
		CheckDlgButton(IDC_EXCELFILE, TRUE);
		OnBnClickedExcelfile();
	}
	else if(jobModeStr.CompareNoCase(_T("TEXTFILE")) == 0){
		CheckDlgButton(IDC_TEXTFILE, TRUE);
		OnBnClickedTextfile();
	}

	CString zAddStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("ZAXIS"));
	if(zAddStr.CompareNoCase(_T("")) == 0 || zAddStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_ZAXIS, TRUE);
	else
		CheckDlgButton(IDC_ZAXIS, FALSE);

	CString exceptStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("EXCEPT"));
	if(exceptStr.CompareNoCase(_T("")) == 0 || exceptStr.CompareNoCase(_T("TRUE")) == 0)
		CheckDlgButton(IDC_EXCEPT, TRUE);
	else
		CheckDlgButton(IDC_EXCEPT, FALSE);

	CString textAddStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("ADDTEXT"));
	if(textAddStr.CompareNoCase(_T("")) == 0 || textAddStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_CONTEXT, FALSE);
	else
		CheckDlgButton(IDC_CONTEXT, TRUE);

	CString jobSortStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("SORT"));
	if(jobSortStr.CompareNoCase(_T("")) == 0 || jobSortStr.CompareNoCase(_T("FALSE")) == 0)
		CheckDlgButton(IDC_CHKARRAY, FALSE);
	else
		CheckDlgButton(IDC_CHKARRAY, TRUE);

	CString sortArrowStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("SORTCOURSE"));
	if(sortArrowStr.CompareNoCase(_T("")) == 0 || sortArrowStr.CompareNoCase(_T("RIGHT")) == 0)
		CheckDlgButton(IDC_RIGHT, TRUE);
	else if(sortArrowStr.CompareNoCase(_T("LEFT")) == 0)
		CheckDlgButton(IDC_LEFT, TRUE);
	else if(sortArrowStr.CompareNoCase(_T("DOWN")) == 0)
		CheckDlgButton(IDC_DOWN, TRUE);
	else if(sortArrowStr.CompareNoCase(_T("UP")) == 0)
		CheckDlgButton(IDC_UP, TRUE);

	CString pointValue = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("POINTCOUNT"));
	if(pointValue.IsEmpty())
		pointValue = _T("4");
	SetDlgItemText(IDC_POINTEDIT, pointValue);

	CString divisionStr = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("DIVISION"));
	if(divisionStr.CompareNoCase(_T("")) == 0 || divisionStr.CompareNoCase(_T("1")) == 0)
		CheckDlgButton(IDC_Colon, TRUE);
	else if(divisionStr.CompareNoCase(_T("2")) == 0)
		CheckDlgButton(IDC_SPACE, TRUE);
	else if(divisionStr.CompareNoCase(_T("3")) == 0)
		CheckDlgButton(IDC_SET, TRUE);

	CString divisionValue = iniFile.readINI(_T("config.ini"), _T("CEX"), _T("DIVISIONSTRING"));
	SetDlgItemText(IDC_SETSPACE, divisionValue);
	OnBnClickedChkarray();
	OnBnClickedText();
	OnBnClickedExcel();
	OnBnClickedExcelfile();
	OnBnClickedTextfile();
	OnBnClickedChkarray();
	return TRUE;
}

void CCEX::OnBnClickedOk(){
	Arc = Text = Circle = Polyline = Point = Block = _T("Empty");
	if(CArc.GetCheck()){
		Arc = _T("ARC");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBARC"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBARC"), _T("FALSE"));

	if(CText.GetCheck()){
		Text = _T("TEXT");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBTEXT"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBTEXT"), _T("FALSE"));

	if(CCircle.GetCheck()){
		Circle = _T("CIRCLE");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBCIRCLE"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBCIRCLE"), _T("FALSE"));
	
	if(CDonut.GetCheck()){
		Polyline = _T("*POLYLINE");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBDONUT"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBDONUT"), _T("FALSE"));
	
	if(CPolyLine.GetCheck()){
		Polyline = _T("*POLYLINE");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBPOLYLINE"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBPOLYLINE"), _T("FALSE"));
	
	if(CPoint.GetCheck()){
		Point = _T("POINT");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBPOINT"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBPOINT"), _T("FALSE"));
	
	if(CBlock.GetCheck()){
		Block = _T("INSERT");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBBLOCK"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBBLOCK"), _T("FALSE"));
	
	if(CLine.GetCheck()){
		Line = _T("LINE");
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBLINE"), _T("TRUE"));
	}
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBLINE"), _T("FALSE"));

	if(coordinate == 1)
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("COORDINATE"), _T("CIVIL"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("COORDINATE"), _T("CAD"));
	
	if(IsDlgButtonChecked(IDC_EXCEL))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBMODE"), _T("EXCEL"));
	else if(IsDlgButtonChecked(IDC_EXCELFILE))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBMODE"), _T("EXCELFILE"));
	else if(IsDlgButtonChecked(IDC_TEXTFILE))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("JOBMODE"), _T("TEXTFILE"));

	if(IsDlgButtonChecked(IDC_ZAXIS))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("ZAXIS"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("ZAXIS"), _T("FLASE"));

	if(IsDlgButtonChecked(IDC_EXCEPT))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("EXCEPT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("EXCEPT"), _T("FLASE"));

	if(IsDlgButtonChecked(IDC_CONTEXT))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("ADDTEXT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("ADDTEXT"), _T("FLASE"));

	if(IsDlgButtonChecked(IDC_CHKARRAY))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("SORT"), _T("TRUE"));
	else
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("SORT"), _T("FLASE"));

	if(IsDlgButtonChecked(IDC_RIGHT))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("SORTCOURSE"), _T("RIGHT"));
	else if(IsDlgButtonChecked(IDC_LEFT))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("SORTCOURSE"), _T("LEFT"));
	else if(IsDlgButtonChecked(IDC_DOWN))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("SORTCOURSE"), _T("DOWN"));
	else if(IsDlgButtonChecked(IDC_UP))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("SORTCOURSE"), _T("UP"));

	CString pointValue;
	GetDlgItemText(IDC_POINTEDIT, pointValue);
	iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("POINTCOUNT"), pointValue);

	if(IsDlgButtonChecked(IDC_Colon))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("DIVISION"), _T("1"));
	else if(IsDlgButtonChecked(IDC_SPACE))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("DIVISION"), _T("2"));
	else if(IsDlgButtonChecked(IDC_SET))
		iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("DIVISION"), _T("3"));

	CString divisionValue;
	GetDlgItemText(IDC_POINTEDIT, divisionValue);
	iniFile.writeINI(_T("config.ini"), _T("CEX"), _T("DIVISIONSTRING"), divisionValue);

	CString filter, strMsg;
	filter = Arc + _T(" ") + Text + _T(" ") + Circle + _T(" ") + Polyline + _T(" ") + Point + _T(" ") + Block + _T(" ") + Line;
	filter.Replace(_T(" "), _T(","));
	filter.Replace(_T("Empty,"), _T("Empty"));
	filter.Replace(_T("Empty"), _T(""));
	if(selExTxt == 1 || selExTxt == 2 || selExTxt == 3){
		CString path;
		GetDlgItem(IDC_PATHINPUT)->GetWindowTextW(path);
		if(path != "" || selExTxt == 1){
			CAcUiDialog::OnOK();
			if(CArc.GetCheck() || CText.GetCheck() || CCircle.GetCheck() || CDonut.GetCheck() || CPolyLine.GetCheck() || CPoint.GetCheck() || CBlock.GetCheck() || CLine.GetCheck())
				ExpressPoint(filter);
		} else {
			strMsg.LoadStringW(IDS_PATH_CHECK); // "경로를 지정하지 않았습니다. 경로를 지정해주세요"
			AfxMessageBox(strMsg);
		}
	}
}

void CCEX::ExpressPoint(CString filter){
	ads_name ss;
	ads_point p1, p2;
	CString strMsg;
	resbuf *eb1 = acutBuildList(RTDXF0, filter, 0);
	strMsg.LoadStringW(IDS_SELECT_ENTITY);
	const wchar_t* prompts[2] = { strMsg, L"", };
	//if (acedSSGet(NULL, NULL, NULL, eb1, ss) != RTNORM) {
	CWnd* pCadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pCadWnd->EnableWindow(TRUE);
	this->ShowWindow(SW_HIDE);
	pCadWnd->SetFocus();
	if(acedSSGet(L":$", prompts, NULL, eb1, ss) != RTNORM){
		CompleteEditorCommand();
		return;
	}
	pCadWnd->EnableWindow(FALSE);
	this->ShowWindow(SW_SHOW);
	this->EnableWindow(TRUE);
	this->SetFocus();
	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	acedSSLength(ss, &len);

	bool chkLine = false;
	bool chkArc = false;
	bool chkpLine = false;
	bool chkCircle = false;
	bool chkDonut = false;
	bool chkBlock = false;
	bool chkText = false;
	bool chkPoint = false;

	int cntLine = 0;
	int cntArc = 0;
	int cntpLine = 0;
	int cntCircle = 0;
	int cntDonut = 0;
	int cntBlock = 0;
	int cntText = 0;
	int cntPoint = 0;

	AcDbObjectIdArray allIdArr;
	Acad::ErrorStatus es;
	for(int i = 0; i < len; i++){
		ads_name en;
		acedSSName(ss, i, en);
		AcDbObjectId idBlk;
		es=acdbGetObjectId(idBlk, en);
		if(es != Acad::eOk){
			AfxMessageBox(_T("엔티티명을 ObjectId로 변환 오류!"));
			CompleteEditorCommand();
			acedSSFree(ss);
			return;
		}
		allIdArr.append(idBlk);

		// 객체 열기
		AcDbEntityPointer pEnt(idBlk, AcDb::kForRead);
		es=pEnt.openStatus();
		if(es != Acad::eOk){
			AfxMessageBox(_T("블럭 열기 오류!"));
			CompleteEditorCommand();
			acedSSFree(ss);
			return;
		}
	}
	if(allIdArr.length() != 0){
		if(selExTxt == 1 || selExTxt == 2)
			createExcel(allIdArr, coordinate);
		else if(selExTxt == 3)
			makeText(allIdArr);
	}
	acedSSFree(ss);
}

void CCEX::OnBnClickedExcel(){
	if(IsDlgButtonChecked(IDC_EXCEL)){
		GetDlgItem(IDC_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_PATHINPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPACE)->EnableWindow(FALSE);	
		GetDlgItem(IDC_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETSPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_Colon)->EnableWindow(FALSE);
		selExTxt = 1;
	}
}

void CCEX::OnBnClickedExcelfile(){
	if(IsDlgButtonChecked(IDC_EXCELFILE)){
		GetDlgItem(IDC_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_PATHINPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPACE)->EnableWindow(FALSE);	
		GetDlgItem(IDC_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETSPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_Colon)->EnableWindow(FALSE);
		selExTxt = 2;
	}
}

void CCEX::OnBnClickedTextfile(){
	if(IsDlgButtonChecked(IDC_TEXTFILE)){
		GetDlgItem(IDC_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_PATHINPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_SETSPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_Colon)->EnableWindow(TRUE);
		selExTxt = 3;
	}
}

void CCEX::OnBnClickedText(){
	bool chkBtn = IsDlgButtonChecked(IDC_TEXT);
	if(chkBtn)
		GetDlgItem(IDC_CONTEXT)->EnableWindow(TRUE);
	else if(!chkBtn)
		GetDlgItem(IDC_CONTEXT)->EnableWindow(FALSE);
}

void CCEX::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

void CCEX::OnBnClickedChkarray(){
	bool chkBtn = IsDlgButtonChecked(IDC_CHKARRAY);
	if(chkBtn){
		GetDlgItem(IDC_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_UP)->EnableWindow(TRUE);
	}
	else if(!chkBtn){
		GetDlgItem(IDC_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_UP)->EnableWindow(FALSE);
	}
}

void CCEX::OnBnClickedPath(){
	if(selExTxt == 2){
		CString name_filter = "Excel (*.xlsx)|*.xlsx|";
		CFileDialog ins_dlg(FALSE, _T("xlsx"), _T("*.xlsx"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);
		if(ins_dlg.DoModal() == IDOK)
			SetDlgItemText(IDC_PATHINPUT, ins_dlg.GetPathName());
	}
	else if(selExTxt == 3){
		CString name_filter = "Txt (*.txt)|*.txt|";
		CFileDialog ins_dlg(FALSE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);
		if(ins_dlg.DoModal() == IDOK)
			SetDlgItemText(IDC_PATHINPUT, ins_dlg.GetPathName());
	}
}

void CCEX::OnBnClickedSet(){
	bool chk = (IsDlgButtonChecked(IDC_SET));
	if(chk)
		GetDlgItem(IDC_SPACE)->ShowWindow(TRUE);
	else if(!chk)
		GetDlgItem(IDC_SPACE)->ShowWindow(FALSE);
}

void CCEX::OnBnClickedMinus(){
	CString cnt;
	int count = GetDlgItemInt(IDC_POINTEDIT);
	if(count != 0)
		--count;	
	cnt.Format(_T("%d"), count);
	SetDlgItemText(IDC_POINTEDIT, cnt);
}

void CCEX::OnBnClickedPlus(){
	CString cnt;
	int count = GetDlgItemInt(IDC_POINTEDIT);
	++count;	
	cnt.Format(_T("%d"), count);
	SetDlgItemText(IDC_POINTEDIT, cnt);
}

void CCEX::OnBnClickedContext(){
	CString strMsg;
	if(CLine.GetCheck() || CArc.GetCheck() || CPolyLine.GetCheck() || CDonut.GetCheck() || CBlock.GetCheck() || CPoint.GetCheck() || CCircle.GetCheck()){
		strMsg.LoadStringW(IDS_EXPORT_COORDS_CHK); // "이 기능을 사용하려면 좌표를 내보낼 대상을 문자만 선택해 주십시오."
		AfxMessageBox(strMsg);
		CContext.SetCheck(false);
	}
}

void CCEX::OnBnClickedRadCivil(){
	coordinate = 1;
}

void CCEX::OnBnClickedRadCad(){
	coordinate = 2;
}



void CCEX::OnBnClickedCexHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/CEX.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
