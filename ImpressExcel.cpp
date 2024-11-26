#include "StdAfx.h"
#include "CIM.h"
#include <tlhelp32.h>
#include "afxdialogex.h"
#include "CRange.h"
#include "CRanges.h"
#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"

void CCIM::ImpressExcel(){
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

	UpdateData(TRUE);

	int column1 = 0;
	int column2 = 1;
	int pointcnt;
	pointcnt = GetDlgItemInt(IDC_PCNT);
	CString cnt, strMsg;
	cnt.Format(_T("%%.%df"), pointcnt);

	// app 엑셀 오브젝트	
	//CWnd *pWnd;
	//pWnd = CWnd::FindWindow(NULL, _T("Excel"));			
	//if(!app.CreateDispatch(L"Excel.Application")){// 엑셀 실행
	//	AfxMessageBox(L"엑셀을 열 수 없습니다.");
	//	app.Quit();
	//}	

	CString path;
	GetDlgItem(IDC_PATHINPUT)->GetWindowText(path);
	IUnknown *pUnk = NULL;

	CoInitialize(NULL);

	CLSID clsid;
	HRESULT hrchk = CLSIDFromProgID(L"Excel.Application", &clsid); 
	if (FAILED(hrchk)) {
		strMsg.LoadStringW(IDS_SETUP_EXCEL); // 엑셀이 설치되어 있지 않습니다.
		AfxMessageBox(strMsg);
		return;
	}

	// 엑셀 실행 중인지 체크
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);

	//이미 실행중인 Excel 이 있는지 확인
	if(path.IsEmpty()){
		if(SUCCEEDED(hr)){
			hr = pUnk->QueryInterface(IID_IDispatch, (void **)&lpdisp);
			app.AttachDispatch(lpdisp);

			if (!lpdisp)AfxMessageBox(L"Fail");

			app.get_ActiveWindow();
			app.get_Visible();

			lpdisp=app.get_Workbooks();
			ASSERT(lpdisp);
			books.AttachDispatch(lpdisp);

			lpdisp = app.get_ActiveWorkbook();
			book.AttachDispatch(lpdisp);

			sheets.AttachDispatch(lpdisp);

			lpdisp = app.get_ActiveSheet();
			ASSERT(lpdisp);
			sheet.AttachDispatch(lpdisp);

			/*long lCnt=books.get_Count();
			CString str;
			lpdisp=books.get_Item(COleVariant((short)1));
			ASSERT(lpdisp);
			book.AttachDispatch(lpdisp);
			str=book.get_Name();*/
		} 
		else if (FAILED(hr))
		{
			if (!app.CreateDispatch(_T("Excel.Application"))) {
				strMsg.LoadStringW(IDS_FIND_EXCEL); // "엑셀을 찾을 수 없습니다."
				AfxMessageBox(strMsg);
				return;
			}
			app.get_ActiveWindow();
			app.put_Visible(TRUE);

			//Get workbooks
			lpdisp = app.get_Workbooks();
			ASSERT(lpDisp);
			books.AttachDispatch(lpdisp);

			//Get WorkBook
			lpdisp = books.Add(VOptional);	//새로운 엑셀 객체를 생성하였을 경우
			ASSERT(lpDisp);
			book.AttachDispatch(lpdisp);

			//Get workSheets
			lpdisp = book.get_Sheets();
			ASSERT(lpDisp);
			sheets.AttachDispatch(lpdisp);

			lpdisp = sheets.Add(VOptional, VOptional, COleVariant((short)(1)), VOptional); // sheet 1개 추가
			ASSERT(lpdisp);
			lpdisp = sheets.get_Item(COleVariant((short)(1)));
			ASSERT(lpDisp);
			sheet.AttachDispatch(lpdisp);
			sheet.put_Name(L"Sheet 1");
		}
		else{
			strMsg.LoadStringW(IDS_NOTOPEN_EXCEL); // "엑셀이 열리지 않습니다."
			AfxMessageBox(strMsg);
			app.Quit();
			return;
		}
	}
	else if(!path.IsEmpty()){
		if(!app.CreateDispatch(L"Excel.Application")){
			strMsg.LoadStringW(IDS_DONOTOPEN_EXCEL); // "엑셀을 열 수 없습니다."
			AfxMessageBox(strMsg);
			app.Quit();
			return;
		}
		lpdisp=app.get_Workbooks();
		ASSERT(lpdisp);
		books.AttachDispatch(lpdisp);

		if(path.IsEmpty()){
			strMsg.LoadStringW(IDS_NOTSET_PATH); // "경로를 지정하지 않았습니다"
			AfxMessageBox(strMsg);
			app.Quit();
			return;
		}

		lpdisp = books.Open(path, VFalse, VFalse, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional);
		ASSERT(lpdisp);
		book.AttachDispatch(lpdisp);
	}

	/*if(NULL != pUnk)
		pUnk->Release();*/

	/*lpdisp = book.get_Sheets();
	ASSERT(lpdisp);
	sheets.AttachDispatch(lpdisp);
	lpdisp = sheets.get_Item(COleVariant((short)1));
	ASSERT(lpdisp);
	sheet.AttachDispatch(lpdisp);*/

	//범위선택 대화상자 열기
	COleVariant getCell, selectSheet;     //범위 변수
	getCell.pdispVal = NULL;

	/*if(!app.get_Visible())
		app.put_Visible(TRUE);*/

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	if(getCell.pdispVal == NULL){		
		getCell = app.InputBox(IDStoCString(IDS_IMPORT_EXCEL_COORDS)/*_T("엑셀 좌표 가져오기"*/, 
			COleVariant(IDStoCString(IDS_EXCEL_TO_CAD)/*"엑셀표를 캐드로 가져오기"*/), selectSheet, VOptional, VOptional, VOptional, VOptional, COleVariant(short(8)));
		//범위 미지정 또는 종료
		if(getCell.pdispVal == NULL){
			AfxMessageBox(IDStoCString(IDS_SET_EXCEL_AREA)/*_T("범위를 지정하지 않았습니다.")*/);
			/* 김성곤 대리 수정(엑셀이 종료되는 현상 수정)
			app.put_Visible(false);
			app.put_DisplayAlerts(false);
			books.Close();
			book.Close(VFalse, VOptional, VOptional);
			app.Quit();
			*/
			getCell.Detach();
			sheet.ReleaseDispatch();
			sheets.ReleaseDispatch();
			book.ReleaseDispatch();
			books.ReleaseDispatch();
			//app.put_Visible(true);
			app.ReleaseDispatch();

			if (pUnk != NULL)
			{
				pUnk->Release();
			}

			::CoUninitialize();

			return;
		}
	}

	startrng.AttachDispatch(getCell.pdispVal);
	VARIANT ret = startrng.get_Value2();
	COleSafeArray sa(ret);

	long lNumRows;
	long lNumCols;
	sa.GetUBound(1, &lNumRows);
	sa.GetUBound(2, &lNumCols);

	if(lNumCols>4){
		AfxMessageBox(IDStoCString(IDS_EXCEL_LINE_CHECK)/*_T("행 수가 맞지 않습니다.(4행 이상)")*/);
		app.put_Visible(false);
		app.put_DisplayAlerts(false);
		books.Close();
		book.Close(VFalse, VOptional, VOptional);
		app.Quit();
		return;
	}

	app.put_WindowState(2);
	
	CoUninitialize();

	long index[2], index2[2], index3[2], index4[2];
	VARIANT val, val2, val3, val4, tmp_val;
	int r,c;

	bool changeXY = IsDlgButtonChecked(IDC_XYCHANGE);
	bool chkFirstTXT = IsDlgButtonChecked(IDC_FIRST_TXT);
	bool chk2dPoly = IsDlgButtonChecked(IDC_2DPOLY);
	bool chk3dPoly = IsDlgButtonChecked(IDC_3DPOLY);

	AcDb2dPolyline *P2dline = new AcDb2dPolyline;
	AcDb3dPolyline *P3dline = new AcDb3dPolyline;
	//AcDbPoint *Ppoint = new AcDbPoint;

	AcGePoint2d p2d;
	AcGePoint3d p3d;

	for(r=1;r<=lNumRows;r++){
		double x = NULL, y = NULL, z = 0.0;
		CString text;
		text.Empty();
		index[0]=r;
		index2[0]=r;
		index3[0]=r;
		index4[0]=r;
		if(lNumCols == 2){
			// 2개 선택 시 1번째 열 문자 실행 불가로 처리
			// X Y 는 꼭 존재해야 하는 상황
			if (chkFirstTXT){
				((CButton*)(GetDlgItem(IDC_FIRST_TXT)))->SetCheck(0);
				chkFirstTXT = 0;
			}
			index[1]=1;
			sa.GetElement(index, &val);
			index2[1]=2;
			sa.GetElement(index2, &val2);
			/*if(val.vt != VT_R8 || val2.vt != VT_R8)
				return;*/

			x = getVARIANT(val); 
			y = getVARIANT(val2);

			if(!changeXY)
				p3d.set(x, y, z);
			else if(changeXY)
				p3d.set(y, x, z);
			acdbUcs2Wcs(asDblArray(p3d),asDblArray(p3d),Adesk::kFalse); // 김성곤 대리 추가
			if(CConline.GetCheck()){
				if(chk2dPoly){
					AcDb2dVertex *P2dvert = new AcDb2dVertex;					
					P2dvert->setPosition(p3d);
					P2dvert->close();
					P2dline->appendVertex(P2dvert);
					P2dline->close();
				}
				else if(chk3dPoly){
					AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
					P3dvert->setPosition(p3d);
					P3dvert->close();
					P3dline->appendVertex(P3dvert);
					P3dline->close();
				}
			}
			if(BCutpoint.GetCheck()){
				bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
				bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
				bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
				if(chkPoint)
					appendPoint(p3d);
				else if(chkCircle)
					appendCircle(p3d);
				else if(chkBlock)
					appendBlock(p3d);
			}
			if(BZIns.GetCheck())
				appendZVal(p3d, cnt);
		}
		else if(lNumCols == 3){
			index[1]=1;
			sa.GetElement(index, &val);
			index2[1]=2;
			sa.GetElement(index2, &val2);
			index3[1]=3;
			sa.GetElement(index3, &val3);

			/*if(val.vt != VT_R8 || val2.vt != VT_R8){
				app.put_Visible(false);
				app.put_DisplayAlerts(false);
				books.Close();
				book.Close(VFalse, VOptional, VOptional);
				app.Quit();
				return;
			}
			if(val3.vt == VT_R8){
				if(BZChk)
					z = val3.dblVal;			
				else if(val3.vt == VT_BSTR)
					text = val3.bstrVal;
			}*/

			// 체크 시 문자, X, Y
			if (chkFirstTXT) {
				if (val.vt == VT_BSTR)
					text.Format(L"%s", val.bstrVal);
				else 
					text.Format(L"%g", getVARIANT(val));
				x = getVARIANT(val2);
				y = getVARIANT(val3);
				z = 0.0f;
			} 
			// 미 체크 시 X, Y, Z
			else {
				x = getVARIANT(val);
				y = getVARIANT(val2);

				if (BZChk)
					z = getVARIANT(val3);
				//else if (val3.vt == VT_BSTR)
				//	text.Format(L"%s", getVARIANT(val3));
			}
			
			/*x = val.dblVal;
			y = val2.dblVal;*/
			if(!changeXY)
				p3d.set(x, y, z);
			else if(changeXY)
				p3d.set(y, x, z);
			acdbUcs2Wcs(asDblArray(p3d),asDblArray(p3d),Adesk::kFalse); // 김성곤 대리 추가
			if(CConline.GetCheck()){
				if(chk2dPoly){
					AcDb2dVertex *P2dvert = new AcDb2dVertex;
					P2dvert->setPosition(p3d);
					P2dvert->close();
					P2dline->appendVertex(P2dvert);
					P2dline->close();
				}
				else if(chk3dPoly){
					AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
					P3dvert->setPosition(p3d);
					P3dvert->close();
					P3dline->appendVertex(P3dvert);
					P3dline->close();
				}
			}

			if(CText.GetCheck())
				appendText(text, p3d);
			if(BCutpoint.GetCheck()){
				bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
				bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
				bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
				if(chkPoint)
					appendPoint(p3d);
				else if(chkCircle)
					appendCircle(p3d);
				else if(chkBlock)
					appendBlock(p3d);
			}
			if(BZIns.GetCheck())
				appendZVal(p3d, cnt);
		}
		else if(lNumCols == 4){
			index[1]=1;
			sa.GetElement(index, &val); // 문자
			index2[1]=2;
			sa.GetElement(index2, &val2); // x값
			index3[1]=3;
			sa.GetElement(index3, &val3); // y값
			index4[1]=4;
			sa.GetElement(index4, &val4); // z값

			/*if(val2.vt != VT_R8 || val3.vt != VT_R8){
				app.put_Visible(false);
				app.put_DisplayAlerts(false);
				books.Close();			
				book.Close(VFalse, VOptional, VOptional);
				app.Quit();
				return;
			}
			if(val4.vt == VT_R8){
				if(BZChk)
					z = val4.dblVal;
				else if(val4.vt == VT_BSTR)
					text = val4.bstrVal;
			}
			if(val.vt == VT_BSTR && val4.vt != VT_BSTR)
				text = val.bstrVal;
			x = val2.dblVal;
			y = val3.dblVal;*/

			// 체크 시 : 문자 X Y Z
			if (chkFirstTXT) {
				if (val.vt == VT_BSTR)
					text.Format(L"%s", val.bstrVal);
				else
					text.Format(L"%g", getVARIANT(val));
				x = getVARIANT(val2);
				y = getVARIANT(val3);
				if (BZChk)
					z = getVARIANT(val4);
			}
			// 미체크 시 : X Y Z 문자 
			else {
				x = getVARIANT(val);
				y = getVARIANT(val2);
				if (BZChk)
					z = getVARIANT(val3);
				if (val4.vt == VT_BSTR)
					text.Format(L"%s", val4.bstrVal);
				else
					text.Format(L"%g", getVARIANT(val4));
			}

			/*if (BZChk)
				z = getVARIANT(val4);
			else if (val4.vt == VT_BSTR)
				text.Format(L"%s", getVARIANT(val4));
			if (val.vt == VT_BSTR)
				text = val.bstrVal;*/

			if(!changeXY)
				p3d.set(x, y, z);
			else if(changeXY)
				p3d.set(y, x, z);
			acdbUcs2Wcs(asDblArray(p3d),asDblArray(p3d),Adesk::kFalse); // 김성곤 대리 추가
			if(CConline.GetCheck()){
				if(chk2dPoly){
					AcDb2dVertex *P2dvert = new AcDb2dVertex;
					P2dvert->setPosition(p3d);
					P2dvert->close();
					P2dline->appendVertex(P2dvert);
					P2dline->close();
				}
				else if(chk3dPoly){
					AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
					P3dvert->setPosition(p3d);
					P3dvert->close();
					P3dline->appendVertex(P3dvert);
					P3dline->close();
				}
			}

			if(CText.GetCheck())
				appendText(text, p3d);
			if(BCutpoint.GetCheck()){
				bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
				bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
				bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
				if(chkPoint)
					appendPoint(p3d);
				else if(chkCircle)
					appendCircle(p3d);
				else if(chkBlock)
					appendBlock(p3d);
			}
			if(BZIns.GetCheck())
				appendZVal(p3d, cnt);
		}		
	}
	if(CConline.GetCheck()){
		if(chk2dPoly)
			append2dPoly(P2dline);
		else if(chk3dPoly)
			append3dPoly(P3dline);
	}

	if (P2dline != NULL) 
		P2dline->close();
	if (P3dline != NULL)
		P3dline->close();

	startrng.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.ReleaseDispatch();

	if (pUnk != NULL)
	{
		pUnk->Release();
	}
	::CoUninitialize();

	acedGetAcadDwgView()->SetFocus();
}

