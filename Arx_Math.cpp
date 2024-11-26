// Arx_Math.cpp: implementation of the CArx_Math class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Arx_Draw.h"
#include "Arx_Xdata.h"
#include "Arx_User.h"
#include "Arx_Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArx_Math::CArx_Math()
{
	
}

CArx_Math::~CArx_Math()
{

}

double CArx_Math::uds_Aangle(double startAng, double endAng)
{
	double startAng_D = RTD(startAng);
	double endAng_D   = RTD(endAng);

	if (startAng_D > endAng_D)
		return DTR( 360 - (startAng_D-endAng_D) );
	else
		return DTR( endAng_D-startAng_D );
}

// pc�� �߽���, ps ������, pe ����
double CArx_Math::uds_3ptAngle(ads_point pc, ads_point ps, ads_point pe)
{
	return uds_Aangle(ads_angle(pc,ps), ads_angle(pc,pe));
}

double CArx_Math::uds_btwang(AcGePoint3d pc, AcGePoint3d ps, AcGePoint3d pe)
{
	AcGeVector3d v1, v2;
	v1=ps-pc;
	v2=pe-pc;
	return(v1.angleTo(v2));
}
// �������� ��ȯ
double CArx_Math::uds_3ptAngle(AcGePoint3d pc, AcGePoint3d ps, AcGePoint3d pe)
{
//	AcGeVector3d v1, v2;
//	v1=ps-pc;
//	v2=pe-pc;
//	return(v1.angleTo(v2));
	return uds_Aangle(arx_angle(pc,ps), arx_angle(pc,pe));
}

BOOL CArx_Math::arx_coline(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3)
{
	if (p1[2] != p2[2] || p2[2] != p3[2])
		return false;
	if (arx_eqPoint(p1, p2) || (arx_eqPoint(p2, p3) || arx_eqPoint(p1, p3)))
		return true;

	double ang = tan(arx_angle(p1,p2));
	double dx = p3[0] - p1[0];
	double dy = p3[1] - p1[1];

	if ( fabs(ang - dy/dx) <= 0.001 )
		return true;
	return false;
}

////���� ���� �����ΰ� �����ΰ� �Ǻ� 
int CArx_Math::arx_hand(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3)
{
	double ang1 = arx_angle(p1, p2);
	double ang2 = arx_angle(p1, p3);

	if ((ang2 > ang1) && (ang2 > DTR(180)))
		ang2 = ang2 - DTR(360);
	double ang22 = ang2 + DTR(180);

	if (arx_coline(p1,p2,p3))
		return 0;
	else if (ang1 > ang2 && ang1 < ang22)
		return 1;
	else
		return -1;
}

////���� ���� �����ΰ� �����ΰ� �Ǻ� 
int CArx_Math::lr_hand(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3)
{
	double Ax=(double)0,Bx=(double)0,Cx=(double)0,Ay=(double)0,By=(double)0,Cy=(double)0;
	double L=(double)0,s=(double)0;

	Ax=p1[0];Ay=p1[1];Bx=p2[0];By=p2[1];Cx=p3[0];Cy=p3[1];
/*
  s<0      C is left of AB
  s>0      C is right of AB
  s=0      C is on AB
*/
	L = sqrt((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));
	s = ((Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay)) / (L * L);

	if (s == 0)
		return 0;
	else if (s < 0)  
		return -1;
	else
		return 1;

}
////���� ���� �����ΰ� �����ΰ� �Ǻ� 
int CArx_Math::ads_lr_hand(ads_point p1, ads_point p2, ads_point p3)
{
	double Ax=(double)0,Bx=(double)0,Cx=(double)0,Ay=(double)0,By=(double)0,Cy=(double)0;
	double L=(double)0,s=(double)0;

	Ax=p1[X];Ay=p1[Y];Bx=p2[X];By=p2[Y];Cx=p3[X];Cy=p3[Y];
/*
  s<0      C is left of AB
  s>0      C is right of AB
  s=0      C is on AB
*/
	L = sqrt((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));
	s = ((Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay)) / (L * L);

	if (s == 0)
		return 0;
	else if (s < 0)  
		return -1;
	else
		return 1;

}
////���� ������ ��� ��ġ�ϴ°�???
int CArx_Math::fb_hand(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3)
{
	double Ax=(double)0,Bx=(double)0,Cx=(double)0,Ay=(double)0,By=(double)0,Cy=(double)0;
	double L=(double)0,r=(double)0;

	Ax=p1[0];Ay=p1[1];Bx=p2[0];By=p2[1];Cx=p3[0];Cy=p3[1];
/*
  r=0      P = A
  r=1      P = B
  r<0      P is on the backward extension of AB
  r>1      P is on the forward extension of AB
  0<r<1    P is interior to AB
*/
	L = sqrt((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));
	r = ((Ay - Cy) * (Ay - By) - (Ax - Cx) * (Bx - Ax)) / (L * L);

	if ((0 <= r)&&(r <= 1))  ///���� ���л� �ִٸ� 
		return 0;
	else if (r < 0)
		return 2;
	else              ///r>1
		return 3;

}
////���� ������ ��� ��ġ�ϴ°�???
// closest�ΰ�
AcGePoint3d CArx_Math::gl_Perpt(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d p3)
{
	double Ax=(double)0,Bx=(double)0,Cx=(double)0,Ay=(double)0,By=(double)0,Cy=(double)0;
	double L=(double)0,r=(double)0;
	AcGePoint3d Pt;

	Ax=p1[0];Ay=p1[1];Bx=p2[0];By=p2[1];Cx=p3[0];Cy=p3[1];

	L = sqrt((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));
	r = ((Ay - Cy) * (Ay - By) - (Ax - Cx) * (Bx - Ax)) / (L * L);

	Pt.set(Ax+r*(Bx-Ax),Ay+r*(By-Ay),0);
	return Pt;
}

