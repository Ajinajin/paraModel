
#include "ArmDesignWidget.h"
#include "BezierOpt.h"
#include <QIcon>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QColorDialog>

#include "WarheadDataConvert.h"


VECLSTPT oglCurveData;

//------------------------------------------------------------------------------
// Constructor

ArmDesignWidget::ArmDesignWidget(QWidget *parent) : QWidget(parent)
{
    iSelectedPt = -1;
    showHelp = showControlLines = false;
	showControlLines = true; 
	bCalCen = true; 
	bCalPartShape = true; 
	bCalDesign = true; 
    curveColor = DRAWBEZIER_COLOR;

    setMinimumSize(MINIMUM_SIZE);
    setWindowTitle("Bezier Curve");
//  setWindowIcon(QIcon(":/bezier.png"));
}

//------------------------------------------------------------------------------
// ��ʾ���߶̵�

void ArmDesignWidget::showEvent(QShowEvent *)
{
//     lstUpCurveCtrlPts << FIRST_POINT << LAST_POINT;
}

//------------------------------------------------------------------------------
// ��ʾˢ��

void ArmDesignWidget::TransPos(VSHAPE & vShape, int nXY[2])
{
	int nShapeNum = vShape.size();
	int nPtNum = 0;
	int i = 0;
	SimpleShape oShape;
	VSHAPE vTmpShape = vShape;
	vShape.clear();
	for (i = 0; i < nShapeNum; i++)
	{
		oShape = vTmpShape[i];
		nPtNum = oShape.vCorner.size();
		if( nPtNum > 0 )
		{
			for (int j = 0; j < nPtNum; j++)
			{
				oShape.vCorner[j].nXY[0] += nXY[0];
				oShape.vCorner[j].nXY[1] += nXY[1];
			}
		}
		nPtNum = oShape.lstCtrlPts.size();
		if( nPtNum > 0 )
		{
			QPoint oOff(nXY[0], nXY[1]); 
			for (int j = 0; j < nPtNum; j++)
			{
				oShape.lstCtrlPts[j] += oOff;
			}
		}
		oShape.nCen[0] += nXY[0];
		oShape.nCen[1] += nXY[1];
		vShape.push_back(oShape);
	}

	return;
}
// ƽ��
void ArmDesignWidget::TransPos(VPIXELPOS & vPt, int nXY[2])
{
	int nShapeNum = vPt.size();
	int nPtNum = 0; 
	int i = 0; 
	PixelPos oPixel; 
	VPIXELPOS vTmp = vPt;
	vPt.clear(); 
	for ( i = 0 ; i < nShapeNum ; i++ )
	{
		oPixel = vTmp[i];
		oPixel.nXY[0] += nXY[0];
		oPixel.nXY[1] += nXY[1];
		vPt.push_back(oPixel); 
	}

	return; 
}
// ƽ��
void ArmDesignWidget::TransPos(LISTPT & vPt, int nXY[2])
{
	int nShapeNum = vPt.size();
	int nPtNum = 0; 
	int i = 0; 
	QPointF oPixel; 
	LISTPT vTmp = vPt;
	vPt.clear(); 
	for ( i = 0 ; i < nShapeNum ; i++ )
	{
		oPixel = QPoint(vTmp[i].x()+nXY[0],vTmp[i].y()+nXY[1]);
		vPt.push_back(oPixel); 
	}

	return; 
}
// ƽ��
void ArmDesignWidget::TransPos(LSTEDGEDES & vEdge, int nXY[2])
{
	int nShapeNum = vEdge.size();
	int nPtNum = 0; 
	int i = 0; 
	QPointF oPixel; 
	LSTEDGEDES vTmp = vEdge;
	EdgeDes oEdge; 
	vEdge.clear();
	for ( i = 0 ; i < nShapeNum ; i++ )
	{
		oEdge = vTmp[i]; 
		nPtNum = oEdge.lstCtrlPts.size(); 
		for ( int j = 0 ; j < nPtNum ;j++ )
		{
			oPixel = QPointF(oEdge.lstCtrlPts[j].x()+nXY[0],oEdge.lstCtrlPts[j].y()+nXY[1]);
			oEdge.lstCtrlPts[j] = oPixel;
		}
		vEdge.push_back(oEdge); 
	}

	return; 
}
// ƽ��
void ArmDesignWidget::TransPos(VECLSTPT & vvPt, int nXY[2])
{
	int nShapeNum = vvPt.size();
	int nPtNum = 0; 
	int i = 0; 
	VECLSTPT vvTmp = vvPt;
	LISTPT lstTmp; 
	vvPt.clear(); 
	for ( i = 0 ; i < nShapeNum ; i++ )
	{
		TransPos(vvTmp[i], nXY); 
		vvPt.push_back(vvTmp[i]); 
	}

	return; 
}
// �Գ�
void ArmDesignWidget::ReflectPos(VPIXELPOS & vPt, int nXY[2])
{
	int nShapeNum = vPt.size();
	int nPtNum = 0; 
	int i = 0; 
	PixelPos oPixel; 
	VPIXELPOS vTmp = vPt;
	vPt.clear(); 
	for ( i = 0 ; i < nShapeNum ; i++ )
	{
		oPixel = vTmp[i];
		// ���ҶԳ� ����
		if (nXY[0] != 0)
			oPixel.nXY[0] = 2*nXY[0] - oPixel.nXY[0];
		// ���ҶԳ� ����
		else
			oPixel.nXY[1] = 2 * nXY[1] - oPixel.nXY[1];
		vPt.push_back(oPixel); 
	}

	return; 
}