void CCIM::appendZVal(AcGePoint3d p3d, CString cnt){
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;

	AcDbText *pZVal = new AcDbText;
	CString tS;
	GetDlgItemText(IDC_TEXTSIZE4, tS);
	double TSize = _tstof(tS);
	int Col = GetDlgItemInt(IDC_ZVALCOLOR);
	CString ZValue;
	ZValue.Format(cnt, p3d.z);
	pZVal->setDatabaseDefaults();
	pZVal->setHeight(TSize);
	pZVal->setTextString(ZValue);
	pZVal->setLayer(ZValLayer);
	pZVal->setColorIndex(Col);
	CString XOffSet;
	CString YOffSet;
	GetDlgItemText(IDC_XOFF4, XOffSet);
	GetDlgItemText(IDC_YOFF4, YOffSet);
	double Xoff = _tstof(XOffSet);
	double Yoff = _tstof(YOffSet);
	if(Xoff == 0.0){
		pZVal->setHorizontalMode(AcDb::kTextCenter);
		pZVal->setAlignmentPoint(p3d);
	}	
	else if(Xoff > 0.0){
		pZVal->setHorizontalMode(AcDb::kTextLeft);
		AcGePoint3d pX3d;
		double X = p3d.x + Xoff, Y = p3d.y, Z = p3d.z;
		pX3d.set(X, Y, Z);
		pZVal->setAlignmentPoint(pX3d);
	}
	else if(Xoff < 0.0){
		pZVal->setHorizontalMode(AcDb::kTextRight);
		AcGePoint3d pX3d;
		double X = p3d.x + Xoff, Y = p3d.y, Z = p3d.z;
		pX3d.set(X, Y, Z);
		pZVal->setAlignmentPoint(pX3d);
	}
	if(Yoff == 0.0){
		pZVal->setVerticalMode(AcDb::kTextVertMid);
		pZVal->setAlignmentPoint(p3d);
	}
	else if(Yoff > 0.0){
		pZVal->setVerticalMode(AcDb::kTextBottom);
		AcGePoint3d pY3d;
		double X = p3d.x, Y = p3d.y + Yoff, Z = p3d.z;
		pY3d.set(X, Y, Z);
		pZVal->setAlignmentPoint(pY3d);
	}
	else if(Yoff < 0.0){
		pZVal->setVerticalMode(AcDb::kTextTop);
		AcGePoint3d pY3d;
		double X = p3d.x, Y = p3d.y + Yoff, Z = p3d.z;
		pY3d.set(X, Y, Z);
		pZVal->setAlignmentPoint(pY3d);
	}
	//pZVal->setDatabaseDefaults();
	pBTR->appendAcDbEntity(pZVal);
	pZVal->close();
	pBTR->close();
}

