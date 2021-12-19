
#include "BezierWidget.h"
#include "BezierOpt.h"
#include <QIcon>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QColorDialog>

#include "WarheadDataConvert.h"

//------------------------------------------------------------------------------
// Constructor

BezierWidget::BezierWidget(QWidget *parent) : QWidget(parent)
{
    iSelected = -1;
    showHelp = showControlLines = false;
	showControlLines = true; 
	bCalCen = true; 
	bCalPartShape = true; 
    curveColor = DRAWBEZIER_COLOR;

    setMinimumSize(MINIMUM_SIZE);
    setWindowTitle("Bezier Curve");
//  setWindowIcon(QIcon(":/bezier.png"));
}

//------------------------------------------------------------------------------
// 显示曲线短点

void BezierWidget::showEvent(QShowEvent *)
{
//     lstUpCtrlPts << FIRST_POINT << LAST_POINT;
}

//------------------------------------------------------------------------------
// 显示刷新

void BezierWidget::TransPos(VSHAPE & vShape, int nXY[2])
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
// 平移
void BezierWidget::TransPos(VPIXELPOS & vPt, int nXY[2])
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
// 对称
void BezierWidget::ReflectPos(VPIXELPOS & vPt, int nXY[2])
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
		// 左右对称 镜像
		if (nXY[0] != 0)
			oPixel.nXY[0] = 2*nXY[0] - oPixel.nXY[0];
		// 左右对称 镜像
		else
			oPixel.nXY[1] = 2 * nXY[1] - oPixel.nXY[1];
		vPt.push_back(oPixel); 
	}

	return; 
}

QColor BezierWidget::ColorHelper(int unitIdx)
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

// 初始化连接点
int BezierWidget::CalLinkPt()
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
// 由数据大边 外壳控制点生成曲线
int BezierWidget::CalCurvePt(LISTPT const& lstCtrlPt,LISTPT& lstCurvePt)
{
	// 计算曲线点
	double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;

	lstCurvePt.clear(); 
	QPoint oStart = CalBezier(lstCurvePt, 0.0);
	for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
	{
		oStart = CalBezier(lstCtrlPt, t);
		lstCurvePt.push_back(oStart); 
	}

	return 0; 
}

