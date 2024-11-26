#pragma once
#include "afxwin.h"


// CCFM ��ȭ �����Դϴ�.

class CCFM : public CAcUiDialog
{
	DECLARE_DYNAMIC(CCFM)

public:
	CCFM(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCFM();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CFM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCorsslinesel();
	afx_msg void OnBnClickedCancel();
	// ��� ���̾�
	CComboBox m_btnLayer;
	CString m_strLayer;
	CAcUiSelectButton m_btnEntSel;
	// Ⱦ�� �߽� ���μ� ����
	CString m_strCrossVLineLen;
	// Ⱦ�� �ۼ� �Ÿ� �Ҽ���
	CString m_strCrossDstDecimalPoint;
	// Ⱦ�� �ۼ� ���� �Ҽ���
	CString m_strCrossHgtDecimalPoint;
	// ���ݰ� ���� ����
	CString m_strGroundLevel;
	// Ⱦ�� �ۼ� �� ���� �Է�
	BOOL m_bCheckStationInput;
	CString m_strCheckStation;
	// �������� �Ÿ� ���� ǥ��
	BOOL m_bCheckDstHgtDisp;
	// �������� �Ÿ� ���� ǥ�� - ����
	int m_nDispStandard;
	// Z���� 0�� ��, ���� ����
	BOOL m_bCheckExceptZeroZ;
	// �����Ǵ� �������� ����
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
