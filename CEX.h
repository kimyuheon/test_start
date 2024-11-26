#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "afxwin.h"
#include "afxdialogex.h"
#include "atlimage.h"
#include "IniFile.h"

// CCEX 대화 상자입니다.
class arcClass{
	public:
	AcGePoint3d start, end, center;
	CString arc;
};

class lineClass{
	public:
	AcGePoint3d start, end;
	CString line;
};

class polylineClass{
	public:
	AcGePoint3d start, end;
	CString polyline;
};

class donutClass{
	public:
	AcGePoint3d start, end;
	CString donut;
};

class pointClass{
	public:
	AcGePoint3d Point;
	CString point;
};

class blockClass{
	public:
	AcGePoint3d Insert;
	CString block;
};

class textClass{
	public:
	AcGePoint3d insert;
	CString text;
	CString conText;
};

class circleClass{
	public:
	AcGePoint3d insert;
	CString circle;
};

class CCEX : public CAcUiDialog{
	DECLARE_DYNAMIC(CCEX)
public:
	CCEX(CWnd* pParent = NULL);
	virtual ~CCEX();
	enum {IDD = IDD_CEX};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedText();
	afx_msg void OnBnClickedPlus();
	afx_msg void OnBnClickedPath();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedMinus();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadCad();
	afx_msg void OnBnClickedContext();
	afx_msg void OnBnClickedRadCivil();
	afx_msg void OnBnClickedTextfile();
	afx_msg void OnBnClickedChkarray();
	afx_msg void OnBnClickedExcelfile();
	
	void createExcel(AcDbObjectIdArray Arr);
	void createExcel(AcDbObjectIdArray Arr, int coordinateJob);

	void ExpressPoint(CString filter);
	void makeText(AcDbObjectIdArray arr);
	void makeText(AcDbObjectIdArray arr, int coordinate);
	BOOL KillProcess(CString _sProcessName);

	CStatic bitmap1;
	
	CButton CBlock;
	CButton CArc;
	CButton CChkArray;
	CButton CCircle;
	CButton CContext;
	CButton CDonut;
	CButton CExcept;
	CButton CLine;
	CButton CPath;
	CButton CPoint;
	CButton CPolyLine;
	CButton CText;
	CButton CZAxis;

	CEdit CPathInput;
	CEdit CPointEdit;
	CScrollBar CPointScr;

	CString Arc;
	CString Text;
	CString Circle;
	CString Line;
	CString Block;
	CString Polyline;
	CString Point;

	int selExTxt;
	int coordinate;

	CIniFile iniFile;
	afx_msg void OnBnClickedCexHelp();
};