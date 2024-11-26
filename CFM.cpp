// CFM.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CFM.h"
#include "afxdialogex.h"
#include "EntMake.h"
#include "Utils.h"
#include <utility>


// CCFM ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCFM, CAcUiDialog)

CCFM::CCFM(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CCFM::IDD, pParent)
	, m_strCrossVLineLen(_T(""))
	, m_strCrossDstDecimalPoint(_T(""))
	, m_strCrossHgtDecimalPoint(_T(""))
	, m_strGroundLevel(_T(""))
	, m_bCheckStationInput(FALSE)
	, m_bCheckDstHgtDisp(FALSE)
	, m_nDispStandard(0)
	, m_bCheckExceptZeroZ(FALSE)
	, m_bCheckOnlyCrossPt(FALSE)
{

	m_strLayer=_T("0");
}

CCFM::~CCFM()
{
}

void CCFM::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CROSSLAYER, m_btnLayer);
	DDX_Control(pDX, IDC_BTNSELENT, m_btnEntSel);
	DDX_Text(pDX, IDC_EDIT_CROSSVLINELEN, m_strCrossVLineLen);
	DDX_Text(pDX, IDC_EDIT_CROSSDSTDECPT, m_strCrossDstDecimalPoint);
	DDX_Text(pDX, IDC_EDIT_CROSSHGTDECPT, m_strCrossHgtDecimalPoint);
	DDX_Text(pDX, IDC_EDIT_GROUNDLEVEL, m_strGroundLevel);
	DDX_Check(pDX, IDC_CHECKSTATIONINPUT, m_bCheckStationInput);
	DDX_Check(pDX, IDC_CHECKDSTHGTDISP, m_bCheckDstHgtDisp);
	DDX_Radio(pDX, IDC_RADIORELATIVE, m_nDispStandard);
	DDX_Check(pDX, IDC_CHECKEXCEPTZEROZ, m_bCheckExceptZeroZ);
	DDX_Check(pDX, IDC_CHECKONLYCROSSPT, m_bCheckOnlyCrossPt);
}


BEGIN_MESSAGE_MAP(CCFM, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_CORSSLINESEL, &CCFM::OnBnClickedButtonCorsslinesel)
	ON_BN_CLICKED(IDCANCEL, &CCFM::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_CROSSDSTDECPT, &CCFM::OnDeltaposSpnCrossdstdecpt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPN_CROSSHGTDECPT, &CCFM::OnDeltaposSpnCrosshgtdecpt)
	ON_BN_CLICKED(IDC_CHECKDSTHGTDISP, &CCFM::OnBnClickedCheckdsthgtdisp)
	ON_BN_CLICKED(IDC_BTNSELENT, &CCFM::OnBnClickedBtnselent)
END_MESSAGE_MAP()


// CCFM �޽��� ó�����Դϴ�.