double CArx_Math::arx_angle(AcGePoint3d p0, AcGePoint3d p1)
{
	
	CArx_User m_User;
	
	ads_point p0D, p1D;
	m_User.arx_adspt(p0D,p0);
	m_User.arx_adspt(p1D,p1);

	return ads_angle(p0D,p1D);
}

BOOL CArx_Math::arx_eqPoint(AcGePoint3d p1, AcGePoint3d p2)
{
	return p1.isEqualTo(p2);
}

BOOL CArx_Math::arx_perp(AcGePoint3d& p1, AcGePoint3d& p2, AcGePoint3d& p3)
{
    double angle=(double)0;
    AcGePoint3d p22,p33;

	if(arx_eqPoint(p1,p2) || arx_eqPoint(p1,p3))
       return TRUE;

	angle = arx_angle(p2,p3)+DTR(90);
	p22=Polar(p2,100,RTD(angle));
	p33=Polar(p3,100,RTD(angle));

	//if (arx_hand(p2,p22,p1)==arx_hand(p3,p33,p1))
	if (lr_hand(p2,p22,p1)==lr_hand(p3,p33,p1))
		return FALSE;
	return TRUE;
}

////������ ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CArx_Math::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}

////���� ���� ������ ��ȯ�Ͽ� �����Ѵ�.
double CArx_Math::RTD(double radian)
{//ads_printf(_T("\n RTD() radian=%f \n"),radian);
	return (radian * 180.0 / PI);
}
// 360���·� ���� ��ȯ
double CArx_Math::arx_angleA(AcGePoint3d st_pt,AcGePoint3d ed_pt)
{
	CArx_User m_User;
	
	ads_point pt1, pt2;
	m_User.arx_adspt(pt1,st_pt);
	m_User.arx_adspt(pt2,ed_pt);
	double ang=floor(RTD(ads_angle(pt1,pt2))+0.5);
	if(ang >= 360)
		ang -= 360;
	return ang;
}

// 360���·� ���� ��ȯ  �ݿø� ����
double CArx_Math::arx_angleD(const AcGePoint3d& pt1, const AcGePoint3d& pt2)
{
	ads_point p1,p2;
	p1[X]=pt1[X];	p1[Y]=pt1[Y];	p1[Z]=pt1[Z];
	p2[X]=pt2[X];	p2[Y]=pt2[Y];	p2[Z]=pt2[Z];
	double ang=RTD(ads_angle(p1,p2));
	if(ang >= 360)
		ang -= 360;
	return ang;
}

void CArx_Math::getclosestPoint(AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d givenPnt, AcGePoint3d& closestPnt)
{
	AcGeVector3d v;
	AcGeMatrix3d mat;
	double  l=(double)0, ang=(double)0, dst=(double)0;
	
	v=ep-sp;
	v.normalize();
	l=sp.distanceTo(givenPnt);
	ang=uds_btwang(sp, givenPnt, ep);
	dst=l*cos(ang);
	v=v*dst;
	mat.setToTranslation(v);
	closestPnt=sp;
	closestPnt.transformBy(mat);		
}

