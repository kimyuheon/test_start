#include "stdafx.h"
#include "WCZ.h"
#include "afxdialogex.h"

// WCZ 대화 상자입니다.
IMPLEMENT_DYNAMIC(CWCZ, CAcUiDialog)
	CWCZ::CWCZ(CWnd* pParent)
	: CAcUiDialog(CWCZ::IDD, pParent){
}

CWCZ::~CWCZ(){
}

void CWCZ::DoDataExchange(CDataExchange* pDX){
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLAYER, CLayer);
	DDX_Control(pDX, IDC_CSTYLE, CStyle);
	DDX_Control(pDX, IDC_TEXTSEL, m_selectBtn);
}

BEGIN_MESSAGE_MAP(CWCZ, CAcUiDialog)
	ON_BN_CLICKED(IDC_TEXTSEL, &CWCZ::OnBnClickedTextsel)
	ON_BN_CLICKED(IDOK, &CWCZ::OnBnClickedOk)
	ON_BN_CLICKED(ID_WCZ_HELP, &CWCZ::OnBnClickedWczHelp)
END_MESSAGE_MAP()

void CWCZ::OnBnClickedTextsel(){
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	ads_name txt;
	ads_point pt;
	AcDbObjectId TextId;

	CString strMsg;
	strMsg.LoadStringW(IDS_SET_TEXT); // "텍스트를 지정하세요."
	if(acedEntSel(strMsg, txt, pt) != RTNORM)
		CompleteEditorCommand();
	if(acdbGetObjectId(TextId, txt) == Acad::eOk){
		AcDbEntityPointer pEnt(TextId, AcDb::kForRead);
		if(pEnt.openStatus()==Acad::eOk){
			if(pEnt != NULL){
				if(pEnt->isKindOf(AcDbText::desc())){
					AcDbText *pText = AcDbText::cast(pEnt);
					double height, width;
					height = pText->height();
					width = pText->widthFactor();

					CString heightC;
					heightC.Format(_T("%g"), height);

					CString widthC;
					widthC.Format(_T("%g"), width);

					SetDlgItemText(IDC_TEXTHEIGHT, heightC);
					SetDlgItemText(IDC_TEXTWIDTH, widthC);
					CString layer = pText->layer();

					pText->textStyle();
					CLayer.SetCurSel(CLayer.FindString(0, layer));
					CStyle.SetCurSel(CStyle.FindString(0, findTextStyle(pText->textStyle())));
				}
			}
		}
	}
	CompleteEditorCommand();
}

CString CWCZ::findTextStyle(AcDbObjectId StyleId){	
	AcDbTextStyleTableRecordPointer pTextPoint(StyleId, AcDb::kForRead);
	TCHAR* name;
	pTextPoint->getName(name);
	return name;
}

BOOL CWCZ::OnInitDialog(){
	CAcUiDialog::OnInitDialog();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);
	m_selectBtn.AutoLoad();

	initLayer();
	initStyle();
	CLayer.SetCurSel(0);
	CStyle.SetCurSel(0);

	// 텍스트 높이와 너비, 단위 값을 초기화한다.
	CString textHeightStr = iniFile.readINI(_T("config.ini"), _T("WCZ"), _T("HEIGHT"));
	if(textHeightStr.CompareNoCase(_T("")) == 0)
		textHeightStr.Format(_T("1.000"));
	SetDlgItemText(IDC_TEXTHEIGHT, textHeightStr);

	CString textWidthStr = iniFile.readINI(_T("config.ini"), _T("WCZ"), _T("WIDTH"));
	if(textWidthStr.CompareNoCase(_T("")) == 0)
		textWidthStr.Format(_T("1.000"));
	SetDlgItemText(IDC_TEXTWIDTH, textWidthStr);

	CString textCelvationStr = iniFile.readINI(_T("config.ini"), _T("WCZ"), _T("CELVATION"));
	if(textCelvationStr.CompareNoCase(_T("")) == 0)
		textCelvationStr.Format(_T("5"));
	SetDlgItemText(IDC_CELVATION, textCelvationStr);

	return TRUE;
}

void CWCZ::initLayer(){	
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
		CLayer.AddString(name);
	}	
	delete pLayerTblIter;
	delete pLayerTbl;
}

void CWCZ::initStyle(){
	AcDbTextStyleTable* pTextStyleTbl = NULL;
	AcDbTextStyleTablePointer pTextStylePoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextStyleTblIter;
	pTextStylePoint->newIterator(pTextStyleTblIter);

	AcDbObjectId StyleId;
	for(pTextStyleTblIter->start(); !pTextStyleTblIter->done(); pTextStyleTblIter->step()){
		pTextStyleTblIter->getRecordId(StyleId);
		AcDbTextStyleTableRecordPointer pTextStyleTblRcd(StyleId, AcDb::kForRead);
		TCHAR* name;
		pTextStyleTblRcd->getName(name);
		CStyle.AddString(name);
	}	
	delete pTextStyleTblIter;
	delete pTextStyleTbl;
}

static double getRadian(double degree){
	double rad = 0;
	rad = degree * 3.141592 / 180;
	return rad;
}

