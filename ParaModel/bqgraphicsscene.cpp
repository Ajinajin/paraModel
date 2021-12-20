#include "bqgraphicsscene.h"
#include "BezierOpt.h"
#include "bqgraphicsitem.h"

BQGraphicsScene::BQGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    is_creating_BPolygon = false;
	for (int x = 0; x <= 10000; x += 10)
		//水平网格线 起点（x，y），终点（x，y），颜色  size是终点x坐标在这里也是网格线长度
		this->addLine(0, x, 10000, x, QPen(Qt::red));
	// Now add the horizontal lines,paint them green
	for (int y = 0; y <= 10000; y += 10)
		//垂直网格线
		this->addLine(y, 0, y, 10000, QPen(Qt::red));
}

void BQGraphicsScene::startCreate()
{
    is_creating_BPolygon = true;
    m_list.clear();
}

void BQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	oScePt = event->scenePos();
	QPointF p = event->scenePos();
	// 曲线控制相关功能函数
		// 圆筒形弹体不添加曲线控制点
	if (nArmHeadType == 0)
		return;
	// 画布上的点击点
	QPointF oCurPos = oScePt;
	// 和外壳控制点做适配

	int nPtNum = listUpCurveCtrlPt.size();
	// 判断是否在控制点上点击
	for (int i = nPtNum - 1; i >= 0; i--)
        {
		if (qAbs(listUpCurveCtrlPt[i].x() - oCurPos.x()) < POINT_TOLLERANCE && qAbs(listUpCurveCtrlPt[i].y() - oCurPos.y()) < POINT_TOLLERANCE)
		{
			if (event->button() == Qt::LeftButton)
			{
				iUpSelected = i;
				update();
			}
			else if (event->button() == Qt::RightButton && i != 0 && i != listUpCurveCtrlPt.size() - 1)
			{
				listUpCurveCtrlPt.removeAt(i);
				update();
			}

			return;
		}
	}

	// 添加控制点判断
	if (event->button() != Qt::LeftButton || listUpCurveCtrlPt.size() == MAX_POINTS)
		return;
	// 判断是否在控制线上
	QPoint oPixel(oCurPos.x(), oCurPos.y());
	for (int i = listUpCurveCtrlPt.size() - 1; i >= 1; i--)
	{
		if (belongs(oPixel, i - 1, i, listUpCurveCtrlPt))
		{
			listUpCurveCtrlPt.insert(i, oCurPos);
			newUpPoints.push(i);
			iUpSelected = i;
			// 对称更新
			QPointF oDnPt(oCurPos.x(),600 - oCurPos.y()); 
			listDnCurveCtrlPt.insert(i, oDnPt);
			QList<QGraphicsItem*> lstItem = this->items(); 
			int k = 0; 
			for (  k = 0 ; k < 10 ; k++ )
// 			for (k = 0; k < lstItem.size(); k++)
			{
				BGraphicsItem *pItem = (BGraphicsItem*)lstItem[k]; 
				if (pItem->nUnitIdx == 11)
				{
					BCurveLine *pCurve = (BCurveLine*)pItem;
					pCurve->lstCtrlPt = listUpCurveCtrlPt; 
				}
				if (pItem->nUnitIdx == 12)
				{
					BCurveLine *pCurve = (BCurveLine*)pItem;
					pCurve->lstCtrlPt = listDnCurveCtrlPt;
				}
			}
			update(); 
			break;
		}
	}
	

    if (is_creating_BPolygon) 
	{
		switch (event->buttons())
		{
		case Qt::LeftButton:
		{
            m_list.push_back(p);
            emit updatePoint(p, m_list, false);
		}
		break;
		case Qt::RightButton:
		{
			if (m_list.size() >= 3)
			{
                emit updatePoint(p, m_list, true);
                emit createFinished();
                is_creating_BPolygon = false;
                m_list.clear();
            }
		}
		break;
		default: 
			break;
		}
        }
	else 
	{
        QGraphicsScene::mousePressEvent(event);
    }
}

void BQGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	oScePt = event->scenePos();
	QPointF p = event->scenePos();

	if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton && iUpSelected >= 0)
	{
		if (iUpSelected >= 0  && iUpSelected < listUpCurveCtrlPt.size()-1 )
		{
			listUpCurveCtrlPt[iUpSelected] = event->pos();
			listDnCurveCtrlPt[iUpSelected].setY(600 - event->pos().y());
			update();
		}
	}

    if (is_creating_BPolygon) 
	{

    } 
	else 
	{
        QGraphicsScene::mouseMoveEvent(event);
    }
}

//void BQGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//	if (event->button() == Qt::LeftButton)
//	{
//		iUpSelected = -1;
//		update();
//	}
//
//	return; 
//}
bool BQGraphicsScene::belongs(const QPoint &point, int iPoint1, int iPoint2, QList<QPointF> const& lstPt)
{
	const QPointF &a = lstPt.at(iPoint1);
	const QPointF &b = lstPt.at(iPoint2);
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