double CCIM::getVARIANT(VARIANT& val)
{
	double dVal;

	switch (val.vt)
	{
	case VT_EMPTY:
		dVal = NULL;
		break;
	case VT_NULL:
		dVal = 0.0;
		break;
	case VT_I4://int
		dVal = val.intVal;
		break;
	case VT_R8://double
		dVal = val.dblVal;
		break;
	case VT_BSTR:
		dVal = wcstod(val.bstrVal, NULL);
		break;

	default:
		break;
	}

	return dVal;
}

void CCIM::appendText(CString text, AcGePoint3d p3d){
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;

	AcDbText *pText = new AcDbText;
	CString tS;
	GetDlgItemText(IDC_TEXTSIZE, tS);
	double TSize = _tstof(tS);

	int col = GetDlgItemInt(IDC_TCOLOR);
	pText->setDatabaseDefaults();
	pText->setHeight(TSize);
	pText->setTextString(text);
	pText->setPosition(p3d);	
	pText->setLayer(TextLayer);
	pText->setColorIndex(col);
	CString XOffSet;
	CString YOffSet;
	GetDlgItemText(IDC_XOFF, XOffSet);
	GetDlgItemText(IDC_YOFF, YOffSet);
	double Xoff = _wtof(XOffSet);
	double Yoff = _wtof(YOffSet);
	if(Xoff == 0.0){
		pText->setHorizontalMode(AcDb::kTextCenter);
		pText->setAlignmentPoint(p3d);
	}
	if(Yoff == 0.0){
		pText->setVerticalMode(AcDb::kTextVertMid);
		pText->setAlignmentPoint(p3d);
	}
	if(Xoff > 0.0){
		pText->setHorizontalMode(AcDb::kTextLeft);
		AcGePoint3d pX3d;
		double X = p3d.x + Xoff, Y = p3d.y, Z = p3d.z;
		pX3d.set(X, Y, Z);
		pText->setAlignmentPoint(pX3d);
	}
	else if(Xoff < 0.0){
		pText->setHorizontalMode(AcDb::kTextRight);
		AcGePoint3d pX3d;
		double X = p3d.x + Xoff, Y = p3d.y, Z = p3d.z;
		pX3d.set(X, Y, Z);
		pText->setAlignmentPoint(pX3d);
	}
	if(Yoff > 0.0){
		pText->setVerticalMode(AcDb::kTextBottom);
		AcGePoint3d pY3d;
		double X = p3d.x, Y = p3d.y + Yoff, Z = p3d.z;
		pY3d.set(X, Y, Z);
		pText->setAlignmentPoint(pY3d);
	}
	else if(Yoff < 0.0){
		pText->setVerticalMode(AcDb::kTextTop);
		AcGePoint3d pY3d;
		double X = p3d.x, Y = p3d.y + Yoff, Z = p3d.z;
		pY3d.set(X, Y, Z);
		pText->setAlignmentPoint(pY3d);
	}
	pBTR->appendAcDbEntity(pText);
	pBTR->close();
	pText->close();
}

