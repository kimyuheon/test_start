// Arx_Xdata.h: interface for the CArx_Xdata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARX_XDATA_H__3230584A_022D_11D4_B03C_00A0242716EF__INCLUDED_)
#define AFX_ARX_XDATA_H__3230584A_022D_11D4_B03C_00A0242716EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

AcDbObject* uds_openObject(AcDbObjectId entID, BOOL bWritable, BOOL openErased=false);
AcDbEntity* uds_openEntity(AcDbObjectId entID, BOOL bWritable, BOOL openErased=false);


class CArx_Xdata : public AcRxObject//CComeCommon  
{
public:
	CArx_Xdata();
	virtual ~CArx_Xdata();

public:
	
	Acad::ErrorStatus es;

	AcDbObjectIdArray uparry;
	AcDbObjectIdArray downarry;

public:
	// PIP Bore
	int     gl_GetPipeBore(ads_name ename);
	int		gl_GetPipeBore(AcDbObject* pEnt);
	int		gl_GetPipeBore(AcDbObjectId objId);
	// PIP Xdata
	void	gl_GetPIPXdata(ads_name ename, CString& sKind, int& bore, double& ins_thk);
	void	gl_GetPIPXdata(AcDbObjectId EntId, CString& sKind, int& bore, double& ins_thk);
	void	gl_GetPIPXdata(AcDbObject* pEnt, CString& sKind, int& bore, double& ins_thk);
	// PC Kind
	CString gl_GetPipeKind(ads_name ename);
	CString gl_GetPipeKind(AcDbObject* pEnt);
	CString gl_GetPipeKind(AcDbObjectId objId);
	// PC Xdata
	void gl_GetPCXdata(ads_name ename, CString& sKind, CString& sFore, CString& sBack, CString& sMc);
	void gl_GetPCXdata(AcDbObjectId EntId, CString& sKind, CString& sFore, CString& sBack, CString& sMc);
	void gl_GetPCXdata(AcDbObject* pEnt, CString& sKind, CString& sFore, CString& sBack, CString& sMc);

	bool isXdata(ads_name ename, LPCTSTR lpApp);
	bool isXdata(AcDbObjectId objId, LPCTSTR lpApp);
	bool isXdata(AcDbObject* pEnt, LPCTSTR lpApp);
	struct resbuf* gfenameToXdataPiP(AcDbObjectId obj,TCHAR* str);
	AcDbObjectId DLineSelect(ads_point pt1,ads_name ent,TCHAR *str,resbuf* filter);
	CString gfreturn_handle(ads_name lst_name);

	AcDbObjectId entitySSelect(AcGePoint3d pt1,
							   AcGePoint3d pt2,
							   TCHAR *str);

	AcDbObjectId DLentitySSelect(AcGePoint3d pt1,
							   AcGePoint3d pt2,
							   TCHAR *str);
	
	AcDbObjectId entitySSelect(AcGePoint3d pt,
							   TCHAR *str,
							   resbuf* filter);
	AcDbObjectId entitySSelect(const ads_point pt1,
							  const ads_point pt2,
							  TCHAR *str);
	AcDbObjectId entitySSelect(const ads_point pt,
							  TCHAR *str,
						      resbuf* filter);
	AcDbObjectId DLentitySSelect(ads_point pt1,
							  ads_point pt2,
							  TCHAR *str);
	AcDbObjectId AllentitySSelect(AcGePoint3d pt1,
									   AcGePoint3d pt2);
	AcDbObjectId AllentitySSelect(const ads_point pt1,
									 const ads_point pt2);
	void AllentitySSelect(const ads_point pt1,
						 const ads_point pt2,
						 ads_name& _trename);
	bool entityEqual(ads_name en1,
					 ads_name en2);
	void lf_Xdata(ads_name ename,resbuf*& pRb);
	int lf_IReturnList(ads_name ename,int type);
	long lf_LReturnList(ads_name ename,int type);
	double lf_RReturnList(ads_name ename,int type);
	void  lf_SReturnList(ads_name ename,int type, CString &buffer);
	void  lf_SHReturnList(ads_name ename,int type, CString &buffer); ///마지막 1005번값 
	
