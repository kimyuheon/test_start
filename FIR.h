#pragma once
class CFIR
{
public:
	CFIR(void);
	~CFIR(void);

	void main();
	Acad::ErrorStatus getSysVar(LPCTSTR varName, int& val);
	Acad::ErrorStatus setSysVar(LPCTSTR varName, int val);
	Acad::ErrorStatus setVar(LPCTSTR varName, const resbuf* newVal);
	double DTR(double angle);
	double RTD(double angle);
};

