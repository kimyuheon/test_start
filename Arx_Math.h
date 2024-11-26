// Arx_Math.h: interface for the CArx_Math class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARX_MATH_H__32305844_022D_11D4_B03C_00A0242716EF__INCLUDED_)
#define AFX_ARX_MATH_H__32305844_022D_11D4_B03C_00A0242716EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "stdafx.h"


// ���Ȱ����� ���� ��� ��ũ�� �Լ�
#define	RAD_ANG(x)	(x*3.1415926535897932384626433832795/180)
#define	ANGLE(x)		(x*3.1415926535897932384626433832795/180)
// ���Ȱ� = pi*(����/180)
// 0,90,180,270,360���� ���� ���� ��
#define	ANGLE0		0
#define	ANGLE45		0.78539816339744830961566084581988
#define	ANGLE90		1.5707963267948966192313216916398
#define	ANGLE135		2.3561944901923449288469825374596
#define	ANGLE180		3.1415926535897932384626433832795
#define	ANGLE225		3.9269908169872415480783042290994
#define	ANGLE270		4.7123889803846898576939650749193
#define	ANGLE315		5.4977871437821381673096259207391
#define	ANGLE360		6.283185307179586476925286766559





class CArx_Math : public AcRxObject//CComeCommon  
{
public:
	CArx_Math();
	virtual ~CArx_Math();

private:
	
	Acad::ErrorStatus es;

public:
	double uds_Aangle(double startAng, double endAng);
	double uds_3ptAngle(ads_point pc, ads_point ps, ads_point pe);
	double uds_3ptAngle(AcGePoint3d pc, AcGePoint3d ps, AcGePoint3d pe);
	void	getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt);
	BOOL arx_coline(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3);
	int arx_hand(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3);
	int lr_hand(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3);
	int ads_lr_hand(ads_point p1, ads_point p2, ads_point p3);
	int fb_hand(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3);

	double uds_btwang(AcGePoint3d pc, AcGePoint3d ps, AcGePoint3d pe);
	double arx_angle(AcGePoint3d p0, AcGePoint3d pt);
	double arx_angleA(AcGePoint3d st_pt,AcGePoint3d ed_pt);
	BOOL arx_eqPoint(AcGePoint3d p1, AcGePoint3d p2);
	BOOL arx_perp(AcGePoint3d& p1, AcGePoint3d& p2, AcGePoint3d& p3);
	double RTD(double radian);
	double DTR(double angle);
	void	polar(AcGePoint3d sp, AcGePoint3d ep, double dst, AcGePoint3d& rPnt);
	void	Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result);
	// 20091028 ����ȣ �߰�  Polar�� ���� result�� �������� �����߰��� polar�� acutPolar �Լ��� ����Ͽ� ���Ѵ�  Polar�� �ణ ������ �ִ°Ͱ��Ƽ� ���� �������
	AcGePoint3d polar(const AcGePoint3d& base_pt, const double& angle, const double& distance);
	AcGePoint3d Polar(AcGePoint3d base_pt,double len,double angle);
	AcGePoint3d gl_Perpt(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3);
	AcGeVector3d	uniVector(const AcGePoint3d& pt1, const AcGePoint3d& pt2);
	AcGeVector3d	uniVector(AcDbObjectId objId);
	// 20091028 ����ȣ �߰� 360���·� ���� ��ȯ  �ݿø� ����
	double arx_angleD(const AcGePoint3d& pt1, const AcGePoint3d& pt2);







	// 360�� �Ǵ� 180�� �̻��� ������ ��� �������� ���Ѵ�
	// 20070604����ȣ
//	void m360(double& a);
//	void m180(double& a);
//	template<typename T>
//	T m360(T a);
//	template<typename T>
//	T m180(T a);

	double m360(double a);
	double m180(double a);
	// �ΰ��� �ٲ�
//	void swap(int& a, int& b);
	template <typename T>
	void swap(T& a, T& b);

/*
	// ���� ���� ����
	void tolerance(
		double angle,	// ���� ����
		double range,	// ��� ���� ����
		double& ang1,	// ��� �������� �� ��
		double& ang2);	// ��� �������� ���� ��
	// 360�� ���� ����
*/

	int round(double x);	// �ݿø� �Լ�

	double getAngle(AcGePoint3d& p1,AcGePoint3d& p2);
	// �� ���� ������ ���ϱ�
	AcGePoint3d getCenterPt(AcGePoint3d sp,AcGePoint3d ep);

	// �� ���� �������� ���Ѵ�
	AcGePoint3d getIntersPt(AcGePoint3d p1,		// ��1 ������
							AcGePoint3d p2,		// ��1 ����
							AcGePoint3d p3,		// ��2 ������
							AcGePoint3d p4);	// ��2 ����

	// ���� �ѷ��� ���ϱ�
	double getCircumference(const double& radius);

	AcGePoint3d GetDirOnLine(ads_name en, AcGePoint3d p1);	// ���μ��� ��ġ�� p1�� ���� �߽��� �������� �������� �������� �Ǵ�

	// 20100708 ����ȣ
	static bool IsEqual(const double& val1, const double& val2, const double& tolerance);

	// 20171007 �Ѵ���
	CString CheckInputNumber(CString str);
};

#endif // !defined(AFX_ARX_MATH_H__32305844_022D_11D4_B03C_00A0242716EF__INCLUDED_)
