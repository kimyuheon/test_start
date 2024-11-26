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
	int m_lineStat;				// 작업모드 1.가로선  2.지시선	
	double m_Scale;				// 축척
	CString m_ScaleStr;
	int m_Length;				// 가로선 길이
	int m_DotCnt;				// 소수점
	CString m_PreFaceStr;		// 머리말
	CString m_TextHeight_Str;	// 글자 높이
	CString m_ELValue_str;		// EL 높이

	CString m_LayerStr;			// 도면층


	CFEL dlg;
};

