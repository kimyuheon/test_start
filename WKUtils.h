#pragma once
class LockDocManager
{
private:
	Acad::ErrorStatus	es;
	AcApDocument	*m_pDoc;
public:
	LockDocManager()
	{
		m_pDoc = curDoc();
		if(m_pDoc!=NULL){
			es=acDocManager->lockDocument(m_pDoc);
			if( es!=Acad::eOk ){	
				ads_printf(_T("\nLockDocManager::LockDocManager() Error! es(%d)\n"),es);//		ads_printf(_T("\n%s()�Ϸ�.\n"),_T(__FUNCTION__));
			}
		}
	};
	~LockDocManager()
	{
		es=acDocManager->unlockDocument(m_pDoc);
		if( es!=Acad::eOk )	ads_printf(_T("\nLockDocManager::~LockDocManager() Error! es(%d)\n"),es);//		ads_printf(_T("\n%s()�Ϸ�.\n"),_T(__FUNCTION__));
	};
};

class CWKUtils
{
public:
	CWKUtils(void);
	~CWKUtils(void);

	Acad::ErrorStatus postToDatabase (AcDbEntity *pEnt, AcDbObjectId& EntID);

	void			Polar(const AcGePoint3d &base, double angle, double dist, AcGePoint3d &result);
	void			Polar(const AcGePoint3d &base, const AcGePoint3d &toPnt, double length, AcGePoint3d &result);
	void			Polar(const AcGePoint3d &base, const AcGeVector3d &vector, double dist, AcGePoint3d &result);
	void Transform(AcGePoint3d& pt, AcGeVector3d vec);
	static double RTD(const double rads);
	static double DTR(const double degrees);	
	AcDbObjectId CreateTextStyle(CString fontName,CString bigFontName,CString textStyleName);
		void fLayerInformation();
	BOOL GetTextStyle(CString& name, AcDbObjectId& Style);
	//����ڷκ��� Xref �� ���� �޴´�.
	BOOL GetXrefEntity(CString msg,AcDbObjectId& ObjId,CString& Xrefname);
	BOOL GetPoint(AcGePoint3d& point);
	AcDbObjectId GetLayer(CString& LayerName,CString msg);
	AcDbObjectIdArray GetLayers(CString msg);
	//Ư�� ��ä�� ���̾� ���̵� ��ȯ�϶� 
	AcDbObjectId GetLayer(AcDbObjectId& Entid);
	void RenameLayer(CString SourceName , CString TargetName);
	//������ �÷� ��������
	int GetlayerColor(AcDbObjectId LyId);
	//������ �÷� ��������
	BOOL SetlayerColor(int ColorIndex,AcDbObjectId LyId);
	
	BOOL SetlayerLock(AcDbDatabase* db, CString& Lyername, bool bisLock);
	//�������� �̸����� ������ �������� 
	BOOL FindLayer(CString name,AcDbObjectId& LyId);

	//���� ���鿡 ��ƼƼ �����ϱ�
	Acad::ErrorStatus AddEntityToModelSpace(AcDbObjectId &objId, AcDbEntity* pEntity, bool bCloseEntity);
	//��Ͽ��� Ư�� �±׸� �̾� ����
	AcGePoint3dArray GetBlockReferenceAttributeTextBox(AcDbBlockReference *pBlockRef, CString sAttributeTag);
	//�Ӽ���� ���� �������� 
	void GetBlockReferenceAttributeList(AcDbObjectId blkid, CArray<AttBlock,AttBlock>& sAttributeTag);
	void GetBlockReferenceAttributeList(AcDbObjectId blkid, AttBlockArray& sAttributeTag);
	void SetBlockReferenceAttribute(AcDbObjectId blkid, CString sAttributeTag, CString strValue);
	//��� ���� �ؽ�Ʈ �ڽ��� ������.
	AcGePoint3dArray GetAttributeBlkRefVisibleBox(AcDbBlockReference *pBlockRef);
	//�ؽ�Ʈ �ڽ� ���
	AcGePoint3dArray GetTextBox(AcDbObjectId idText);
	//�ؽ�Ʈ�� �ʺ� ������
	double GetTextWidth(AcDbObjectId idText);
	//�ؽ�Ʈ�� �ʺ� ������
	double GetTextWidth(CString sText, CString sTextStyle, double dTextHeight);
	//�ؽ�Ʈ ��Ÿ�� ID ���
	AcDbObjectId GetTextStyleId(CString sTextStyleName);
	//����� �̸� ��������
	CString getBlockReferenceName(AcDbBlockReference *pBlkRef);
	//��� ������ ������
	void iterateLayers(AcDbObjectIdArray& layerarr);
	//���� ���鿡 �ö��  ������ ������ Xref ���ܤ�
	void OnlyCurDwgiterateLayers(AcDbObjectIdArray& layerarr) ;
	
	//Xref�� ����ϴ� ������ ������ 
	void XRefiterateLayers(AcDbObjectIdArray& layerarr,CString XRefName) ;
	//������ ����
	BOOL createLayer(CString name,int ColorIndex,AcDbObjectId& LyId);
	
