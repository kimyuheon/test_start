#pragma once
#include "resource.h"
#include "WKUtils.h"

class CTextProcess
{
public:
	CTextProcess(void);
	~CTextProcess(void);

	//bisArea =  ���� ����� �Ұ����� �������� ���� 
	
	
	AcDbObjectId insertTextCMD(AcGePoint3d insPt, double textHeight,CString StyleName,double widthFactor,double angle,CString Lyname,int colorindex,CString strText,AcDb::TextVertMode& TextVPos,AcDb::TextHorzMode& TextHPos);
	
};
