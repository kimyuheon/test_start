#include "stdafx.h"
#include "CEX.h"
#include "afxdialogex.h"
#include "CRange.h"
#include "CRanges.h"
#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include <TlHelp32.h>

void CCEX::createExcel(AcDbObjectIdArray arr){
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
	UpdateData(FALSE);

	int column1 = 0;
	int column2 = 1;
	int pointcnt;
	pointcnt = GetDlgItemInt(IDC_POINTEDIT);
	CString cnt, strMsg;
	cnt.Format(_T("%%.%df"), pointcnt);		

	// app ¿¢¼¿ ¿ÀºêÁ§Æ®
	if(!app.CreateDispatch(_T("Excel.Application"))){
		AfxMessageBox(_T("¿¢¼¿À» ¿­ ¼ö ¾ø½À´Ï´Ù."));
		app.Quit();
	}

	//Get workbooks
	lpdisp = app.get_Workbooks();
	ASSERT(lpdisp);
	books.AttachDispatch(lpdisp);

	//Get WorkBook
	lpdisp = books.Add(VOptional);	//»õ·Î¿î ¿¢¼¿ °´Ã¼¸¦ »ý¼ºÇÏ¿´À» °æ¿ì
	ASSERT(lpdisp);
	book.AttachDispatch(lpdisp);

	//Get workSheets
	lpdisp = book.get_Sheets();
	ASSERT(lpdisp);
	sheets.AttachDispatch(lpdisp);

	lpdisp = sheets.Add(VOptional, VOptional, COleVariant((short)(3)), VOptional); // sheet 3°³ Ãß°¡
	ASSERT(lpdisp);
	lpdisp = sheets.get_Item(COleVariant((short)(1)));
	ASSERT(lpdisp);
	sheet.AttachDispatch(lpdisp);
	sheet.put_Name(L"Sheet 1");
	lpdisp = sheets.get_Item(COleVariant((short)(2)));
	ASSERT(lpdisp);
	sheet.AttachDispatch(lpdisp);
	sheet.put_Name(L"Sheet 2");
	lpdisp = sheets.get_Item(COleVariant((short)(3)));
	ASSERT(lpdisp);
	sheet.AttachDispatch(lpdisp);
	sheet.put_Name(L"Sheet 3");

	//¹üÀ§¼±ÅÃ ´ëÈ­»óÀÚ ¿­±â
	COleVariant getCell, selectSheet;     //¹üÀ§ º¯¼ö
	getCell.pdispVal = NULL;

	if(!app.get_Visible())	app.put_Visible(TRUE);

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	if(getCell.pdispVal == NULL){		
		getCell = app.InputBox(IDStoCString(IDS_EXCEL_AREA_SEL)/*"¹üÀ§ ¼±ÅÃ"*/,
			COleVariant(IDStoCString(IDS_EXCEL_TO_CAD)/*"¿¢¼¿Ç¥¸¦ Ä³µå·Î °¡Á®¿À±â"*/), selectSheet, VOptional, VOptional, VOptional, VOptional, COleVariant(short(8)));
		//¹üÀ§ ¹ÌÁöÁ¤ ¶Ç´Â Á¾·á
		if(getCell.pdispVal == NULL){
			app.put_Visible(false);
			strMsg.LoadStringW(IDS_SET_EXCEL_AREA); // "¹üÀ§¸¦ ÁöÁ¤ÇÏÁö ¾Ê¾Ò½À´Ï´Ù."
			AfxMessageBox(strMsg);
			app.put_DisplayAlerts(false);
			
			getCell.Detach();
			sheet.ReleaseDispatch();
			sheets.ReleaseDispatch();
			book.ReleaseDispatch();
			books.ReleaseDispatch();
			app.ReleaseDispatch();

			book.Close(VFalse, VOptional, VOptional);
			app.Quit();

			return;
		}
	}

	startrng.AttachDispatch(getCell.pdispVal);

	AcGePoint3dArray chkArr;
	int j = 1;
	for(int i = 0; i<arr.length(); i++){
		AcDbEntityPointer pEnt(arr[i], AcDb::kForRead);
		Acad::ErrorStatus es=pEnt.openStatus();
		if(es!=Acad::eOk){
			//AfxMessageBox(_T("ºí·° ¿­±â ¿À·ù"));
			app.put_DisplayAlerts(false);
			book.Close(VFalse, VOptional, VOptional);

			sheet.ReleaseDispatch();
			sheets.ReleaseDispatch();
			book.ReleaseDispatch();
			books.ReleaseDispatch();
			app.Quit();
			app.ReleaseDispatch();
			return;
		}

		if(pEnt->isKindOf(AcDbArc::desc())){
			AcDbArc *pArc(AcDbArc::cast(pEnt));
			double startX, startY, startZ, endX, endY, endZ, centerX, centerY, centerZ;
			AcGePoint3d start, end, center;
			pArc->getStartPoint(start);
			center.set(pArc->center().x, pArc->center().y, pArc->center().z);
			pArc->getEndPoint(end);

			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			acdbWcs2Ucs(asDblArray(center),asDblArray(center),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			acdbWcs2Ucs(asDblArray(end),asDblArray(end),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			pArc->close();
			startX = start.x;
			startY = start.y;
			startZ = start.z;

			endX = end.x;
			endY = end.y;
			endZ = end.z;

			centerX = center.x;
			centerY = center.y;
			centerZ = center.z;

			CString startXStr;
			startXStr.Format(cnt, startX);			
			CString startYStr;
			startYStr.Format(cnt, startY);
			CString startZStr;
			startZStr.Format(cnt, startZ);
			CString cenXStr;
			cenXStr.Format(cnt, centerX);
			CString cenYStr;
			cenYStr.Format(cnt, centerY);
			CString cenZStr;
			cenZStr.Format(cnt, centerZ);
			CString endXStr;
			endXStr.Format(cnt, endX);
			CString endYStr;
			endYStr.Format(cnt, endY);
			CString endZStr;
			endZStr.Format(cnt, endZ);

			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(start);
					chkArr.append(end);
					chkArr.append(pArc->center());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));						
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}
				}
				else{
					if(chkArr.find(start) == -1){
						chkArr.append(start);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						}
						j++;
					}
					if(chkArr.find(end) == -1){
						chkArr.append(end);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						}
						j++;
					}				
					if(chkArr.find(pArc->center()) == -1){
						chkArr.append(pArc->center());
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));							
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						}
						j++;
					}
				}
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}
			}
		}	
		else if(pEnt->isKindOf(AcDbLine::desc())){
			AcDbLine *Line(AcDbLine::cast(pEnt));
			double startX, startY, startZ, endX, endY, endZ;
			AcGePoint3d start, end;

			Line->getStartPoint(start);
			Line->getEndPoint(end);

			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			acdbWcs2Ucs(asDblArray(end),asDblArray(end),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			startX = start.x;
			startY = start.y;
			startZ = start.z;
			endX = end.x;
			endY = end.y;
			endZ = end.z;
			Line->close();

			CString startXStr;
			startXStr.Format(cnt, startX);
			CString startYStr;
			startYStr.Format(cnt, startY);
			CString startZStr;
			startZStr.Format(cnt, startZ);
			CString endXStr;
			endXStr.Format(cnt, endX);
			CString endYStr;
			endYStr.Format(cnt, endY);
			CString endZStr;
			endZStr.Format(cnt, endZ);
			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(start);
					chkArr.append(end);
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						j++;
					}				
				}
				else{
					if(chkArr.find(start) == -1){
						chkArr.append(start);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
							j++;
						}
					}
					if(chkArr.find(end) == -1){
						chkArr.append(end);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
							j++;
						}
					}
				}
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
				}				
			}
		}
		else if(pEnt->isKindOf(AcDbCircle::desc())){
			AcDbCircle *Circle(AcDbCircle::cast(pEnt));
			AcGePoint3d center;
			double cenX, cenY, cenZ;
			center = Circle->center();

			acdbWcs2Ucs(asDblArray(center),asDblArray(center),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			cenX = center.x;
			cenY = center.y;
			cenZ = center.z;		

			CString cenXStr;
			cenXStr.Format(cnt, cenX);
			CString cenYStr;
			cenYStr.Format(cnt, cenY);
			CString cenZStr;
			cenZStr.Format(cnt, cenZ);
			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(center);
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}				
				}
				else if(chkArr.length() != 0){
					if(chkArr.find(center) == -1){
						chkArr.append(center);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
					}
				}				
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}				
			}
			Circle->close();
		}
		else if(pEnt->isKindOf(AcDbPolyline::desc())){
			AcDbPolyline *pline(AcDbPolyline::cast(pEnt));
			double startX, startY, startZ, cenX, cenY, cenZ, endX, endY, endZ;
			AcGePoint3d start, ver, cen;
			pline->getStartPoint(start);

			if(pline->hasBulges() && pline->numVerts() == 2 && CDonut.GetCheck()){							
				double bulge, bulge2;
				pline->getBulgeAt(0, bulge);
				pline->getBulgeAt(1, bulge2);
				if(bulge == bulge2 == 1.0){
					pline->getPointAt(1, ver);
					acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
					AcGeVector3d v1;
					v1 = (ver - start).normal();
					double dist = ver.distanceTo(start)/2;
					cen = start + v1 * dist;
					if(CExcept.GetCheck()){
						if(chkArr.length() == 0){
							chkArr.append(cen);
							CString cenXStr;
							cenXStr.Format(cnt, cen.x);
							CString cenYStr;
							cenYStr.Format(cnt, cen.y);
							CString cenZStr;
							cenZStr.Format(cnt, cen.z);
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
								j++;
							}
						}
						else if(chkArr.length() != 0){
							if(chkArr.find(cen) == -1){
								chkArr.append(cen);
								CString cenXStr;
								cenXStr.Format(cnt, cen.x);
								CString cenYStr;
								cenYStr.Format(cnt, cen.y);
								CString cenZStr;
								cenZStr.Format(cnt, cen.z);
								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
									j++;
								}
							}
						}
					}
					else if(!CExcept.GetCheck()){						
						CString cenXStr;
						cenXStr.Format(cnt, cen.x);
						CString cenYStr;
						cenYStr.Format(cnt, cen.y);
						CString cenZStr;
						cenZStr.Format(cnt, cen.z);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
					}
				}
				else if(CPolyLine.GetCheck()){					
					int vertcnt = pline->numVerts();
					for(int i=0; i<vertcnt; i++){						
						pline->getPointAt(i, ver);
						acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
						if(CExcept.GetCheck()){
							if(chkArr.length() == 0){
								chkArr.append(ver);
								CString verX;
								verX.Format(cnt, ver.x);
								CString verY;
								verY.Format(cnt, ver.y);
								CString verZ;
								verZ.Format(cnt, ver.z);
								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}								
							}
							else if(chkArr.length() != 0){
								if(chkArr.find(ver) == -1){
									chkArr.append(ver);
									CString verX;
									verX.Format(cnt, ver.x);
									CString verY;
									verY.Format(cnt, ver.y);
									CString verZ;
									verZ.Format(cnt, ver.z);
									if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}								
								}
							}
						}
						else{
							CString verX;
							verX.Format(cnt, ver.x);
							CString verY;
							verY.Format(cnt, ver.y);
							CString verZ;
							verZ.Format(cnt, ver.z);
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}								
						}
					}					
				}
			}
			else if(CPolyLine.GetCheck()){
				int vertcnt = pline->numVerts();
				for(int i=0; i<vertcnt; i++){
					int vertcnt = pline->numVerts();
					for(int i=0; i<vertcnt; i++){						
						pline->getPointAt(i, ver);
						acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
						if(CExcept.GetCheck()){
							if(chkArr.length() == 0){
								chkArr.append(ver);
								CString verX;
								verX.Format(cnt, ver.x);
								CString verY;
								verY.Format(cnt, ver.y);
								CString verZ;
								verZ.Format(cnt, ver.z);
								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}								
							}
							else if(chkArr.length() != 0){
								if(chkArr.find(ver) == -1){
									chkArr.append(ver);
									CString verX;
									verX.Format(cnt, ver.x);
									CString verY;
									verY.Format(cnt, ver.y);
									CString verZ;
									verZ.Format(cnt, ver.z);
									if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}								
								}
							}
						}
						else{
							CString verX;
							verX.Format(cnt, ver.x);
							CString verY;
							verY.Format(cnt, ver.y);
							CString verZ;
							verZ.Format(cnt, ver.z);
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}								
						}
					}
				}
			}
		}
		else if(pEnt->isKindOf(AcDbPoint::desc())){
			AcDbPoint *Point(AcDbPoint::cast(pEnt));
			AcGePoint3d ver;
			ver.set(Point->position().x, Point->position().y, Point->position().z);
			acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			double pointX, pointY, pointZ;
			pointX = ver.x;
			pointY = ver.y;
			pointZ = ver.z;
			CString pX;
			pX.Format(cnt, pointX);
			CString pY;
			pY.Format(cnt, pointY);
			CString pZ;
			pZ.Format(cnt, pointZ);
			if(chkArr.length() == 0){
				chkArr.append(Point->position());
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));					
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
					j++;
				}
			}
			else{
				if(chkArr.find(Point->position()) == -1){
					chkArr.append(Point->position());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						j++;
					}
				}
			}
			Point->close();
		}
		else if(pEnt->isKindOf(AcDbBlockReference::desc())){
			AcDbBlockReference *pBR(AcDbBlockReference::cast(pEnt));			
			double pointX, pointY, pointZ;
			AcGePoint3d ver;
			ver.set(pBR->position().x, pBR->position().y, pBR->position().z);
			acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			pointX = ver.x;
			pointY = ver.y;
			pointZ = ver.z;
			CString pX;
			pX.Format(cnt, pointX);
			CString pY;
			pY.Format(cnt, pointY);
			CString pZ;
			pZ.Format(cnt, pointZ);

			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(pBR->position());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));					
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));					
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));					
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)3), COleVariant(pZ));					
					}
					j++;
				}
				else{
					if(chkArr.find(pBR->position()) == -1){
						chkArr.append(pBR->position());
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						}
						j++;
					}
				}
			}
			else {
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));	
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));	
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));	
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));	
				}
				j++;
			}
			pBR->close();
		}
		else if(pEnt->isKindOf(AcDbText::desc())){
			AcDbText *Text(AcDbText::cast(pEnt));
			double pointX, pointY, pointZ;

			AcGePoint3d ver;
			ver.set(Text->position().x, Text->position().y, Text->position().z);
			acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			pointX = ver.x;
			pointY = ver.y;
			pointZ = ver.z;
			CString pX;
			pX.Format(cnt, pointX);
			CString pY;
			pY.Format(cnt, pointY);
			CString pZ;
			pZ.Format(cnt, pointZ);
			CString conText;
			if(CContext.GetCheck())
				conText = Text->textString();

			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(Text->position());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
					}
					j++;
				}
				else if(chkArr.length() != 0) {
					if(chkArr.find(Text->position()) == -1){
						chkArr.append(Text->position());
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
						}
						j++;
					}
				}			
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
				}
				j++;
			}
			Text->close();
		}
	}

	if(selExTxt == 2){
		app.put_DisplayAlerts(false);
		CString path;
		GetDlgItem(IDC_PATHINPUT)->GetWindowText(path);
		book.SaveAs(COleVariant(path), VOptional, VOptional, VOptional, VOptional, VOptional, 1, VOptional, VOptional, VOptional, VOptional, VOptional);
		book.Close(VTrue, VOptional, VOptional);

		startrng.ReleaseDispatch();
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		app.Quit();
		app.ReleaseDispatch();
	}
}

