#include "bpointitem.h"
#include "bqgraphicsitem.h"
#include <qdebug.h>

BPointItem::BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, PointType type)
    : QAbstractGraphicsShapeItem(parent)
    , m_point(p)
    , m_type(type)
{
    this->setPos(m_point);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
		           QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    switch (type) {
    case Center:
        this->setCursor(Qt::OpenHandCursor);
        break;
    case Edge:
        this->setCursor(Qt::PointingHandCursor);
        break;
    case Special:
        this->setCursor(Qt::PointingHandCursor);
        break;
    default: break;
    }
}

QRectF BPointItem::boundingRect() const
{
	//通过这个区域拖动
    return QRectF(-4, -4, 8, 8);

}

void BPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    this->setPos(m_point);

    switch (m_type) {
    case Center:
		//中间点的形状
        painter->drawEllipse(-4, -4, 8, 8);
        break;
    case Edge:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    case Special:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    default: break;
    }
}

void BPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton ) {
        qreal dx = event->scenePos().x() - event->lastScenePos().x();
        qreal dy = event->scenePos().y() - event->lastScenePos().y();

        BGraphicsItem* item = static_cast<BGraphicsItem *>(this->parentItem());
        BGraphicsItem::ItemType itemType = item->getType();

        switch (m_type) {
        case Center: {
			//移动吸附 -1000因为初始位置在1000，1000，setpos是根据初始位置的偏移量计算，pos会有问题
			QPointF qqq;
			qqq = event->scenePos();
			qqq.setX(int((qqq.x() - 1000) / 20) * 20);
			qqq.setY(int((qqq.y() - 1000) / 20) * 20);
			item->setPos(qqq);
			
			//qDebug() << qqq.x();
            this->scene()->update();
        } break;
        case Edge: {
			//改变大小
		    m_point = this->mapToParent(event->pos());
			//this->setPos(m_point);
			this->scene()->update();
            switch (itemType) {
            case BGraphicsItem::ItemType::Ellipse: {
                BEllipse *ellipse = dynamic_cast<BEllipse *>(item);
                ellipse->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Circle: {
                BCircle *circle = dynamic_cast<BCircle *>(item);
                circle->setEdge(m_point);
                circle->updateRadius();
            } break;
            case BGraphicsItem::ItemType::Rectangle: {
                BRectangle *rectangle = dynamic_cast<BRectangle *>(item);
                rectangle->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Square: {
                BSquare *square = dynamic_cast<BSquare *>(item);
                qreal ret = m_point.x() > m_point.y() ? m_point.x() : m_point.y();
                m_point.setX(ret);
                m_point.setY(ret);
                square->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Polygon: {
                BPolygon *polygon = dynamic_cast<BPolygon *>(item);
                polygon->updatePolygon(QPointF(event->lastScenePos().x(), event->lastScenePos().y()),
                                       QPointF(event->scenePos().x(), event->scenePos().y()));
            } break;

            default: break;
            }
        } break;
        
        default: break;
        }
    }
}

//------------------------------------------------------------------------------

void BPointItemList::setRandColor()
{
    this->setColor(QColor(qrand()%256, qrand()%256, qrand()%256));
}

void BPointItemList::setColor(const QColor color)
{
    for (auto &temp : *this)
    {
        temp->setBrush(QBrush(color));
    }
}

void BPointItemList::setVisible(bool visible)
{
    for (auto &temp : *this)
    {
        temp->setVisible(visible);
    }
}