void CArx_Math::polar(AcGePoint3d sp, AcGePoint3d ep, double dst, AcGePoint3d& rPnt)
{
	AcGeVector3d v;
	AcGeMatrix3d mat;
	
	v=ep-sp;
	v.normalize();
	v=v*dst;
	mat.setToTranslation(v);
	rPnt=sp;
	rPnt.transformBy(mat);		
}

// 360����
// 20091028 ����ȣ �߰�  ������ ���簢���� ���̸� ���Ҷ� �ణ�� ������ �ִ°Ͱ��Ƽ� ���� ������� Polar �Լ��� �Ѵ� ��������� �������� ���� �߰��� �� �Լ��� acutPolar �Լ��� ����Ѵ�
AcGePoint3d CArx_Math::polar(const AcGePoint3d& base_pt, const double& angle, const double& distance)
{
	double angRadian=DTR(angle);
	AcGePoint3d ptResult;
	ads_point basePnt,resultPnt;

	basePnt[X]=base_pt[X];
	basePnt[Y]=base_pt[Y];
	basePnt[Z]=base_pt[Z];

	acutPolar(basePnt, angRadian, distance, resultPnt);

	ptResult.set(resultPnt[X],resultPnt[Y],resultPnt[Z]);

	return ptResult;
}

///��� ��ǥ���� ���Ѵ�.
// 360 ����  �Ű����� ���� ����  ��,����,����
AcGePoint3d CArx_Math::Polar(AcGePoint3d base_pt,double len,double angle)
{
//	ads_printf(_T("\n polar len=%.1f \n"),len);
	double yaxis_len=(double)0,xaxis_len=(double)0;
	AcGePoint3d result_pt;
	if (angle > 360.0)
		angle = angle - 360.0;
	if (angle < 0)
		angle = 360.0 + angle;

	yaxis_len = sin(DTR(angle))*len;
	xaxis_len = sqrt(pow(len,2.0)-pow(yaxis_len,2.0));
	if (angle > 90 && angle < 270)
		xaxis_len = xaxis_len * -1;
	
	result_pt.set(base_pt[0]+xaxis_len,base_pt[1]+yaxis_len,0);
	return result_pt;
}

// ���� ����  �Ű����� ���� ����  ��,����,����
void  CArx_Math::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z );
}

// ������ �̷�� �������͸� ���Ѵ�.
AcGeVector3d CArx_Math::uniVector(const AcGePoint3d& pt1, const AcGePoint3d& pt2)
{
	AcGeVector3d v1(pt1.asVector());
	AcGeVector3d v2(pt2.asVector());

	AcGeVector3d vector = v2 - v1;
	vector.normalize();
	return vector;
}

AcGeVector3d CArx_Math::uniVector(AcDbObjectId objId)
{
	AcGePoint3d sp, ep;
	CArx_User	user;
	user.arx_getLine(objId, sp, ep);
	return uniVector(sp, ep);
}




/////
// 20070604 ����ȣ
// 360���� ���� �������� ���Ѵ�
/*
void CArx_Math::m360(double& a)
{
	while(a>=ANGLE360)
		a-=ANGLE360;

//	return a;
}

// 180���� ���� �������� ���Ѵ�
void CArx_Math::m180(double& a)
{
	while(a>=ANGLE180)
		a-=ANGLE180;

//	return a;
}
*/
// 360���� ���� �������� ���Ѵ�
/*
template <typename T>
T CArx_Math::m360(T a)
{
	while(a>=ANGLE360)
		a-=ANGLE360;

	return a;
}

// 180���� ���� �������� ���Ѵ�
template <typename T>
T CArx_Math::m180(T a)
{
	while(a>=ANGLE180)
		a-=ANGLE180;

	return a;
}*/

double CArx_Math::m360(double a)
{
	while(a>=ANGLE360)
		a-=ANGLE360;

	return a;
}

// 180���� ���� �������� ���Ѵ�
double CArx_Math::m180(double a)
{
	while(a>=ANGLE180)
		a-=ANGLE180;

	return a;
}

// floor�Լ��� �̿��� �ݿø� �Լ�
int CArx_Math::round(double x)
{
	int d=0;
	d = (int)floor(x+0.5);//ads_printf(_T("\n 1019 CArx_Math::round() x=%f �����=%d \n"),x,d);
	return d;
}



// �ӽ� ������ �ʿ���� ������ ������ �̿��� �������Լ�
template <typename T>
void CArx_Math::swap(T& a, T& b)
{
	a += b;
	b = a-b;
	a -= b;
}
/*
void CArx_Math::swap(int& a, int& b)
{
	a += b;
	b = a-b;
	a -= b;
}*/

