#pragma once
#include "StdAfx.h"
#include "IniFile.h"
#include "FEL.h"

class CFELCmd
{
public:
	CFELCmd();
	~CFELCmd();

	void LoadSetting();
	void SaveSetting();
	void command();
	CString CheckInputNumber(CString str);
	double DTR(double angle);
	double RTD(double radian);
	bool CreatePLine(AcGePoint3dArray ptArray, double LineWidth, CString LineType, double LineTypeScale, CString Layer);
	bool CreateText(AcGePoint3d pt, double Height, double Rotate, CString TxtStr, CString Layer, int mode);
	bool CreateSolid(AcGePoint3d Solpt1, AcGePoint3d Solpt2, AcGePoint3d Solpt3, CString Layer);
	CString curLayerStyle(AcDbObjectId a_layId);

	CIniFile iniFile;
	int m_lineStat;				// �۾���� 1.���μ�  2.���ü�	
	double m_Scale;				// ��ô
	CString m_ScaleStr;
	int m_Length;				// ���μ� ����
	int m_DotCnt;				// �Ҽ���
	CString m_PreFaceStr;		// �Ӹ���
	CString m_TextHeight_Str;	// ���� ����
	CString m_ELValue_str;		// EL ����

	CString m_LayerStr;			// ������


	CFEL dlg;
};

