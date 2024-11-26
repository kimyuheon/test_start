#pragma once
#include "afxwin.h"
#include "resource.h"
#include "IniFile.h"

// CCOA 대화 상자입니다.
class CCOA : public CAcUiDialog {
	DECLARE_DYNAMIC(CCOA)
public:
	CCOA(CWnd* pParent = NULL);
	virtual ~CCOA();
	enum {IDD = IDD_COA};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBcolor();
	afx_msg void OnBnClickedBselect();
	afx_msg void OnEnUpdateEdtscale();
	afx_msg void OnCbnSelchangeCbxlayer();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	CString CheckInputNumber(CString str);
	void SelectColorChange(int colorIndex);
	void LayerChangeGetColor(CString layerName);
		
	CIniFile iniFile;
	CEdit m_ScaleCtrl;
	CStatic m_ctrPic;
	CAcUiComboBox m_CbxLayerCtr;
	CAcUiSelectButton m_BtnSelect;

	LPCTSTR setLayer;
	COLORREF btnAfterClr;			//색상 정보 변수

	short setColorIdx;
	
	CString m_ScaleStr;
	afx_msg void OnBnClickedCoaHelp();
};