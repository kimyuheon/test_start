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

// pc가 중심점, ps 시작점, pe 끝점
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
// 라디안으로 반환
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

////점이 선의 좌측인가 우측인가 판별 
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

////점이 선의 좌측인가 우측인가 판별 
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
////점이 선의 좌측인가 우측인가 판별 
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
////점이 선상의 어디에 위치하는가???
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

	if ((0 <= r)&&(r <= 1))  ///점이 선분상에 있다면 
		return 0;
	else if (r < 0)
		return 2;
	else              ///r>1
		return 3;

}
////점이 선상의 어디에 위치하는가???
// closest인가
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

////각도를 라디안 값으로 변환하여 리턴한다.
double CArx_Math::DTR(double angle)
{//ads_printf(_T("\n DTR() angle=%f \n"),angle);
	return (angle / 180.0 * PI);
}

////라디안 값을 각도로 변환하여 리턴한다.
double CArx_Math::RTD(double radian)
{//ads_printf(_T("\n RTD() radian=%f \n"),radian);
	return (radian * 180.0 / PI);
}
// 360형태로 각도 반환
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

// 360형태로 각도 반환  반올림 없음
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

// 360각도
// 20091028 원진호 추가  기울어진 직사각형의 길이를 구할때 약간의 오차가 있는것같아서 새로 만들었다 Polar 함수는 둘다 직접계산해 구하지만 새로 추가된 이 함수는 acutPolar 함수를 사용한다
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

///상대 좌표값을 구한다.
// 360 각도  매개변수 순서 조심  점,길이,각도
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

// 라디안 각도  매개변수 순서 조심  점,각도,길이
void  CArx_Math::Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result)
{
	result.set(base.x + dist*cos(angle), base.y + dist*sin(angle), base.z );
}

// 두점이 이루는 단위벡터를 구한다.
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
// 20070604 원진호
// 360으로 나눈 나머지를 구한다
/*
void CArx_Math::m360(double& a)
{
	while(a>=ANGLE360)
		a-=ANGLE360;

//	return a;
}

// 180으로 나눈 나머지를 구한다
void CArx_Math::m180(double& a)
{
	while(a>=ANGLE180)
		a-=ANGLE180;

//	return a;
}
*/
// 360으로 나눈 나머지를 구한다
/*
template <typename T>
T CArx_Math::m360(T a)
{
	while(a>=ANGLE360)
		a-=ANGLE360;

	return a;
}

// 180으로 나눈 나머지를 구한다
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

// 180으로 나눈 나머지를 구한다
double CArx_Math::m180(double a)
{
	while(a>=ANGLE180)
		a-=ANGLE180;

	return a;
}

// floor함수를 이용한 반올림 함수
int CArx_Math::round(double x)
{
	int d=0;
	d = (int)floor(x+0.5);//ads_printf(_T("\n 1019 CArx_Math::round() x=%f 결과값=%d \n"),x,d);
	return d;
}



// 임시 변수가 필요없는 덧셈과 뺄셈을 이용한 스워핑함수
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

double CArx_Math::getAngle(AcGePoint3d& p1,			// 선1의 시작점
				AcGePoint3d& p2)			// 선1의 끝점
{
	ads_real pp1[3],pp2[3];	

	// 선의 각도를 구한다
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

	acdbInters(pp1,pp2,pp3,pp4,0,pp5);	// 5번 인자가 0이면 가상의 연장선끼리의 교차점을 구한다 0이아니면 주어진 선분범위에서만 찾는다

	IntersPt.x=pp5[0];
	IntersPt.y=pp5[1];
	IntersPt.z=pp5[2];

	return IntersPt;
}

AcGePoint3d CArx_Math::GetDirOnLine(ads_name en, AcGePoint3d p1)	// 라인선상에 위치한 p1이 라인 중심점 기준으로 우측인지 좌측인지 판단
{
	AcGePoint3d sp,ep;
	int es=0;
	double dst=0,ang=0;

	// en객체의 시작점 끝점 얻어온다
	AcDbObjectId enobj;
	AcDbLine* pLine;
	acdbGetObjectId(enobj,en);
	acdbOpenObject(pLine,enobj,AcDb::kForRead);
	sp = pLine->startPoint();
	ep = pLine->endPoint();
	es = pLine->close();

	// p1이 시작점과 가까운지 끝점에 가까운지 판단
	if(sp.distanceTo(p1) < ep.distanceTo(p1)){
		return sp;
	}
	else{
		return ep;
	}

/*	
	dst=sp.distanceTo(ep);
	ang=getAngle(sp,ep);	// 라디안 각도 구하기
	ang=RTD(ang);			// 라디안각도 -> 360각도
	cp = Polar(sp,dst/2,ang);	// 360각도를 전달하여 중심점을 찾는다
*/
	return p1;
}


// 두 점의 중점을 구하기
AcGePoint3d CArx_Math::getCenterPt(AcGePoint3d sp,AcGePoint3d ep)
{
	AcGePoint3d cp;
	double dst=0,ang=0;

	dst=sp.distanceTo(ep);
	ang=getAngle(sp,ep);	// 라디안 각도 구하기
	ang=RTD(ang);			// 라디안각도 -> 360각도
	cp = Polar(sp,dst/2,ang);	// 360각도를 전달하여 중심점을 찾는다

	return cp;
}

// 원의 둘레를 구한다
double CArx_Math::getCircumference(const double& radius)
{
	double dblCircumference=0, twice=2;
	dblCircumference = twice * PI * radius ;
	return dblCircumference;
}

// 컴퓨터는 2진수를 사용하고 사람은 10진수를 사용한다 double의 데이터를 2진수와 10진수를 왔다갔다 변환되면서 약같의 오차가 생길수 있다
// 20100708 원진호
bool CArx_Math::IsEqual(const double& val1, const double& val2, const double& tolerance)
{
	if( abs(val1-val2)<tolerance )
		return true;
	else
		return false;
}

//숫자와 점 체크
CString CArx_Math::CheckInputNumber(CString str){
	TCHAR* getAtChar;
	for(int i = 0; i < str.GetLength(); i++){
		getAtChar = (TCHAR*)(LPCTSTR)str.Mid(i,1);
		int ascii = __toascii(*getAtChar);
		if(!((ascii >= 48 && ascii <= 57) || ascii == 46))	str.Remove(*getAtChar);
	}
	return str;
}