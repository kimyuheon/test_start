#pragma once

class CWKColorHashData
{
public:
	AcDbObjectId LayerId;
	int ColorIndex;
	CString LayerName;
};
class CWKColorHashTable
{
public:
	CWKColorHashTable(void);
	~CWKColorHashTable(void);
private:
	CArray <CWKColorHashData,CWKColorHashData> m_data;

public:
	//�ڷ� �ʱ�ȭ
	void ReSetContent();
	//������ �߰�
	BOOL Add(AcDbObjectId LayerId,int coloridx,CString name);
	//�ڷ� ã��
	AcDbObjectId  Find(int coloridx,CString& name);
	//�ڷ� ����
	BOOL Delete(CString name);
};
