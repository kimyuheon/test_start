// GCS.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GCS.h"
#include "afxdialogex.h"
#include "CRange.h"
#include "CRanges.h"
#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "tlhelp32.h"
#include "CIM.h"
#include <Ole2.h>
#include <afxdisp.h>
#include <tlhelp32.h>


// CGCS ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGCS, CAcUiDialog)

	CGCS::CGCS(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CGCS::IDD, pParent),b_RadBtn(0), m_DistStr(_T("")), m_RefroadStr(_T("")), m_CorroadStr(_T("")), Chk_ApptStr(_T("")), Chk_NotationStr(_T("")), cbxInt_Str(_T(""))
{

}

CGCS::~CGCS()
{
}

void CGCS::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	//����
	DDX_Radio(pDX, IDC_GCS_RADSTATION, b_RadBtn);
	//STATIC
	DDX_Text(pDX, IDC_GCS_EDTDIST, m_DistStr);
	DDX_Text(pDX, IDC_GCS_EDTREFROAD, m_RefroadStr);
	DDX_Text(pDX, IDC_GCS_EDTCORROAD, m_CorroadStr);
	//�޺��ڽ�
	DDX_Control(pDX, IDC_GCS_CBXDOT, m_cbxDot);
	//üũ�ڽ�
	DDX_Control(pDX, IDC_GCS_CHECK1, m_Chk1_btn);
	DDX_Control(pDX, IDC_GCS_CHECK2, m_Chk2_btn);

}


BEGIN_MESSAGE_MAP(CGCS, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &CGCS::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGCS::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GCS_RADSTATION, &CGCS::OnBnClickedGcsRadstation)
	ON_BN_CLICKED(IDC_GCS_RADNUMBER, &CGCS::OnBnClickedGcsRadnumber)
	ON_CBN_SELCHANGE(IDC_GCS_CBXDOT, &CGCS::OnCbnSelchangeGcsCbxdot)
	ON_EN_UPDATE(IDC_GCS_EDTDIST, &CGCS::OnEnUpdateGcsEdtdist)
	ON_BN_CLICKED(IDC_GCS_CHECK1, &CGCS::OnBnClickedGcsCheck1)
	ON_EN_UPDATE(IDC_GCS_EDTREFROAD, &CGCS::OnEnUpdateGcsEdtrefroad)
	ON_EN_UPDATE(IDC_GCS_EDTCORROAD, &CGCS::OnEnUpdateGcsEdtcorroad)
	ON_BN_CLICKED(IDC_GCS_CHECK2, &CGCS::OnBnClickedGcsCheck2)
	ON_BN_CLICKED(ID_GCS_HELP, &CGCS::OnBnClickedGcsHelp)
END_MESSAGE_MAP()


// CGCS �޽��� ó�����Դϴ�.


BOOL CGCS::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	//ImpressExcel();
	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	//�Ҽ���
	m_cbxDot.AddString(_T("0"));m_cbxDot.AddString(_T("1"));m_cbxDot.AddString(_T("2"));
	m_cbxDot.AddString(_T("4"));m_cbxDot.AddString(_T("5"));

	//���� ǥ�� STA = 1, NO = 2
	CString tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("JOBMODE"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		OnBnClickedGcsRadstation();
	else
		OnBnClickedGcsRadnumber();
	//�޺��ڽ� �Ҽ���
	tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("cbxDot"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0 || tempStr.CompareNoCase(_T("1")) == 0)
		m_cbxDot.SetCurSel(0);
	else{
		SetDlgItemText(IDC_GCS_CBXDOT, tempStr);
		cbxInt = _ttoi(tempStr);
		m_cbxDot.SetCurSel(cbxInt);
	}
	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("Dist"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GCS_EDTDIST, _T("20.00"));
	else
		SetDlgItemText(IDC_GCS_EDTDIST, tempStr);
	//üũ�ڽ� ���� ���� ���鿡�� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("Chk_Appt"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		b_Chk1 = false;
		m_Chk1_btn.SetCheck(FALSE);
		OnBnClickedGcsCheck1();
	}else{
		b_Chk1 = true;
		m_Chk1_btn.SetCheck(TRUE);
		OnBnClickedGcsCheck1();
	}
	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("Redroad"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GCS_EDTREFROAD, _T("0"));
	else
		SetDlgItemText(IDC_GCS_EDTREFROAD, tempStr);
	//���� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("Corroad"));
	if(tempStr.CompareNoCase(_T("")) == 0)
		SetDlgItemText(IDC_GCS_EDTCORROAD, _T("0"));
	else
		SetDlgItemText(IDC_GCS_EDTCORROAD, tempStr);
	//üũ�ڽ� ���� ���� ���鿡�� ����
	tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("Chk_Notation"));
	if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("0")) == 0){
		b_Chk2 = false;
		m_Chk2_btn.SetCheck(FALSE);
		OnBnClickedGcsCheck2();
	}else{
		b_Chk2 = true;
		m_Chk2_btn.SetCheck(TRUE);
		OnBnClickedGcsCheck2();
	}


	return TRUE; 
}


