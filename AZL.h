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
	AcDbObjectId CreatePoint(AcGePoint3d pos);//������
	CString CheckInputNumber(CString str);
	CString ExtractAzimuth(CString str);
	AcDbObjectId CreateLine(AcGePoint3d spt, AcGePoint3d ept);//���λ���
	void highlight(AcDbObjectId objId);
};

