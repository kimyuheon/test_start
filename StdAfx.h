#pragma once

#pragma pack (push, 8)
#pragma warning(disable: 4786 4996)

#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//- Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#include <map>

#include <afxwin.h>				//- MFC core and standard components
#include <afxext.h>				//- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//- MFC OLE classes
#include <afxodlgs.h>			//- MFC OLE dialog classes
#include <afxdisp.h>			//- MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//- MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//- MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//- MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "arxHeaders.h"

#include "DocData.h" //- Your document specific data class holder
#include <afxcontrolbars.h>

extern AcApDataManager<CDocData> DocVars;

#include "resource.h"
#include "math.h"
#include <vector>

#include "resource.h"
#define PI 3.14159265358
#define ErrVal			10000000		// 10,000,000

struct AttBlock
{
	CString AttributeTag;
	CString strValue;
	AcDbObjectId blkID;
};

typedef AcArray<AttBlock> AttBlockArray;
typedef AcArray<AttBlockArray> AttBlockArrayArray;

#ifdef _2020 
	#define CADVer _T("2021")
#elif  _2021
	#define CADVer _T("2021")
#elif _2021M
	#define CADVer _T("2021")
#elif _2022
	#define CADVer _T("2022")
#endif

#ifdef _ACAD
	#include "acedCmdNF.h"
	#define acedCommand	acedCommandS
	#define ads_command	acedCommand
	extern void ads_regen();
	extern int acdbSetDbmod(class AcDbDatabase*, int);
	bool __cdecl ExtensionLoader_IsLoaded(ACHAR const* dllpath);
#elif _2025
	#include "acedCmdNF.h"
	#define acedCommand	zcedCommandS
	#define ads_command	zcedCommand
#endif

#ifdef _ZCAD
	void ads_regen();
#endif

extern ACHAR g_modulePath[_MAX_PATH];
extern CString g_CADLang;
extern HINSTANCE g_hCurrResource;
CString ReadStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey);
CString ChangeName(CString a_str, CString a_add);
CString LanguagetoName(CString Name, int iLang, bool bKorinCad = false);
CString GetHelpPath(CString a_cadlang, CString a_WORKS_lang, CString a_help);
CString ReadCADLang(int a_cadProd);
bool loadResourcedllChk();
CWnd* ModelessDlgHide(CWnd* pDlg);
void ModelessDlgShow(CWnd* pCad, CWnd* pDlg);

enum cad {
	ZWCAD = 0,
	AUTOCAD = 1,
	ZYXCAD = 2
};

enum LANG {
	ENG = 0,
	CHN = 1
};

#pragma pack (pop)