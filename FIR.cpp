#include "StdAfx.h"
#include "FIR.h"
#include "Arx_Math.h"


CFIR::CFIR(void)
{
}


CFIR::~CFIR(void)
{
}

void CFIR::main()
{
	int Old_Osmode;
	double dist_TL, Dimang, Ang_IA, dist_CL, dist_R,m_Sub, dist_SL;
	AcGePoint3d spt, ept, IPpt, CntrPt, virpt1, virpt2;
	AcGeVector3d vec1,vec2;
	CString temp, temp2, strMsg;

	getSysVar(_T("OSMODE"), Old_Osmode);
	setSysVar(_T("OSMODE"), 673);

	strMsg.LoadStringW(IDS_GEL_START_SEL); // "\n시작 점 선택 : "
	if(acedGetPoint(0, strMsg, asDblArray(spt))!=RTNORM){
		setSysVar(_T("OSMODE"), Old_Osmode);
		return;
	}	

	strMsg.LoadStringW(IDS_ID_PT_SEL); // "\nIP 점 선택 : "
	if(acedGetPoint(asDblArray(spt), strMsg, asDblArray(IPpt))!=RTNORM){
		setSysVar(_T("OSMODE"), Old_Osmode);
		return;
	}

	strMsg.LoadStringW(IDS_END_PT_SEL); // "\n종료 점 선택 : "
	if(acedGetPoint(asDblArray(IPpt), strMsg, asDblArray(ept))!=RTNORM){
		setSysVar(_T("OSMODE"), Old_Osmode);
		return;
	}

	strMsg.LoadStringW(IDS_CROSS_CEN_SEL); // "\n중심선이 지나갈 점 선택 : "
	if(acedGetPoint(asDblArray(IPpt), strMsg, asDblArray(CntrPt))!=RTNORM){
		setSysVar(_T("OSMODE"), Old_Osmode);
		return;
	}
	
	dist_TL = spt.distanceTo(IPpt);

	vec1 = IPpt - spt;
	vec2 = IPpt - ept;

	Dimang = vec1.angleTo(vec2);
	temp.Format(_T("%%.%df"),5);
	temp2.Format(temp, RTD(Dimang));
	Dimang = _tstof(temp2);
	Ang_IA =  180 - Dimang;
	dist_SL = IPpt.distanceTo(CntrPt);
	//m_Sub = tanh(Ang_IA/2);
	//dist_R = dist_TL / m_Sub;
	m_Sub = 1 / (cos(DTR(Ang_IA*0.5)));
	
	dist_R = dist_SL / (m_Sub-1);
	temp.Format(_T("%%.%df"),5);
	temp2.Format(temp,dist_R);
	dist_R = _tstof(temp2);
	dist_CL = 2*PI*dist_R * Ang_IA/360;
	
	
	temp.Format(_T("\nR = %%.%df, CL = %%.%df"), 5,5);
	acutPrintf(temp,dist_R,dist_CL);

	setSysVar(_T("OSMODE"),Old_Osmode);
}

Acad::ErrorStatus CFIR::getSysVar(LPCTSTR varName, int& val){
	resbuf rb;
	if(ads_getvar(varName, &rb) == RTNORM){
		ASSERT(rb.restype == RTSHORT);
		val = rb.resval.rint;
		return(Acad::eOk);
	}
	else
		return(Acad::eInvalidInput);
}

Acad::ErrorStatus CFIR::setSysVar(LPCTSTR varName, int val){
	ASSERT(varName != NULL);

	resbuf rb;
	rb.restype = RTSHORT;
	rb.resval.rint = (short)val;

	return(setVar(varName, &rb));
}

Acad::ErrorStatus CFIR::setVar(LPCTSTR varName, const resbuf* newVal){
	CString str;
	if (ads_setvar(varName, newVal) != RTNORM) {
		str.Format(_T("Could not set system variable \"%s\"."), varName);
		return(Acad::eInvalidInput);
	}
	else {
		return(Acad::eOk);
	}
}

////각도를 라디안 값으로 변환하여 리턴한다.
double CFIR::DTR(double angle)
{
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CFIR::RTD(double radian)
{
	return (radian * 180.0 / PI);
}