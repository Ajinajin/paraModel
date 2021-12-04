#include "bqgraphicsview.h"

#include "ParaType.h"

BQGraphicsView::BQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	// 隐藏水平/竖直滚动条
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// 设置场景范围
	//setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
	setSceneRect(0, 0, 2000, 2000);
	// 反锯齿
	setRenderHints(QPainter::Antialiasing);
}

void BQGraphicsView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
	emit GraphicsViewFocus(true);
}

void BQGraphicsView::wheelEvent(QWheelEvent* event)
{
	double factor = event->delta() > 0 ? 1.1 : 1 / 1.1;

	this->viewport()->update();

}

