#pragma once
#include "resource.h"
#include "IniFile.h"

class CDLT{
public:
	CDLT(void);
	~CDLT(void);
private:
	int ds_clr;
public:
	CIniFile iniFile;
	AcDbObjectIdArray uparry;
	AcDbObjectIdArray downarry;
	Acad::ErrorStatus es;
	CString CADLinetypeStr;
	bool bUserInput;

	void DrainLineType();
	CString LineTypeLoad(TCHAR *Type, TCHAR *Mirror);
	Acad::ErrorStatus setVar(LPCTSTR varName, const resbuf* newVal);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, int& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, double& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, CString& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, AcGePoint2d& val);
	Acad::ErrorStatus getSysVar(LPCTSTR varName, AcGePoint3d& val);
	Acad::ErrorStatus setSysVar(LPCTSTR varName, int val);
	Acad::ErrorStatus setSysVar(LPCTSTR varName, double val);
	Acad::ErrorStatus setSysVar(LPCTSTR varName, const TCHAR* val);
	void arx_setObList(ads_name enn,int index,bool chk);
	void arx_getObList(ads_name& enn,int index,bool chk);
	AcGePoint3d lf_3DReturnList(ads_name ename,int type);
	void lf_Xdata(ads_name ename,resbuf*& pRb);
	int type_return(int val);
};