#pragma once
#include "IniFile.h"
#include "afxwin.h"

// CFE 대화 상자입니다.
class CFE : public CAcUiDialog {
	DECLARE_DYNAMIC(CFE)
public:
	CFE(CWnd* pParent = NULL);
	virtual ~CFE();
	enum {IDD = IDD_FE};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnUpdateFelayer();
	afx_msg void OnEnUpdateFescale();
	afx_msg void OnEnUpdateFeheight();
	afx_msg void OnEnUpdateFedecimal();
	afx_msg void OnEnUpdateFepreface();
	afx_msg void OnBnClickedRadiofeup();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedRadiofedown();
	afx_msg void OnBnClickedCheckfehpoint();
	afx_msg void OnBnClickedCheckfeleader();
	afx_msg void OnBnClickedCheckfehorizontal();
	
	void CreatePoint(AcGePoint3d pos, CString Layer);

	bool CreateLayer(CString Layer, int color);
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer);
	bool CreateText(AcGePoint3d pt, double Height, double Width, double Rotate, CString TxtStr, CString Layer, int mode);
	double DTR(double angle);
	CString CheckInputNumber(CString str);
	void CheckInputNumber(CEdit *cedit, bool minus);

	CIniFile iniFile;
	CEdit m_TextHeight_Ctrl;

	CAcUiSelectButton m_bSelect;
	CButton m_Chkpoint_btn, m_Horizontal_btn, m_HorizontalSTpt_btn, m_Leader_btn;
	
	bool b_Chkpoint, b_Horizontal, b_HorizontalSTpt, b_Leader;
	int DotCnt;
	int R_upNdown;
	
	CString m_ScaleStr;
	CString m_LayerStr;	
	CString m_DecimalStr;
	CString m_PreFaceStr;
	CString m_ELValue_Str;
	CString m_TxtHeightStr;
	CString m_CheckPoint_Str, m_Horizontal_Str, m_HorizontalSTpt_Str, m_Leader_Str; 
};