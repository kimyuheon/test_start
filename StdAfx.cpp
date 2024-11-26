// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//------ StdAfx.cpp : source file that includes just the standard includes
//------  StdAfx.pch will be the pre-compiled header
//------  StdAfx.obj will contain the pre-compiled type information
//-----------------------------------------------------------------------------
#include "StdAfx.h"

CString g_CADLang;

CString ReadStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey)
{
	CString strValue = _T("");
	HKEY hSubKey = NULL; // open the key 
	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
	{
		DWORD buf_size = 0; // 문자열의 크기를 먼저 읽어온다. 
		if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, NULL, &buf_size) == ERROR_SUCCESS)
		{
			// 메모리 할당하고..., 
			TCHAR* pBuf = new TCHAR[buf_size + 1]; // 실제 값을 읽어온다. 
			if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, (LPBYTE)pBuf, &buf_size) == ERROR_SUCCESS)
			{
				pBuf[buf_size] = _T('\0');
				strValue = CString(pBuf);
			}
			// to avoid leakage 
			delete[] pBuf;
		} // 키 닫기 
		::RegCloseKey(hSubKey);
	} return strValue;
}

CString ChangeName(CString a_str, CString a_add)
{
	CString Name;

	int ichk = a_str.Find(L"_light");
	if (ichk != -1) { // _light 일 시
		CString tmp(a_str);
		int iFind = tmp.ReverseFind('_');
		tmp.Delete(iFind, tmp.GetLength() - 1);

		CString tmp1(a_str);
		tmp1.Delete(0, iFind);

		Name = tmp + a_add + tmp1;
	}
	else {
		CString tmp(a_str);
		int iFind = tmp.ReverseFind('.');
		tmp.Delete(iFind, tmp.GetLength() - 1);

		CString tmp1(a_str);
		tmp1.Delete(0, iFind);

		Name = tmp + a_add + tmp1;
	}

	return Name;
}

CString LanguagetoName(CString Name, int iLang, bool bKorinCad)
{
	CString rName(Name);

	switch (iLang) // 한글판이 아니면 WORKS로 변경
	{
	case LANG::ENG:
	{
		rName.Replace(L"ZDREAM", L"WORKS");
		if (bKorinCad == false)
			rName = L"\\en-US\\" + ChangeName(rName, L"_eng");
		break;
	}
	case LANG::CHN:
	{
		rName.Replace(L"ZDREAM", L"WORKS");
		if (bKorinCad == false)
			rName = L"\\zh-CN\\" + ChangeName(rName, L"_chn");
		break;
	}
	default:
		break;
	}

	return rName;
}

CString GetHelpPath(CString a_cadlang, CString a_WORKS_lang, CString a_help)
{
	CString helpStr = a_help;
	if (a_cadlang.CompareNoCase(L"ko-KR") != 0 &&
		a_cadlang.Find(L"412") == -1) { // ZWCAD/ZYXCAD 한글판이 아니면
		if (a_WORKS_lang.CompareNoCase(L"en-US") == 0) {
			//AfxMessageBox(L"영문판 확인!!!");
			//CAD : 한글판 X / Works : 영어
			helpStr = LanguagetoName(a_help, LANG::ENG);
		} else if (a_WORKS_lang.CompareNoCase(L"zh-CN") == 0) {
			//CAD : 한글판 X / Works : 중국어
			helpStr = LanguagetoName(a_help, LANG::CHN);
		} else if (a_WORKS_lang.CompareNoCase(L"ko-KR") == 0) {
			//CAD : 한글판 X / Works : 한국어
			helpStr = LanguagetoName(a_help, LANG::ENG, 1);
		}
	} else if (a_cadlang.Find(L"412") == -1 &&
				a_cadlang.Find(L"ko-KR") == -1) { // AUTOCAD 한글판이 아니면
		if (a_WORKS_lang.CompareNoCase(L"en-US") == 0) {
			//AfxMessageBox(L"영문판 확인!!!");
			//CAD : 한글판 X / Works : 영어
			helpStr = LanguagetoName(a_help, LANG::ENG);
		} else if (a_WORKS_lang.CompareNoCase(L"zh-CN") == 0) {
			//CAD : 한글판 X / Works : 중국어
			helpStr = LanguagetoName(a_help, LANG::CHN);
		} else if (a_WORKS_lang.CompareNoCase(L"ko-KR") == 0) {
			//CAD : 한글판 X / Works : 한국어
			helpStr = LanguagetoName(a_help, LANG::ENG, 1);
		}
	} else if (a_cadlang.CompareNoCase(L"ko-KR") == 0) { // ZWCAD/ZYXCAD 한글판
		return helpStr; // 한글판은 무조건 a_help 값 그대로
		/*
		if (a_WORKS_lang.CompareNoCase(L"en-US") == 0) {
			//AfxMessageBox(L"영문판 확인!!!");
			helpStr = LanguagetoName(a_help, LANG::ENG);
		}
		else if (a_WORKS_lang.CompareNoCase(L"zh-CN") == 0) {
			helpStr = LanguagetoName(a_help, LANG::CHN);
		}
		*/
	} else if (a_cadlang.Find(L"412") != -1) { // AUTOCAD 한글판
		helpStr.Replace(L"ZDREAM.chm", L"WORKS.chm");
		return helpStr; // 한글판은 무조건 a_help 값 그대로
		/*
		if (a_WORKS_lang.CompareNoCase(L"en-US") == 0) {
			//AfxMessageBox(L"영문판 확인!!!");
			helpStr = LanguagetoName(a_help, LANG::ENG);
		}
		else if (a_WORKS_lang.CompareNoCase(L"zh-CN") == 0) {
			helpStr = LanguagetoName(a_help, LANG::CHN);
		}
		*/
	}

	return helpStr;
}

