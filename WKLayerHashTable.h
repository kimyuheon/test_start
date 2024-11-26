#pragma once

class CWKLayerHashData
{
public:
	CString LayerName;
	AcDbObjectId LayerId;
	BOOL bIsOff;
	BOOL bIsFrozen;
	BOOL bIsLock;
	AcCmColor Color;
	AcDbObjectId LineTypeId;
	AcDb::LineWeight LineWeight;
	int ColorIndex;
	BOOL Plottable;
	
	
};
class CWKLayerHashTable
{
public:
	CWKLayerHashTable(void);
	~CWKLayerHashTable(void);
private:
	CArray <CWKLayerHashData,CWKLayerHashData> m_data;

public:
	//자료 초기화
	void ReSetContent();
	//데이터 추가
	BOOL Add(AcDbObjectId LayerId,int coloridx,CString name,BOOL bisoff,BOOL bisfrozen,BOOL bislock,AcCmColor color,AcDbObjectId lineid,AcDb::LineWeight lineweight,BOOL bisplottable);
	//자료 찾기
	BOOL  Find(CString LayerName,CWKLayerHashData& data);
	//자료 삭제
	BOOL Delete(CString name);
};
