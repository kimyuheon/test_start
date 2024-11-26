#pragma once
#include "StdAfx.h"
#include "IniFile.h"

class CXY{

public:
	CXY(void);
	~CXY(void);

	void command();
	void createXY(ads_point p1, ads_point p2, ads_point p3);
	
	int modePrt;
	int pntCnt;
	double tsize;
	CIniFile iniFile;
};