void CGCS::OnBnClickedOk(){
	UpdateData();
	CAcUiDialog::OnOK();
	CString strMsg;
	if(FindExcel() == false){//���� ��������
		strMsg.LoadStringW(IDS_RUN_CHECK_EXCEL); // "Excel�� ������� �ʾҽ��ϴ�."
		AfxMessageBox(strMsg);
		CGCS GCS;
		GCS.DoModal();
	}else{

		if(!(JobMode == 1 || JobMode == 2))JobMode = 1;
		JobMode_Str.Format(_T("%d"), JobMode);
		iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("JOBMODE"), JobMode_Str);
		iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("Dist"), m_DistStr);
		iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("Redroad"), m_RefroadStr);
		iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("Corroad"), m_CorroadStr);


		AcGePoint3d RefPt,CorPt, AnglePt, Close_RefPt ,Close_CorPt, AnglePt1, AnglePt2;
		CString tempKword_ref, tempKword_cor, YNDirect_ref, YNDirect_cor;
		double CloseRefPt_dst, CloseCorPt_dst, YNDirect_refF, YNDirect_corF;
		TCHAR kWord[20];

		BeginEditorCommand();

		bool dllchk = false;
		dllchk = loadResourcedllChk();
		if (!dllchk)
			g_hCurrResource = NULL;
		CAcModuleResourceOverride resOverride(g_hCurrResource);

		while(true){
			strMsg.LoadStringW(IDS_LOAD_LINE_SEL); // "���� ���� ���� ���� : "
			int stat = acedEntSel(strMsg, RefEns, asDblArray(RefPt));

			if(stat == RTCAN){
				acedSSFree(RefEns);
				return;
			}else if(stat == RTERROR){
				resbuf zValueStr;
				ads_getvar(_T("ERRNO"), &zValueStr);
				if(zValueStr.resval.rint == 52){
					acedSSFree(RefEns);
					return;
				}else
					continue;
			}else if(stat == RTNORM){

				acdbGetObjectId(objId_ref, RefEns);

				AcDbEntityPointer pEnt(objId_ref, AcDb::kForRead);
				if(pEnt.openStatus() == Acad::eOk){
					if(pEnt->isKindOf(AcDbLine::desc())){
						AcDbLine* pLine = AcDbLine::cast(pEnt);
						pLine->getClosestPointTo(RefPt, Close_RefPt);
						pLine->getDistAtPoint(Close_RefPt,CloseRefPt_dst);
						pLine->getPointAtDist(CloseRefPt_dst+0.1, AnglePt);
						pLine->getStartPoint(RefSpt);
						pLine->getEndPoint(RefEpt);
						pLine->close();
					}else if(pEnt->isKindOf(AcDbPolyline::desc())){
						AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
						pPLine->getClosestPointTo(RefPt, Close_RefPt);
						pPLine->getDistAtPoint(Close_RefPt,CloseRefPt_dst);
						pPLine->getPointAtDist(CloseRefPt_dst+0.1, AnglePt);
						pPLine->getStartPoint(RefSpt);
						pPLine->getEndPoint(RefEpt);
						pPLine->close();
					}else if(pEnt->isKindOf(AcDbArc::desc())){
						AcDbArc* pArc = AcDbArc::cast(pEnt);
						pArc->getClosestPointTo(RefPt, Close_RefPt);
						pArc->getDistAtPoint(Close_RefPt,CloseRefPt_dst);
						pArc->getPointAtDist(CloseRefPt_dst+0.1, AnglePt);
						pArc->getStartPoint(RefSpt);
						pArc->getEndPoint(RefEpt);
						pArc->close();
					}else if(pEnt->isKindOf(AcDbSpline::desc())){
						AcDbSpline* pSPLine = AcDbSpline::cast(pEnt);
						pSPLine->getClosestPointTo(RefPt, Close_RefPt);
						pSPLine->getDistAtPoint(Close_RefPt,CloseRefPt_dst);
						pSPLine->getPointAtDist(CloseRefPt_dst+0.1, AnglePt);
						pSPLine->getStartPoint(RefSpt);
						pSPLine->getEndPoint(RefEpt);
						pSPLine->close();
					}else{
						strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "\n**������ ��ü�� ������ �ƴմϴ�.**"
						acutPrintf(strMsg);
						continue;
					}
				}
				pEnt->close();
				break;
			}else
				continue;

		}//���� ���� ���� while end

		//���� ����ȭ��ǥ
		struct resbuf* sys1 = NULL,*sys2 = NULL;
		ads_real do1=0.0, vs=0.0, ss=0.0;
		sys1=ads_newrb(RTREAL);    
		sys2=ads_newrb(RT3DPOINT);
		ads_getvar(_T("VIEWSIZE"), sys1); 
		ads_getvar(_T("SCREENSIZE"), sys2);
		vs=sys1->resval.rreal;
		ss = sys2->resval.rpoint[Y];
		do1=vs/ss;

		ads_polar(asDblArray(Close_RefPt), ads_angle(asDblArray(Close_RefPt), asDblArray(AnglePt))+DTR(160.0),50 * do1,asDblArray(AnglePt1));
		ads_polar(asDblArray(Close_RefPt), ads_angle(asDblArray(Close_RefPt), asDblArray(AnglePt))-DTR(160.0),50 * do1,asDblArray(AnglePt2));
		acedGrDraw(asDblArray(AnglePt1),asDblArray(Close_RefPt),1,0);
		acedGrDraw(asDblArray(AnglePt2),asDblArray(Close_RefPt),1,0);

		while(true){//���� ������� Yes or No
			CString tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("YNDirect_ref"));
			if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("Y")) == 0)
				tempStr.Format(_T("Y"));
			else
				tempStr;

			if(tempStr == _T("Y"))
				YNDirect_ref.Format(_T("Yes(Y)"));
			else if(tempStr == _T("N"))
				YNDirect_ref.Format(_T("No(N)"));

			strMsg.LoadStringW(IDS_LINE_DIRECT_CHK); // "\n���� ���� ������ �½��ϱ�? [��(Y)/�ƴϿ�(N)] <%s> : "
			tempKword_ref.Format(strMsg, YNDirect_ref);
			acedInitGet(0, _T("Y y N n"));
			int stat = acedGetReal(tempKword_ref, &YNDirect_refF/*(TCHAR*)(LPCTSTR)DirYN_str*/);

			if(stat == RTCAN){
				acedSSFree(RefEns);
				return;
			}else if(stat == RTNONE){
				DirYN_Str_ref = tempStr;
			}else if(stat == RTNORM){
				continue;
			}else if(stat == RTKWORD){
				if(acedGetInput(kWord) != RTNORM){
					acedSSFree(RefEns);
					return;
				}
				if(!_tcscmp(_T("Y"),kWord) || !_tcscmp(_T("y"),kWord))
					DirYN_Str_ref.Format(kWord);
				else if(!_tcscmp(_T("N"),kWord) || !_tcscmp(_T("n"),kWord))
					DirYN_Str_ref.Format(kWord);
			}
			iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("YNDirect_ref"), DirYN_Str_ref);
			break;
		}//���� ���� while end
		if(b_Chk1 == true){//�������� ���鿡�� ����
			strMsg.LoadStringW(IDS_BASE_LOAD_POINT); // "\n���� ���� ���� ������ �� �� ���� : "
			if(acedGetPoint(0, strMsg, asDblArray(DirPt_ref)) != RTNORM){
				acedSSFree(RefEns);
				return;
			}
			strMsg.LoadStringW(IDS_POINT_INPUT); // "\n������ ���� ���� �Է� : "
			if(acedGetReal(strMsg, &DirSurvey_refF) != RTNORM){
				acedSSFree(RefEns);
				return;
			}
		}

		while(true){
			strMsg.LoadStringW(IDS_LOAD_LINE_SEL); // "���� ���� ���� ���� : "
			int stat = acedEntSel(strMsg, CorEns, asDblArray(CorPt));

			if(stat == RTCAN){
				acedSSFree(RefEns);
				acedSSFree(CorEns);
				return;
			}else if(stat == RTERROR){
				resbuf zValueStr;
				ads_getvar(_T("ERRNO"), &zValueStr);
				if(zValueStr.resval.rint == 52){
					acedSSFree(RefEns);
					acedSSFree(CorEns);
					return;
				}else
					continue;
			}else if(stat == RTNORM){
				acdbGetObjectId(objId_cor, CorEns);

				AcDbEntityPointer pEnt(objId_cor, AcDb::kForRead);
				if(pEnt.openStatus() == Acad::eOk){
					if(pEnt->isKindOf(AcDbLine::desc())){
						AcDbLine* pLine = AcDbLine::cast(pEnt);
						pLine->getClosestPointTo(CorPt, Close_CorPt);
						pLine->getDistAtPoint(Close_CorPt,CloseCorPt_dst);
						pLine->getPointAtDist(CloseCorPt_dst+0.1, AnglePt);
						pLine->getStartPoint(CorSpt);
						pLine->getEndPoint(CorEpt);
						pLine->close();
					}else if(pEnt->isKindOf(AcDbPolyline::desc())){
						AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
						pPLine->getClosestPointTo(CorPt, Close_CorPt);
						pPLine->getDistAtPoint(Close_CorPt,CloseCorPt_dst);
						pPLine->getPointAtDist(CloseCorPt_dst+0.1, AnglePt);
						pPLine->getStartPoint(CorSpt);
						pPLine->getEndPoint(CorEpt);
						pPLine->close();
					}else if(pEnt->isKindOf(AcDbArc::desc())){
						AcDbArc* pArc = AcDbArc::cast(pEnt);
						pArc->getClosestPointTo(CorPt, Close_CorPt);
						pArc->getDistAtPoint(Close_CorPt,CloseCorPt_dst);
						pArc->getPointAtDist(CloseCorPt_dst+0.1, AnglePt);
						pArc->getStartPoint(CorSpt);
						pArc->getEndPoint(CorEpt);
						pArc->close();
					}else if(pEnt->isKindOf(AcDbSpline::desc())){
						AcDbSpline* pSPLine = AcDbSpline::cast(pEnt);
						pSPLine->getClosestPointTo(CorPt, Close_CorPt);
						pSPLine->getDistAtPoint(Close_CorPt,CloseCorPt_dst);
						pSPLine->getPointAtDist(CloseCorPt_dst+0.1, AnglePt);
						pSPLine->getStartPoint(CorSpt);
						pSPLine->getEndPoint(CorEpt);
						pSPLine->close();
					}else{
						strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "\n**������ ��ü�� ������ �ƴմϴ�.**"
						acutPrintf(strMsg);
						continue;
					}
				}
				pEnt->close();
				break;
			}else
				continue;

		}//���� ���� ���� while end

		//���� ����ȭ��ǥ
		ads_polar(asDblArray(Close_CorPt), ads_angle(asDblArray(Close_CorPt), asDblArray(AnglePt))+DTR(160.0),50 * do1,asDblArray(AnglePt1));
		ads_polar(asDblArray(Close_CorPt), ads_angle(asDblArray(Close_CorPt), asDblArray(AnglePt))-DTR(160.0),50 * do1,asDblArray(AnglePt2));
		acedGrDraw(asDblArray(AnglePt1),asDblArray(Close_CorPt),1,0);
		acedGrDraw(asDblArray(AnglePt2),asDblArray(Close_CorPt),1,0);

		while(true){//���� ������� Yes or No
			CString tempStr = iniFile.readINI(_T("config.ini"), _T("GCS"), _T("YNDirect_cor"));
			if(tempStr.CompareNoCase(_T("")) == 0 || tempStr.CompareNoCase(_T("Y")) == 0)
				tempStr.Format(_T("Y"));
			else
				tempStr;

			if(tempStr == _T("Y"))
				YNDirect_cor .Format(_T("Yes(Y)"));
			else if(tempStr == _T("N"))
				YNDirect_cor.Format(_T("No(N)"));

			strMsg.LoadStringW(IDS_LINE_DIRECT_CHK); // "\n���� ���� ������ �½��ϱ�? [��(Y)/�ƴϿ�(N)] <%s> : "
			tempKword_cor.Format(strMsg, YNDirect_cor);
			acedInitGet(0, _T("Y y N n"));
			int stat = acedGetReal(tempKword_cor, &YNDirect_corF/*(TCHAR*)(LPCTSTR)DirYN_str*/);

			if(stat == RTCAN){
				acedSSFree(RefEns);
				acedSSFree(CorEns);
				return;
			}else if(stat == RTNONE){
				DirYN_Str_cor = tempStr;
			}else if(stat == RTNORM){
				continue;
			}else if(stat == RTKWORD){
				if(acedGetInput(kWord) != RTNORM){
					acedSSFree(RefEns);
					acedSSFree(CorEns);
					return;
				}					
				if(!_tcscmp(_T("Y"),kWord) || !_tcscmp(_T("y"),kWord))
					DirYN_Str_cor.Format(kWord);
				else if(!_tcscmp(_T("N"),kWord) || !_tcscmp(_T("n"),kWord))
					DirYN_Str_cor.Format(kWord);
			}
			iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("YNDirect_cor"), DirYN_Str_cor);	
			break;
		}//���� ���� while end
		if(b_Chk1 == true){//�������� ���鿡�� ����
			strMsg.LoadStringW(IDS_BASE_LOAD_POINT); // "\n���� ���� ���� ������ �� �� ���� : "
			if(acedGetPoint(0, strMsg, asDblArray(DirPt_cor)) != RTNORM){
				acedSSFree(RefEns);
				acedSSFree(CorEns);
				return;
			}					
			strMsg.LoadStringW(IDS_POINT_INPUT); // "\n������ ���� ���� �Է� : "
			if(acedGetReal(strMsg, &DirSurvey_corF) != RTNORM){
				acedSSFree(RefEns);
				acedSSFree(CorEns);
				return;
			}					
		}
		CompleteEditorCommand();

		ImpressExcel();

		ads_ssfree(RefEns);
		ads_ssfree(CorEns);
		for(int i=0; i<ObjIdArray.length(); i++){
			EraseEntity(ObjIdArray.at(i));
		}

	}

}


