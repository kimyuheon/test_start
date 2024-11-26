#pragma once
#include "resource.h"
#include "Arx_Draw.h"
#include "Arx_Lib.h"
#include "afxwin.h"
#include "IniFile.h"

#define FLCNT 120
#define FLLAST 100
#define FLDIST 32.5

// CNSLO 대화 상자입니다.

class CNSLO : public CAcUiDialog{
	DECLARE_DYNAMIC(CNSLO)

public:
	CNSLO(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CNSLO();
	enum { IDD = IDD_NSLO };		// 대화 상자 데이터입니다.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedBcolor();
	afx_msg void OnBnClickedBselect();
	afx_msg void OnStnClickedSttbit1();
	afx_msg void OnStnClickedSttbit2();
	afx_msg void OnStnClickedSttbit3();
	afx_msg void OnEnUpdateEdtmaxdist();
	afx_msg void OnEnUpdateEdtautodist();
	afx_msg void OnBnClickedChkdrawline();
	afx_msg void OnBnClickedChkautodraw();	
	afx_msg void OnCbnSelchangeCbxlayer();
	afx_msg void OnBnClickedChkchangelayer();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	Acad::ErrorStatus MakeBlock(CString blkName);

	bool FindBlock(CString blkName);
	CString CheckInputNumber(CString str);	
	void SelectColorChange(int colorIndex);
	void LayerChangeGetColor(CString layerName);
	void DrawBlock(CString blkName, AcGePoint3d insertPt, double ang, AcGeScale3d blkScale);
	void DrawSlopeLine(AcDbBlockTableRecord* pBlockTableRecord, AcGePoint3d sp, AcGePoint3d ep);
	void DrawSlopePLine(AcDbBlockTableRecord* pBlockTableRecord, AcGePoint3d sp, AcGePoint3d ep);
		
	Acad::ErrorStatus es;

	AcDbObjectId blkId;
	CAcUiComboBox m_CbxLayerCtr;
	CAcUiSelectButton m_BtnSelect;
	
	int jobStat;
	int baseLineStat;

	BOOL m_LineDraw;
	BOOL m_AutoDraw;
	BOOL m_ChangeLayer;

	short setColorIdx;
	COLORREF btnAfterClr;		//색상 정보 변수

	LPCTSTR setLayer;
	CEdit m_EditMaxDist_Ctrl;
	CEdit m_EditAutoDraw_Ctrl;

	CString m_EditMaxDist_Str;
	CString m_EditAutoDraw_Str;
	
	CIniFile iniFile;

	double GetRadian(double degree);
	Acad::ErrorStatus setSysVar(LPCTSTR varName, int val);
	Acad::ErrorStatus setVar(LPCTSTR varName, const resbuf* newVal);
};
