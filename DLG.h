#pragma once
#include "resource.h"
#include "afxwin.h"
#include "IniFile.h"

// CDLG 대화 상자입니다.
class CDLG : public CAcUiDialog{
	DECLARE_DYNAMIC(CDLG)
public:
	CDLG(CWnd* pParent = NULL);
	virtual ~CDLG();
	enum{IDD = IDD_DLG};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnUpdateEdtVscale();
	afx_msg void OnBnClickedBtnColor();
	afx_msg void OnEnUpdateEdtGriddist();
	afx_msg void OnCbnSelchangeCbxLayer();
	afx_msg void OnBnClickedChkStandard();
	afx_msg void OnBnClickedChkContinuous();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	void SelectColorChange(int colorIndex);
	int LayerChangeGetColor(CString layerName);
	CString CheckInputNumber(CString str);
	
	CIniFile iniFile;
	CEdit m_GridColumnDistCtr;
	CAcUiComboBox m_layerCbx;
	CEdit m_LineScaleCtr;

	COLORREF btnAfterClr;	//색상 정보 변수
	CEdit m_VScaleCtr;
	CString m_VScaleStr;
	CString m_LineScaleStr;
	CString LinetypeStr;
	TCHAR* layername;

	bool dwgStandard;
	bool continueLine;
	
	int m_curColorIndex;	//색상 인덱스
	int colorIndex;			//layer 색상 인덱스

	CString m_GridColumnDistStr;
	
	afx_msg void OnEnUpdateEdtLinescale();
	afx_msg void OnBnClickedDlgHelp();
};