void CGCS::ImpressExcel() {


	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant VTrue((short)TRUE);
	COleVariant VFalse((short)FALSE);
	CApplication app;
	CRange startrng;	
	LPDISPATCH lpdisp;
	CWorksheet sheet;
	CWorkbook book;
	CWorkbooks books;
	CWorksheets sheets;
	int column1 = 0;
	int column2 = 1;
	int pointcnt;
	// app ���� ������Ʈ	
	::CoInitialize(NULL);

	IDispatch* pdispExcelApp =NULL;
	IUnknown *pUnk=NULL;
	CLSID clsExcelApp;
	CString strMsg;

	if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp))) 
	{
		strMsg.LoadStringW(IDS_SETUP_EXCEL); // "������ ��ġ�Ǿ� ���� �ʽ��ϴ�."
		MessageBox(strMsg, _T("Failed"), MB_OK | MB_ICONSTOP);
	}


	HRESULT hr = GetActiveObject(clsExcelApp, NULL, (IUnknown**)&pUnk);
	if(SUCCEEDED(hr)){
		hr = pUnk->QueryInterface(IID_IDispatch, (void **)&pdispExcelApp);
	}
	else
	{
		strMsg.LoadStringW(IDS_FIND_EXCEL); // "������ ã�� �� �����ϴ�."
		AfxMessageBox(strMsg);
	}


	//App ã�� ���н� ����
	if(NULL == pdispExcelApp){
		acedSSFree(RefEns);
		acedSSFree(CorEns);
		return;
	}					


	//Application

	app.AttachDispatch(pdispExcelApp);

	//Workbooks

	lpdisp=app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);

	//Books
	lpdisp=books.get_Item(COleVariant((short)1));
	ASSERT(lpdisp);
	book.AttachDispatch(lpdisp);

	//Sheets
	lpdisp = book.get_Sheets();
	ASSERT(lpdisp); 
	sheets.AttachDispatch(lpdisp); 

	//Sheet
	lpdisp = sheets.get_Item(COleVariant((short)1));
	ASSERT(lpdisp);
	sheet.AttachDispatch(lpdisp);


	//�������� ��ȭ���� ����
	COleVariant getCell, selectSheet;     //���� ����
	getCell.pdispVal = NULL;

	if(app.get_Visible() == false)
		app.put_Visible(true);

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	if(getCell.pdispVal == NULL){         
		getCell = app.InputBox(IDStoCString(IDS_IMPORT_EXCEL_COORDS)/*"���� ��ǥ ��������"*/, 
			COleVariant(IDStoCString(IDS_EXCEL_TO_CAD)/*"����ǥ�� ĳ��� ��������"*/), selectSheet, VOptional, VOptional, VOptional, VOptional, COleVariant(short(8)));
		//���� ������ �Ǵ� ����
		if(getCell.pdispVal == NULL){
			AfxMessageBox(IDStoCString(IDS_SET_EXCEL_AREA)/*_T("������ �������� �ʾҽ��ϴ�.")*/);
			acedSSFree(RefEns);
			acedSSFree(CorEns);
			return;
		}
	}    
	//���� ����
	startrng.AttachDispatch(getCell.pdispVal);

	//��Ʈ ��������
	CString SelectXlsSheet; //���� ��Ʈ
	int SelectXlsIndex = 0;
	lpdisp = startrng.get_Parent();
	sheet.AttachDispatch(lpdisp);
	SelectXlsSheet = sheet.get_Name();
	SelectXlsIndex = sheet.get_Index();

	int iRows, iCols;//�� �� ���� 
	CRange rowRange = startrng.get_Rows();		//��ü ���� ��
	iRows = rowRange.get_Count();				//��ü �� ���� �� ��
	CRange colRange = startrng.get_Columns();	//��ü ���� ��
	iCols = colRange.get_Count();				//��ü �� ���� �� ��

	CellAttTable cellAttTable;

	CRange curRange;
	long ReferenceStyle = 0;
	VARIANT varMerge; //����üũ
	AcGePoint3d SurPt, Close_SurPt;//����, ������ ���������� �������
	AcGePoint3d Close_CorPt,AnglePt, RefroadPt, CorroadPt, Close_DirPt_cor, Close_DirPt_ref;
	double CloseCorPt_dst, Close_DirPt_corF,Close_DirPt_refF;
	double CorDist;//�������� ���������� �Ÿ�
	AcDbObjectId pLineId;//������ ���� ������ ���� ID


	AcGePoint3d InterPt;//�������ο� ���ص��� �������� ������
	AcGePoint3dArray ptArray;//������ �迭
	CString CellStr,NumStr;

	//������ ��������

	for(long icurCol = 1; icurCol <= iCols; icurCol++){
		CellAttArray atts;
		for(long icurRow = 1; icurRow <= iRows; icurRow++){
			curRange.AttachDispatch(startrng.get_Item(COleVariant(icurRow), COleVariant(icurCol)).pdispVal);

			CellAtt cellAtt;
			cellAtt.IsVertical = curRange.get_VerticalAlignment().intVal;
			cellAtt.IsHorizon = curRange.get_HorizontalAlignment().intVal;

			cellAtt.IsMerge = false;
			cellAtt.IsRow = curRange.get_Row();
			cellAtt.IsCol = curRange.get_Column();
			cellAtt.IsRange = curRange.get_Address(COleVariant(icurRow), COleVariant(icurCol),ReferenceStyle, VOptional, VOptional);

			cellAtt.IsString = curRange.get_Text().bstrVal;//�� ��
			atts.push_back(cellAtt);
			curRange.ReleaseDispatch();
			CellStr = cellAtt.IsString;
			//������ �� �� ��ȯ
			if(JobMode == 1){
				NumStr = CheckInputNumber(CellStr, 1);

			}else if(JobMode == 2)
				NumStr = CheckInputNumber(CellStr, 2);

			//���� �Է°� 
			double SurValue = _tstof(NumStr);

			//���ص��� ��ü
			AcDbEntityPointer pEnt(objId_ref, AcDb::kForWrite);
			if(pEnt.openStatus() == Acad::eOk){
				if(pEnt->isKindOf(AcDbLine::desc())){
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					if(DirYN_Str_ref.CompareNoCase(_T("N")) == 0){//��������� NO�ϋ� ���۳� swqp
						pLine->reverseCurve();
						pLine->getStartPoint(RefSpt);
						pLine->getEndPoint(RefEpt);
					}
					if(b_Chk1 == true){//���鿡�� �������� üũ��
						pLine->getClosestPointTo(DirPt_ref,Close_DirPt_ref);
						pLine->getDistAtPoint(Close_DirPt_ref, Close_DirPt_refF);
						SurValue = SurValue + Close_DirPt_refF - DirSurvey_refF;//�����Է°� + ������~���������Ÿ� - ����������
					}else{//���ص��� ���� ����
						SurValue = SurValue - m_Refroad;
					}
					pLine->getPointAtDist(SurValue, SurPt);//����ڰ� ������ �Է��� ���� 
					pLine->close();
				}else if(pEnt->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt);
					if(DirYN_Str_ref.CompareNoCase(_T("N")) == 0){
						pPLine->reverseCurve();
						pPLine->getStartPoint(RefSpt);
						pPLine->getEndPoint(RefEpt);
					}
					if(b_Chk1 == true){//���鿡�� �������� üũ��
						pPLine->getClosestPointTo(DirPt_ref,Close_DirPt_ref);
						pPLine->getDistAtPoint(Close_DirPt_ref, DirSurvey_refF);
						SurValue = SurValue + Close_DirPt_refF - DirSurvey_refF;
					}else{//���ص��� ���� ����
						SurValue = SurValue - m_Refroad;
					}
					pPLine->getPointAtDist(SurValue, SurPt);
					pPLine->close();
				}else if(pEnt->isKindOf(AcDbArc::desc())){
					AcDbArc* pArc = AcDbArc::cast(pEnt);
					if(DirYN_Str_ref.CompareNoCase(_T("N")) == 0){
						pArc->reverseCurve();
						pArc->getStartPoint(RefSpt);
						pArc->getEndPoint(RefEpt);
					}
					if(b_Chk1 == true){//���鿡�� �������� üũ��
						pArc->getClosestPointTo(DirPt_ref,Close_DirPt_ref);
						pArc->getDistAtPoint(Close_DirPt_ref, DirSurvey_refF);
						SurValue = SurValue + Close_DirPt_refF - DirSurvey_refF;
					}else{//���ص��� ���� ����
						SurValue = SurValue - m_Refroad;
					}
					pArc->getPointAtDist(SurValue, SurPt);
					pArc->close();
				}else if(pEnt->isKindOf(AcDbSpline::desc())){
					AcDbSpline* pSPLine = AcDbSpline::cast(pEnt);
					if(DirYN_Str_ref.CompareNoCase(_T("N")) == 0){
						pSPLine->reverseCurve();
						pSPLine->getStartPoint(RefSpt);
						pSPLine->getEndPoint(RefEpt);
					}
					if(b_Chk1 == true){//���鿡�� �������� üũ��
						pSPLine->getClosestPointTo(DirPt_ref,Close_DirPt_ref);
						pSPLine->getDistAtPoint(Close_DirPt_ref, DirSurvey_refF);
						SurValue = SurValue + Close_DirPt_refF - DirSurvey_refF;
					}else{//���ص��� ���� ����
						SurValue = SurValue - m_Refroad;
					}
					pSPLine->getPointAtDist(SurValue, SurPt);
					pSPLine->close();
				}else{
					strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "\n**������ ��ü�� ������ �ƴմϴ�.**"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();
			//inters �ϱ����� ���ص����� ������
			acutPolar(asDblArray(SurPt),ads_angle(asDblArray(RefSpt),asDblArray(RefEpt)) + DTR(90.0), 1, asDblArray(AnglePt));
			//intersectwith�� �ϱ����� ���λ���
			pLineId = CreateLine(SurPt, AnglePt);
			AcDbEntityPointer pEntPtr(pLineId,AcDb::kForRead);
			//CheckObject(objId_cor);//�������� ������ ���� ���ϱ� ����
			//�������� ��ü
			AcDbEntityPointer pEnt2(objId_cor, AcDb::kForWrite);

			if(pEnt2.openStatus() == Acad::eOk){
				if(pEnt2->isKindOf(AcDbLine::desc())){
					AcDbLine* pLine = AcDbLine::cast(pEnt2);
					if(DirYN_Str_cor.CompareNoCase(_T("N")) == 0){
						pLine->reverseCurve();
						pLine->getStartPoint(RefSpt);
						pLine->getEndPoint(RefEpt);
					}
					pLine->intersectWith(pEntPtr,AcDb::kExtendBoth,ptArray);
					InterPt = ptArray.at(0);//������
					pLine->getClosestPointTo(InterPt, Close_CorPt);
					pLine->getDistAtPoint(Close_CorPt, CorDist);
					if(b_Chk1 == true){//���鿡�� ���� ���� ������
						pLine->getClosestPointTo(DirPt_cor,Close_DirPt_cor);
						pLine->getDistAtPoint(Close_DirPt_cor, Close_DirPt_corF);
						CorDist = CorDist - DirSurvey_corF-Close_DirPt_corF;//���������� �Ÿ� - �������� �� -������~���������Ÿ�
					}else{//�������� ��������
						CorDist = CorDist - m_Corroad;
					}
					pLine->close();
				}else if(pEnt2->isKindOf(AcDbPolyline::desc())){
					AcDbPolyline* pPLine = AcDbPolyline::cast(pEnt2);
					if(DirYN_Str_cor.CompareNoCase(_T("N")) == 0){
						pPLine->reverseCurve();
						pPLine->getStartPoint(CorSpt);
						pPLine->getEndPoint(CorEpt);
					}
					pPLine->intersectWith(pEntPtr,AcDb::kExtendBoth,ptArray);
					InterPt = ptArray.at(0);
					pPLine->getClosestPointTo(InterPt, Close_CorPt);
					pPLine->getDistAtPoint(Close_CorPt, CorDist);
					if(b_Chk1 == true){//���鿡�� ���� ���� ������
						pPLine->getClosestPointTo(DirPt_cor,Close_DirPt_cor);
						pPLine->getDistAtPoint(Close_DirPt_cor, DirSurvey_corF);
						CorDist = CorDist - DirSurvey_corF-Close_DirPt_corF;
					}else{//�������� ��������
						CorDist = CorDist - m_Corroad;
					}
					pPLine->close();
				}else if(pEnt2->isKindOf(AcDbArc::desc())){
					AcDbArc* pArc = AcDbArc::cast(pEnt2);
					if(DirYN_Str_cor.CompareNoCase(_T("N")) == 0){
						pArc->reverseCurve();
						pArc->getStartPoint(CorSpt);
						pArc->getEndPoint(CorEpt);
					}
					pArc->intersectWith(pEntPtr,AcDb::kExtendBoth,ptArray);
					InterPt = ptArray.at(0);
					pArc->getClosestPointTo(InterPt, Close_CorPt);
					pArc->getDistAtPoint(Close_CorPt, CorDist);
					if(b_Chk1 == true){//���鿡�� ���� ���� ������
						pArc->getClosestPointTo(DirPt_cor,Close_DirPt_cor);
						pArc->getDistAtPoint(Close_DirPt_cor, DirSurvey_corF);
						CorDist = CorDist - DirSurvey_corF-Close_DirPt_corF;
					}else{//�������� ��������
						CorDist = CorDist - m_Corroad;
					}
					pArc->close();
				}else if(pEnt2->isKindOf(AcDbSpline::desc())){
					AcDbSpline* pSPLine = AcDbSpline::cast(pEnt2);
					if(DirYN_Str_cor.CompareNoCase(_T("N")) == 0){
						pSPLine->reverseCurve();
						pSPLine->getStartPoint(CorSpt);
						pSPLine->getEndPoint(CorEpt);
					}
					pSPLine->intersectWith(pEntPtr,AcDb::kExtendBoth,ptArray);
					InterPt = ptArray.at(0);
					pSPLine->getClosestPointTo(InterPt, Close_CorPt);
					pSPLine->getDistAtPoint(Close_CorPt, CorDist);
					if(b_Chk1 == true){//���鿡�� ���� ���� ������
						pSPLine->getClosestPointTo(DirPt_cor,Close_DirPt_cor);
						pSPLine->getDistAtPoint(Close_DirPt_cor, DirSurvey_corF);
						CorDist = CorDist - DirSurvey_corF-Close_DirPt_corF;
					}else{//�������� ��������
						CorDist = CorDist - m_Corroad;
					}
					pSPLine->close();
				}else{
					strMsg.LoadStringW(IDS_SELECT_CHK_LINE); // "\n**������ ��ü�� ������ �ƴմϴ�.**"
					acutPrintf(strMsg);
					continue;
				}
			}
			pEnt->close();

			//�޺��ڽ� �Ҽ��� �ڸ��� ���
			m_cbxDot.GetLBText(m_cbxDot.GetCurSel(), m_DotStr);
			m_Dot = _tstof(m_DotStr);
			CString ResultDistStr,SepDistStr,tempDot;
			double SepDist = ads_distance(asDblArray(SurPt),asDblArray(InterPt));//�̰ݰŸ�
			tempDot.Format(_T("%%.%df"), m_Dot);
			ResultDistStr.Format(tempDot, CorDist);
			SepDistStr.Format(_T("%.4f"), SepDist);
			if(JobMode == 2){//ǥ���� = NO�϶�
				CString FormatDot;
				int essence = CorDist / m_Dist;
				double Remainder = fmod(CorDist, m_Dist);
				FormatDot.Format(_T("%d+%%.%df"),essence, m_Dot);
				ResultDistStr.Format(FormatDot, Remainder);
			}
			//���� ���� �� �ֱ� (���۰Ÿ� - ����� �Է°��� ������ �� NULL)
			if(CorDist < 0 || SurValue < 0){
				startrng.put_Item(COleVariant((long)icurRow), COleVariant((long)2), COleVariant());
				if(b_Chk2 == true)//�̰ݰŸ�ǥ�� true��
					startrng.put_Item(COleVariant((long)icurRow), COleVariant((long)3), COleVariant());
			}else{
				startrng.put_Item(COleVariant((long)icurRow), COleVariant((long)2), COleVariant(ResultDistStr));
				if(b_Chk2 == true)//�̰ݰŸ�ǥ�� true��
					startrng.put_Item(COleVariant((long)icurRow), COleVariant((long)3), COleVariant(SepDistStr));
			}

			ptArray.removeAll();//����Ʈ�迭 �ʱ�ȭ
			ObjIdArray.append(pLineId);
		}//for ���� �� end
	}//for ���� �� end
	acedSSFree(RefEns);
	acedSSFree(CorEns);

}