void CCIM::appendPoint(AcGePoint3d p3d){
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;

	CString size;
	GetDlgItemText(IDC_POINTSIZE, size);
	double pSize = _wtof(size);
	int Col = GetDlgItemInt(IDC_POINTPICCOLOR);

	AcDbPoint *Ppoint = new AcDbPoint;
	Ppoint->setPosition(p3d);
	Ppoint->setThickness(pSize);
	Ppoint->setLayer(PointLayer);	
	Ppoint->setColorIndex(PointColIndex);
	Ppoint->setDatabaseDefaults();	
	pBTR->appendAcDbEntity(Ppoint);
	Ppoint->close();
	pBTR->close();
}

void CCIM::append2dPoly(AcDb2dPolyline *p2dPoly){
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;

	int Col = GetDlgItemInt(IDC_CONLINEPICCOLOR);
	p2dPoly->setLayer(ConLineLayer);
	p2dPoly->setColorIndex(Col);
	pBTR->appendAcDbEntity(p2dPoly);
	p2dPoly->close();
	pBTR->close();
}

void CCIM::append3dPoly(AcDb3dPolyline *p3dPoly){
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;

	int Col = GetDlgItemInt(IDC_CONLINEPICCOLOR);
	p3dPoly->setLayer(ConLineLayer);
	p3dPoly->setColorIndex(Col);
	CString test;
	test.Format(_T("%d"), ConlineColIndex);
	pBTR->appendAcDbEntity(p3dPoly);
	p3dPoly->close();
	pBTR->close();
}

