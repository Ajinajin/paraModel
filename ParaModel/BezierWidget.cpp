
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
	for ( i = 0 ; i < nShapeNum ; i++ )
	{
		oShape = vTmpShape[i];
		nPtNum = oShape.vCorner.size(); 
		for ( int j = 0 ; j < nPtNum ; j++ )
		{
			oShape.vCorner[j].nXY[0] += nXY[0]; 
			oShape.vCorner[j].nXY[1] += nXY[1]; 
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
	if (unitIdx == 1)
	{
		return QColor(47, 65, 80);
	}
	else if (unitIdx == 2)
	{
		return QColor(69, 173, 206);
	}
	else if (unitIdx == 3)
	{
		return QColor(62, 179, 203);
	}
	else if (unitIdx == 4)
	{
		return QColor(255, 255, 255);
	}
	else if (unitIdx == 5)
	{
		return QColor(232, 220, 102);
	}
	else if (unitIdx == 6)
	{
		return QColor(170, 101, 96);
	}
	else if (unitIdx == 7)
	{
		return QColor(254, 235, 248);
	}
	else if (unitIdx == 8)
	{
		return QColor(204, 178, 102);
	}
	else if (unitIdx == 9)
	{
		return QColor(255, 127, 0);
	}
	else if (unitIdx == 11)
	{
		return QColor(255, 131, 158);
	}
	else if (unitIdx == 12)
	{
		return QColor(255, 255, 122);
	}
	else if (unitIdx == 13)
	{
		return QColor(190, 255, 255);
	}
	return QColor(72, 104, 146);
}

// 画战斗部
void BezierWidget::DrawArmHead(QPainter * pPaint, VSHAPE const& vShape)
{
	int nShapeNum = vShape.size(); 
	int i = 0; 
	SimpleShape oCurShape; 
	for (i = 0; i < nShapeNum; i++)
	{
		oCurShape = vShape[i]; 
		// 矩形
		if (oCurShape.unitType == 1)
		{
			pPaint->setBrush(ColorHelper(oCurShape.unitIdx));
			pPaint->drawRect(oCurShape.nCen[0]-oCurShape.nWH[0]/2, oCurShape.nCen[1]-oCurShape.nWH[1]/2, oCurShape.nWH[0], oCurShape.nWH[1]); 
		}
		// 曲线
		else if (oCurShape.unitType == 3)
		{
			// 画控制点
			pPaint->setPen(DRAWPOINTS_PEN);
			int nPtNum = oCurShape.vCorner.size(); 
			for (int j = 0; j < nPtNum ; j++ )
			{
				QPoint oPt(oCurShape.vCorner[j].nXY[0], oCurShape.vCorner[j].nXY[1]); 
				pPaint->drawPoint(oPt);
			}
			// 画控制样条线
			if (showControlLines)
			{
				pPaint->setPen(DRAWCONTROL_PEN);
				QPoint oSPt(oCurShape.vCorner[0].nXY[0], oCurShape.vCorner[0].nXY[1]);
				for (int j = 1; j < nPtNum; j++)
				{
					QPoint oEPt(oCurShape.vCorner[j].nXY[0], oCurShape.vCorner[j].nXY[1]);
					pPaint->drawLine(oSPt,oEPt);
					oSPt = oEPt; 
				}
			}
			// 画曲线
			double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
			double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;

			pPaint->setPen(QPen(curveColor, DRAWBEZIER_TICK));

			QPoint oStart = CalBezier(oCurShape.vCorner,0.0);
			QPoint oEnd;
			for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
			{
				// 		painter.drawPoint(bezier(t));
				oEnd = CalBezier(oCurShape.vCorner,t);
				pPaint->drawLine(oStart, oEnd);
				oStart = oEnd;
			}
		}
		// 直线
		else if (oCurShape.unitType == 4)
		{
// 			QPen pen = QPen(ColorHelper(oCurShape.unitIdx), oCurShape.nWH[0]);
			QPen pen = QPen(ColorHelper(1), 3);
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
	}
	return; 
}
void BezierWidget::paintEvent(QPaintEvent *)
{
    double DIFT_BEZIER = 1.0 / NPOINTS_BEZIER;
    double DIFT_WEIGHTS = 1.0 / NPOINTS_WEIGHTS;

	if (bCalCen)
	{
		bCalCen = false; 
		int nMoveXY[2]; 
		QSize oCurSize = size(); 
		if (vDrawShape.size() > 0)
		{
			vDrawShape = vOriginShape;
			nMoveXY[0] = oCurSize.width() / 2 + (vDrawShape[0].nCen[0] - vDrawShape[1].nCen[0]) / 2 + vDrawShape[0].nWH[0] / 2;
			nMoveXY[1] = oCurSize.height() / 2;
			TransPos(vDrawShape, nMoveXY);

			// 			int nXY[2] = { 0,-33 };
			lstUpCtrlPts.clear();
			int nPt = vDrawShape[2].vCorner.size();
			for (int i = 0; i < nPt; i++)
			{
				QPoint oPt(vDrawShape[2].vCorner[i].nXY[0], vDrawShape[2].vCorner[i].nXY[1]);
				lstUpCtrlPts.push_back(oPt);
			}
		}

	}
    QPainter painter(this);
	// 画战斗部
	// 先进行坐标偏离 保证可以全部画出
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
    painter.setPen(QPen(curveColor, DRAWBEZIER_TICK));

	QPoint oStart = bezier(0.0); 
	QPoint oEnd ;
    for (double t = 0.0; t <= 1.0; t += DIFT_BEZIER)
	{
		oEnd = bezier(t); 
		painter.drawLine(oStart, oEnd); 
		oStart = oEnd; 
	}

}

//------------------------------------------------------------------------------
//鼠标按键消息
void TransPtToPixel(QList<QPoint> lstPt, VPIXELPOS& vPt)
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
			lstUpCtrlPts.insert(i, event->pos());
            newPoints.push(i);
            iSelected = i;

			TransPtToPixel(lstUpCtrlPts, vDrawShape[2].vCorner); 
			int nXY[2] = { 0,-33 }; 
			vDrawShape[6].vCorner = vDrawShape[2].vCorner; 
			TransPos(vDrawShape[6].vCorner, nXY);

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
		lstUpCtrlPts[iSelected] = event->pos();
		TransPtToPixel(lstUpCtrlPts, vDrawShape[2].vCorner);
		int nXY[2] = { 0,-33 };
		vDrawShape[6].vCorner = vDrawShape[2].vCorner;
		TransPos(vDrawShape[6].vCorner, nXY);
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

QPoint BezierWidget::CalBezier(VPIXELPOS vCorner,double t)
{
	const int n = vCorner.size() - 1;
    QPointF sum;

    for (int i = 0; i <= n; i++)
	{
		QPoint oPt(vCorner[i].nXY[0], vCorner[i].nXY[1]); 
		sum += QPointF(oPt) * bernstein(i, n, t);
	}

    return sum.toPoint();
}
//------------------------------------------------------------------------------
// 确定在那两个控制点的连接样条上

bool BezierWidget::belongs(const QPoint &point, int iPoint1, int iPoint2)
{
    const QPoint &a = lstUpCtrlPts.at(iPoint1);
    const QPoint &b = lstUpCtrlPts.at(iPoint2);
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