void CGCS::OnBnClickedCancel(){
	CAcUiDialog::OnCancel();
}

//���� ǥ�� STA
void CGCS::OnBnClickedGcsRadstation(){
	UpdateData(TRUE);
	JobMode = 1;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 0;
	GetDlgItem(IDC_GCS_STATIC_DIST)->EnableWindow(FALSE);//���� ���� ��Ȱ��ȭ
	GetDlgItem(IDC_GCS_EDTDIST)->EnableWindow(FALSE);
	iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//���� ǥ�� NO
void CGCS::OnBnClickedGcsRadnumber(){
	UpdateData(TRUE);
	JobMode = 2;
	JobMode_Str.Format(_T("%d"), JobMode);
	b_RadBtn = 1;
	GetDlgItem(IDC_GCS_STATIC_DIST)->EnableWindow(TRUE);//���� ���� ��Ȱ��ȭ
	GetDlgItem(IDC_GCS_EDTDIST)->EnableWindow(TRUE);
	iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("JOBMODE"), JobMode_Str);
	UpdateData(FALSE);
}

//�޺��ڽ� �Ҽ���
void CGCS::OnCbnSelchangeGcsCbxdot(){
	UpdateData(TRUE);
	cbxInt = m_cbxDot.GetCurSel();
	if(cbxInt == -1)
		cbxInt=0;
	cbxInt_Str.Format(_T("%d"), cbxInt);
	iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("cbxDot"), cbxInt_Str);
	UpdateData(FALSE);
}

