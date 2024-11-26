#pragma once
#include "afxwin.h"


// CCFM 대화 상자입니다.

class CCFM : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCFM)

public:
	CCFM(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCFM();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CFM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCorsslinesel();
	afx_msg void OnBnClickedCancel();
	// 등고선 레이어
	CComboBox m_btnLayer;
	CString m_strLayer;
	CAcUiSelectButton m_btnEntSel;
	// 횡단 중심 세로선 길이
	CString m_strCrossVLineLen;
	// 횡단 작성 거리 소수점
	CString m_strCrossDstDecimalPoint;
	// 횡단 작성 높이 소수점
	CString m_strCrossHgtDecimalPoint;
	// 지반고 문자 높이
	CString m_strGroundLevel;
	// 횡단 작성 후 측점 입력
	BOOL m_bCheckStationInput;
	CString m_strCheckStation;
	// 절점마다 거리 높이 표시
	BOOL m_bCheckDstHgtDisp;
	// 절점마다 거리 높이 표시 - 기준
	int m_nDispStandard;
	// Z값이 0인 점, 선은 제외
	BOOL m_bCheckExceptZeroZ;
	// 교차되는 점까지만 추출
	BOOL m_bCheckOnlyCrossPt;
	afx_msg void OnDeltaposSpnCrossdstdecpt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpnCrosshgtdecpt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckdsthgtdisp();
private:
	AcDbObjectId m_idCrossLine;
	AcGePoint3d  m_sp, m_ep, m_ip;

	void DrawCrossSectionLine(AcDbObjectId crossEn, AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d);
public:
	afx_msg void OnBnClickedBtnselent();
};