BOOL CCFM::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//������ �̸� ����
	CStringArray layers;
	Utils::GetLayerNames(layers);

	HICON hIcon = LoadIcon(_hdllInstance, MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	for(int i=0;i<layers.GetCount();i++){
		m_btnLayer.AddString(layers.GetAt(i));
	}//end-if
	if(m_btnLayer.GetCount()>0) {
		AcDbDatabase* pCurDb = acdbHostApplicationServices()->workingDatabase();
		AcDbObjectId layerId = pCurDb->clayer();
		CString strLayer;
		Utils::GetLayerName(layerId, strLayer);
		int nFind=m_btnLayer.FindStringExact(0, strLayer);
		if(nFind!=-1) m_btnLayer.SetCurSel(nFind);
		else m_btnLayer.SetCurSel(0);
	}


	// ��ü���� ��ư �ʱ�ȭ
	m_btnEntSel.AutoLoad();
	m_strCrossVLineLen=L"0.300";
	m_strCrossDstDecimalPoint = L"2";
	m_strCrossHgtDecimalPoint = L"2";
	m_strGroundLevel=L"0.300";
	m_bCheckExceptZeroZ=TRUE;

	if(m_bCheckDstHgtDisp==TRUE){
		GetDlgItem(IDC_RADIORELATIVE)->EnableWindow();
		GetDlgItem(IDC_RADIOABSOLUTE)->EnableWindow();
	}else{
		GetDlgItem(IDC_RADIORELATIVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIOABSOLUTE)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



// Ⱦ�� ����
void CCFM::DrawCrossSectionLine(AcDbObjectId idCross, AcGePoint3d sp, AcGePoint3d ep, AcGePoint3d ip)
{
	AcGePlane pln;
	AcDbPolyline poly2d1;
	AcGePoint3d spCrosest, epCrosest;



	// Ⱦ���ۼ����� �Ҽ��� ����(m_strCrossHgtDecimalPoint)
	double dblCrossVLineLen=_ttof(m_strCrossVLineLen);
	CString strTxt(_T(""));	
	CString crossVLineFmt("%.");
	crossVLineFmt=crossVLineFmt+m_strCrossHgtDecimalPoint+L"f";
	
	// Ⱦ���ۼ��Ÿ� �Ҽ��� ����(m_strCrossHgtDecimalPoint)
	CString crossHLineFmt("%.");
	crossHLineFmt=crossHLineFmt+m_strCrossDstDecimalPoint+L"f";



	AcGePoint3d spCrossLine, epCrossLine;
	double dstCenter;
	//1. Ⱦ�ܼ��� ����,����	
	AcDbObjectPointer<AcDbEntity> pECrossLine(idCross, AcDb::kForRead);  // Ⱦ�ܼ�
	if(pECrossLine.openStatus() == Acad::eOk){
		
		
		if(pECrossLine->isA() == AcDbLine::desc()){
			AcDbLine* pLine=AcDbLine::cast(pECrossLine);
			if(pLine == NULL){
				pECrossLine->close();
				return; // Ⱦ�� ���� �Ұ�
			}			
			pLine->getStartPoint(spCrossLine);
			pLine->getEndPoint(epCrossLine);
			pLine->getClosestPointTo(sp, spCrosest);
			pLine->getClosestPointTo(ep, epCrosest);						
			AcGePoint2d pt1=spCrossLine.convert2d(pln);
			AcGePoint2d pt2=epCrossLine.convert2d(pln);
			poly2d1.addVertexAt(0, pt1);
			poly2d1.addVertexAt(0, pt2);
			

		}else if(pECrossLine->isA() == AcDbPolyline::desc()) {
			AcDbPolyline* pPolyLine=AcDbPolyline::cast(pECrossLine);
			if(pPolyLine == NULL){
				pECrossLine->close();
				return; // Ⱦ�� ���� �Ұ�
			}			
			pPolyLine->getStartPoint(spCrossLine);
			pPolyLine->getEndPoint(epCrossLine);			
			pPolyLine->getClosestPointTo(sp, spCrosest);
			pPolyLine->getClosestPointTo(ep, epCrosest);
			AcGePoint2d pt2d;
			for (unsigned int i=0; i<pPolyLine->numVerts(); i++){
				pPolyLine->getPointAt(i, pt2d);
				poly2d1.addVertexAt(0, pt2d);
			}	

		}else if(pECrossLine->isA() == AcDb2dPolyline::desc()) {
			AcDb2dPolyline* pPolyLine=AcDb2dPolyline::cast(pECrossLine);
			if(pPolyLine == NULL){
				pECrossLine->close();
				return; // Ⱦ�� ���� �Ұ�
			}		
			AcGePoint3dArray pts;
			AcDb2dVertex* vertex;
			AcDbObjectIterator* vertexIter = pPolyLine->vertexIterator();
			for (; !vertexIter->done(); vertexIter->step()) {
				if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
					if (vertex->vertexType() != AcDb::k2dSplineCtlVertex) {
						pts.append(pPolyLine->vertexPosition(*vertex));        // returns WCS		
					}
					vertex->close();
				}
			}
			delete vertexIter;
			pPolyLine->getStartPoint(spCrossLine);
			pPolyLine->getEndPoint(epCrossLine);			
			pPolyLine->getClosestPointTo(sp, spCrosest);
			pPolyLine->getClosestPointTo(ep, epCrosest);
			AcGePoint2d pt2d;
			for (unsigned int i=0; i<pts.length(); i++){
				pt2d=pts[i].convert2d(pln);
				poly2d1.addVertexAt(0, pt2d);
			}	
		}else if(pECrossLine->isA() == AcDb3dPolyline::desc()) {
			AcDb3dPolyline* pPolyLine=AcDb3dPolyline::cast(pECrossLine);
			if(pPolyLine == NULL){
				pECrossLine->close();
				return; // Ⱦ�� ���� �Ұ�
			}		
			AcGePoint3dArray pts;
			AcDb2dVertex* vertex;
			AcDbObjectIterator* vertexIter = pPolyLine->vertexIterator();
			for (; !vertexIter->done(); vertexIter->step()) {
				if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) {
					if (vertex->vertexType() != AcDb::k3dControlVertex) {
						pts.append(vertex->position());     
					}
					vertex->close();
				}
			}
			delete vertexIter;
			pPolyLine->getStartPoint(spCrossLine);
			pPolyLine->getEndPoint(epCrossLine);			
			pPolyLine->getClosestPointTo(sp, spCrosest);
			pPolyLine->getClosestPointTo(ep, epCrosest);
			AcGePoint2d pt2d;
			for (unsigned int i=0; i<pts.length(); i++){
				pt2d=pts[i].convert2d(pln);
				poly2d1.addVertexAt(0, pt2d);
			}

		}
	}else return;

	// 5. �־��� �� �Ǵ� �������� �����Ѵ�.		
	AcDbObjectIdArray ids;

	ads_name ss;
	struct resbuf *filter=ads_buildlist(-4, "<OR", 
										RTDXF0, _T("LINE"),  
										RTDXF0, _T("POLYLINE"),  
										RTDXF0, _T("LWPOLYLINE"),  
										-4, "OR>",
										RTNONE);
	if(ads_ssget(_T("C"), asDblArray(spCrossLine), asDblArray(epCrossLine), NULL, ss) != RTNORM){		
		ads_ssfree(ss);
		return;
	}
	if(filter) ads_relrb(filter);
	Utils::ss2Array(ss, ids);



	AcGePoint2d sp2d=sp.convert2d(pln);
	AcGePoint2d ep2d=ep.convert2d(pln);	
	AcGePoint3d sp3d, ep3d;
	poly2d1.getStartPoint(sp3d);
	poly2d1.getEndPoint(ep3d);
	AcGePoint2d spCrossLine2d = sp3d.convert2d(pln);
	AcGePoint2d epCrossLine2d = ep3d.convert2d(pln);
	
	bool bForwardDirection=true;
	if(spCrosest.distanceTo(sp3d) > spCrosest.distanceTo(ep3d)){
		poly2d1.reverseCurve();
		bForwardDirection=false;
	}
	
	long sslength=ids.length();
	const ACHAR* temp = NULL;
	
	
	AcGePoint3dArray pts2;
	// Ⱦ�ܼ��� ����� ������ ����
	for(long i = 0; i < sslength; i++){		
		AcDbObjectPointer<AcDbEntity> pEContourLine(ids[i], AcDb::kForRead); // ���	
		
		if(pEContourLine.openStatus() == Acad::eOk){
			AcGePoint2d pt2d;
			AcGePoint3dArray pts;
			if(pEContourLine->isA() == AcDbPolyline::desc()) {
				AcDbPolyline* pPolyLine=AcDbPolyline::cast(pEContourLine);
				if(pPolyLine != NULL){
					AcDbPolyline poly2d2;
					for (unsigned int i=0; i<pPolyLine->numVerts(); i++){
						pPolyLine->getPointAt(i, pt2d);
						poly2d2.addVertexAt(i, pt2d);
					}//end-for
					// Ⱦ�ܼ��� ����� ������ ���ϱ�
					AcGePoint3dArray IntersectionPoints;
					if(poly2d1.intersectWith(&poly2d2, AcDb::kOnBothOperands, IntersectionPoints)==Acad::eOk){		
						for(int k=0;k<IntersectionPoints.length();k++){
							AcGePoint3d ptInt=IntersectionPoints.at(k);
							ptInt.z=pPolyLine->elevation();  // Elevation
							if(m_bCheckExceptZeroZ==TRUE){ // ���� 0�� ��, ���� ����
								strTxt.Format(crossVLineFmt,ptInt.z);
								ptInt.z=_ttof(strTxt);
								if(fabs(ptInt.z)>0.000001) pts2.append(ptInt);							
							}else pts2.append(ptInt);
						}
					}//end-if
				}//end-if

			}else if(pEContourLine->isA() == AcDb2dPolyline::desc()) {
				AcDb2dPolyline* pPolyLine=AcDb2dPolyline::cast(pEContourLine);
				if(pPolyLine != NULL){
					AcDbObjectIterator* pIter = pPolyLine->vertexIterator();
					for (; !pIter->done(); pIter->step()) {
						AcDbObjectId vid = pIter->objectId();
						AcDbObjectPointer<AcDb2dVertex> pVertex(vid, AcDb::kForRead);
						if(pVertex.openStatus() == Acad::eOk)
						{						
							if (pVertex->vertexType() != AcDb::k2dSplineCtlVertex) {
								AcGePoint3d pt = pVertex->position();
								pts.append(pt);     
							}
						}
					}
					delete pIter;

	
					AcDbPolyline poly2d2;
					for (unsigned int i=0; i<pts.length(); i++){
						pt2d=pts[i].convert2d(pln);
						poly2d2.addVertexAt(i, pt2d);
					}//end-for
					// Ⱦ�ܼ��� ����� ������ ���ϱ�
					AcGePoint3dArray IntersectionPoints;
					if(poly2d1.intersectWith(&poly2d2, AcDb::kOnBothOperands, IntersectionPoints)==Acad::eOk){		
						for(int k=0;k<IntersectionPoints.length();k++){
							AcGePoint3d ptInt=IntersectionPoints.at(k);							
							ptInt.z=pPolyLine->elevation();  // Elevation	
							if(m_bCheckExceptZeroZ==TRUE){ // ���� 0�� ��, ���� ����
								strTxt.Format(crossVLineFmt,ptInt.z);
								ptInt.z=_ttof(strTxt);
								if(fabs(ptInt.z)>0.000001) pts2.append(ptInt);							
							}else pts2.append(ptInt);
					
						}
					}//end-if
				}//end-if

			}else if(pEContourLine->isA() == AcDb3dPolyline::desc()) {
				AcDb3dPolyline* pPolyLine=AcDb3dPolyline::cast(pEContourLine);
				if(pPolyLine != NULL){
					AcDbObjectIterator* pIter = pPolyLine->vertexIterator();
					for (; !pIter->done(); pIter->step()) {
						AcDbObjectId vid = pIter->objectId();
						AcDbObjectPointer<AcDb3dPolylineVertex> pVertex(vid, AcDb::kForRead);
						if(pVertex.openStatus() == Acad::eOk)
						{						
							if (pVertex->vertexType() != AcDb::k3dControlVertex) {
								AcGePoint3d pt = pVertex->position();
								pts.append(pt);     
							}
						}
					}
					delete pIter;

					AcDbPolyline poly2d2;
					for (unsigned int i=0; i<pts.length(); i++){
						pt2d=pts[i].convert2d(pln);
						poly2d2.addVertexAt(i, pt2d);
					}//end-for

					// Ⱦ�ܼ��� ����� ������ ���ϱ�
					AcGePoint3dArray IntersectionPoints;			
					if(poly2d1.intersectWith(&poly2d2, AcDb::kOnBothOperands, IntersectionPoints)==Acad::eOk){	
						for(int k=0;k<IntersectionPoints.length();k++){
							AcGePoint3d ptInt=IntersectionPoints.at(k);								
							AcGePoint3d p1 = ptInt;
							AcGePoint3d p2 = ptInt;
							p2.z += 100;
							AcDbLine line(p1, p2);
							AcGePoint3dArray ItsPoints;
							
							if(pPolyLine->intersectWith(&line,AcDb::kExtendArg, ItsPoints)==Acad::eOk){								
								ptInt.z=ItsPoints[0].z;
							}//end-if
							if(m_bCheckExceptZeroZ==TRUE){ // ���� 0�� ��, ���� ����
								strTxt.Format(crossVLineFmt,ptInt.z);
								ptInt.z=_ttof(strTxt);
								if(fabs(ptInt.z)>0.000001) pts2.append(ptInt);							
							}else pts2.append(ptInt);
						
						}
					}//end-if
				}//end-if


			}else if(pEContourLine->isA() == AcDbLine::desc()) {
				AcDbLine* pLine=AcDbLine::cast(pEContourLine);
				if(pLine != NULL){
					AcDbPolyline poly2d2;
					AcGePoint3d spContour, epContour;
					pLine->getStartPoint(spContour);					
					pLine->getEndPoint(epContour);
					poly2d2.addVertexAt(0, spContour.convert2d(pln));
					poly2d2.addVertexAt(1, epContour.convert2d(pln));
					// Ⱦ�ܼ��� ����� ������ ���ϱ�
					AcGePoint3dArray IntersectionPoints;
					if(poly2d1.intersectWith(&poly2d2, AcDb::kOnBothOperands, IntersectionPoints)==Acad::eOk){		
						for(int k=0;k<IntersectionPoints.length();k++){
							AcGePoint3d ptInt=IntersectionPoints.at(k);
							AcGePoint3d p1 = ptInt;
							AcGePoint3d p2 = ptInt;
							p2.z += 100;
							AcDbLine line(p1, p2);
							AcGePoint3dArray ItsPoints;
							
							if(pLine->intersectWith(&line,AcDb::kExtendArg, ItsPoints)==Acad::eOk){								
								ptInt.z=ItsPoints[0].z;
							}//end-if							
							if(m_bCheckExceptZeroZ==TRUE){ // Z���� 0�� ��, ���� ����
								strTxt.Format(crossVLineFmt,ptInt.z);
								ptInt.z=_ttof(strTxt);
								if(fabs(ptInt.z)>0.000001) pts2.append(ptInt);							
							}else pts2.append(ptInt);							
						}//end-for
					}//end-if
				}//end-if

			}//end-if-else

			pEContourLine->close();
		}
		pECrossLine->close();
	}//end-if

	if(pts2.length()<1) return;


	// ����
	AcGePoint3dArray ptsSorted;
	AcGePoint3d pt3d, ptMin;
	AcGePoint2d pt2d;
	poly2d1.getStartPoint(pt3d);
	spCrossLine2d=pt3d.convert2d(pln);
	while(pts2.length()>0){
		double minDst = DBL_MAX, dst=0;	
		int nMinInx=-1;
		AcGePoint3d pt;	
		for(int i=0;i<pts2.length();i++){
			pt3d=pts2.at(i);
			pt2d=pt3d.convert2d(pln);
			dst=spCrossLine2d.distanceTo(pt2d);
			if(dst<minDst) {
				ptMin=pts2.at(i);
				minDst=dst;
				nMinInx=i;
			}			
		}//end-for
		if(nMinInx!=-1){
			pts2.removeAt(nMinInx);
			ptsSorted.append(ptMin);
		}//end-if
	}//end-if


	// ������������
	AcGePoint3d ptCrossSP, ptCrossEP;
	poly2d1.getStartPoint(ptCrossSP);
	poly2d1.getEndPoint(ptCrossEP);

	if(m_bCheckOnlyCrossPt==FALSE){	
		// ������ ������ �߰�	
		ptCrossSP.z=ptsSorted.first().z;					
		ptsSorted.insertAt(0, ptCrossSP);
		// ������ ���� �߰�
		ptCrossEP.z=ptsSorted.last().z;
		ptsSorted.append(ptCrossEP);
	}

	AcDbPolyline poly2d11;
	for(int i=0;i<ptsSorted.length();i++){
		pt2d= ptsSorted[i].convert2d(pln);
		poly2d11.addVertexAt(i,pt2d); 
	}
	poly2d11.getDistAtPoint(epCrosest, dstCenter);
	
	
	// Ⱦ�ܸ� ����
	AcGePoint3dArray ptsCrossSection;
	AcGePoint2dArray ptsCrossSection2d;
	AcGePoint3d ptStart;
	ptStart = ptsSorted.first();
	AcGePoint2d ptStart2d = ptStart.convert2d(pln);
	ptStart.z=ptsSorted[0].z;
	ptsCrossSection.append(ptStart);
	ptsCrossSection2d.append(ptStart2d);
	double elevation=0;	
	ptStart.z=elevation;
	ptsCrossSection.setPhysicalLength(ptsSorted.length()+1);
	for(int i=1;i<ptsSorted.length();i++){
		pt3d= ptsSorted[i];
		AcGePoint3d pt3dSection=ptStart + AcGeVector3d::kXAxis * ptStart2d.distanceTo(pt3d.convert2d(pln));
		pt3dSection.z=pt3d.z;
		ptsCrossSection.append(pt3dSection);
		ptsCrossSection2d.append(pt3dSection.convert2d(pln));
	}
	//, m_bCheckExceptZeroZ(FALSE)
	EntMake ent;
	ent.setLayerAndColor(m_strLayer, 256);
	
	
	// �߽����� ���̰�
	double centerElevation=0;
	ep = ptStart + AcGeVector3d::kXAxis*dstCenter;
	AcGePoint3d ptCenter(ep);
	ep2d = ep.convert2d(pln);
	int inxCenter=-1;
	for(int i=0;i<ptsCrossSection2d.length()-1;i++){		
		AcGePoint2d pt1, pt2;
		pt1 = ptsCrossSection2d[i];
		pt2 = ptsCrossSection2d[i+1];		
		if(fabs((pt1.distanceTo(ep2d) + pt2.distanceTo(ep2d) ) - pt1.distanceTo(pt2))<0.01){
			AcGePoint3d pt3d1 = ptsCrossSection[i];
			AcGePoint3d pt3d2 = ptsCrossSection[i+1];
			inxCenter=i+1;
			centerElevation=Utils::Interpolation(pt3d1.x, pt3d1.z, pt3d2.x, pt3d2.z, ep.x);	
			break;
		}//end-if
	}//end-if

	strTxt.Format(crossVLineFmt,centerElevation);
	centerElevation=_ttof(strTxt);

	
	// Ⱦ�ܰŸ� ����
	AcDbPolyline poly2dCrossSection;
	for(int i=0;i<ptsCrossSection.length();i++){
		poly2dCrossSection.addVertexAt(i, ptsCrossSection[i].convert2d(pln));
	}
	double dstC=0;  // ���������� �߽��������� �Ÿ�
	double dstA, dstB;
	poly2dCrossSection.getDistAtPoint(ptCenter, dstC);

	AcGePoint3dArray ptsCrossSectionBefore, ptsCrossSectionAfter;
	for(int i=0;i<inxCenter;i++) ptsCrossSectionBefore.append(ptsCrossSection[i]);
	for(int i=inxCenter;i<ptsCrossSection.length();i++) ptsCrossSectionAfter.append(ptsCrossSection[i]);

	AcGeVector3d vForward = AcGeVector3d::kXAxis;
	ptsCrossSectionBefore.reverse();
	for(int i=0;i<ptsCrossSectionBefore.length();i++){
		poly2dCrossSection.getDistAtPoint(ptsCrossSectionBefore[i], dstB);
		dstA=dstC-dstB;
		strTxt.Format(crossHLineFmt,dstA);
		dstA=_ttof(strTxt);
		
		ptsCrossSectionBefore[i]=ptCenter-vForward*dstA;
	}
	ptCenter.z=centerElevation;
	ptsCrossSectionBefore.reverse();
	ptsCrossSectionBefore.append(ptCenter);
	ptsSorted.insertAt(inxCenter, ptCenter);
	for(int i=0;i<ptsCrossSectionAfter.length();i++){
		poly2dCrossSection.getDistAtPoint(ptsCrossSectionAfter[i], dstB);
		dstA=dstB-dstC;
		strTxt.Format(crossHLineFmt,dstA);
		dstA=_ttof(strTxt);
		ptsCrossSectionAfter[i]=ptCenter+vForward*dstA;
	}
	ptsCrossSection.removeAll();
	ptsCrossSection.append(ptsCrossSectionBefore);
	ptsCrossSection.append(ptsCrossSectionAfter);

	//-----------------------------------------------------------------------------
	for(int i=0;i<ptsCrossSection.length();i++){
		ptsCrossSection[i].y=ptsSorted[i].z;
	}
	AcDbObjectId idNew=ent.makePolyline(ptsCrossSection);
	
	ep.y=centerElevation;
	AcGeVector3d vMove=ip-ep;
	if(idNew != AcDbObjectId::kNull && idNew.isValid() ){		
		Utils::MoveObject(idNew, vMove);
	}

	// Ⱦ�� �߽� ���μ�
	AcGePoint3d pt1 = ep+AcGeVector3d::kYAxis*dblCrossVLineLen*0.5;
	AcGePoint3d pt2 = ep-AcGeVector3d::kYAxis*dblCrossVLineLen*0.5;
	idNew=ent.makeLine(pt1, pt2);
	if(idNew != AcDbObjectId::kNull && idNew.isValid() ){		
		Utils::MoveObject(idNew, vMove);
	}

	// Ⱦ�� �߽�
	//-------------------------------------------------------------
	pt3d = ep-AcGeVector3d::kYAxis*dblCrossVLineLen;
	double dblGroundLevelTxtHgt=_ttof(m_strGroundLevel);
	// ���� �μ�
	strTxt.Format(crossVLineFmt, centerElevation);
	idNew=ent.makeText(pt3d, dblGroundLevelTxtHgt, 0, strTxt, TextHorzMode::kTextCenter, TextVertMode::kTextTop, AcGeVector3d::kZAxis);
	if(idNew != AcDbObjectId::kNull && idNew.isValid() ){		
		Utils::MoveObject(idNew, vMove);
	}
	//-------------------------------------------------------------
	// 4. Ⱦ�� �ۼ� �� ���� �Է�
	m_strCheckStation=_T("");
	if(m_bCheckStationInput){
		CString prompt;
		TCHAR val[512];
		int result;

		prompt.Format(_T("\n%s: "), _T("���� �Է�"));
		result = acedGetString(0, prompt, val);

		if (result == RTNORM) {
			m_strCheckStation = val;
		}
	}


	// �������� �Ÿ�/���� ǥ��
	CString strH, strV;
	if(m_bCheckDstHgtDisp){
		int lenCrossSection=ptsCrossSection.length();
		for(int i=0;i<lenCrossSection;i++){
			if(m_bCheckOnlyCrossPt==FALSE){	
				if(i==0 || i==lenCrossSection-1) continue;
			}
			if(i==inxCenter) continue;
			// �Ÿ�
			dstA=fabs(ptCenter.x-ptsCrossSection[i].x);
			strH.Format(crossHLineFmt,dstA);
			// ����
			if(m_nDispStandard==0) dstA=ptsCrossSection[i].y-centerElevation;
			else dstA=ptsCrossSection[i].y;
			strV.Format(crossVLineFmt,dstA);
			strTxt.Format(_T("%s/%s"), strH, strV);
			idNew=ent.makeText(ptsCrossSection[i], dblGroundLevelTxtHgt*0.5, kRad45, strTxt, TextHorzMode::kTextLeft, TextVertMode::kTextBottom, AcGeVector3d::kZAxis);
			if(idNew != AcDbObjectId::kNull && idNew.isValid() ){		
				Utils::MoveObject(idNew, vMove);
			}
		}//end-for
	}//end-if

	// ���� ǥ��
	if(m_bCheckStationInput && m_strCheckStation.GetLength()>0){
		pt3d = ep + AcGeVector3d::kYAxis * dblGroundLevelTxtHgt*3.0;
		idNew=ent.makeText(pt3d, dblGroundLevelTxtHgt, 0, m_strCheckStation, TextHorzMode::kTextMid, TextVertMode::kTextVertMid, AcGeVector3d::kZAxis);
		if(idNew != AcDbObjectId::kNull && idNew.isValid() ){		
			Utils::MoveObject(idNew, vMove);
		}//end-if
	}//end-if

}