//���� ����
void CGCS::OnEnUpdateGcsEdtdist(){
	UpdateData(TRUE);
	m_Dist = _tstof(m_DistStr);
	UpdateData(FALSE);
}

////üũ�ڽ� ���� ���� ���鿡�� ����
void CGCS::OnBnClickedGcsCheck1(){
	b_Chk1 = IsDlgButtonChecked(IDC_GCS_CHECK1);
	if(b_Chk1 == 0){
		m_Chk1_btn.SetCheck(FALSE);
		Chk_ApptStr = _T("0");
		GetDlgItem(IDC_GCS_STATIC_REFROAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_GCS_STATIC_CORROAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_GCS_EDTREFROAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_GCS_EDTCORROAD)->EnableWindow(TRUE);
	}
	else{
		m_Chk1_btn.SetCheck(TRUE);
		Chk_ApptStr = _T("1");
		GetDlgItem(IDC_GCS_STATIC_REFROAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_GCS_STATIC_CORROAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_GCS_EDTREFROAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_GCS_EDTCORROAD)->EnableWindow(FALSE);
	}
	iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("Chk_Appt"), Chk_ApptStr);
}

//���� ����
void CGCS::OnEnUpdateGcsEdtrefroad(){
	UpdateData(TRUE);
	m_Refroad = _tstof(m_RefroadStr);
	UpdateData(FALSE);
}