double CArx_Math::getAngle(AcGePoint3d& p1,			// ��1�� ������
				AcGePoint3d& p2)			// ��1�� ����
{
	ads_real pp1[3],pp2[3];	

	// ���� ������ ���Ѵ�
	ads_real angle=0;
	
	pp1[0]=(ads_real)p1.x;
	pp1[1]=(ads_real)p1.y;
	pp1[2]=(ads_real)p1.z;
	
	pp2[0]=(ads_real)p2.x;
	pp2[1]=(ads_real)p2.y;
	pp2[2]=(ads_real)p2.z;

	return (double)acutAngle(pp1,pp2);
}


AcGePoint3d CArx_Math::getIntersPt(AcGePoint3d p1,
								   AcGePoint3d p2,
								   AcGePoint3d p3,
								   AcGePoint3d p4)
{
	AcGePoint3d IntersPt;

	ads_real pp1[3],pp2[3],pp3[3],pp4[3],pp5[3];

	pp1[0]=(ads_real)p1.x;
	pp1[1]=(ads_real)p1.y;
	pp1[2]=(ads_real)p1.z;

	pp2[0]=(ads_real)p2.x;
	pp2[1]=(ads_real)p2.y;
	pp2[2]=(ads_real)p2.z;

	pp3[0]=(ads_real)p3.x;
	pp3[1]=(ads_real)p3.y;
	pp3[2]=(ads_real)p3.z;

	pp4[0]=(ads_real)p4.x;
	pp4[1]=(ads_real)p4.y;
	pp4[2]=(ads_real)p4.z;

	acdbInters(pp1,pp2,pp3,pp4,0,pp5);	// 5�� ���ڰ� 0�̸� ������ ���弱������ �������� ���Ѵ� 0�̾ƴϸ� �־��� ���й��������� ã�´�

	IntersPt.x=pp5[0];
	IntersPt.y=pp5[1];
	IntersPt.z=pp5[2];

	return IntersPt;
}

AcGePoint3d CArx_Math::GetDirOnLine(ads_name en, AcGePoint3d p1)	// ���μ��� ��ġ�� p1�� ���� �߽��� �������� �������� �������� �Ǵ�
{
	AcGePoint3d sp,ep;
	int es=0;
	double dst=0,ang=0;

	// en��ü�� ������ ���� ���´�
	AcDbObjectId enobj;
	AcDbLine* pLine;
	acdbGetObjectId(enobj,en);
	acdbOpenObject(pLine,enobj,AcDb::kForRead);
	sp = pLine->startPoint();
	ep = pLine->endPoint();
	es = pLine->close();

	// p1�� �������� ������� ������ ������� �Ǵ�
	if(sp.distanceTo(p1) < ep.distanceTo(p1)){
		return sp;
	}
	else{
		return ep;
	}

/*	
	dst=sp.distanceTo(ep);
	ang=getAngle(sp,ep);	// ���� ���� ���ϱ�
	ang=RTD(ang);			// ���Ȱ��� -> 360����
	cp = Polar(sp,dst/2,ang);	// 360������ �����Ͽ� �߽����� ã�´�
*/
	return p1;
}


// �� ���� ������ ���ϱ�
AcGePoint3d CArx_Math::getCenterPt(AcGePoint3d sp,AcGePoint3d ep)
{
	AcGePoint3d cp;
	double dst=0,ang=0;

	dst=sp.distanceTo(ep);
	ang=getAngle(sp,ep);	// ���� ���� ���ϱ�
	ang=RTD(ang);			// ���Ȱ��� -> 360����
	cp = Polar(sp,dst/2,ang);	// 360������ �����Ͽ� �߽����� ã�´�

	return cp;
}

// ���� �ѷ��� ���Ѵ�
double CArx_Math::getCircumference(const double& radius)
{
	double dblCircumference=0, twice=2;
	dblCircumference = twice * PI * radius ;
	return dblCircumference;
}

// ��ǻ�ʹ� 2������ ����ϰ� ����� 10������ ����Ѵ� double�� �����͸� 2������ 10������ �Դٰ��� ��ȯ�Ǹ鼭 �ర�� ������ ����� �ִ�
// 20100708 ����ȣ
bool CArx_Math::IsEqual(const double& val1, const double& val2, const double& tolerance)
{
	if( abs(val1-val2)<tolerance )
		return true;
	else
		return false;
}

//���ڿ� �� üũ
CString CArx_Math::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))	str.Remove(*getAtChar);
	}
	return str;
}