CString ReadCADLang(int a_cadProd)
{
	CString Company;
	CString CurProd;
	CString CurVer;

	//Product
	//ACAD			SOFTWARE\Autodesk	CurProd
	//ZCAD/ZYXCAD	SOFTWARE\ZWSOFT		CurProduct
	if (a_cadProd == cad::ZWCAD) {
		Company = L"Software\\ZWSOFT";
		CurProd = L"CurProduct";
		CurVer = L"CurVer";
	}
	else if (a_cadProd == cad::AUTOCAD) {
		Company = L"Software\\Autodesk";
		CurProd = L"CurProd";
		CurVer = L"CurVer";
	}
	else if (a_cadProd == cad::ZYXCAD) {
		Company = L"Software\\ZYXTECHNOLOGY";
		CurProd = L"CurProduct";
		CurVer = L"CurVer";
	}

	CString t_path, t_str;
	t_path.Format(L"%s", Company);
	t_str = ReadStringValueInRegistry(HKEY_CURRENT_USER, t_path, CurProd);

	//Version
	//ACAD			SOFTWARE\Autodesk\AUTOCAD	CurVer
	//ZCAD/ZYXCAD	SOFTWARE\ZWSOFT\ZWCAD		CurVer
	t_path.Format(L"%s\\%s", t_path, t_str);
	t_str = ReadStringValueInRegistry(HKEY_CURRENT_USER, t_path, CurVer);

	//Language (Version과 항목이 같음)
	//ACAD			SOFTWARE\Autodesk\AUTOCAD\R24.3		CurVer
	//ZCAD/ZYXCAD	SOFTWARE\ZWSOFT\ZWCAD\2024			CurVer
	t_path.Format(L"%s\\%s", t_path, t_str);
	t_str = ReadStringValueInRegistry(HKEY_CURRENT_USER, t_path, CurVer);

	// AUTOCAD		: ACAD-7101:412 412:한국 409:영문 804:중문
	// ZWCAD/ZYXCAD	: ko-KR:한국 en-US:영어
	return t_str;
}

bool loadResourcedllChk()
{
	bool chk = false;
	CString dll_Path = g_modulePath;
	int findNUM = dll_Path.ReverseFind('\\');
	dll_Path.Delete(findNUM, dll_Path.GetLength() - 1);

	// HKEY_CURRENT_USER\SOFTWARE\ZDREAM Language 값 확인
	CString lang =
		ReadStringValueInRegistry(HKEY_CURRENT_USER, L"SOFTWARE\\WORKS", L"Language");

	if (g_hCurrResource)
		FreeLibrary(g_hCurrResource);

	// ko-KR en-US 
	if (lang.CompareNoCase(L"en-US") == 0) {
		CString dllfile = L"Civil_eng.dll";
		//CString dllfile = fileName;
		dll_Path.Format(_T("%s\\en-US\\%s"), dll_Path, dllfile);
		g_hCurrResource = LoadLibrary(dll_Path);
		if (g_hCurrResource)
			chk = true;
		else
			chk = false;
	}
	else {
		g_hCurrResource = NULL;
		chk = true;;
	}

	return chk;
}

CWnd* ModelessDlgHide(CWnd* pDlg)
{
	CWnd* pCadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pCadWnd->EnableWindow(TRUE);
	pDlg->ShowWindow(SW_HIDE);
	pCadWnd->SetFocus();

	return pCadWnd;
}

void ModelessDlgShow(CWnd* pCad, CWnd* pDlg)
{
	//pCad->EnableWindow(FALSE);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->EnableWindow(TRUE);
	pDlg->SetFocus();
}
