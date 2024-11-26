#pragma once
class EntMake
{
public:
	EntMake(void);
	~EntMake(void);

  // data members
    int				m_colorindex;
	CString			m_strLayer;
	AcDbObjectId	m_textstyle;
	AcDbObjectId    m_idNew;
	CString			m_strLinetype;
	double			m_textsize;
	double			m_ltscl;
	AcDb::LineWeight m_LW;

	AcDbDatabase*	pDb;
	TCHAR			m_BlockName[128];

	void		setLayerAndColor(LPCTSTR lpszLayer, int colorindex);

	void		SetProperty(AcDbEntity* pEnt);
	void		GetProperty(AcDbEntity* pEnt);
	Acad::ErrorStatus appendToBlock(AcDbEntity* pEnt, AcDbObjectId& idEnt, const TCHAR * pcBlockName=ACDB_MODEL_SPACE, AcDbDatabase* pDb=NULL);

	// functions

	AcDbObjectId	makePolyline(AcGePoint3dArray& vertices);
	AcDbObjectId	make2dPolyline(AcDb::Poly2dType type, AcGePoint3dArray& vertices, double elevation=0,	Adesk::Boolean closed=Adesk::kFalse, double defStartWidth=0, double defEndWidth=0, AcGeDoubleArray* bulges=NULL);
	AcDbObjectId	makeText(AcGePoint3d ptPos, double hgt, double rot, LPCTSTR textString, AcDb::TextHorzMode horzMode, AcDb::TextVertMode vertMode, AcGeVector3d norm);
	AcDbObjectId	makeLine(const AcGePoint3d& pt1, const AcGePoint3d& pt2);
	// Helper
	
};