// 由外壳曲线平移得到其他曲线 按坐标从大到小排列
int BezierWidget::CalAllCurvePt(LISTPT const&  lstCurvePt, VECLSTPT& vlstCurvePt)
{
	vlstCurvePt.clear(); 
	LISTPT oCurList = lstCurvePt; 
	vlstCurvePt.push_back(oCurList);
	float fOffY[8] = { 0 }; 
	fOffY[1] = -fT[1]; 
	fOffY[2] = -(fR[1]-fR[2]-fT[2]);
	fOffY[3] = -(fR[1] - fR[2]);
	// 同一侧的递减
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
	// 不同侧的对称
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

int MergeLoopShape(LISTPT const& lpt1, LISTPT const& lpt2, LISTPT const& lFrontPt,LISTPT const & lBackPt, int nIdx, LISTPT& lShapePt)
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
// 计算上下 外壳 内衬区域的轮廓点集
int BezierWidget::CalAllPartPt(LISTPT const&  lstLinkFPt,LISTPT const&  lstLinkBPt, VECLSTPT const& vlstCurvePt, VECLSTPT& vlstPartPt)
{
	vlstPartPt.clear(); 
	LISTPT lstShape; 
	// 次序为从大到小 外壳 内衬 内衬 外壳
	for ( int i = 0 ; i < 4 ; i++ )
	{
		MergeLoopShape(vlstCurvePt[i * 2], vlstCurvePt[i * 2 + 1], lstLinkFPt, lstLinkBPt, i*2, lstShape); 
		vlstPartPt.push_back(lstShape); 
	}
	// 得到炸药区域的轮廓点集
	MergeLoopShape(vlstCurvePt[3], vlstCurvePt[4], lstLinkFPt, lstLinkBPt, 3, lstShape); 
	vlstPartPt.push_back(lstShape); 
	return 0; 
}

// 画战斗部
void BezierWidget::DrawSubPart(QPainter * pPaint, SimpleShape const& oCurShape)
{
	QPen linePen(ColorHelper(0), 2);
	linePen.setCapStyle(Qt::RoundCap);
	// 矩形
	if (oCurShape.unitType == 1)
	{
		pPaint->setPen(linePen);
		pPaint->setBrush(ColorHelper(oCurShape.unitIdx));
		pPaint->drawRect(oCurShape.nCen[0] - oCurShape.nWH[0] / 2, oCurShape.nCen[1] - oCurShape.nWH[1] / 2, oCurShape.nWH[0], oCurShape.nWH[1]);
	}
	// 多边形

	else if (oCurShape.unitType == 3)
	{
		// 画曲线
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
	// 直线
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

void BezierWidget::DrawArmHead(QPainter * pPaint, VSHAPE const& vShape)
{
	if (vShape.empty())
		return; 
	// 画前后盖
	DrawSubPart(pPaint, vShape[0]); 
	DrawSubPart(pPaint, vShape[1]);

	QPen linePen(ColorHelper(0), 2);
	linePen.setCapStyle(Qt::RoundCap);
	int nPartNum = vlstDrawPartPt.size();
	for ( int i = 0 ; i < nPartNum ; i++ )
	{
		pPaint->setPen(linePen);

		QPointF oStart = vlstDrawPartPt[i][0];
		int nPtNum = vlstDrawPartPt[i].size();
		QPainterPath path;
		QPointF oEnd;
		path.moveTo(vlstDrawPartPt[i][0]); 
		for (int j = 0; j < nPtNum; j++)
		{
			path.lineTo(vlstDrawPartPt[i][j]);
		}

		QColor color = ColorHelper(i+2);
		pPaint->fillPath(path, color);
	}
// 	// 画外壳
// 	DrawSubPart(pPaint, vShape[2]);
// 	DrawSubPart(pPaint, vShape[5]);
// 	// 画内衬
// 	DrawSubPart(pPaint, vShape[3]);
// 	DrawSubPart(pPaint, vShape[4]);
// 	// 画弹药
// 	DrawSubPart(pPaint, vShape[6]);
	// 画弹芯
	DrawSubPart(pPaint, vShape[7]);


	return; 
}

// 基于图纸坐标计算四个弯曲区域轮廓在图纸中的坐标
// 需要先有 vDesignShape lstUpCtrlPts数据
// 需要在控制点变动的时候计算
int BezierWidget::CalDesignShape()
{
	int nRe = 0; 
	// 前盖后盖直接转 0 1 不管
	// 2345 是 四条曲线 转为4个区域轮廓

	// 计算连接点
	nRe = CalLinkPt();
	// 计算上方外壳曲线
	nRe = CalCurvePt(lstUpCtrlPts, lstUpCurvePts); 
	// 计算所有曲线
	nRe = CalAllCurvePt(lstUpCurvePts, vlstCurvePt);
	// 计算所有轮廓点
	nRe = CalAllPartPt(lstLinkFrontPt, lstLinkBackPt, vlstCurvePt, vlstPartPt); 

	return 0; 
}

void BezierWidget::paintEvent(QPaintEvent *)
{
    double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
    double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;

	// 是否需要计算图纸形状尺寸
	// 是否需要居中计算
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

			lstUpCtrlPts.clear();
			int nPt = vDrawShape[2].vCorner.size();
			for (int i = 0; i < nPt; i++)
			{
				QPoint oPt(vDrawShape[2].vCorner[i].nXY[0], vDrawShape[2].vCorner[i].nXY[1]);
				lstUpCtrlPts.push_back(oPt);
			}
			// 移动曲线轮廓坐标 居中
			int nPartNum = vlstPartPt.size(); 
			vlstDrawPartPt = vlstPartPt; 
			QPoint oMove(nMoveXY[0], nMoveXY[1]); 
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
	// 画战斗部
	DrawArmHead(&painter, vDrawShape); 

    painter.setPen(DRAWPOINTS_PEN);

	int n = lstUpCtrlPts.size() - 1;
    // 画控制点
    for (int i = n-1; i >= 0; i--)
        painter.drawPoint(lstUpCtrlPts[i]);

    // 画控制样条线
    if (showControlLines) 
	{
        painter.setPen(DRAWCONTROL_PEN);

        for (int i = n - 1; i >= 0; i--)
            painter.drawLine(lstUpCtrlPts[i], lstUpCtrlPts[i + 1]);
    }

    // Bezier曲线
//     painter.setPen(QPen(curveColor, DRAWBEZIER_TICK));
// 
// 	QPoint oStart = bezier(0.0); 
// 	QPoint oEnd ;
//     for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
// 	{
// 		oEnd = bezier(t); 
// 		painter.drawLine(oStart, oEnd); 
// 		oStart = oEnd; 
// 	}

}

//------------------------------------------------------------------------------
//鼠标按键消息
void TransPtToPixel(QList<QPointF> lstPt, VPIXELPOS& vPt)
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

//鼠标按键消息
void BezierWidget::mousePressEvent(QMouseEvent *event)
{
	QPoint oPos = event->pos(); 
    // 判断是否在控制点上点击
    for (int i = lstUpCtrlPts.size() - 1; i >= 0; i--)
	{
        if (qAbs(lstUpCtrlPts[i].x() - event->x()) < POINT_TOLLERANCE && qAbs(lstUpCtrlPts[i].y() - event->y()) < POINT_TOLLERANCE)
		{
            if (event->button() == Qt::LeftButton) 
			{
                iSelected = i;
            }
            else if (event->button() == Qt::RightButton && i != 0 && i != lstUpCtrlPts.size() - 1)
			{
				lstUpCtrlPts.removeAt(i);
            }

			update();
            return;
        }
    }

    // 添加控制点判断
    if (event->button() != Qt::LeftButton || lstUpCtrlPts.size() == MAX_POINTS)
        return;
	// 判断是否在控制线上
    for (int i = lstUpCtrlPts.size() - 1; i >= 1; i--)
	{
        if (belongs(event->pos(), i - 1, i)) 
		{
			bCalPartShape = true;

			lstUpCtrlPts.insert(i, event->pos());
            newPoints.push(i);
            iSelected = i;

			TransPtToPixel(lstUpCtrlPts, vDrawShape[2].vCorner); 
			
			PixelPos oOri = vDesignShape[2].vCorner[0]; 
			PixelPos oDraw = vDrawShape[2].vCorner[0]; 
			// 将vDrawShape数据反写回vDesignShape中
			int nXY[2] = { 0 };
			nXY[0] = -nMoveXY[0]; 
			nXY[1] = -nMoveXY[1]; 
			vDesignShape[2].vCorner = vDrawShape[2].vCorner; 
			TransPos(vDesignShape[2].vCorner, nXY);

			lstUpCtrlPts.clear();
			int nPt = vDesignShape[2].vCorner.size();
			for (int i = 0; i < nPt; i++)
			{
				QPoint oPt(vDesignShape[2].vCorner[i].nXY[0], vDesignShape[2].vCorner[i].nXY[1]);
				lstUpCtrlPts.push_back(oPt);
			}

			if (bCalPartShape)
			{
				bCalPartShape = false;
				CalDesignShape();
				bCalCen = true;
			}

            update();
            break;
        }
    }

	return; 
}

//------------------------------------------------------------------------------
// 释放鼠标 释放选择点

void BezierWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) 
	{
        iSelected = -1;
        update();
    }
}

//------------------------------------------------------------------------------
// 移动鼠标

void BezierWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton && iSelected >= 0)
	{
		bCalPartShape = true;

		QPoint oNewPos = event->pos(); 
		int nPtMove[2]; 
		nPtMove[0] = oNewPos.x() - lstUpCtrlPts[iSelected].x(); 
		nPtMove[1] = oNewPos.y() - lstUpCtrlPts[iSelected].y(); 
		if (nPtMove[0] > 50 || nPtMove[1] > 50)
		{
			return; 

			QString sInfo = QString("Ori : %1 %2 ").arg(lstUpCtrlPts[iSelected].x()).arg(lstUpCtrlPts[iSelected].y());
			emit OutLog(sInfo);
			sInfo = QString("now : %1 %2 ").arg(oNewPos.x()).arg(oNewPos.y());
			emit OutLog(sInfo);
			sInfo = QString("move : %1 %2 ").arg(nPtMove[0]).arg(nPtMove[1]);
			emit OutLog(sInfo);
		}
		lstUpCtrlPts[iSelected] = event->pos();
		TransPtToPixel(lstUpCtrlPts, vDrawShape[2].vCorner);

		int nXY[2] = { 0 };
		nXY[0] = -nMoveXY[0];
		nXY[1] = -nMoveXY[1];
		vDesignShape[2].vCorner = vDrawShape[2].vCorner;
		TransPos(vDesignShape[2].vCorner, nXY);

// 		QString sInfo = QString("move : %1 %2 ").arg(nXY[0]).arg(nXY[1]);
// 		emit OutLog(sInfo);

		lstUpCtrlPts.clear();

		int nPt = vDesignShape[2].vCorner.size();
		for (int i = 0; i < nPt; i++)
		{
			QPoint oPt(vDesignShape[2].vCorner[i].nXY[0], vDesignShape[2].vCorner[i].nXY[1]);
			lstUpCtrlPts.push_back(oPt);

// 			sInfo = QString("move : %1 %2 ").arg(oPt.x()).arg(oPt.y());
// 			emit OutLog(sInfo);
		}

		if (bCalPartShape)
		{
			bCalPartShape = false;
			CalDesignShape();
			bCalCen = true;
		}

        update();
    }
}

