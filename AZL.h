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
	AcDbObjectId CreatePoint(AcGePoint3d pos);//점생성
	CString CheckInputNumber(CString str);
	CString ExtractAzimuth(CString str);
	AcDbObjectId CreateLine(AcGePoint3d spt, AcGePoint3d ept);//라인생성
	void highlight(AcDbObjectId objId);
};

