// Arx_User.h: interface for the CArx_User class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ARX_USER_H__32305843_022D_11D4_B03C_00A0242716EF__INCLUDED_)
#define AFX_ARX_USER_H__32305843_022D_11D4_B03C_00A0242716EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "arx_xdata.h"
#include "arx_math.h"
#include "arx_draw.h"//

#define   namecpy(to, from) (*(to)=*(from), (to)[1]=(from)[1])
#define   ptcpy(to, from)   (*(to)=*(from), (to)[1]=(from)[1], (to)[2]=(from)[2])
#define   PtClear(x)        (x[X]=x[Y]=x[Z]=0.0)
#define   S(x)              (x)->resval.rstring
#define   I(x)              (x)->resval.rint
#define   L(x)              (x)->resval.rlong
#define   E(x)              (x)->resval.rlname
#define   R(x)              (x)->resval.rreal
#define   P(x)              (x)->resval.rpoint

template <class T> void uds_swap(T& a, T& b)
{
	T c(a);
	a=b;
	b=c;
}


typedef AcArray<double>  dblArray;

class CArx_User : public AcRxObject//CComeCommon  
{
public:
	CArx_User();
	virtual ~CArx_User();
private:
	Acad::ErrorStatus es;	

public:
	CArx_Xdata m_Xdata;
	CArx_Math  m_Math;
	CArx_Draw  m_Draw;//

public:
	struct resbuf* GetPntBlock(ads_name blk_ename);
	void GetLineBlock(ads_name blk_ename, AcGePoint3dArray& pnts, dblArray& dsts);
	CString gf_150Over(int bore1, int bore2);
	Acad::ErrorStatus BlockAttSet(AcDbDatabase* pD);

	void SetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString vvalue);
	void SetRegistry(LPCTSTR sKey, CString vname, CString vvalue);
	void gf_SetRegistry(CString vname, CString vvalue);
	void gf_SetRegistry(HKEY hKey, CString vname, CString vvalue);
	CString GetRegistry(HKEY hKey, LPCTSTR strCompany, LPCTSTR strProduct, CString vname, CString& vvalue);
	CString GetRegistry(LPCTSTR sKey, CString vname, CString& vvalue);
	CString gf_GetRegistry(CString vname, CString& vvalue);
	CString gf_GetRegistry(HKEY hKey, CString vname, CString& vvalue);

	struct resbuf * ads_assoc(int grp, struct resbuf *rb);
	void gf_WLentity(ads_name cname, ads_point cpnt, double bore, ads_name& ent1, ads_name& ent2);
	void gf_WPoint(ads_name ename, ads_point pnt);
	void GetPipe(ads_point pt1, ads_name &ename);
	
	///double ���� string ������ �ٲ۴�.
	CString valtoa(double value);

	///int ���� string ������ �ٲ۴�.
	CString valtoa(int value);

	//

	// 45�� �����Լ�
	void ads_setAngle45(ads_point ps, ads_point& pe);
	// ����ڰ� ������ ������ ���� ����
	void ads_setAngleUser(double uang,ads_point ps, ads_point& pe);
	// ���� ���� ����, ���� ���� ������ �޾� ������ ���ϰ� ����� ������ ���Ѵ�
	void ads_setAngleUser(double uang,ads_point tmp_pt,ads_point ps, ads_point& pe);

	void ads_ptCopy(ads_point ps,ads_point pe);
	void ads_arxpt(AcGePoint3d& ps,ads_point pe);
	void arx_adspt(ads_point& pe,AcGePoint3d ps);
	void arx_getLine(AcDbObjectId objIdd, AcGePoint3d& ps, AcGePoint3d& pe);
	void arx_exPoint(AcGePoint3d& p1, AcGePoint3d& p2);

	int arx_ptInter(AcGePoint3d pt1, AcGePoint3d pt2,
							AcGePoint3d pt3,AcGePoint3d pt4,
							int env,AcGePoint3d& pt5);
	CString lfreturn_handle();
	
	void gfObjIDDel(AcDbObjectId objId);
	void gfenameDel(ads_name en);

	int gfMulCount(CString mm);

	BOOL gf_entsel(TCHAR *msg,ads_name& en,int& bore,AcGePoint3d& sel_pt,
							 AcGePoint3d& stpt,AcGePoint3d& edpt);

	BOOL gf_dlentsel(ads_name en,double ang,ads_name& ename,AcGePoint3d inpt,
							AcGePoint3d& stpt,AcGePoint3d& edpt);

	resbuf*	ptArrayToResbuf(const AcGePoint3dArray& ptArray,int cnt);

	// �߰��Ѱ���
	void GetSymbolPoint(ads_name en, AcGePoint3dArray& pts);
	// �迭�� �޴´�
	void GetSymbolPoint(ads_name en, AcGePoint3d* pts);


	// ����ȣ 20080215
	// ads_name ��ü�� ������ ������ AcGePoint3d������ ��ȯ�Ѵ�
	void getStartEndPnt(ads_name en,AcGePoint3d& sp,AcGePoint3d& ep);

	// ����ȣ 20080219
	// sp,ep�� �޾� en��ü�� ������ �������� �����Ѵ�
	void setStartEndPnt(ads_name en,AcGePoint3d sp,AcGePoint3d ep);

	// ����ȣ 20070530
	// �ӽ� ������ �ʿ���� ������ ������ �̿��� �������Լ�
	void swap(double&, double&);
	void swap(int&, int&);
	void swap(AcGePoint3d& pnt1,AcGePoint3d& pnt2);

	// 20100706 ����ȣ
	bool getLine(ads_point pt1, ads_name &ename);
	void getLine(const AcGePoint3d& pt, AcDbObjectId& idLine);

	// 20100708 ����ȣ
	static CString GetName(ads_name en);
	static CString GetName(AcDbObjectId& id);
};
void ssNoLock(ads_name & tss);
bool isOnLockedCurLayer();
bool isOnLockedObj(ads_name ename);
bool isOnLockedObj(AcDbObjectId ojbId);

#endif // !defined(AFX_ARX_USER_H__32305843_022D_11D4_B03C_00A0242716EF__INCLUDED_)
