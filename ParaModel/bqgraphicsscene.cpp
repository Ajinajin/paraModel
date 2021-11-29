#include "bqgraphicsscene.h"

BQGraphicsScene::BQGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    is_creating_BPolygon = false;
	for (int x = 0; x <= 2000; x += 20)
		//水平网格线 起点（x，y），终点（x，y），颜色  size是终点x坐标在这里也是网格线长度
		this->addLine(0, x, 2000, x, QPen(Qt::red));
	// Now add the horizontal lines,paint them green
	for (int y = 0; y <= 2000; y += 20)
		//垂直网格线
		this->addLine(y, 0, y, 2000, QPen(Qt::red));
}

void BQGraphicsScene::startCreate()
{
    is_creating_BPolygon = true;
    m_list.clear();
}

void BQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_creating_BPolygon) {
        QPointF p(event->scenePos().x(), event->scenePos().y());

        switch ( event->buttons() )
        {
        case Qt::LeftButton: {
            m_list.push_back(p);
            emit updatePoint(p, m_list, false);
        } break;
        case Qt::RightButton: {
            if (m_list.size() >= 3) {
                emit updatePoint(p, m_list, true);
                emit createFinished();
                is_creating_BPolygon = false;
                m_list.clear();
            }
        } break;
        default: break;
        }
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}
