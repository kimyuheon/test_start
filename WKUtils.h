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
				ads_printf(_T("\nLockDocManager::LockDocManager() Error! es(%d)\n"),es);//		ads_printf(_T("\n%s()완료.\n"),_T(__FUNCTION__));
			}
		}
	};
	~LockDocManager()
	{
		es=acDocManager->unlockDocument(m_pDoc);
		if( es!=Acad::eOk )	ads_printf(_T("\nLockDocManager::~LockDocManager() Error! es(%d)\n"),es);//		ads_printf(_T("\n%s()완료.\n"),_T(__FUNCTION__));
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
	//사용자로부터 Xref 을 선택 받는다.
	BOOL GetXrefEntity(CString msg,AcDbObjectId& ObjId,CString& Xrefname);
	BOOL GetPoint(AcGePoint3d& point);
	AcDbObjectId GetLayer(CString& LayerName,CString msg);
	AcDbObjectIdArray GetLayers(CString msg);
	//특정 객채의 레이어 아이디를 반환하라 
	AcDbObjectId GetLayer(AcDbObjectId& Entid);
	void RenameLayer(CString SourceName , CString TargetName);
	//도면층 컬러 가져오기
	int GetlayerColor(AcDbObjectId LyId);
	//도면층 컬러 가져오기
	BOOL SetlayerColor(int ColorIndex,AcDbObjectId LyId);
	
	BOOL SetlayerLock(AcDbDatabase* db, CString& Lyername, bool bisLock);
	//도면층의 이름으로 도면층 가져오기 
	BOOL FindLayer(CString name,AcDbObjectId& LyId);

	//현제 도면에 엔티티 삽입하기
	Acad::ErrorStatus AddEntityToModelSpace(AcDbObjectId &objId, AcDbEntity* pEntity, bool bCloseEntity);
	//블록에서 특정 태그만 뽑아 오기
	AcGePoint3dArray GetBlockReferenceAttributeTextBox(AcDbBlockReference *pBlockRef, CString sAttributeTag);
	//속성블록 정보 가져오기 
	void GetBlockReferenceAttributeList(AcDbObjectId blkid, CArray<AttBlock,AttBlock>& sAttributeTag);
	void GetBlockReferenceAttributeList(AcDbObjectId blkid, AttBlockArray& sAttributeTag);
	void SetBlockReferenceAttribute(AcDbObjectId blkid, CString sAttributeTag, CString strValue);
	//블록 내의 텍스트 박스만 모은다.
	AcGePoint3dArray GetAttributeBlkRefVisibleBox(AcDbBlockReference *pBlockRef);
	//텍스트 박스 얻기
	AcGePoint3dArray GetTextBox(AcDbObjectId idText);
	//텍스트의 너비 얻어오기
	double GetTextWidth(AcDbObjectId idText);
	//텍스트의 너비 얻어오기
	double GetTextWidth(CString sText, CString sTextStyle, double dTextHeight);
	//텍스트 스타일 ID 얻기
	AcDbObjectId GetTextStyleId(CString sTextStyleName);
	//블록의 이름 가져오기
	CString getBlockReferenceName(AcDbBlockReference *pBlkRef);
	//모든 도면층 모으기
	void iterateLayers(AcDbObjectIdArray& layerarr);
	//현재 도면에 올라몬  도면층 모으기 Xref 제외ㄴ
	void OnlyCurDwgiterateLayers(AcDbObjectIdArray& layerarr) ;
	
	//Xref가 사용하는 도면층 모으기 
	void XRefiterateLayers(AcDbObjectIdArray& layerarr,CString XRefName) ;
	//도면층 생성
	BOOL createLayer(CString name,int ColorIndex,AcDbObjectId& LyId);
	
	//도면층 이름 가져오기
	void getLayerName(AcDbObjectId idLayer, CString& strLayerName);
	//도면층 이름 저장하기
	void setLayerName(AcDbObjectId idLayer, CString& strLayerName);
	//특정 도면층의 모든 객체를 다른 도면층로 옮긴다.
	long MoveAllEntityInLayer(CString &strSoureLyName,AcDbObjectId& TargetLyId,AcDbDatabase* pDB);
	void GetAllEntity(CString LayerName, AcDbObjectIdArray& entities);
	//도면층에 걸린 모든 객체를 삭제하자
	long DeleteAllEntityInLayer(CString& strLyName,AcDbDatabase* pDB);
	long DeleteAllEntityInLayer(AcDbObjectId &LyId,AcDbDatabase* pDB);      
	long SelectEntityInLayer(AcDbObjectIdArray& IdArray,CString& strLyName);

	//특정 조건의 도면층만 모아온다.
	void LayerFilter(AcDbObjectIdArray& sourcearr,AcDbObjectIdArray& targetarr,BOOL bIsFrozen,BOOL bIsOff);

	void LayerFilterDelete(void);
	void layersReferencedPurge ();
	BOOL PurgeLayer(AcDbObjectIdArray& ids);
	//선택한 객체의 도면층만 복원을 클릭 했을때 사용자에게 외부참조된 폼속의 객체의 도면층와 폼의 경로를 얻어오자
	BOOL GetXrefObjectLayerAndXrefPath(CString& LayerName,CString& XrefPath);
	//Xref 의 경로를 알아오기
	CString XrefGetPath(AcDbObjectId id);
	//컬러 픽커 대화상자
	void GetColorPicker(int & newColorIndex);
	//트루컬러 픽커 대화상자
	void GetTrueColorPicker(int & newColorIndex,AcDbObjectId LyId=NULL);
	//라인타입의 이름을 가져온다.
	CString GetLineTypeName(AcDbObjectId& linetype_id);
	//현재 도면에 올라와있는 모든 라인타입을 가져온다.
	BOOL  GetAllLineType(AcDbObjectIdArray& layerarr);
	//외부 파일에서 모든 라인 타입을 가져온다.
	BOOL  GetAllLineType(AcDbObjectIdArray& layerarr ,CString Path);
	BOOL  GetAllLineType(AcDbDatabase* db , AcDbObjectIdArray& layerarr ,CString Path);
	
	//파일 열기 대화상자
	BOOL FileOpen(CString& PathName,CString& FileName);
	//파일 저장 대화상자
	BOOL FileSave(CString DefaultFileName,CString& PathName,CString& FileName);
	//폴더 선택 받기
	BOOL GetFolderPath(CString& pszPathname );
	//라인 웨이트 를 스트링으로 변경 한다.
	LPCTSTR lineWeightTypeToStr(AcDb::LineWeight type, CString& str);
	//스트링을 라인 웨이트로 변경 
	int StrTolineWeightType(CString str, AcDb::LineWeight& type);
	//대표적인 색상의 이름을 가져온다.
	LPCTSTR RGBToColorName(int r , int g, int b , CString& str);
	//블록 분해 하기
	/* bisoriginlayer = 원래 블록의 도면층로 폭파 여부
	LayerName = 다른도면층로 폭파시 도면층 이름
	bisNewlayer = 새 도면층에 폭파할것인지 여부
	bisbylayer = 색상 by layer 인지 여부
	bisbylock = 색상 by block 인지 여부
	bisorigincolor = 원래 색상 여부
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
//블록이지만 외부 참조인지 여부
BOOL IsXref(AcDbObjectId& ObjId);
//도면층 필터 삭제
void ASDKLayerFilterDelete_DeleteLayerFilters(void);
//사용자가 지정한 레이어를 출력이 가능하도록 하거나 출력 불가능하게 지정 
BOOL SetlayerPlottable(AcDbObjectId& LyId,bool bisPlottable);
BOOL getTextStyleList(CStringArray& lst);
void GetAllLayers(CStringArray& lst);
BOOL SSGET(AcDbObjectIdArray& ids,CString strfilter);
BOOL SSGET(AcDbObjectIdArray& ids,AcGePoint3d p1,AcGePoint3d p2 ,CString strfilter);
void GetTextStyleName(AcDbObjectId idText, CString& strStyleName);
bool AsdkGetBlockAttribute(AcDbObjectId objId, CString& name, AcGePoint3d& pos,AcDbExtents& Extents);

};
