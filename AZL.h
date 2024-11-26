#pragma once

#include "IniFile.h"

class CAZL
{
public:
	CAZL(void);
	~CAZL(void);

	CIniFile iniFile;
	CString DstrRe, MstrRe, SstrRe;

	void MainDraw();
	AcDbObjectId CreatePoint(AcGePoint3d pos);//痢积己
	CString CheckInputNumber(CString str);
	CString ExtractAzimuth(CString str);
	AcDbObjectId CreateLine(AcGePoint3d spt, AcGePoint3d ept);//扼牢积己
	void highlight(AcDbObjectId objId);
};

