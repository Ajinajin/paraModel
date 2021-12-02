#include "GLWidget.h"
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <qdebug.h> 


MyItem::MyItem()
{
    brushColor = Qt::red;
      
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);

}
void MyItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*,QWidget*)
{ 
    painter->setBrush(this->brushColor);

    int m = points.size();
    qDebug() << m << endl;
    // 至少是一条线
    if (m < 6) { 
        return;
    }
    painter->setPen(QPen(colors[int(points[m - 1])], points[m - 2]));
    qDebug() << points.back() << endl;

    //painter->setBrush(brushColor);
    //painter->drawRect(-10, -10, 20, 20);

    //static const QPointF pts[m] = {
    //QPointF(200.0, 80.0),
    //QPointF(220.0, 10.0),
    //QPointF(270.0, 30.0),
    //QPointF(310.0, 70.0)
    //};
    std::vector<QPointF> pts;
    for (int i = 0; i < points.size() - 2; i += 2) {
        pts.push_back(QPointF(points[i], points[i + 1]));
    }
    // 使用四个点绘制多边形
    painter->drawPolygon(pts.data(), pts.size());

}

QRectF MyItem::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-10 - adjust, -10 - adjust,
        20 + adjust, 20 + adjust);
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	//QGraphicsItem::mousePressEvent(event);
}
void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseMoveEvent(event);
}
void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseReleaseEvent(event);
}

void MyItem::handleKeyPressEvent(QKeyEvent* event) {
    QGraphicsItem::keyPressEvent(event);
}

void MyItem::handleKeyRleaseEvent(QKeyEvent* event) {
    QGraphicsItem::keyPressEvent(event);
}