	//������ �̸� ��������
	void getLayerName(AcDbObjectId idLayer, CString& strLayerName);
	//������ �̸� �����ϱ�
	void setLayerName(AcDbObjectId idLayer, CString& strLayerName);
	//Ư�� �������� ��� ��ü�� �ٸ� �������� �ű��.
	long MoveAllEntityInLayer(CString &strSoureLyName,AcDbObjectId& TargetLyId,AcDbDatabase* pDB);
	void GetAllEntity(CString LayerName, AcDbObjectIdArray& entities);
	//�������� �ɸ� ��� ��ü�� ��������
	long DeleteAllEntityInLayer(CString& strLyName,AcDbDatabase* pDB);
	long DeleteAllEntityInLayer(AcDbObjectId &LyId,AcDbDatabase* pDB);      
	long SelectEntityInLayer(AcDbObjectIdArray& IdArray,CString& strLyName);

	//Ư�� ������ �������� ��ƿ´�.
	void LayerFilter(AcDbObjectIdArray& sourcearr,AcDbObjectIdArray& targetarr,BOOL bIsFrozen,BOOL bIsOff);

	void LayerFilterDelete(void);
	void layersReferencedPurge ();
	BOOL PurgeLayer(AcDbObjectIdArray& ids);
	//������ ��ü�� �������� ������ Ŭ�� ������ ����ڿ��� �ܺ������� ������ ��ü�� �������� ���� ��θ� ������
	BOOL GetXrefObjectLayerAndXrefPath(CString& LayerName,CString& XrefPath);
	//Xref �� ��θ� �˾ƿ���
	CString XrefGetPath(AcDbObjectId id);
	//�÷� ��Ŀ ��ȭ����
	void GetColorPicker(int & newColorIndex);
	//Ʈ���÷� ��Ŀ ��ȭ����
	void GetTrueColorPicker(int & newColorIndex,AcDbObjectId LyId=NULL);
	//����Ÿ���� �̸��� �����´�.
	CString GetLineTypeName(AcDbObjectId& linetype_id);
	//���� ���鿡 �ö���ִ� ��� ����Ÿ���� �����´�.
	BOOL  GetAllLineType(AcDbObjectIdArray& layerarr);
	//�ܺ� ���Ͽ��� ��� ���� Ÿ���� �����´�.
	BOOL  GetAllLineType(AcDbObjectIdArray& layerarr ,CString Path);
	BOOL  GetAllLineType(AcDbDatabase* db , AcDbObjectIdArray& layerarr ,CString Path);
	
	//���� ���� ��ȭ����
	BOOL FileOpen(CString& PathName,CString& FileName);
	//���� ���� ��ȭ����
	BOOL FileSave(CString DefaultFileName,CString& PathName,CString& FileName);
	//���� ���� �ޱ�
	BOOL GetFolderPath(CString& pszPathname );
	//���� ����Ʈ �� ��Ʈ������ ���� �Ѵ�.
	LPCTSTR lineWeightTypeToStr(AcDb::LineWeight type, CString& str);
	//��Ʈ���� ���� ����Ʈ�� ���� 
	int StrTolineWeightType(CString str, AcDb::LineWeight& type);
	//��ǥ���� ������ �̸��� �����´�.
	LPCTSTR RGBToColorName(int r , int g, int b , CString& str);
	//��� ���� �ϱ�
	/* bisoriginlayer = ���� ����� �������� ���� ����
	LayerName = �ٸ��������� ���Ľ� ������ �̸�
	bisNewlayer = �� �������� �����Ұ����� ����
	bisbylayer = ���� by layer ���� ����
	bisbylock = ���� by block ���� ����
	bisorigincolor = ���� ���� ����
	*/
void explodeBlockReference( AcDbObjectId BlockId,
						   BOOL bisoriginlayer=FALSE,
						   CString LayerName=_T(""),
						   BOOL bisNewlayer=FALSE,
						   BOOL bisbylayer=FALSE,
						   BOOL bisbylock=FALSE ,
						   BOOL bisorigincolor = TRUE);

BOOL BlockSSGET(AcDbObjectIdArray& ids,CString Msg);
void ss2Array(ads_name ss, AcDbObjectIdArray& ids);
void ss2Array_all(ads_name ss, AcDbObjectIdArray& ids);
BOOL enameToObjId(ads_name ent, AcDbObjectId& objId);
//��������� �ܺ� �������� ����
BOOL IsXref(AcDbObjectId& ObjId);
//������ ���� ����
void ASDKLayerFilterDelete_DeleteLayerFilters(void);
//����ڰ� ������ ���̾ ����� �����ϵ��� �ϰų� ��� �Ұ����ϰ� ���� 
BOOL SetlayerPlottable(AcDbObjectId& LyId,bool bisPlottable);
BOOL getTextStyleList(CStringArray& lst);
void GetAllLayers(CStringArray& lst);
BOOL SSGET(AcDbObjectIdArray& ids,CString strfilter);
BOOL SSGET(AcDbObjectIdArray& ids,AcGePoint3d p1,AcGePoint3d p2 ,CString strfilter);
void GetTextStyleName(AcDbObjectId idText, CString& strStyleName);
bool AsdkGetBlockAttribute(AcDbObjectId objId, CString& name, AcGePoint3d& pos,AcDbExtents& Extents);

};