QColor ArmDesignWidget::ColorHelper(int unitIdx)
{
	if (unitIdx == 0)
	{
		return QColor(14,255,2);
	}
	else if (unitIdx == 1)
	{
		return QColor(204,178,102);
	}
	else if (unitIdx == 2)
	{
		return QColor(255, 127, 5);
	}
	else if (unitIdx == 3)
	{
		return QColor(131, 156, 255);
	}
	else if (unitIdx == 4)
	{
		return QColor(4, 31, 5);
	}
	else if (unitIdx == 5)
	{
		return QColor(2, 255, 255);
	}
	else if (unitIdx == 6)
	{
		return QColor(254, 235, 248);
	}
	else if (unitIdx == 7)
	{
		return QColor(255, 0, 255);
	}
	else if (unitIdx == 8)
	{
		return QColor(255, 255, 255);
	}
	return QColor(255, 255, 255);
}

// ��ʼ�����ӵ�
int ArmDesignWidget::CalLinkPt()
{
	lstLinkFrontPt.clear(); 
	QPoint oPt(-fT[8], fR[1]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(fR[1] - fT[1]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(fR[2] + fT[2]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(fR[2]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(-fR[2]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(-fR[2]-fT[2]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(-fR[1]+fT[1]); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(-fR[1]);
	lstLinkFrontPt.push_back(oPt);

	lstLinkBackPt = lstLinkFrontPt; 
	int nPtNum = lstLinkFrontPt.size();
	for ( int i = 0 ; i < nPtNum; i++ )
	{
		lstLinkBackPt[i].setX(-fH[1] - fT[8]); 
	}
	return 0; 
}

// ��ʼ�����ӵ�
int ArmDesignWidget::CalMultiPartLinkPt()
{
	if (oArmHeadDesign.oDesignData.vCtrlPts.size() <= 0)
		return 1; 
	lstLinkFrontPt.clear(); 
	QPointF oPt = oArmHeadDesign.oDesignData.vCtrlPts[0]; 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(oPt.y() - fT[1]); 
	lstLinkFrontPt.push_back(oPt); 

	int nYOff = fR[1] - fR[2] - fT[2]-fT[1]; 
	oPt.setY(oPt.y() - nYOff);
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(oPt.y() - fT[2]);
	lstLinkFrontPt.push_back(oPt); 

	oPt.setY(-oPt.y()); 
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(oPt.y() - fT[2]);
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(oPt.y() - nYOff);
	lstLinkFrontPt.push_back(oPt); 
	oPt.setY(oPt.y() - fT[1]);
	lstLinkFrontPt.push_back(oPt);

	lstLinkBackPt = lstLinkFrontPt; 
	int nPtNum = lstLinkFrontPt.size();
	oPt = oArmHeadDesign.oDesignData.vCtrlPts[oArmHeadDesign.oDesignData.vCtrlPts.size()-1];
	nYOff = lstLinkFrontPt[0].y() - oPt.y(); 
	for ( int i = 0 ; i < nPtNum/2; i++ )
	{
		lstLinkBackPt[i].setX(oPt.x()); 
		lstLinkBackPt[i].setY(lstLinkFrontPt[i].y() - nYOff);
		lstLinkBackPt[nPtNum-1-i].setX(oPt.x()); 
		lstLinkBackPt[nPtNum-1-i].setY(-lstLinkBackPt[i].y());
	}
	return 0; 
}

// �����ݴ�� ��ǿ��Ƶ���������
int ArmDesignWidget::CalCurvePt(LISTPT const& lstCtrlPt,LISTPT& lstCurvePt, LISTPT& lst3DDraw,int nDrawStep)
{
	// �������ߵ�
	double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;

	lstCurvePt.clear(); 
	QPoint oStart = CalBezier(lstCurvePt, 0.0);
	for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
	{
		oStart = CalBezier(lstCtrlPt, t);
		lstCurvePt.push_back(oStart); 
	}

	// �������з�
	int nPtNum = lstCtrlPt.size(); 
	float fXRange = qAbs(lstCtrlPt[0].x() - lstCtrlPt[nPtNum -1].x());
	int nRangeLen = nDrawStep;				// Ĭ��10mm���
	int nCalNum = fXRange / nRangeLen + 0.5;
	int nStep = lstCurvePt.size() / nCalNum;

	lst3DDraw.clear();
	for (int i = 0; i < nCalNum - 1; i++)
	{
		lst3DDraw.push_back(lstCurvePt[i*nStep]);
	}
	lst3DDraw.push_back(lstCtrlPt[nPtNum - 1]);

	return 0; 
}

// ���������ƽ�Ƶõ��������� ������Ӵ�С����
int ArmDesignWidget::CalAllCurvePt(LISTPT const&  lstCurvePt, VECLSTPT& vlstCurvePt)
{
	vlstCurvePt.clear(); 
	LISTPT oCurList = lstCurvePt; 
	vlstCurvePt.push_back(oCurList);
	float fOffY[8] = { 0 }; 
	fOffY[1] = -fT[1]; 
	fOffY[2] = -(fR[1]-fR[2]-fT[2]);
	fOffY[3] = -(fR[1] - fR[2]);
	// ͬһ��ĵݼ�
	int i = 1; 
	int nPtNum = oCurList.size(); 
	for ( i = 1 ; i < 4 ; i++ )
	{
		for ( int j = 0; j < nPtNum ; j++ )
		{
			oCurList[j].setY(lstCurvePt[j].y() + fOffY[i]);
		}
		vlstCurvePt.push_back(oCurList); 
	}
	// ��ͬ��ĶԳ�
	for ( i = 3 ; i >= 0 ; i-- )
	{
		oCurList = vlstCurvePt[i]; 
		for ( int j = 0; j < nPtNum ; j++ )
		{
			oCurList[j].setY(-oCurList[j].y()); 
		}
		vlstCurvePt.push_back(oCurList); 
	}
	return 0 ; 
}

int ArmDesignWidget::MergeLoopShape(LISTPT const& lpt1, LISTPT const& lpt2, LISTPT const& lFrontPt,LISTPT const & lBackPt, int nIdx, LISTPT& lShapePt)
{
	lShapePt.clear(); 
	lShapePt.push_back(lFrontPt[nIdx]);
	lShapePt = lpt1; 
	lShapePt.push_back(lBackPt[nIdx]);
	lShapePt.push_back(lBackPt[nIdx+1]);
	int nPtNum = lpt2.size(); 
	for ( int i = nPtNum - 1 ; i >= 0 ; i--)
	{
		lShapePt.push_back(lpt2[i]); 
	}
	lShapePt.push_back(lFrontPt[nIdx+1]);
	lShapePt.push_back(lFrontPt[nIdx]);
	
	return 0; 
}
// �������� ��� �ڳ�����������㼯
int ArmDesignWidget::CalAllPartPt(LISTPT const&  lstLinkFPt,LISTPT const&  lstLinkBPt, VECLSTPT const& vlstCurvePt, VECLSTPT& vlstPartPt)
{
	vlstPartPt.clear(); 
	LISTPT lstShape; 
	// ����Ϊ�Ӵ�С ��� �ڳ� �ڳ� ���
	for ( int i = 0 ; i < 4 ; i++ )
	{
		MergeLoopShape(vlstCurvePt[i * 2], vlstCurvePt[i * 2 + 1], lstLinkFPt, lstLinkBPt, i*2, lstShape); 
		vlstPartPt.push_back(lstShape); 
	}
	// �õ�ըҩ����������㼯
	MergeLoopShape(vlstCurvePt[3], vlstCurvePt[4], lstLinkFPt, lstLinkBPt, 3, lstShape); 
	vlstPartPt.push_back(lstShape); 
	return 0; 
}

// ��ս����
void ArmDesignWidget::DrawSubPart(QPainter * pPaint, SimpleShape const& oCurShape)
{
	QPen linePen(ColorHelper(0), 2);
	linePen.setCapStyle(Qt::RoundCap);
	// ����
	if (oCurShape.unitType == 1)
	{
		pPaint->setPen(linePen);
		pPaint->setBrush(ColorHelper(oCurShape.unitIdx));
		pPaint->drawRect(oCurShape.nCen[0] - oCurShape.nWH[0] / 2, oCurShape.nCen[1] - oCurShape.nWH[1] / 2, oCurShape.nWH[0], oCurShape.nWH[1]);
	}
	// �����

	else if (oCurShape.unitType == 3)
	{
		// ������
// 		QPen CurvePen(ColorHelper(oCurShape.unitIdx), oCurShape.nWH[0]);
		pPaint->setPen(linePen);

		QPoint oStart = oCurShape.lstCtrlPts[0];
		int nPtNum = oCurShape.lstCtrlPts.size(); 
		QPainterPath path;
		QPoint oEnd;
		for (int i = 1 ; i < nPtNum ; i++ )
		{
			path.lineTo(oCurShape.lstCtrlPts[i]);
		}
		QColor color = ColorHelper(oCurShape.unitIdx);
		pPaint->fillPath(path, color);
	}
	// ֱ��
	else if (oCurShape.unitType == 4)
	{
		QPen pen = QPen(ColorHelper(oCurShape.unitIdx), oCurShape.nWH[0]);
		pen.setStyle(Qt::SolidLine);
		pPaint->setPen(pen);
		if (oCurShape.unitIdx == 7 || oCurShape.unitIdx == 4)
		{
			pPaint->setBrush(ColorHelper(oCurShape.unitIdx));
		}
		QPointF oStart(oCurShape.vCorner[0].nXY[0], oCurShape.vCorner[0].nXY[1]);
		for (size_t j = 1; j < oCurShape.vCorner.size(); j++)
		{
			QPointF oEnd(oCurShape.vCorner[j].nXY[0], oCurShape.vCorner[j].nXY[1]);
			pPaint->drawLine(oStart, oEnd);
			oStart = oEnd;
		}

	}
	return; 
}

void ArmDesignWidget::DrawArmHead(QPainter * pPaint,VECLSTPT const& vAllDraw, VSHAPE const& vShape)
{
	if (vShape.empty() && vAllDraw.empty() )
		return; 
	// ��ǰ���
	DrawSubPart(pPaint, vShape[0]); 
	DrawSubPart(pPaint, vShape[1]);

	QPen linePen(ColorHelper(0), 2);
	linePen.setCapStyle(Qt::RoundCap);
	int nPartNum = vAllDraw.size();
	LISTPT lstCur; 
	for ( int i = 0 ; i < nPartNum ; i++ )
	{
		lstCur = vAllDraw[i]; 
		int nPtsNum = lstCur.size(); 
		pPaint->setPen(linePen);

		QPointF oStart = lstCur[0];
		QPainterPath path;
		QPointF oEnd;
		path.moveTo(lstCur[0]);
		for (int j = 0; j < nPtsNum; j++)
		{
			path.lineTo(lstCur[j]);

		}

		for (int j = 0; j < nPtsNum-1; j++)
		{
			pPaint->drawLine(lstCur[j],lstCur[j+1]);
		}

		QColor color = ColorHelper(i+2);
		// δ��� ��ʱ�����
// 		pPaint->fillPath(path, color);
	}
	// ����о
	DrawSubPart(pPaint, vShape[7]);


	return; 
}
// void ArmDesignWidget::DrawArmHead(QPainter * pPaint, VSHAPE const& vShape)
// {
// 	if (vShape.empty())
// 		return; 
// 	// ��ǰ���
// 	DrawSubPart(pPaint, vShape[0]); 
// 	DrawSubPart(pPaint, vShape[1]);
// 
// 	QPen linePen(ColorHelper(0), 2);
// 	linePen.setCapStyle(Qt::RoundCap);
// 	int nPartNum = vlstDrawPartPt.size();
// 	for ( int i = 0 ; i < nPartNum ; i++ )
// 	{
// 		pPaint->setPen(linePen);
// 
// 		QPointF oStart = vlstDrawPartPt[i][0];
// 		int nPtNum = vlstDrawPartPt[i].size();
// 		QPainterPath path;
// 		QPointF oEnd;
// 		path.moveTo(vlstDrawPartPt[i][0]); 
// 		for (int j = 0; j < nPtNum; j++)
// 		{
// 			path.lineTo(vlstDrawPartPt[i][j]);
// 		}
// 
// 		QColor color = ColorHelper(i+2);
// 		pPaint->fillPath(path, color);
// 	}
// // 	// �����
// // 	DrawSubPart(pPaint, vShape[2]);
// // 	DrawSubPart(pPaint, vShape[5]);
// // 	// ���ڳ�
// // 	DrawSubPart(pPaint, vShape[3]);
// // 	DrawSubPart(pPaint, vShape[4]);
// // 	// ����ҩ
// // 	DrawSubPart(pPaint, vShape[6]);
// 	// ����о
// 	DrawSubPart(pPaint, vShape[7]);
// 
// 
// 	return; 
// }

// ����ͼֽ��������ĸ���������������ͼֽ�е�����
// ��Ҫ���� vDesignShape lstUpCurveCtrlPts����
// ��Ҫ�ڿ��Ƶ�䶯��ʱ�����
int MergeMultiToOnePts(VECLSTPT const& vLstPts, LISTPT& lstOne)
{
	if (vLstPts.size() <= 0)
		return 1; 

	int nLstNum = vLstPts.size(); 
	int i = 1; 
	lstOne.clear(); 
	lstOne = vLstPts[0]; 
	for ( i = 1 ; i < nLstNum ; i++ )
	{
		int nPtNum = vLstPts[i].size(); 
		for (int j = 0; j < nPtNum; j++)
			lstOne.push_back(vLstPts[i][j]); 
	}
	return 0; 
}

int ArmDesignWidget::CalDesignShape()
{
	int nRe = 0; 
	// ǰ�Ǻ��ֱ��ת 0 1 ����
	// 2345 �� �������� תΪ4����������

	// �������ӵ�
	nRe = CalMultiPartLinkPt();
	if (nRe != 0)
		return nRe; 
	// �����Ϸ��������
	LISTPT lst3DDraw;
	LISTPT lstOriPts; 
	int nDrawStep = 20; 
	nRe = CalDesignEdgePts(nDrawStep);
	MergeMultiToOnePts(vAllPartDesignPts, lstOriPts); 
	MergeMultiToOnePts(vAllPartPtsFor3D, lst3DDraw); 
	// ������������
	nRe = CalAllCurvePt(lstOriPts, vlstCurvePt);
	nRe = CalAllCurvePt(lst3DDraw, vlst3DCurvePt);
	// ��������������
	nRe = CalAllPartPt(lstLinkFrontPt, lstLinkBackPt, vlstCurvePt, vlstPartPt); 
	nRe = CalAllPartPt(lstLinkFrontPt, lstLinkBackPt, vlst3DCurvePt, vlst3DPartPt);

	
	oglCurveData = vlst3DCurvePt;

	//test
	int a = 0;
	return 0; 
}
// int ArmDesignWidget::CalDesignShape()
// {
// 	int nRe = 0; 
// 	// ǰ�Ǻ��ֱ��ת 0 1 ����
// 	// 2345 �� �������� תΪ4����������
// 
// 	// �������ӵ�
// 	nRe = CalLinkPt();
// 	// �����Ϸ��������
// 	LISTPT lst3DDraw;
// 	int nDrawStep = 20; 
// 	nRe = CalCurvePt(lstUpCurveCtrlPts, lstUpCurvePts, lst3DDraw,nDrawStep);
// 	// ������������
// 	nRe = CalAllCurvePt(lstUpCurvePts, vlstCurvePt);
// 	// ��������������
// 	nRe = CalAllPartPt(lstLinkFrontPt, lstLinkBackPt, vlstCurvePt, vlstPartPt); 
// 
// 	return 0; 
// }

// ����Բ���� ���㶨Բ��
int ArmDesignWidget::CalArcPts(LISTPT const& lstOriPts, LISTPT& lstArcPts, LISTPT& lst3DDraw,int nDrawStep)
{
	if (lstOriPts.size() < 3)
		return 1; 
	double Pi = 3.14159265; 
	//����ȷ��һ��Բ,Ӳ�⣬�ɵõ����ʽ������һ�ߵ�ʱ�����޷��γ�Բ��  -- ֱ�����������ж�Ӧ�Ĵ��룬https://blog.csdn.net/liyuanbhu/article/details/52891868
	double fX[3], fY[3]; 
	int i = 0; 
	for ( i = 0 ; i < 3 ; i++ )
	{
		fX[i] = lstOriPts[i].x();
		fY[i] = lstOriPts[i].y();
	}
	double a = fX[0] - fX[1];
	double b = fY[0] - fY[1];
	double c = fX[0] - fX[2];
	double d = fY[0] - fY[2];
	double e = ((fX[0] * fX[0] - fX[1] * fX[1]) + (fY[0] * fY[0] - fY[1] * fY[1])) / 2.0;
	double f = ((fX[0] * fX[0] - fX[2] * fX[2]) + (fY[0] * fY[0] - fY[2] * fY[2])) / 2.0;
	double det = b * c - a * d;
	if ((fX[0] == fX[1] && fX[0] == fX[2]) || (fY[0] == fY[1] && fY[0] == fY[2]))
	{
		//����һ��,
		return false;
	}
	double x0 = -(d * e - b * f) / det;
	double y0 = -(a * f - c * e) / det;
	QPointF m_center; 
	m_center.setX(x0);
	m_center.setY(y0);
	double m_r = hypot(fX[0] - x0, fY[0] - y0);
	// ������Բ�ĵ�ͬ��
	int nFlag = 0;
	for ( i = 0 ; i < 3 ; i++ )
	{
		if (m_center.y() - fY[i] > 0)
			nFlag += 1;
		else
			nFlag -= 1; 
	}
	if (nFlag != 3 && nFlag != -3)
		return 1; 
	//��������Ķ�Ӧ�ĽǶ�
	double angle1 = acos((fX[0] - x0) / (hypot((fX[0] - x0), (fY[0] - y0))));
	double angle2 = acos((fX[1] - x0) / (hypot((fX[1] - x0), (fY[1] - y0))));
	double angle3 = acos((fX[2] - x0) / (hypot((fX[2] - x0), (fY[2] - y0))));

	int nAdd = 1; 
	if (fY[0] - y0 < 0) 
	{
		angle1 = 2 * Pi - angle1;  //��1�ڵ�����������
		angle3 = 2 * Pi - angle3; //��3�ڵ�����������
		nAdd = -1; 
	}

	lstArcPts.clear(); 
	LISTPT lstTmp; 
	float fS = angle1; 
	int nCalNum = 100; 
	float fStep = qAbs(angle3 - angle1) / nCalNum;
	for ( i = 0 ; i < nCalNum-1 ; i++ )
	{
		fS = angle1 + nAdd * i * fStep; 
		QPointF lstArcPts(m_center.x() + m_r * cos(fS), m_center.y() + m_r * sin(fS)); 
		lstTmp.push_back(lstArcPts); 
	}
	lstTmp.push_back(lstOriPts[2]); 
	lstArcPts = lstTmp; 
	// �������з�
	float fXRange = qAbs(fX[0] - fX[2]); 
	int nRangeLen = nDrawStep;				// Ĭ��10mm���
	nCalNum = fXRange / nRangeLen + 0.5;
	int nStep = lstTmp.size() / nCalNum; 

	lst3DDraw.clear(); 
	for ( i = 0 ; i < nCalNum-1 ; i++ )
	{
		lst3DDraw.push_back(lstTmp[i*nStep]);
	}
	lst3DDraw.push_back(lstTmp[lstTmp.size() - 1]);

	return 0; 
}

// ����������ǵ� ���ͼֽ����
int ArmDesignWidget::CalDesignEdgePts(int nDrawStep)
{
	// ������������εĵ� ���ͼֽ����
	int nPartNum = oArmHeadDesign.oDesignData.lstEdgeDes.size(); 
	EdgeDes oCurPartEdge; 
	int i = 0; 
	VECLSTPT vAllDrawPts; 
	VECLSTPT vAll3DPts; 
	LISTPT oCurPartPts; 
	LISTPT o3DPts; 
	for ( i = 0 ; i < nPartNum ; i++ )
	{
		oCurPartPts.clear(); 
		o3DPts.clear(); 
		oCurPartEdge = oArmHeadDesign.oDesignData.lstEdgeDes[i]; 
		// ֱ�߶�
		if (oCurPartEdge.nEdgeType == 0)
		{
			oCurPartPts = oCurPartEdge.lstCtrlPts; 
			o3DPts = oCurPartEdge.lstCtrlPts;
		}
		// ����Բ��
		else if (oCurPartEdge.nEdgeType == 1)
		{
			// ����Բ�������еĵ�
			CalArcPts(oCurPartEdge.lstCtrlPts, oCurPartPts,o3DPts, nDrawStep); 
		}
		// Bezier����
		else if (oCurPartEdge.nEdgeType == 2)
		{
			CalCurvePt(oCurPartEdge.lstCtrlPts, oCurPartPts, o3DPts, nDrawStep); 
		}
		vAllDrawPts.push_back(oCurPartPts); 
		vAll3DPts.push_back(o3DPts); 
	}
	bCalDesign = true; 

	vAllPartDesignPts = vAllDrawPts; 
	vAllPartPtsFor3D = vAll3DPts; 

	return 0; 
}

void ArmDesignWidget::paintEvent(QPaintEvent *)
{
    double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
    double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;

	// �Ƿ���Ҫ����ͼֽ��״�ߴ�
	if (bCalDesign)
	{
		CalDesignEdgePts(20); 
		bCalDesign = false; 
	}
	// �Ƿ���Ҫ���м���
	if (bCalCen)
	{
		bCalCen = false; 
		QSize oCurSize = size(); 
		if (vDesignShape.size() > 0)
		{
			vDrawShape = vDesignShape;
			nMoveXY[0] = oCurSize.width() / 2 + (fH[1]+fT[8]+fT[9]) / 2;
			nMoveXY[1] = oCurSize.height() / 2;
			TransPos(vDrawShape, nMoveXY);

			vAllPartDrawPts = vAllPartDesignPts;
			nMoveXY[0] = oCurSize.width() / 2 + (fH[1]+fT[8]+fT[9]) / 2;
			nMoveXY[1] = oCurSize.height() / 2;
			TransPos(vAllPartPtsFor3D, nMoveXY);
			TransPos(vAllPartDrawPts, nMoveXY);
			// ���Ƶ㼯����
			lstUpEdgeCtrlPts = oArmHeadDesign.oDesignData.lstEdgeDes; 
			lstUpEdgeDrawCtrlPts = lstUpEdgeCtrlPts; 
			TransPos(lstUpEdgeDrawCtrlPts, nMoveXY); 
			// �ƶ������������� ���� ��ϸ����
// 			int nPartNum = vlstPartPt.size(); 
// 			vlstDrawPartPt = vlstPartPt; 
// 			QPointF oMove(nMoveXY[0], nMoveXY[1]); 
// 			for (int i = 0; i < nPartNum; i++)
// 			{
// 				int nPtNum = vlstDrawPartPt[i].size(); 
// 				for ( int j = 0 ; j < nPtNum ; j++ )
// 				{
// 					vlstDrawPartPt[i][j] += oMove; 
// 				}
// 			}
			// �ƶ������������� ���� �ֲ�����
			int nPartNum = vlst3DPartPt.size(); 
			vlstDrawPartPt = vlst3DPartPt;
			QPointF oMove(nMoveXY[0], nMoveXY[1]); 
			for (int i = 0; i < nPartNum; i++)
			{
				int nPtNum = vlstDrawPartPt[i].size(); 
				for ( int j = 0 ; j < nPtNum ; j++ )
				{
					vlstDrawPartPt[i][j] += oMove; 
				}
			}
		}
	}
    QPainter painter(this);
	// ��ս����
	DrawArmHead(&painter, vlstDrawPartPt, vDrawShape);


	// ��������/��
    if (showControlLines) 
	{
		int nEdgeNum = lstUpEdgeDrawCtrlPts.size();
		int nPtNum = 0;
		EdgeDes oCurEdge; 
		// �����Ƶ�
		painter.setPen(DRAWPOINTS_PEN);
		for (int i = 0; i < nEdgeNum; i++)
		{
			oCurEdge = lstUpEdgeDrawCtrlPts[i]; 
			if ( oCurEdge.nEdgeType == 2 )
			{
				continue;
			}
			nPtNum = oCurEdge.lstCtrlPts.size(); 
			for ( int j = 0 ; j < nPtNum ;j++)
			{
				painter.drawPoint(oCurEdge.lstCtrlPts[j]);
			}
		}
		QPen oEditPen(Qt::red, 8.0); 
		painter.setPen(oEditPen); 
		for(int i = 0; i < nEdgeNum; i++)
		{
			oCurEdge = lstUpEdgeDrawCtrlPts[i];
			if (oCurEdge.nEdgeType != 2)
			{
				continue;
			}
			nPtNum = oCurEdge.lstCtrlPts.size();
			for (int j = 1; j < nPtNum-1; j++)
			{
				painter.drawPoint(oCurEdge.lstCtrlPts[j]);
			}
		}
		QPen oEditLinePen(Qt::blue, 2.0,Qt::DashLine);
		painter.setPen(oEditLinePen);
		// ������������
		for (int i = 0; i < nEdgeNum; i++)
		{
			oCurEdge = lstUpEdgeDrawCtrlPts[i]; 
			if( oCurEdge.nEdgeType != 2 )
				continue;
			nPtNum = oCurEdge.lstCtrlPts.size(); 
			for ( int j = 1 ; j < nPtNum ;j++)
			{
				painter.drawLine(oCurEdge.lstCtrlPts[j-1],oCurEdge.lstCtrlPts[j]);
			}
		}

		// ������������
		painter.setPen(DRAWCONTROL_PEN);
		for (int i = 0; i < nEdgeNum; i++)
		{
			oCurEdge = lstUpEdgeDrawCtrlPts[i]; 
			if( oCurEdge.nEdgeType == 2 )
				continue;
			nPtNum = oCurEdge.lstCtrlPts.size(); 
			for ( int j = 1 ; j < nPtNum ;j++)
			{
				painter.drawLine(oCurEdge.lstCtrlPts[j-1],oCurEdge.lstCtrlPts[j]);
			}
		}
    }

}
// void ArmDesignWidget::paintEvent(QPaintEvent *)
// {
//     double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
//     double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;
// 
// 	// �Ƿ���Ҫ����ͼֽ��״�ߴ�
// 	// �Ƿ���Ҫ���м���
// 	if (bCalCen)
// 	{
// 		bCalCen = false; 
// 		QSize oCurSize = size(); 
// 		if (vDesignShape.size() > 0)
// 		{
// 			vDrawShape = vDesignShape;
// 			nMoveXY[0] = oCurSize.width() / 2 + (fH[1]+fT[8]+fT[9]) / 2;
// 			nMoveXY[1] = oCurSize.height() / 2;
// 			TransPos(vDrawShape, nMoveXY);
// 
// 			lstUpCurveCtrlPts.clear();
// 			int nPt = vDrawShape[2].vCorner.size();
// 			for (int i = 0; i < nPt; i++)
// 			{
// 				QPoint oPt(vDrawShape[2].vCorner[i].nXY[0], vDrawShape[2].vCorner[i].nXY[1]);
// 				lstUpCurveCtrlPts.push_back(oPt);
// 			}
// 			// �ƶ������������� ����
// 			int nPartNum = vlstPartPt.size(); 
// 			vlstDrawPartPt = vlstPartPt; 
// 			QPoint oMove(nMoveXY[0], nMoveXY[1]); 
// 			for (int i = 0; i < nPartNum; i++)
// 			{
// 				int nPtNum = vlstDrawPartPt[i].size(); 
// 				for ( int j = 0 ; j < nPtNum ; j++ )
// 				{
// 					vlstDrawPartPt[i][j] += oMove; 
// 				}
// 			}
// 		}
// 	}
//     QPainter painter(this);
// 	// ��ս����
// 	DrawArmHead(&painter, vDrawShape); 
// 
//     painter.setPen(DRAWPOINTS_PEN);
// 
// 	int n = lstUpCurveCtrlPts.size() - 1;
//     // �����Ƶ�
//     for (int i = n-1; i >= 0; i--)
//         painter.drawPoint(lstUpCurveCtrlPts[i]);
// 
//     // ������������
//     if (showControlLines) 
// 	{
//         painter.setPen(DRAWCONTROL_PEN);
// 
//         for (int i = n - 1; i >= 0; i--)
//             painter.drawLine(lstUpCurveCtrlPts[i], lstUpCurveCtrlPts[i + 1]);
//     }
// 
// }

//------------------------------------------------------------------------------
//��갴����Ϣ
void ArmDesignWidget::TransPtToPixel(QList<QPointF> lstPt, VPIXELPOS& vPt)
{
	PixelPos oPixel; 
	VPIXELPOS vTmp = vPt; 
	vPt.clear(); 

	int i = 0; 
	int nPtNum = lstPt.size(); 
	oPixel.nLineWidth = 1; 
	for (i = 0; i < nPtNum; i++)
	{
		oPixel.nXY[0] = lstPt[i].x(); 
		oPixel.nXY[1] = lstPt[i].y(); 
		vPt.push_back(oPixel); 
	}

	return; 
}

// �ж��Ƿ��ǿɱ༭���Ƶ�
int IsEditCtrlPt(QPoint oPt, LSTEDGEDES const& vEdge, int& nSelPt, int& nSelEdgeIdx)
{
	nSelEdgeIdx = -1; 
	nSelPt = -1; 
	int nEdgeNum = vEdge.size(); 
	int nPtNum = 0; 
	int i = 0; 
	int j = 0; 
	EdgeDes oEdge; 
	for ( i = 0 ; i < nEdgeNum ; i++ )
	{
		oEdge = vEdge[i]; 
		// Ŀǰֻ�������߶ν����༭
		if( oEdge.nEdgeType != 2 )
			continue;
		nPtNum = oEdge.lstCtrlPts.size(); 
		// ����ѡ��β������ ��Ӱ���ڽ�����
		for ( j = 1 ; j < nPtNum-1 ; j++ )
		{
			if (qAbs(oEdge.lstCtrlPts[j].x() - oPt.x()) < POINT_TOLLERANCE && qAbs(oEdge.lstCtrlPts[j].y() - oPt.y()) < POINT_TOLLERANCE)
			{
				nSelEdgeIdx = i;
				nSelPt = j;
				return 0;
			}
		}
	}
	return 0; 
}

//��갴����Ϣ
void ArmDesignWidget::mousePressEvent(QMouseEvent *event)
{
	QPoint oPos = event->pos(); 
    // �ж��Ƿ��ڿ��Ƶ��ϵ��
	int nRe = IsEditCtrlPt(oPos, lstUpEdgeDrawCtrlPts, iSelectedPt, iSelectedEdge); 
	if (iSelectedPt >= 0)
	{
		if (event->button() == Qt::RightButton)
		{
			lstUpEdgeDrawCtrlPts[iSelectedEdge].lstCtrlPts.removeAt(iSelectedPt); 
			lstUpEdgeCtrlPts[iSelectedEdge].lstCtrlPts.removeAt(iSelectedPt);
			oArmHeadDesign.oDesignData.lstEdgeDes = lstUpEdgeCtrlPts;
		}
		update();
		return;
    }

    // ��ӿ��Ƶ��ж�
    if (event->button() != Qt::LeftButton)
        return;
	// �ж��Ƿ��ڿ�������
	if (belongs(oPos, lstUpEdgeDrawCtrlPts, iSelectedPt, iSelectedEdge))
	{
		// �����¼����־
		bCalPartShape = true;
		bCalDesign = true;

		lstUpEdgeDrawCtrlPts[iSelectedEdge].lstCtrlPts.insert(iSelectedPt, oPos);
		newPoints.push(iSelectedPt);

		TransPtToPixel(lstUpCurveCtrlPts, vDrawShape[2].vCorner);

		QPointF oOri = lstUpEdgeCtrlPts[0].lstCtrlPts[0];
		QPointF oDraw = lstUpEdgeDrawCtrlPts[0].lstCtrlPts[0];
		// �����������תΪͼֽ���ݲ����뵽��Ӧ������
		int nXY[2] = { 0 };
		nXY[0] = -nMoveXY[0];
		nXY[1] = -nMoveXY[1];
		QPointF oPtDesign(oPos.x() + nXY[0], oPos.y() + nXY[1]);
		lstUpEdgeCtrlPts[iSelectedEdge].lstCtrlPts.insert(iSelectedPt, oPtDesign);
		oArmHeadDesign.oDesignData.lstEdgeDes = lstUpEdgeCtrlPts;
	}
	if (bCalDesign)
	{
		CalDesignShape();
		bCalDesign = false;
		bCalCen = true;
	}
	update();

	return; 
}

//------------------------------------------------------------------------------
// �ͷ���� �ͷ�ѡ���

void ArmDesignWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) 
	{
        iSelectedPt = -1;
		iSelectedEdge = -1; 
        update();
    }
}

//------------------------------------------------------------------------------
// �ƶ����

void ArmDesignWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton && iSelectedPt >= 0)
	{
		bCalDesign = true;

		QPoint oNewPos = event->pos(); 
		int nPtMove[2]; 
		nPtMove[0] = oNewPos.x() - lstUpEdgeDrawCtrlPts[iSelectedEdge].lstCtrlPts[iSelectedPt].x();
		nPtMove[1] = oNewPos.y() - lstUpEdgeDrawCtrlPts[iSelectedEdge].lstCtrlPts[iSelectedPt].y();
		if (nPtMove[0] > 50 || nPtMove[1] > 50)
		{
			return; 

			QString sInfo = QString("Ori : %1 %2 ").arg(lstUpEdgeDrawCtrlPts[iSelectedEdge].lstCtrlPts[iSelectedPt].x()).arg(lstUpCurveCtrlPts[iSelectedPt].y());
			emit OutLog(sInfo);
			sInfo = QString("now : %1 %2 ").arg(oNewPos.x()).arg(oNewPos.y());
			emit OutLog(sInfo);
			sInfo = QString("move : %1 %2 ").arg(nPtMove[0]).arg(nPtMove[1]);
			emit OutLog(sInfo);
		}
		lstUpEdgeDrawCtrlPts[iSelectedEdge].lstCtrlPts[iSelectedPt] = oNewPos;

		int nXY[2] = { 0 };
		nXY[0] = -nMoveXY[0];
		nXY[1] = -nMoveXY[1];
		QPointF oPtDesign(oNewPos.x() + nXY[0], oNewPos.y() + nXY[1]);
		lstUpEdgeCtrlPts[iSelectedEdge].lstCtrlPts[iSelectedPt] = oPtDesign;
		oArmHeadDesign.oDesignData.lstEdgeDes = lstUpEdgeCtrlPts;

		lstUpCurveCtrlPts.clear();

		if (bCalDesign)
		{
			bCalDesign = false;
			CalDesignShape();
			bCalCen = true;
		}

        update();
    }
}

void ArmDesignWidget::resizeEvent(QResizeEvent *event)
{
	bCalCen = true; 
	bCalDesign = false; 
	// ������ ��ʱ������� ����ֻ�ǽ���ʱ����
	int nRe = CalDesignShape();

	QSize oCurSize = event->size(); 
	int nXY[2]; 
	nXY[0] = (oCurSize.width() - oPreSize.width()) / 2; 
	nXY[1] = (oCurSize.height() - oPreSize.height()) / 2;

	TransPos(vDrawShape, nXY); 
	oPreSize = oCurSize; 

	QString sInfo = QString("Pre : %1 %2 Cur : %3 %4").arg(oPreSize.width()).arg(oPreSize.height()).arg(oCurSize.width()).arg(oCurSize.height()); 
	emit OutLog(sInfo); 
	return; 
}

//------------------------------------------------------------------------------
// �����¼�

void ArmDesignWidget::keyPressEvent(QKeyEvent *event)
{
    int iPoint;

    switch (event->key()) 
	{
    case Qt::Key_Escape:
        if (!newPoints.empty()) 
		{
            iPoint = newPoints.pop();
			lstUpCurveCtrlPts.removeAt(iPoint);

            if (iPoint == iSelectedPt)
                iSelectedPt = -1;

            update();
        }
        break;

    case Qt::Key_F1:
        showHelp = !showHelp;
        update();
        break;

    case Qt::Key_F2:
        showControlLines = !showControlLines;
        update();
        break;

    case Qt::Key_F3:
        curveColor = QColorDialog::getColor(curveColor, this, "Seleccione color");
        update();
        break;

    case Qt::Key_F4:
        exportCurve();
		break;
    }
}

//------------------------------------------------------------------------------
// �������Bezier�����ϵĵ�

QPoint ArmDesignWidget::bezier(double t)
{
    const int n = lstUpCurveCtrlPts.size() - 1;
    QPointF sum;

    for (int i = 0; i <= n; i++)
	{
// 		double fBe = bernstein(i, n, t);
// 		QPointF oPt(lstUpCurveCtrlPts[i]);
		sum += QPointF(lstUpCurveCtrlPts[i]) * bernstein(i, n, t);
	}

    return sum.toPoint();
}

QPoint ArmDesignWidget::CalBezier(LISTPT const& vCorner,double t)
{
	int nNumPt = vCorner.size() - 1;
    QPointF sum;

    for (int i = 0; i <= nNumPt; i++)
	{
		sum += QPointF(vCorner[i]) * bernstein(i, nNumPt, t);
	}

    return sum.toPoint();
}
//------------------------------------------------------------------------------
// ȷ�������������Ƶ������������

bool ArmDesignWidget::belongs(const QPoint &point, QPointF a, QPointF b)
{
    double tx, ty;
    bool irresolvableTx = false, irresolvableTy = false;

    if (qAbs(b.x() - a.x()) <= POINT_TOLLERANCE) // Linea vertical
        irresolvableTx = true;
    else
        tx = (double)(point.x() - a.x()) / (b.x() - a.x());

    if (qAbs(b.y() - a.y()) <= POINT_TOLLERANCE) // Linea horizontal
        irresolvableTy = true;
    else
        ty = (double)(point.y() - a.y()) / (b.y() - a.y());

    if (irresolvableTx && irresolvableTy)
        return true;
    else if (irresolvableTx)
        return ty > 0.0 && ty < 1.0 && qAbs(a.x() - point.x()) < POINT_TOLLERANCE;
    else if (irresolvableTy)
        return tx > 0.0 && tx < 1.0 && qAbs(a.y() - point.y()) < POINT_TOLLERANCE;
    else
        return tx > 0.0 && tx < 1.0 && qAbs(tx - ty) < PARAM_TOLLERANCE;
}

bool ArmDesignWidget::belongs(const QPoint &point, int iPoint1, int iPoint2)
{
    QPointF a = lstUpCurveCtrlPts.at(iPoint1);
    QPointF b = lstUpCurveCtrlPts.at(iPoint2);
    
	return belongs(point, a, b); 
}

bool ArmDesignWidget::belongs(const QPoint &point, LSTEDGEDES const& vEdge,int& nSelPt,int& nSelEdge)
{
	nSelEdge = -1;
	nSelPt = -1;
	int nEdgeNum = vEdge.size();
	int nPtNum = 0;
	int i = 0;
	int j = 0;
	EdgeDes oEdge;
	for (i = 0; i < nEdgeNum; i++)
	{
		oEdge = vEdge[i];
		// Ŀǰֻ�������߶ν����༭
		if (oEdge.nEdgeType != 2)
			continue;
		nPtNum = oEdge.lstCtrlPts.size();
		// ����ѡ��β������ ��Ӱ���ڽ�����
		for (j = 0; j < nPtNum - 1; j++)
		{
			if ( belongs(point, oEdge.lstCtrlPts[j], oEdge.lstCtrlPts[j+1]) )
			{
				nSelEdge = i;
				nSelPt = j+1;
				return true;
			}
		}
	}

	return false;
}

// ���߼���ؿ����ߵ����ΪͼƬ

void ArmDesignWidget::exportCurve()
{
    static const double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
    static const double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;
    const int n = lstUpCurveCtrlPts.size() - 1;

    QString fileName = QFileDialog::getSaveFileName(this, "Curve Image", QDir::homePath(), "Images (*.bmp *.gif *.jpg *.jpeg *.png);;All Files (*.*)");
    QImage image(size(), QImage::Format_RGB32);
    QPainter painter;

    image.fill(QColor(Qt::white).rgb());
    painter.begin(&image);

    // ���Ƶ�

    painter.setPen(DRAWPOINTS_PEN);

    for (int i = n; i >= 0; i--)
        painter.drawPoint(lstUpCurveCtrlPts[i]);

    // ������

    if (showControlLines) 
	{
        painter.setPen(DRAWCONTROL_PEN);

        for (int i = n - 1; i >= 0; i--)
            painter.drawLine(lstUpCurveCtrlPts[i], lstUpCurveCtrlPts[i + 1]);
    }

    // Bezier����

    painter.setPen(QPen(curveColor, DRAWBEZIER_TICK));

    for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
        painter.drawPoint(bezier(t));

    painter.end();
    image.save(fileName);
}
