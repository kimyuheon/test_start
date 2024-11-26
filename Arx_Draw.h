// Arx_Draw.h: interface for the CArx_Draw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARX_DRAW_H__32305845_022D_11D4_B03C_00A0242716EF__INCLUDED_)
#define AFX_ARX_DRAW_H__32305845_022D_11D4_B03C_00A0242716EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "stdafx.h"
//#include "math.h"		// ����ȣ 20070529

class CArx_Draw : public AcRxObject//CComeCommon  
{
public:
	CArx_Draw();
	virtual ~CArx_Draw();

private:
	
	Acad::ErrorStatus es;

public:
	int m_Cur_Color;					 ///������ COLOR

	TCHAR	m_Entity_Layer[128];       ///������ Layer
	TCHAR	m_Entity_Linetype[128];    ///������ LINETYPE
	TCHAR	m_Entity_Text_Style[128];  ///������ text style
	CString gvAppPath;
	int    gvscl,ds_clr;
	double bupum_size;
	double txt_size;
	double LT_scale;
	double HD_size;
	double m_linetypeScale4Ent;

public:
	void CopyProperty(AcDbObjectId destId, AcDbObjectId srcId);
	void CopyProperty(AcDbEntity *destEnt, AcDbEntity *srcEnt);
	AcDbEntity* openEntity(AcDbObjectId entID, BOOL bWritable, BOOL openErased);
   AcDbObject* openObject(AcDbObjectId entID, BOOL bWritable, BOOL openErased);
	
	// ���� ��ġ�� �����Ѵ�
	bool setLine(ads_name ename, AcGePoint3d pt1, AcGePoint3d pt2);
	void setLine(AcDbLine* pEnt, AcGePoint3d ps, AcGePoint3d pe);
	bool setLine(AcDbObjectId EntId, AcGePoint3d ps, AcGePoint3d pe);

	void setCecolor(AcCmColor m_cecolor);  // ���� ������ �����Ѵ�.
	void ObjectConfigSet(ads_name ent, int Mode);
	CString GetEntityLineType(ads_name  ent);
	CString s;
	int lf_IReturnList(ads_name ename,int type);
	void setSysVar();		// 20080302 ����ȣ ĳ��ý��ۺ����� �����Ѵ�
	void ShowSysVar();	// 20080302 ����ȣ ĳ��ý��ۺ����� �����ش�
	void WriteEnv();
	void ReadEnv();
	void setCurColor(int col);
	void setCurColor();


	CString ccolor;		//COLOR
	CString clayer;		//LAYER
	CString cltype;		//Linetype
	int cosnap;				//OSNAP
	int orthomode;			//ORTHO
	CString ctextstyle;	//TEXTSTYLE

	// acedGrDraw ���
	void DrawGrLine(const AcGePoint3d& p1, const AcGePoint3d& p2);
	void DrawGrLine(const ads_point& p1, const ads_point& p2);

	void pyosi(ads_point pt);
	void pyosi(AcGePoint3d pt1);
	Acad::ErrorStatus initDraw();
	Acad::ErrorStatus 
	createLayer(TCHAR *layer,TCHAR *linetype,const AcCmColor color);


	void gf_UserBlock_ins(AcDbObjectId blockId,
						   AcGePoint3d inpt,
						   double angle);

	// ��� �׸���
	void ads_DrawBlock(AcGePoint3d insertPt,
						  CString m_bname,
                          double x_scale,
						  double y_scale,
						  double angle);

	// ��� �׸��� �׷��� �Ѵ��� 201406
	void ads_DrawBlock(AcGePoint3d insertPt,
						  CString m_bname,
                          double x_scale,
						  double y_scale,
						  double angle,
						  AcDbObjectIdArray& gIds);

	// ��� �׸���
	void ads_DrawBlock(AcGePoint3d insertPt,
						  CString m_bname,
                          double x_scale,
						  double y_scale,
						  double angle,
						  CString cclayer);
	// ��� �׸���
	void DrawBlock(AcGePoint3d insertPt,
		 		  CString m_bname,
                  double x_scale,
				  double y_scale,
				  double angle);

	Acad::ErrorStatus 
		DrawLine(AcGePoint3d spt,AcGePoint3d ept);
	// �����۵� �� �׷����� ���� ������ƮID �߰�, 201406 �Ѵ���
	Acad::ErrorStatus
		DrawLine(AcGePoint3d spt,AcGePoint3d ept, AcDbObjectIdArray& gIds);
	// �ΰ��� ads_point�� �޾� �����۵�, 20080424 ����ȣ
	Acad::ErrorStatus	DrawLine(ads_point pt1,ads_point pt2);

	AcDbObjectId 
		DrawLineID(AcGePoint3d spt,AcGePoint3d ept);

	Acad::ErrorStatus 
		DrawArc(const AcGePoint3d center,
				   const AcGePoint3d spt,
				   const AcGePoint3d ept);

