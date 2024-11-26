#pragma once
#include "resource.h"
#include "IniFile.h"
#include "afxwin.h"

// CXYB 대화 상자입니다.
class CXYB : public CAcUiDialog{
	DECLARE_DYNAMIC(CXYB)
public:
	CXYB(CWnd* pParent = NULL);
	virtual ~CXYB();
	enum {IDD = IDD_XYB};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedChkexplode();
	afx_msg void OnBnClickedChkinsertpoint();

	void InsertBlock(ads_point bp);
	AcDbObjectId AppendEntity(AcDbEntity* pEnt);
	
	CIniFile iniFile;

	CEdit m_xScaleCtrl;
	CEdit m_yScaleCtrl;

	CString blkName;
	CString m_xScaleStr;
	CString m_yScaleStr;
};