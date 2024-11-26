#pragma once
class CPrintFormData
{
public:
	CPrintFormData(void);
	virtual ~CPrintFormData(void);
	AcDbObjectId FormId;
	CString BlockName;
	CString BlockPath;
	CString LayoutName;
	double minpoint_X;
	double minpoint_Y;
	double maxpoint_X;
	double maxpoint_Y;
	AcGeScale3d scale;
	double angle;

};
class CPrintFormDataArray : public CArray<CPrintFormData, CPrintFormData&> {
public:
	CPrintFormDataArray() : CArray<CPrintFormData, CPrintFormData&>() {};

	CPrintFormDataArray(const CPrintFormDataArray &PrintFormDataArray) {
		RemoveAll();
		Append(PrintFormDataArray);	
	}

	void operator= (const CPrintFormDataArray &PrintFormDataArray) {
		RemoveAll();
		Append(PrintFormDataArray);	
	}
	
};