	Acad::ErrorStatus 
		DrawCircle(AcGePoint3d cpt,double radius);

	Acad::ErrorStatus 
		DrawPLine(AcGePoint3dArray ptArr,double width);

	// PolyLine create 2  20091029 ����ȣ �߰� Id�� �ʿ��ؼ� �������
	Acad::ErrorStatus
		DrawPLine(AcGePoint3dArray ptArr,double width, AcDbObjectId& objId);

	Acad::ErrorStatus 
		DrawBox(AcGePoint3d lefttop,AcGePoint3d rightbottom);

	Acad::ErrorStatus
		addToModelSpace(AcDbEntity* pEntity);

	Acad::ErrorStatus
		addToModelSpace(AcDbEntity* pEntity, int iColor);

	Acad::ErrorStatus
		addToModelSpaceID(AcDbEntity* pEntity,AcDbObjectId& objId);
	
	AcDbObjectId getCLayer();
	
	void setCLayer(TCHAR *layer);

	void setCLayer(AcDbObjectId objId);

	void setCurLayer(CString layer);
	void setCurLinetype(CString linetype);

	const AcCmColor getCurColor();
	void setCurTextStyle(CString style);


	// ĳ�� �ý��� �������� ���� ���´�
	Acad::ErrorStatus   getSysVar(LPCTSTR varName, int&);
	Acad::ErrorStatus   getSysVar(LPCTSTR varName, double&);
	Acad::ErrorStatus   getSysVar(LPCTSTR varName, CString&);
	Acad::ErrorStatus   getSysVar(LPCTSTR varName, AcGePoint3d&);
	Acad::ErrorStatus   getSysVar(LPCTSTR varName, AcGePoint2d&);
	// ĳ�� �ý��� ������ ���� �����Ѵ�
	Acad::ErrorStatus   setVar(LPCTSTR varName, const resbuf* newVal);
	Acad::ErrorStatus   setSysVar(LPCTSTR varName, int);
	Acad::ErrorStatus   setSysVar(LPCTSTR varName, double);
	Acad::ErrorStatus   setSysVar(LPCTSTR varName, LPCTSTR);


	void arx_BLineBreak(ads_name& ename1, ads_name& ename2,ads_name ebase);
	void arx_BLineBreak(AcDbObjectId objId, AcGePoint3d pint);
	void arx_BLineBreak(ads_name& en1,ads_name& en2,AcGePoint3d pint,AcGePoint3d pts,AcGePoint3d pte);
	void arx_LineBreak(AcDbObjectId objId, AcGePoint3d p0, double left, double right);
	void arx_MLineBreak(AcDbObjectId objId,AcGePoint3d p0,
							  double left, double right);
	void TextOut(AcGePoint3d textorgpt, TCHAR *as_Str, 
				double ar_TextHeight, double ar_Angle,TCHAR *aligntype);
	//�ؽ�Ʈ ���� �� �׷����� ���� ������ƮID �߰� 201406 �Ѵ���
	void TextOut(AcGePoint3d textorgpt, TCHAR *as_Str, 
				double ar_TextHeight, double ar_Angle,TCHAR *aligntype, AcDbObjectIdArray& gIds);	
	void gf_Linetypecolorchange(ads_name en);
	void gf_Linetypecolorchange(ads_name en,AcGePoint3d pt1,AcGePoint3d pt2);

	CString gfGetSymName(int ptype);
	double gfGetSymDST1(int ptype,double scl);
	double gfGetSymDST2(int ptype,double scl);
	void gf_OneTwoYNIns(int Mode,CString prompt1,CString prompt2,CString YesNo,
							 CString path,double scale,CString trim,CString bend,CString reverse);

	void createGroup(CString gname,AcDbObjectId& groupId);
	void appendGroup(CString gname);
	void appendGroup(CString gname,ads_name ename);
	void removeGroup(CString key);

	//////////////////////////////
	// ����ȣ 20070529
	// �� ���� �Ÿ� ���ϱ�
	double getDistancePt(AcGePoint3d,AcGePoint3d);

	// 20090108 ����ȣ AcGePoint3d ��ȿ�� �˻�
	void CheckPt(CString FUNCTION, AcGePoint3d pt);

	// ���� �۵�2
	Acad::ErrorStatus DrawLine(double,double,double,double);
	// ���� �۵�3
	//// ������ �������� Line�� �����Ѵ�.
	Acad::ErrorStatus DrawLine(AcGePoint3d spt,AcGePoint3d ept, int iColor);

	//ZWCAD2017 ads_commande Insert�� ������ arx insert �߰�, 2016-09-30
	void DrawBlock_ZW2017(AcGePoint3d insertPt, CString m_bname, double x_scale, double y_scale, double angle);
};

#endif // !defined(AFX_ARX_DRAW_H__32305845_022D_11D4_B03C_00A0242716EF__INCLUDED_)