void BezierWidget::resizeEvent(QResizeEvent *event)
{
	bCalCen = true; 

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
// 键盘事件

void BezierWidget::keyPressEvent(QKeyEvent *event)
{
    int iPoint;

    switch (event->key()) 
	{
    case Qt::Key_Escape:
        if (!newPoints.empty()) 
		{
            iPoint = newPoints.pop();
			lstUpCtrlPts.removeAt(iPoint);

            if (iPoint == iSelected)
                iSelected = -1;

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
// 计算计算Bezier曲线上的点

QPoint BezierWidget::bezier(double t)
{
    const int n = lstUpCtrlPts.size() - 1;
    QPointF sum;

    for (int i = 0; i <= n; i++)
	{
// 		double fBe = bernstein(i, n, t);
// 		QPointF oPt(lstUpCtrlPts[i]);
		sum += QPointF(lstUpCtrlPts[i]) * bernstein(i, n, t);
	}

    return sum.toPoint();
}

QPoint BezierWidget::CalBezier(LISTPT const& vCorner,double t)
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
// 确定在那两个控制点的连接样条上

bool BezierWidget::belongs(const QPoint &point, int iPoint1, int iPoint2)
{
    const QPointF &a = lstUpCtrlPts.at(iPoint1);
    const QPointF &b = lstUpCtrlPts.at(iPoint2);
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

// 曲线及相关控制线等输出为图片

void BezierWidget::exportCurve()
{
    static const double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
    static const double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;
    const int n = lstUpCtrlPts.size() - 1;

    QString fileName = QFileDialog::getSaveFileName(this, "Curve Image", QDir::homePath(), "Images (*.bmp *.gif *.jpg *.jpeg *.png);;All Files (*.*)");
    QImage image(size(), QImage::Format_RGB32);
    QPainter painter;

    image.fill(QColor(Qt::white).rgb());
    painter.begin(&image);

    // 控制点

    painter.setPen(DRAWPOINTS_PEN);

    for (int i = n; i >= 0; i--)
        painter.drawPoint(lstUpCtrlPts[i]);

    // 控制线

    if (showControlLines) 
	{
        painter.setPen(DRAWCONTROL_PEN);

        for (int i = n - 1; i >= 0; i--)
            painter.drawLine(lstUpCtrlPts[i], lstUpCtrlPts[i + 1]);
    }

    // Bezier曲线

    painter.setPen(QPen(curveColor, DRAWBEZIER_TICK));

    for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
        painter.drawPoint(bezier(t));

    painter.end();
    image.save(fileName);
}
