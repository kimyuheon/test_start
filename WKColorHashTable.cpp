#include "StdAfx.h"
#include "WKColorHashTable.h"



CWKColorHashTable::CWKColorHashTable(void)
{
}

CWKColorHashTable::~CWKColorHashTable(void)
{
}


void CWKColorHashTable::ReSetContent()
{
	m_data.RemoveAll();
}
BOOL CWKColorHashTable::Add(AcDbObjectId LayerId,int coloridx,CString name)
{
	if(name.CompareNoCase(_T(""))==0)return FALSE;
	CWKColorHashData data;
	data.LayerId = LayerId;
	data.ColorIndex = coloridx;
	data.LayerName = name;
	m_data.Add(data);
	return TRUE;
}
AcDbObjectId CWKColorHashTable::Find(int coloridx,CString& name)
{
	CString ColName = _T("");
	AcDbObjectId LyId = NULL;
	for(int i = 0; i< m_data.GetCount(); i++)
	{
		if(m_data[i].ColorIndex == coloridx)
		{
			ColName = m_data[i].LayerName;
			LyId = m_data[i].LayerId;
			break;
		}
	}
	name = ColName;
	return LyId;
}
BOOL CWKColorHashTable::Delete(CString name)
{
	if(name.CompareNoCase(_T(""))==0)return FALSE;
	for(int i = 0; i< m_data.GetCount(); i++)
	{
		if(m_data[i].LayerName.CompareNoCase(name) == 0)
		{
			m_data.RemoveAt(i);
			break;
		}
	}
	return TRUE;
}