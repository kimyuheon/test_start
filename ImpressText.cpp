
#include "StdAfx.h"
#include "CIM.h"
#include <tlhelp32.h>
#include "afxdialogex.h"

int GetFindCharCount(CString parm_string, char parm_find_char){
	int length = parm_string.GetLength(), find_count = 0; 
	for(int i = 0; i < length; i++){ 
		if(parm_string[i] == parm_find_char){
			find_count++;
		}
	} 
	return find_count; 
}

void CCIM::ImpressText(){
	CStdioFile txt_file;
	CString path, strMsg;
	GetDlgItem(IDC_PATHINPUT)->GetWindowText(path);
	if(path.IsEmpty()){
		strMsg.LoadStringW(IDS_NOTSET_PATH); // "��θ� �������� �ʾҽ��ϴ�"
		AfxMessageBox(strMsg);
		return;
	}
	// ���� ��ο��� ������ �ؽ�Ʈ ������ ����.
	// CFile::modeRead  ->  ������ �б� ���� ����. ( ������ ���ٸ� �����Ѵ�.)
	// CFile::typeText  ->  ������ �Ӽ��� �ؽ�Ʈ �������� �����Ѵ�. ( ���� ����)
	if(txt_file.Open(path, CFile::modeRead | CFile::typeText)){

		CString display_str, str;
		bool changeXY = IsDlgButtonChecked(IDC_XYCHANGE);
		bool chk2dPoly = IsDlgButtonChecked(IDC_2DPOLY);
		bool chk3dPoly = IsDlgButtonChecked(IDC_3DPOLY);

		AcDb2dPolyline *P2dline = new AcDb2dPolyline;
		AcDb3dPolyline *P3dline = new AcDb3dPolyline;

		int pointcnt;
		pointcnt = GetDlgItemInt(IDC_PCNT);
		CString cnt;
		cnt.Format(_T("%%.%df"), pointcnt);

		// ���Ͽ� ���̻� �ؽ�Ʈ ������ ���������� ���پ� �ݺ��ؼ� �о str ������ �����Ѵ�.
		while(txt_file.ReadString(str)){
			CString A1, A2, A3, A4;
			// �о���� ������ �ؽ�Ʈ�� display_str ������ �߰��Ѵ�.
			display_str += str;
			double x = 0.0, y = 0.0, z = 0.0;
			bool chkColon = IsDlgButtonChecked(IDC_Colon);
			bool chkSpace = IsDlgButtonChecked(IDC_SPACE);
			bool chkSet = IsDlgButtonChecked(IDC_SET);
			int count;

			if(chkColon)
				count = GetFindCharCount(str, ',');
			else if(chkSpace)
				count = GetFindCharCount(str, ' ');
			else if(chkSet){
				CString set;
				GetDlgItemText(IDC_SETSPACE, set);
				char* ss = LPSTR(LPCTSTR(set));
				count = GetFindCharCount(str, *ss);
			}

			if(count == 1){
				if(chkColon){
					AfxExtractSubString(A1, str, 0, ',');
					AfxExtractSubString(A2, str, 1, ',');
					x = _wtof(A1);
					y = _wtof(A2);
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
				}
				else if(chkSet){
					CString set;
					GetDlgItemText(IDC_SETSPACE, set);
					char* ss = LPSTR(LPCTSTR(set));

					AfxExtractSubString(A1, str, 0, *ss);
					AfxExtractSubString(A2, str, 1, *ss);
					x = _wtof(A1);
					y = _wtof(A2);
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
				}
				else if(chkSpace){
					AfxExtractSubString(A1, str, 0, ' ');
					AfxExtractSubString(A2, str, 1, ' ');
					x = _wtof(A1);
					y = _wtof(A2);
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
				}
			}
			else if(count == 2){
				if(chkColon){
					CString Text;
					AfxExtractSubString(A1, str, 0, ',');
					AfxExtractSubString(A2, str, 1, ',');
					AfxExtractSubString(A3, str, 2, ',');
					x = _wtof(A1);
					y = _wtof(A2);
					if(BZChk)
						z = _wtof(A3);
					if(CText.GetCheck())
						if(z == 0.0)
							if(isalpha(str.GetAt(0)))
								Text = A3;
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){				
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
					if(CText.GetCheck())
						appendText(Text, p3d);
				}
				if(chkSpace){
					CString Text;
					AfxExtractSubString(A1, str, 0, ' ');
					AfxExtractSubString(A2, str, 1, ' ');
					AfxExtractSubString(A3, str, 2, ' ');
					x = _wtof(A1);
					y = _wtof(A2);
					if(BZChk)
						z = _wtof(A3);
					if(CText.GetCheck())
						if(z == 0.0)
							if(isalpha(str.GetAt(0)))
								Text = A3;					
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){				
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
					if(CText.GetCheck())
						appendText(Text, p3d);
				}
			}
			else if(count == 3){
				if(chkColon){
					CString Text;
					AfxExtractSubString(A1, str, 0, ',');
					AfxExtractSubString(A2, str, 1, ',');
					AfxExtractSubString(A3, str, 2, ',');
					AfxExtractSubString(A4, str, 3, ',');
					x = _wtof(A1);
					y = _wtof(A2);
					if(BZChk)
						z = _wtof(A3);
					if(CText.GetCheck())
						if(z == 0.0)
							Text = A4;					
					if(x == 0.0 || y == 0.0)
						continue;
					AcGePoint3d p3d;
					if(!changeXY)
						p3d.set(x, y, z);
					else if(changeXY)
						p3d.set(y, x, z);
					if(CConline.GetCheck()){				
						if(chk2dPoly){
							AcDb2dVertex *P2dvert = new AcDb2dVertex;
							P2dvert->setPosition(p3d);
							P2dvert->close();
							P2dline->appendVertex(P2dvert);
						}
						else if(chk3dPoly){
							AcDb3dPolylineVertex *P3dvert = new AcDb3dPolylineVertex;
							P3dvert->setPosition(p3d);
							P3dvert->close();
							P3dline->appendVertex(P3dvert);
						}
					}
					if(BCutpoint.GetCheck()){
						bool chkPoint = IsDlgButtonChecked(IDC_POINTBTN);
						bool chkCircle = IsDlgButtonChecked(IDC_CIRCLEBTN);
						bool chkBlock = IsDlgButtonChecked(IDC_BLOCKBTN);
						if(chkPoint)
							appendPoint(p3d);
						else if(chkCircle)
							appendCircle(p3d);
						else if(chkBlock)
							appendBlock(p3d);
					}
					if(BZIns.GetCheck())
						appendZVal(p3d, cnt);
					if(CText.GetCheck())
						appendText(Text, p3d);
				}
			}
			// ��Ƽ���� �Ӽ��� ���� ����Ʈ ��Ʈ���� ���� ���� "\r\n" �� �־���� �ٹٲ��� ���ش�.
			// ReadString �Լ��� ������ �ؽ�Ʈ�� ������ �ش����� �����ִ� "\n" �� �о ������
			// ������ �ؽ�Ʈ�� ���ٳ��� "\n" �� �ִٰ� ������ �ݵ�� "\n" �� �ٿ��� �մϴ�.
			//display_str += "\r\n";
		}
		if(CConline.GetCheck()){
			if(chk2dPoly){
				append2dPoly(P2dline);
			}
			else if(chk3dPoly){
				append3dPoly(P3dline);
			}
		}
		// display_str ������ ����Ǿ� �ִ� ������ IDC_MY_TEXT ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
		//SetDlgItemText(IDC_MY_TEXT, display_str);

		// �۾��� ��ģ�Ŀ� ������ �ݴ´�.
		txt_file.Close();
	}
}