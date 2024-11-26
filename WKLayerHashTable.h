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
	//�ڷ� �ʱ�ȭ
	void ReSetContent();
	//������ �߰�
	BOOL Add(AcDbObjectId LayerId,int coloridx,CString name,BOOL bisoff,BOOL bisfrozen,BOOL bislock,AcCmColor color,AcDbObjectId lineid,AcDb::LineWeight lineweight,BOOL bisplottable);
	//�ڷ� ã��
	BOOL  Find(CString LayerName,CWKLayerHashData& data);
	//�ڷ� ����
	BOOL Delete(CString name);
};
