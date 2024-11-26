#pragma once
#include "IniFile.h"

class CSLL
{
public:
	CSLL(void);
	~CSLL(void);

	CIniFile iniFile;

	int m_HScale,m_VScale;
	double PerResult, RatioResult;
	CString m_HScale_Str, m_VScale_Str, PerResultStr, RatioResultStr;
	bool chkString, chkMinusString;

	void Main();
	CString CheckInputNumber(CString str);
	bool CreateLine(AcGePoint3d spt, AcGePoint3d ept);
	double RTD(double radian);
	double DTR(double angle);
};