	AcGePoint3d lf_3DReturnList(ads_name ename,int type);
	
	CString lf_PCIndexReturn(ads_name ename,int index);
	int lf_FPIPIndexReturn(ads_name ename,int index);
	CString lf_FIREStrReturn(ads_name ename);
	AcGePoint3d lf_FIRE3DReturn(ads_name ename);

	int type_return(int val);

	void gl_AddXdata(ads_name ename,
					  TCHAR saveName[],
					  int dia1,
					  int dia2);

	void gl_AddXdata(AcDbObjectId eId,
					  TCHAR saveName[],
					  int dia1,
					  int dia2);

	void gl_AddXdata(ads_name ename,
					  TCHAR saveName[],
					  TCHAR Str1[],
					  TCHAR Str2[],
					  TCHAR Str3[],
					  TCHAR Str4[]);
	void gl_AddXdata(AcDbObjectId eId,
					  TCHAR saveName[],
					  TCHAR Str1[],
					  TCHAR Str2[],
					  TCHAR Str3[],
					  TCHAR Str4[]);
	void gl_AddXdata(ads_name ename,
					  TCHAR saveName[],
					  TCHAR Str1[],
					  int  dia,
					  double dia1);

	void gl_AddXdata(AcDbObjectId eId,
					  TCHAR saveName[],
					  int  dia,
					  TCHAR Str1[],
					  TCHAR Str2[],
					  TCHAR Str3[]);
	void gl_AddXdata(ads_name ename,
					  TCHAR saveName[],
					  int  dia,
					  TCHAR Str1[],
					  TCHAR Str2[],
					  TCHAR Str3[]);
	///관경 데이타 저장 
	void gl_AddXdata(AcDbObjectId eId,
					  TCHAR saveName[],
					  TCHAR Str1[],
					  int  dia,
					  double dia1);

	void gl_AddXdata(struct resbuf* pRb,AcDbObjectId eId);

	void sym_XdataAdd(TCHAR *hand);
	
	void sym_XdataAdd(TCHAR *hand,ads_name lst_name);

	void sym_XdataAdd(struct resbuf* pRb);

	void sym_XdataAdd(struct resbuf* pRb,ads_name lst_name);

	void gf_PIPXdataAdd(ads_name lst_name,int dia);

	void gf_PIPXdataAddRbore(ads_name lst_name,double rbore);

	void gf_PCXdataAdd(ads_name lst_name,
							  TCHAR* Str1,
							  TCHAR* Str2,
							  TCHAR* Str3,
							  TCHAR* Str4);
	
	void gl_FIREAddXdata(ads_name ename,
						  TCHAR saveName[],
						  TCHAR Str1[],
						  AcGePoint3d pt);

	struct resbuf* gfenameToXdata(ads_name en);

	struct resbuf* gfenameToXdataL(ads_name en);

	struct resbuf* gfenameToXdataPiP(ads_name en,TCHAR* str);

	void lf_SortA(int cnt,AcGePoint3d dir_xpt);

	void lf_SortD(int cnt,AcGePoint3d dir_xpt);

	void arx_getObList(ads_name& enn,int index,bool chk);
	
	void arx_setObList(ads_name enn,int index,bool chk);

	int arx_getColor(ads_name enn);

	void arx_setColor(ads_name enn,int color);

	void arx_highlight(ads_name enn);

	void arx_unhighlight(ads_name enn);

	void gl_PolyAnaly(ads_name enn,AcGePoint3d pt1,AcGePoint3d& spt,
					 AcGePoint3d& ept,AcGePoint3dArray& lst,
								 int& lstv_cnt);
	void gl_LWPolyAnaly(ads_name enn,AcGePoint3d pt1,AcGePoint3d& spt,
					 AcGePoint3d& ept,AcGePoint3dArray& lst,
								 int& lstv_cnt);

	BOOL gf_ssget(ads_name& sname,resbuf* filter,long& len);

};

#endif // !defined(AFX_ARX_XDATA_H__3230584A_022D_11D4_B03C_00A0242716EF__INCLUDED_)