void CCEX::createExcel(AcDbObjectIdArray arr, int coordinateJob){
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
	UpdateData(FALSE);

	int column1 = 0;
	int column2 = 1;
	int pointcnt;
	pointcnt = GetDlgItemInt(IDC_POINTEDIT);
	CString cnt, strMsg;
	cnt.Format(_T("%%.%df"), pointcnt);		

	CLSID clsid;
	IUnknown* pUnk = NULL;
	HRESULT hrchk = CLSIDFromProgID(L"Excel.Application", &clsid);
	if (FAILED(hrchk)) {
		strMsg.LoadStringW(IDS_SETUP_EXCEL); // "¿¢¼¿ÀÌ ¼³Ä¡µÇ¾î ÀÖÁö ¾Ê½À´Ï´Ù.."
		AfxMessageBox(strMsg);
		return;
	}

	// ¿¢¼¿ ½ÇÇà ÁßÀÎÁö Ã¼Å©
	HRESULT hr = GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
	bool bExcel = false;

	if (SUCCEEDED(hr)) {
		bExcel = true;
		hr = pUnk->QueryInterface(IID_IDispatch, (void**)&lpdisp);
		app.AttachDispatch(lpdisp);

		if (!lpdisp)AfxMessageBox(L"Fail");

		app.get_ActiveWindow();
		if (!app.get_Visible())
			app.put_Visible(TRUE);

		lpdisp = app.get_Workbooks();
		ASSERT(lpdisp);
		books.AttachDispatch(lpdisp);

		lpdisp = app.get_ActiveWorkbook();
		book.AttachDispatch(lpdisp);

		sheets.AttachDispatch(lpdisp);

		lpdisp = app.get_ActiveSheet();
		ASSERT(lpdisp);
		sheet.AttachDispatch(lpdisp);
	}

	if (FAILED(hr)){
		// app ¿¢¼¿ ¿ÀºêÁ§Æ®
		bExcel = false;
		if(!app.CreateDispatch(_T("Excel.Application"))){
			strMsg.LoadStringW(IDS_FIND_EXCEL); // "¿¢¼¿À» ¿­ ¼ö ¾ø½À´Ï´Ù."
			AfxMessageBox(strMsg);
			app.Quit();
		}
		app.get_ActiveWindow();
		if (!app.get_Visible())	
			app.put_Visible(TRUE);

		//Get workbooks
		lpdisp = app.get_Workbooks();
		ASSERT(lpdisp);
		books.AttachDispatch(lpdisp);

		//Get WorkBook
		lpdisp = books.Add(VOptional);	//»õ·Î¿î ¿¢¼¿ °´Ã¼¸¦ »ý¼ºÇÏ¿´À» °æ¿ì
		ASSERT(lpdisp);
		book.AttachDispatch(lpdisp);

		//Get workSheets
		lpdisp = book.get_Sheets();
		ASSERT(lpdisp);
		sheets.AttachDispatch(lpdisp);

		lpdisp = sheets.Add(VOptional, VOptional, COleVariant((short)(3)), VOptional); // sheet 3°³ Ãß°¡
		ASSERT(lpdisp);
		lpdisp = sheets.get_Item(COleVariant((short)(1)));
		ASSERT(lpdisp);
		sheet.AttachDispatch(lpdisp);
		sheet.put_Name(L"Sheet 1");
		lpdisp = sheets.get_Item(COleVariant((short)(2)));
		ASSERT(lpdisp);
		sheet.AttachDispatch(lpdisp);
		sheet.put_Name(L"Sheet 2");
		lpdisp = sheets.get_Item(COleVariant((short)(3)));
		ASSERT(lpdisp);
		sheet.AttachDispatch(lpdisp);
		sheet.put_Name(L"Sheet 3");
	}
	//¹üÀ§¼±ÅÃ ´ëÈ­»óÀÚ ¿­±â
	COleVariant getCell, selectSheet;     //¹üÀ§ º¯¼ö
	getCell.pdispVal = NULL;

	//if(!app.get_Visible())	app.put_Visible(TRUE);

	auto IDStoCString = [](int a_ID) -> CString {
		CString IDS;
		IDS.LoadStringW(a_ID);
		return IDS;
		};

	if(getCell.pdispVal == NULL){		
		getCell = app.InputBox(IDStoCString(IDS_EXCEL_AREA_SEL)/*"¹üÀ§ ¼±ÅÃ"*/,
			COleVariant(IDStoCString(IDS_EXPORT_EXCEL_COORDS)/*"¿¢¼¿·Î ÁÂÇ¥ ³»º¸³»±â*/), selectSheet, VOptional, VOptional, VOptional, VOptional, COleVariant(short(8)));
		//¹üÀ§ ¹ÌÁöÁ¤ ¶Ç´Â Á¾·á
		if(getCell.pdispVal == NULL){
			strMsg.LoadStringW(IDS_SET_EXCEL_AREA); // "¹üÀ§¸¦ ÁöÁ¤ÇÏÁö ¾Ê¾Ò½À´Ï´Ù."
			AfxMessageBox(strMsg);
			//app.put_DisplayAlerts(false);
			
			// ¹Ì¸® ¿¢¼¿ÀÌ ½ÇÇàµÇÁö ¾ÊÀ» ½Ã ¿¢¼¿ ²ô±â
			if (!bExcel) {
				book.Close(VFalse, VOptional, VOptional);
				books.Close();
				app.Quit();
			}

			getCell.Detach();
			sheet.ReleaseDispatch();
			sheets.ReleaseDispatch();
			book.ReleaseDispatch();
			books.ReleaseDispatch();
			//app.put_Visible(true);
			//app.Quit();
			app.ReleaseDispatch();

			if (pUnk != NULL)
				pUnk->Release();

			// ¹Ì¸® ¿¢¼¿ÀÌ ½ÇÇàµÇÁö ¾ÊÀ» ½Ã ¿¢¼¿ ÇÁ·Î¼¼½º Á¾·á
			if (!bExcel)
				KillProcess(L"EXCEL.EXE");

			return;
		}
	}

	startrng.AttachDispatch(getCell.pdispVal);

	AcGePoint3dArray chkArr;
	int j = 1;
	for(int i = 0; i<arr.length(); i++){
		AcDbEntityPointer pEnt(arr[i], AcDb::kForRead);
		Acad::ErrorStatus es=pEnt.openStatus();
		if(es!=Acad::eOk){
			//AfxMessageBox(_T("ºí·° ¿­±â ¿À·ù"));
			app.put_DisplayAlerts(false);
			book.Close(VFalse, VOptional, VOptional);

			sheet.ReleaseDispatch();
			sheets.ReleaseDispatch();
			book.ReleaseDispatch();
			books.ReleaseDispatch();
			app.Quit();
			app.ReleaseDispatch();
			KillProcess(L"EXCEL.EXE");
			return;
		}

		if(pEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc *pArc(AcDbArc::cast(pEnt));
			double startX, startY, startZ, endX, endY, endZ, centerX, centerY, centerZ;
			AcGePoint3d start, end, center;
			pArc->getStartPoint(start);
			center.set(pArc->center().x, pArc->center().y, pArc->center().z);
			pArc->getEndPoint(end);
			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			acdbWcs2Ucs(asDblArray(center),asDblArray(center),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			acdbWcs2Ucs(asDblArray(end),asDblArray(end),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			pArc->close();
			if(coordinateJob == 1){
				startX = start.y;
				startY = start.x;
				startZ = start.z;

				endX = end.y;
				endY = end.x;
				endZ = end.z;

				centerX = center.y;
				centerY = center.z;
				centerZ = center.z;
			}
			else{
				startX = start.x;
				startY = start.y;
				startZ = start.z;

				endX = end.x;
				endY = end.y;
				endZ = end.z;

				centerX = center.x;
				centerY = center.y;
				centerZ = center.z;
			}

			CString startXStr, startYStr, startZStr, cenXStr, cenYStr, cenZStr, endXStr, endYStr, endZStr;
			startXStr.Format(cnt, startX);
			startYStr.Format(cnt, startY);
			startZStr.Format(cnt, startZ);
			cenXStr.Format(cnt, centerX);
			cenYStr.Format(cnt, centerY);
			cenZStr.Format(cnt, centerZ);
			endXStr.Format(cnt, endX);
			endYStr.Format(cnt, endY);
			endZStr.Format(cnt, endZ);

			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(start);
					chkArr.append(end);
					chkArr.append(pArc->center());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));						
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}
				}
				else{
					if(chkArr.find(start) == -1){
						chkArr.append(start);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						}
						j++;
					}
					if(chkArr.find(end) == -1){
						chkArr.append(end);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						}
						j++;
					}				
					if(chkArr.find(pArc->center()) == -1){
						chkArr.append(pArc->center());
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));							
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						}
						j++;
					}
				}
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}
			}
		}	
		else if(pEnt->isKindOf(AcDbLine::desc())){
			AcDbLine *Line(AcDbLine::cast(pEnt));
			double startX, startY, startZ, endX, endY, endZ;
			AcGePoint3d start, end;

			Line->getStartPoint(start);
			Line->getEndPoint(end);

			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			acdbWcs2Ucs(asDblArray(end),asDblArray(end),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			if(coordinateJob == 1){
				startX = start.y;
				startY = start.x;
				startZ = start.z;
				endX = end.y;
				endY = end.x;
				endZ = end.z;
			}
			else{
				startX = start.x;
				startY = start.y;
				startZ = start.z;
				endX = end.x;
				endY = end.y;
				endZ = end.z;
			}
			Line->close();

			CString startXStr;
			startXStr.Format(cnt, startX);
			CString startYStr;
			startYStr.Format(cnt, startY);
			CString startZStr;
			startZStr.Format(cnt, startZ);
			CString endXStr;
			endXStr.Format(cnt, endX);
			CString endYStr;
			endYStr.Format(cnt, endY);
			CString endZStr;
			endZStr.Format(cnt, endZ);
			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(start);
					chkArr.append(end);
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
						j++;
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
						j++;
					}				
				}
				else{
					if(chkArr.find(start) == -1){
						chkArr.append(start);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
							j++;
						}
					}
					if(chkArr.find(end) == -1){
						chkArr.append(end);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
							j++;
						}
						else if(CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
							j++;
						}
					}
				}
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(endZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(startYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(startXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(startZStr));
					j++;
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(endYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(endXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(endZStr));
					j++;
				}				
			}
		}
		else if(pEnt->isKindOf(AcDbCircle::desc())){
			AcDbCircle *Circle(AcDbCircle::cast(pEnt));
			AcGePoint3d center;
			double cenX, cenY, cenZ;
			center = Circle->center();

			acdbWcs2Ucs(asDblArray(center),asDblArray(center),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			if(coordinateJob == 1){
				cenX = center.y;
				cenY = center.x;
				cenZ = center.z;
			}
			else{
				cenX = center.x;
				cenY = center.y;
				cenZ = center.z;
			}		

			CString cenXStr;
			cenXStr.Format(cnt, cenX);
			CString cenYStr;
			cenYStr.Format(cnt, cenY);
			CString cenZStr;
			cenZStr.Format(cnt, cenZ);
			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(center);
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
						j++;
					}				
				}
				else if(chkArr.length() != 0){
					if(chkArr.find(center) == -1){
						chkArr.append(center);
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
					}
				}				
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
					j++;
				}				
			}
			Circle->close();
		}
		else if(pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pline(AcDbPolyline::cast(pEnt));
			double startX, startY, startZ, cenX, cenY, cenZ, endX, endY, endZ;
			AcGePoint3d start, ver, cen;
			pline->getStartPoint(start);
			
			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡

			if(pline->hasBulges() && pline->numVerts() == 2 && CDonut.GetCheck())
			{							
				double bulge, bulge2;
				pline->getBulgeAt(0, bulge);
				pline->getBulgeAt(1, bulge2);
				if(bulge == bulge2 == 1.0)
				{
					pline->getPointAt(1, ver);
					acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
					AcGeVector3d v1;
					v1 = (ver - start).normal();
					double dist = ver.distanceTo(start)/2;
					cen = start + v1 * dist;
					if(CExcept.GetCheck()){
						if(chkArr.length() == 0){
							chkArr.append(cen);
							CString cenXStr, cenYStr, cenZStr;

							if(coordinateJob == 1){
								cenXStr.Format(cnt, cen.y);
								cenYStr.Format(cnt, cen.x);
								cenZStr.Format(cnt, cen.z);								
							}
							else{
								cenXStr.Format(cnt, cen.x);
								cenYStr.Format(cnt, cen.y);
								cenZStr.Format(cnt, cen.z);
							}

							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
								j++;
							}
						}
						else if(chkArr.length() != 0){
							if(chkArr.find(cen) == -1){
								chkArr.append(cen);
								CString cenXStr, cenYStr, cenZStr;

								if(coordinateJob == 1){
									cenXStr.Format(cnt, cen.y);
									cenYStr.Format(cnt, cen.x);
									cenZStr.Format(cnt, cen.z);								
								}
								else{
									cenXStr.Format(cnt, cen.x);
									cenYStr.Format(cnt, cen.y);
									cenZStr.Format(cnt, cen.z);
								}

								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
									j++;
								}
							}
						}
					}
					else if(!CExcept.GetCheck()){						
						CString cenXStr, cenYStr, cenZStr;
						if(coordinateJob == 1){
							cenXStr.Format(cnt, cen.y);
							cenYStr.Format(cnt, cen.x);
							cenZStr.Format(cnt, cen.z);								
						}
						else{
							cenXStr.Format(cnt, cen.x);
							cenYStr.Format(cnt, cen.y);
							cenZStr.Format(cnt, cen.z);
						}

						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenXStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenYStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(cenYStr));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(cenXStr));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(cenZStr));
							j++;
						}
					}
				}
				else if(CPolyLine.GetCheck()){					
					int vertcnt = pline->numVerts();
					for(int i=0; i<vertcnt; i++){						
						pline->getPointAt(i, ver);
						acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
						if(CExcept.GetCheck()){
							if(chkArr.length() == 0){
								chkArr.append(ver);
								CString verX, verY, verZ;

								if(coordinateJob == 1){
									verX.Format(cnt, ver.y);								
									verY.Format(cnt, ver.x);								
									verZ.Format(cnt, ver.z);								
								}
								else{
									verX.Format(cnt, ver.x);								
									verY.Format(cnt, ver.y);								
									verZ.Format(cnt, ver.z);
								}

								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}								
							}
							else if(chkArr.length() != 0){
								if(chkArr.find(ver) == -1){
									chkArr.append(ver);
									CString verX, verY, verZ;

									if(coordinateJob == 1){
										verX.Format(cnt, ver.y);								
										verY.Format(cnt, ver.x);								
										verZ.Format(cnt, ver.z);								
									}
									else{
										verX.Format(cnt, ver.x);								
										verY.Format(cnt, ver.y);								
										verZ.Format(cnt, ver.z);
									}
									if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}								
								}
							}
						}
						else{
							CString verX, verY, verZ;
							if(coordinateJob == 1){
								verX.Format(cnt, ver.y);								
								verY.Format(cnt, ver.x);								
								verZ.Format(cnt, ver.z);								
							}
							else{
								verX.Format(cnt, ver.x);								
								verY.Format(cnt, ver.y);								
								verZ.Format(cnt, ver.z);
							}
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}								
						}
					}					
				}
			}
			else if(CPolyLine.GetCheck())
			{
				int vertcnt = pline->numVerts();
				for(int i = 0; i < vertcnt; i++){
					int vertcnt = pline->numVerts();
					for(int i = 0; i < vertcnt; i++){
						pline->getPointAt(i, ver);
						acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
						if(CExcept.GetCheck()){
							if(chkArr.length() == 0){
								chkArr.append(ver);
								CString verX, verY, verZ;

								if(coordinateJob == 1){
									verX.Format(cnt, ver.y);
									verY.Format(cnt, ver.x);
									verZ.Format(cnt, ver.z);
								}
								else{
									verX.Format(cnt, ver.x);
									verY.Format(cnt, ver.y);
									verZ.Format(cnt, ver.z);
								}
								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
							}
							else if(chkArr.length() != 0){
								if(chkArr.find(ver) == -1){
									chkArr.append(ver);
									CString verX, verY, verZ;
									if(coordinateJob == 1){
										verX.Format(cnt, ver.y);
										verY.Format(cnt, ver.x);
										verZ.Format(cnt, ver.z);
									}
									else{
										verX.Format(cnt, ver.x);
										verY.Format(cnt, ver.y);
										verZ.Format(cnt, ver.z);
									}
									if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
										startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
										startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}
									else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
										startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
										startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
										if(CZAxis.GetCheck())
											startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
										j++;
									}
								}
							}
						}
						else{
							CString verX, verY, verZ;

							if(coordinateJob == 1){
								verX.Format(cnt, ver.y);
								verY.Format(cnt, ver.x);
								verZ.Format(cnt, ver.z);
							}
							else{
								verX.Format(cnt, ver.x);
								verY.Format(cnt, ver.y);
								verZ.Format(cnt, ver.z);
							}
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
						}
					}
				}
			}
		}		
		else if(pEnt->isKindOf(AcDb2dPolyline::desc()))
		{
			AcDb2dPolyline *pline(AcDb2dPolyline::cast(pEnt));
			double startX, startY, startZ, cenX, cenY, cenZ, endX, endY, endZ;
			AcGePoint3d start, ver, cen;
			pline->getStartPoint(start);

			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
						
			if(CPolyLine.GetCheck())
			{
				double vertcnt;
				AcGePoint3d EndPoint;
				pline->getEndPoint(EndPoint);
				pline->getParamAtPoint(EndPoint,vertcnt);
				for(int i = 0; i < vertcnt+1; i++)
				{
					pline->getPointAtParam(i, ver);
					acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
					if(CExcept.GetCheck()){
						if(chkArr.length() == 0){
							chkArr.append(ver);
							CString verX, verY, verZ;

							if(coordinateJob == 1){
								verX.Format(cnt, ver.y);
								verY.Format(cnt, ver.x);
								verZ.Format(cnt, ver.z);
							}
							else{
								verX.Format(cnt, ver.x);
								verY.Format(cnt, ver.y);
								verZ.Format(cnt, ver.z);
							}
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
						}
						else if(chkArr.length() != 0){
							if(chkArr.find(ver) == -1){
								chkArr.append(ver);
								CString verX, verY, verZ;
								if(coordinateJob == 1){
									verX.Format(cnt, ver.y);
									verY.Format(cnt, ver.x);
									verZ.Format(cnt, ver.z);
								}
								else{
									verX.Format(cnt, ver.x);
									verY.Format(cnt, ver.y);
									verZ.Format(cnt, ver.z);
								}
								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
							}
						}
					}
					else{
						CString verX, verY, verZ;

						if(coordinateJob == 1){
							verX.Format(cnt, ver.y);
							verY.Format(cnt, ver.x);
							verZ.Format(cnt, ver.z);
						}
						else{
							verX.Format(cnt, ver.x);
							verY.Format(cnt, ver.y);
							verZ.Format(cnt, ver.z);
						}
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
							j++;
						}
					}
				}
				
			}
		}
		else if(pEnt->isKindOf(AcDb3dPolyline::desc()))
		{
			AcDb3dPolyline *pline(AcDb3dPolyline::cast(pEnt));
			double startX, startY, startZ, cenX, cenY, cenZ, endX, endY, endZ;
			AcGePoint3d start, ver, cen;
			pline->getStartPoint(start);

			acdbWcs2Ucs(asDblArray(start),asDblArray(start),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
						
			if(CPolyLine.GetCheck())
			{
				double vertcnt;
				AcGePoint3d EndPoint;
				pline->getEndPoint(EndPoint);
				pline->getParamAtPoint(EndPoint,vertcnt);
				for(int i = 0; i < vertcnt+1; i++)
				{
					pline->getPointAtParam(i, ver);
					acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
					if(CExcept.GetCheck()){
						if(chkArr.length() == 0){
							chkArr.append(ver);
							CString verX, verY, verZ;

							if(coordinateJob == 1){
								verX.Format(cnt, ver.y);
								verY.Format(cnt, ver.x);
								verZ.Format(cnt, ver.z);
							}
							else{
								verX.Format(cnt, ver.x);
								verY.Format(cnt, ver.y);
								verZ.Format(cnt, ver.z);
							}
							if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
								startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
							else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
								startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
								startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
								if(CZAxis.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
								j++;
							}
						}
						else if(chkArr.length() != 0){
							if(chkArr.find(ver) == -1){
								chkArr.append(ver);
								CString verX, verY, verZ;
								if(coordinateJob == 1){
									verX.Format(cnt, ver.y);
									verY.Format(cnt, ver.x);
									verZ.Format(cnt, ver.z);
								}
								else{
									verX.Format(cnt, ver.x);
									verY.Format(cnt, ver.y);
									verZ.Format(cnt, ver.z);
								}
								if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
									startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
									startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
								else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
									startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
									startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
									if(CZAxis.GetCheck())
										startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
									j++;
								}
							}
						}
					}
					else{
						CString verX, verY, verZ;

						if(coordinateJob == 1){
							verX.Format(cnt, ver.y);
							verY.Format(cnt, ver.x);
							verZ.Format(cnt, ver.z);
						}
						else{
							verX.Format(cnt, ver.x);
							verY.Format(cnt, ver.y);
							verZ.Format(cnt, ver.z);
						}
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verX));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verY));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(verY));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(verX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(verZ));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verX));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verY));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
							j++;
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(verY));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(verX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(verZ));
							j++;
						}
					}
				}
				
			}
		}
		else if(pEnt->isKindOf(AcDbPoint::desc()))
		{
			AcDbPoint *Point(AcDbPoint::cast(pEnt));
			double pointX, pointY, pointZ;
			AcGePoint3d ver;
			ver.set(Point->position().x, Point->position().y, Point->position().z);
			acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			if(coordinateJob == 1){
				pointX = ver.y;
				pointY = ver.x;
				pointZ = ver.z;
			}
			else{
				pointX = ver.x;
				pointY = ver.y;
				pointZ = ver.z;
			}

			CString pX, pY, pZ;
			pX.Format(cnt, pointX);
			pY.Format(cnt, pointY);
			pZ.Format(cnt, pointZ);
			if(chkArr.length() == 0){
				chkArr.append(Point->position());
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
					j++;
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
					j++;
				}
			}
			else{
				if(chkArr.find(Point->position()) == -1){
					chkArr.append(Point->position());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						j++;
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						j++;
					}
				}
			}
			Point->close();
		}
		
		else if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference *pBR(AcDbBlockReference::cast(pEnt));
			AcGePoint3d ver;
			ver.set(pBR->position().x, pBR->position().y, pBR->position().z);
			acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			double pointX, pointY, pointZ;
			if(coordinateJob == 1){
				pointX = ver.y;
				pointY = ver.x;
				pointZ = ver.z;
			}
			else{
				pointX = ver.x;
				pointY = ver.y;
				pointZ = ver.z;
			}
			CString pX;
			pX.Format(cnt, pointX);
			CString pY;
			pY.Format(cnt, pointY);
			CString pZ;
			pZ.Format(cnt, pointZ);

			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(pBR->position());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)3), COleVariant(pZ));
					}
					j++;
				}
				else{
					if(chkArr.find(pBR->position()) == -1){
						chkArr.append(pBR->position());
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));		
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
							if(CZAxis.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						}
						j++;
					}
				}
			}
			else {
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
					if(CZAxis.GetCheck())
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
				}
				j++;
			}
			pBR->close();
		}
		else if(pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText *Text(AcDbText::cast(pEnt));
			AcGePoint3d ver;
			ver.set(Text->position().x, Text->position().y, Text->position().z);
			acdbWcs2Ucs(asDblArray(ver),asDblArray(ver),Adesk::kFalse); // ±è¼º°ï ´ë¸® Ãß°¡
			double pointX, pointY, pointZ;
			if(coordinateJob == 1){
				pointX = ver.y;
				pointY = ver.x;
				pointZ = ver.z;
			}
			else{
				pointX = ver.x;
				pointY = ver.y;
				pointZ = ver.z;
			}
			CString pX;
			pX.Format(cnt, pointX);
			CString pY;
			pY.Format(cnt, pointY);
			CString pZ;
			pZ.Format(cnt, pointZ);
			CString conText;
			if(CContext.GetCheck())
				conText = Text->textString();

			if(CExcept.GetCheck()){
				if(chkArr.length() == 0){
					chkArr.append(Text->position());
					if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
						startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
					}
					else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
						startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
						startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
						if(CZAxis.GetCheck()){
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
						}
						else if(!CZAxis.GetCheck())
							if(CContext.GetCheck())
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
					}
					j++;
				}
				else if(chkArr.length() != 0) {
					if(chkArr.find(Text->position()) == -1){
						chkArr.append(Text->position());
						if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
							startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
							startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
						}
						else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
							startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
							startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
							if(CZAxis.GetCheck()){
								startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
							}
							else if(!CZAxis.GetCheck())
								if(CContext.GetCheck())
									startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
						}
						j++;
					}
				}
			}
			else{
				if(!CChkArray.GetCheck() || ((CButton*)GetDlgItem(IDC_RIGHT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pX));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pY));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_LEFT))->GetCheck()){
					startrng.put_Item(COleVariant((long)j), COleVariant((long)1), COleVariant(pY));
					startrng.put_Item(COleVariant((long)j), COleVariant((long)2), COleVariant(pX));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)4), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)j), COleVariant((long)3), COleVariant(conText));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_DOWN))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pX));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pY));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
				}
				else if(CChkArray.GetCheck() && ((CButton*)GetDlgItem(IDC_UP))->GetCheck()){
					startrng.put_Item(COleVariant((long)1), COleVariant((long)j), COleVariant(pY));
					startrng.put_Item(COleVariant((long)2), COleVariant((long)j), COleVariant(pX));
					if(CZAxis.GetCheck()){
						startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(pZ));
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)4), COleVariant((long)j), COleVariant(conText));
					}
					else if(!CZAxis.GetCheck())
						if(CContext.GetCheck())
							startrng.put_Item(COleVariant((long)3), COleVariant((long)j), COleVariant(conText));
				}
				j++;
			}
			Text->close();
		}
	}

	if (selExTxt == 1) {
		startrng.ReleaseDispatch();
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		app.ReleaseDispatch();
	}

	if(selExTxt == 2){
		app.put_DisplayAlerts(false);
		CString path;
		GetDlgItem(IDC_PATHINPUT)->GetWindowText(path);
		book.SaveAs(COleVariant(path), VOptional, VOptional, VOptional, VOptional, VOptional, 1, VOptional, VOptional, VOptional, VOptional, VOptional);
		book.Close(VTrue, VOptional, VOptional);
		startrng.ReleaseDispatch();
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		app.Quit();
		app.ReleaseDispatch();
	}

	if (pUnk != NULL)
	{
		pUnk->Release();
	}
}

BOOL CCEX::KillProcess(CString _sProcessName)
{
	HANDLE         hProcessSnap = NULL;
	DWORD          Return = FALSE;
	PROCESSENTRY32 pe32 = { 0 };

	CString ProcessName = _sProcessName;
	ProcessName.MakeLower();


	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return (DWORD)INVALID_HANDLE_VALUE;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32))
	{
		DWORD Code = 0;
		DWORD dwPriorityClass;

		do {
			HANDLE hProcess;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			dwPriorityClass = GetPriorityClass(hProcess);

			CString Temp = pe32.szExeFile;
			Temp.MakeLower();

			if (Temp == ProcessName)
			{
				if (TerminateProcess(hProcess, 0))
					GetExitCodeProcess(hProcess, &Code);
				else
					return GetLastError();
			}
			CloseHandle(hProcess);
		} while (Process32Next(hProcessSnap, &pe32));
		Return = TRUE;
	}
	else
	{
		Return = FALSE;
	}

	CloseHandle(hProcessSnap);

	return Return;
}