void CWCZ::OnBnClickedOk(){
	BeginEditorCommand();

	bool dllchk = false;
	dllchk = loadResourcedllChk();
	if (!dllchk)
		g_hCurrResource = NULL;
	CAcModuleResourceOverride resOverride(g_hCurrResource);

	ads_point p1, p2;
	CString strMsg;

	strMsg.LoadStringW(IDS_FIRST_SELECT); // "첫 번째 점을 지정하세요"
	if(acedGetPoint(NULL, strMsg, p1) != RTNORM){
		CompleteEditorCommand();
		return;
	}
	strMsg.LoadStringW(IDS_SECOND_SELECT); // "두 번째 점을 지정하세요"
	if(acedGetPoint(p1, strMsg, p2) != RTNORM){
		CompleteEditorCommand();
		return;
	}

	resbuf *eb1 = acutBuildList(RTDXF0, _T("*POLYLINE"), 0);
	AcGePoint3d pt1, pt2;
	pt1 = asPnt3d(p1);
	pt2 = asPnt3d(p2);
	pt1.set(pt1.x, pt1.y, 0.0);
	pt2.set(pt2.x, pt2.y, 0.0);
	ads_name ss;

	if(acedSSGet(_T("C"), p1, p2, eb1, ss) != RTNORM){
		acedSSFree(ss);
		CancelEditorCommand();
		return;
	}

	AcDbObjectIdArray allIdArr;
	// [+] SUP-29742
	// long	len;
	// [-] END
	Adesk::Int32 len;
	acedSSLength(ss, &len);
	Acad::ErrorStatus es;
	for(int i = 0; i < len; i++){
		ads_name en;
		acedSSName(ss, i, en);
		AcDbObjectId idBlk;
		es = acdbGetObjectId(idBlk, en);
		if(es != Acad::eOk){
			acedSSFree(ss);
			CancelEditorCommand();
			return;
		}

		AcDbEntityPointer pEnt(idBlk, AcDb::kForRead);
		es = pEnt.openStatus();
		if(es != Acad::eOk){			
			acedSSFree(ss);
			CancelEditorCommand();
			return;
		}
		if(pEnt != NULL){
			if(pEnt->isKindOf(AcDbPolyline::desc()))
				allIdArr.append(idBlk);
		}
		pEnt->close();
	}

	AcGeVector3d v1;
	v1 = (pt2 - pt1).normal();
	AcDbLine *pLine = new AcDbLine(pt1, pt2);
	for(int i=0; i<allIdArr.length(); i++){		
		AcGePoint3dArray ptArr;
		AcDbPolyline *polyL = NULL;
		if(acdbOpenObject(polyL, allIdArr[i], AcDb::kForRead) != Acad::eOk){
			CancelEditorCommand();
			return;
		}

		AcDbPolyline *polyL2 = new AcDbPolyline();
		polyL2->copyFrom(polyL);
		polyL2->setElevation(0.0);
		pLine->intersectWith(polyL2, AcDb::kOnBothOperands, ptArr);
		polyL2->close();

		double elev = polyL->elevation();
		polyL->close();

		int meter = GetDlgItemInt(IDC_CELVATION);
		CString meterStr;
		meterStr.Format(_T("%d"), meter);
		iniFile.writeINI(_T("config.ini"), _T("WCZ"), _T("CELVATION"), meterStr);

		for(int a = 0; a < ptArr.length(); a++)
			for(int b = a + 1; b < ptArr.length(); b++)
				if(ptArr[a].isEqualTo(ptArr[b]))
					ptArr.remove(ptArr[b]);

		if((int)elev % meter == 0){
			for(int j = 0; j < ptArr.length(); j++){
				AcDbText *pText = new AcDbText;
				pText->setDatabaseDefaults();
				pText->setHorizontalMode(AcDb::kTextMid);
				pText->setVerticalMode(AcDb::kTextVertMid);
				pText->setPosition(ptArr[j]);
				pText->setAlignmentPoint(ptArr[j]);

				CString high;
				GetDlgItemText(IDC_TEXTHEIGHT, high);
				iniFile.writeINI(_T("config.ini"), _T("WCZ"), _T("HEIGHT"), high);

				double height = _wtof(high);
				pText->setHeight(height);

				CString wid;
				GetDlgItemText(IDC_TEXTWIDTH, wid);
				iniFile.writeINI(_T("config.ini"), _T("WCZ"), _T("WIDTH"), wid);

				double width = _wtof(wid);
				pText->setWidthFactor(width);

				CString eleva;
				eleva.Format(_T("%.0f"), elev);
				pText->setTextString(eleva);
				CString layer, style;
				CLayer.GetLBText(CLayer.GetCurSel(), layer);
				CStyle.GetLBText(CStyle.GetCurSel(), style);
				pText->setLayer(layer);
				pText->setTextStyle(findTextStyleName(style));
				AcGePlane pl(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis);
				double rot = v1.angleOnPlane(pl);
				pText->setRotation(rot + getRadian(270));				
				AcDbBlockTableRecord *pBTR = NULL;
				if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
					return;
				pBTR->appendAcDbEntity(pText);
				pBTR->close();
				pText->close();
			}
		}
	}
	pLine->close();
	acedSSFree(ss);
	CompleteEditorCommand();
	CAcUiDialog::OnOK();
}

AcDbObjectId CWCZ::findTextStyleName(CString name){
	AcDbTextStyleTable* pTextTbl = NULL;
	AcDbTextStyleTablePointer pTextPoint(acdbHostApplicationServices()->workingDatabase(), AcDb::kForRead);
	AcDbTextStyleTableIterator* pTextTblIter;
	pTextPoint->newIterator(pTextTblIter);

	AcDbObjectId TextId;
	for(pTextTblIter->start(); !pTextTblIter->done(); pTextTblIter->step()){
		pTextTblIter->getRecordId(TextId);
		AcDbTextStyleTableRecordPointer pTextTblRcd(TextId, AcDb::kForRead);
		TCHAR* name2;
		pTextTblRcd->getName(name2);
		if(wcscmp(name, name2) == 0)
			return TextId;
	}

	delete pTextTblIter;
	delete pTextTbl;
	return NULL;
}

void CWCZ::OnBnClickedWczHelp()
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

	Help_Path.Format(_T("%s\\%s::/1. html/WCZ.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