//���� ����
void CGCS::OnEnUpdateGcsEdtcorroad(){
	UpdateData(TRUE);
	m_Corroad = _tstof(m_CorroadStr);
	UpdateData(FALSE);
}

//üũ�ڽ� �̰� �Ÿ� ǥ��
void CGCS::OnBnClickedGcsCheck2(){
	b_Chk2 = IsDlgButtonChecked(IDC_GCS_CHECK2);
	if(b_Chk2 == 0){
		m_Chk2_btn.SetCheck(FALSE);
		Chk_NotationStr = _T("0");
	}
	else{
		m_Chk2_btn.SetCheck(TRUE);
		Chk_NotationStr = _T("1");
	}
	iniFile.writeINI(_T("config.ini"), _T("GCS"), _T("Chk_Notation"), Chk_NotationStr);
}

////������ ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CGCS::DTR(double angle){
	return (angle / 180.0 * PI);
}

//���� ���μ��� ���� ����
bool CGCS::FindExcel(){
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	CString strFile;
	bool bExistExcel = false;
	if(Process32First(snapshot, &entry) == TRUE){
		while(Process32Next(snapshot, &entry) == TRUE){
			strFile=entry.szExeFile;
			if(strFile.CompareNoCase(_T("excel.exe")) == 0){
				ULONG idProc;
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ParentProcessID);

				bExistExcel = true;
				break;
			}
		}
	}
	CloseHandle(snapshot);
	return bExistExcel;

}
CString CGCS::CheckInputNumber(CString str, int Mode){
	TCHAR* getAtChar;
	CString strNum, strRe = _T(""), strTht = _T("");
	double Tht,Re, Result;

	for(int i = 0; i < str.GetLength(); i++){
		strNum = str.Mid(i,1);	
		getAtChar = (TCHAR*)(LPCTSTR)strNum;
		int ascii = __toascii(*getAtChar);
		if(((ascii >= 48 && ascii <= 57) || ascii == 46) || ascii == 43){//46(.) 43(+)
			if(ascii == 43){
				if(Mode == 1){
					strTht = str.Mid(0,i);
					strTht = strTht + _T("000");
					strRe.Empty();
				}
				else if(Mode == 2){
					strTht = str.Mid(0,i);
					Tht = _tstof(strTht);
					Tht = Tht * m_Dist;
					strTht.Format(_T("%f"), Tht);
					strRe.Empty();
				}
			}
			else{
				strRe.Append(getAtChar);
			}
		}
	}
	Tht = _tstof(strTht);
	Re = _tstof(strRe);
	Result = Tht + Re;
	strRe.Format(_T("%f"),Result);
	return strRe;
} 