// Ⱦ�ܼ� ����
void CCFM::OnBnClickedButtonCorsslinesel()
{
	UpdateData();

	int nLayerInx = m_btnLayer.GetCurSel();
	if(nLayerInx<0){	
		return;
	}


	m_btnLayer.GetLBText(nLayerInx, m_strLayer);

	BeginEditorCommand();
	Acad::ErrorStatus es;
	AcApDocument *pDoc = curDoc();

	try{
		if(pDoc!=NULL) {
			es=acDocManager->lockDocument(pDoc);
		}
	

		// 1. Ⱦ�ܼ� ���� �κ� ����:		
		ads_name crossEn;
		if(ads_entsel(_T("\nȾ�ܼ� ���� �κ� ����: "), crossEn, asDblArray(m_sp)) != RTNORM){
			CancelEditorCommand();
			if(es==Acad::eOk && pDoc) acDocManager->unlockDocument(pDoc);
			return;
		}
		AcDbObjectId idCross;
		acdbGetObjectId(idCross, crossEn);

		// 2. Ⱦ�� �߽��� ����:
		if(ads_getpoint(NULL,_T("\nȾ�� �߽��� ����: "), asDblArray(m_ep)) != RTNORM){
			CancelEditorCommand();
			if(es==Acad::eOk && pDoc) acDocManager->unlockDocument(pDoc);
			return;
		}
		// 3. Ⱦ�� ������ ����:
		if(ads_getpoint(NULL,_T("\nȾ�� ������ ����: "), asDblArray(m_ip)) != RTNORM){
			CancelEditorCommand();
			if(es==Acad::eOk && pDoc) acDocManager->unlockDocument(pDoc);
			return;
		}

		
		DrawCrossSectionLine(idCross, m_sp, m_ep, m_ip);
	
	
	}catch(...)
	{
		
	}
	if(es==Acad::eOk && pDoc) acDocManager->unlockDocument(pDoc);
	CompleteEditorCommand();
}