void CCIM::appendBlock(AcGePoint3d p3d){
	AcDbBlockTable *pBT = NULL;
	if(acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBT, AcDb::kForWrite) != Acad::eOk)
		return;	
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;
	AcDbBlockReference *pBR = new AcDbBlockReference();

	int Col = GetDlgItemInt(IDC_POINTPICCOLOR);
	CString name;
	CBName.GetLBText(CBName.GetCurSel(), name);
	if(pBT->has(name)){
		AcDbObjectId curId;
		pBT->getAt(name, curId);
		pBR->setDatabaseDefaults();
		pBR->setBlockTableRecord(curId);
		pBR->setPosition(p3d);
		CString scaleF;
		GetDlgItemText(IDC_SCALE, scaleF);
		double SC = _wtof(scaleF);
		AcGeScale3d Scale(SC);
		pBR->setScaleFactors(Scale);
		pBR->setLayer(PointLayer);
		pBR->setColorIndex(Col);
		pBTR->appendAcDbEntity(pBR);
	}
	pBR->close();
	pBTR->close();
	pBT->close();
}

void CCIM::appendCircle(AcGePoint3d p3d){
	AcDbBlockTableRecord *pBTR = NULL;
	if(acdbOpenObject(pBTR, acdbHostApplicationServices()->workingDatabase()->currentSpaceId(), AcDb::kForWrite) != Acad::eOk)
		return;

	AcDbCircle *pCircle = new AcDbCircle;
	CString radi;
	GetDlgItemText(IDC_DIAMETER, radi);
	double radius = _wtof(radi);

	int Col = GetDlgItemInt(IDC_POINTPICCOLOR);

	pCircle->setDatabaseDefaults();
	pCircle->setCenter(p3d);
	pCircle->setRadius(radius/2);
	pCircle->setLayer(PointLayer);
	pCircle->setColorIndex(Col);
	pBTR->appendAcDbEntity(pCircle);
	pCircle->close();
	pBTR->close();
}