//���� ����
AcDbObjectId CGCS::CreateLine(AcGePoint3d spt,AcGePoint3d ept){
	Acad::ErrorStatus es;
	AcDbObjectId pLineObjId;
	AcDbBlockTable *pBT;
	AcDbBlockTableRecord *pBTR;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();

	if(pDb == NULL)
		return pLineObjId;

	AcDbLine* pLine = new AcDbLine(spt,ept);
	pLine->setLinetype(_T("BYLAYER"));
	pLine->setLinetypeScale(0);
	pLine->setLayer(0);

	if(pDb->getBlockTable(pBT, AcDb::kForRead) == Acad::eOk){
		if(pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite) == Acad::eOk){
			es = pBTR->appendAcDbEntity(pLineObjId, pLine);
			pBTR->close();
			pLine->close();
		}
		else
			return pLineObjId;
		pBT->close();
	}
	else
		return pLineObjId;

	pLine->close();
	return pLineObjId;
}

void CGCS::EraseEntity(AcDbObjectId objId){
	AcDbEntityPointer pEnt(objId, AcDb::kForRead);
	pEnt->upgradeOpen();
	if(pEnt.openStatus() == Acad::eOk){
		if(pEnt->isKindOf(AcDbLine::desc())){
			AcDbLine* pLine = AcDbLine::cast(pEnt);
			pLine->erase();
			pLine->close();
		}
	}
}

void CGCS::OnBnClickedGcsHelp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	Help_Path.Format(_T("%s\\%s::/1. html/GCS.html"), Help_Path, helpFile);

	ShellExecute(NULL, _T("open"), _T("hh.exe"), Help_Path, NULL, SW_SHOW);
}