void CCFM::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CAcUiDialog::OnCancel();
}


void CCFM::OnDeltaposSpnCrossdstdecpt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	int nCrossDstDecimalPoint = _ttoi(m_strCrossDstDecimalPoint);
	if(pNMUpDown->iDelta <= 0){
		nCrossDstDecimalPoint++;
		if(nCrossDstDecimalPoint>3) nCrossDstDecimalPoint=3;
	}
	else{
		nCrossDstDecimalPoint--;
		if(nCrossDstDecimalPoint<0) nCrossDstDecimalPoint = 0;
	}
	m_strCrossDstDecimalPoint.Format(_T("%d"), nCrossDstDecimalPoint);
	UpdateData(FALSE);

	*pResult = 0;
}


void CCFM::OnDeltaposSpnCrosshgtdecpt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	int nCrossHgtDecimalPoint = _ttoi(m_strCrossHgtDecimalPoint);
	if(pNMUpDown->iDelta <= 0){
		nCrossHgtDecimalPoint++;
		if(nCrossHgtDecimalPoint>3) nCrossHgtDecimalPoint=3;
	}
	else{
		nCrossHgtDecimalPoint--;
		if(nCrossHgtDecimalPoint<0) nCrossHgtDecimalPoint=0;
	}
	m_strCrossHgtDecimalPoint.Format(_T("%d"), nCrossHgtDecimalPoint);
	UpdateData(FALSE);

	*pResult = 0;
}


void CCFM::OnBnClickedCheckdsthgtdisp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	if(m_bCheckDstHgtDisp==TRUE){
		GetDlgItem(IDC_RADIORELATIVE)->EnableWindow();
		GetDlgItem(IDC_RADIOABSOLUTE)->EnableWindow();
	}else{
		GetDlgItem(IDC_RADIORELATIVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIOABSOLUTE)->EnableWindow(FALSE);
	}
}


void CCFM::OnBnClickedBtnselent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->BeginEditorCommand();
	AcGePoint3d pt;
	ads_name en;
	
	if(ads_entsel(_T("\n���̾ ������ ��ü ����: "), en, asDblArray(pt)) == RTNORM){
		AcDbObjectId idSel;
		if(acdbGetObjectId(idSel, en) == Acad::eOk) {
			AcDbObjectPointer<AcDbEntity> pE(idSel, AcDb::kForRead);
			if(pE.openStatus() == Acad::eOk){
				CString strLayer=pE->layer();
				int nFind=m_btnLayer.FindStringExact(0, strLayer);
				if(nFind!=-1) m_btnLayer.SetCurSel(nFind);
				
			}//end-if
		}//end-if
	}

	this->CompleteEditorCommand();
}
