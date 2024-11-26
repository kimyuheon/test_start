#pragma once
#include "IniFile.h"

class CDRL
{
public:
	CDRL(void);
	~CDRL(void);

	CIniFile iniFile;
	void MainDraw();
	bool CreatePLine(AcGePoint3dArray ptArray, CString Layer);
	double DTR(double angle);
	bool CreateLayer(CString Layer, int color);
};

