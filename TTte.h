#pragma once

typedef std::vector<AcDbObjectIdArray> AcDbObjectIdTable;
typedef AcArray<CtXRange*> CtXRangeArray;

// Ä³µåÇ¥¸¦ ¿¢¼¿·Î
class CTTte
{
public:
	CTTte(void);
	~CTTte(void);
	int		GetCurrentExcelVersion();
	bool	go();

	//X-Data
	void			setXData(AcDbEntity* pEnt, const LPCTSTR lpszBookSheetName,  const Adesk::Int32 row, const Adesk::Int16 col);
	void			setXData(AcDbObjectId objId, const LPCTSTR lpszBookSheetName,  const Adesk::Int32 row, const Adesk::Int16 col);
	bool			getXData(AcDbEntity* pEnt, CString& booksheetName, Adesk::Int32& row, Adesk::Int16& col);
	bool			getXData(AcDbObjectId objId, CString& booksheetName, Adesk::Int32& row, Adesk::Int16& col);


	AcDbObjectId	GetMaxY(AcDbObjectIdArray& ids, double& maxY);
	AcDbObjectId	GetMinX(AcDbObjectIdArray& ids, double& minY);
	bool			GetHSubLine(int& row2, AcDbLine* pLineV1, AcGePoint3d& ptInt);
	bool			GetVSubLine(const int& row1, const int& col1, AcDbLine* lu, int& row2, int&col2);
	bool			GetRightDown(const int& row1, const int& col1, int& row2, int& col2);
	bool			GetTextInRange(const int& row1, const int& col1, const int& row2, const int& col2, CString& str);
	void			UpdateXData(LPCTSTR lpszBook, LPCTSTR lpszSheet);
	double			GetMinTextHgt();
	void			GetHorLines(AcDbObjectIdArray& idsLineH);
	void			GetVerLines(AcDbObjectIdArray& idsLineV);
	bool			WriteToExcel();
	bool			updateDwg();
	bool			updateExcel();
	bool			removeXData();
	void			PrintText(AcDbObjectIdArray& ids);
	void			SortY(AcDbObjectIdArray& ids, bool bAscending=true);
	void			SortX(AcDbObjectIdArray& ids, bool bAscending=true);
	void			SortByString(AcDbObjectIdArray& ids, bool bAscending=true);


	
	


private:

	ExcelLib m_excel;

	
	AcDbObjectIdArray	m_idsText;		// ¹®ÀÚ
	
	AcDbObjectIdTable	m_tableH;
	AcDbObjectIdTable	m_tableV;
	double				m_gTol;
	ArxUtilsGe uge;



	int m_nTextSortType;
	int m_nExcelInsertType;
	int m_nTextProcType;
	int m_cbTextProcType;
	int m_nColorIndex;
	CString m_Layer;
	BOOL m_bSyncDwgExcel;
};
