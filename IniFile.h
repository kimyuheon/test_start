#pragma once
#define MAX_LENGTH	255
#include "Arx_Lib.h"


class CIniFile
{
public:
	CIniFile(void);
	~CIniFile(void);
	CString m_Path;

	BOOL OpenINI(CString strFile);
	CString readINI(CString strFile, CString strSubject, CString strName);
	BOOL writeINI(CString strFile, CString strSubject, CString strName, CString strProfile);

	CString GetRegistry(HKEY hKey, CString vname, CString& vvalue);
	CString GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue);
	CString GetRegistry(HKEY hKey, LPCTSTR strCompany, CString vname, CString& vvalue);
	void CreateFolder(CString csPath);
		
	//각도, 라디안 구하기
	double getDegree(double radian);
	double getRadian(double degree);

	//CString에서 숫자 점 체크
	bool CheckCStringDigit(CString str);
	CString CheckInputNumber(CString str);

};
