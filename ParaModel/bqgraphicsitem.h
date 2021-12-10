#ifndef BQGRAPHICSITEM_H
#define BQGRAPHICSITEM_H

#include  <signal.h>
#include <QObject>
#include "bpointitem.h"

#define PI 3.1415926

// 自定义图元 - 基础类
class BGraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
	Q_OBJECT

public:
	enum ItemType {
		Circle = 0,         // 圆
		Ellipse,            // 椭圆
		Rectangle,          // 矩形
		Square,             // 正方形
		Polygon,            // 多边形
	};
	QPointF m_oldleftup;       //中心点，拖动图形
	QPointF m_center;          //中心点，拖动图形
	QPointF m_edge;            //右下角点，拉动大小
	QPointF m_leftup;          //左上角点
	BPointItemList m_pointList;//绘图形的各个点
	Qt::MouseButton operMourse;//当前鼠标操作结束后 
	int nUnitType;				//构件类型
	int nUnitIdx;				//构件id
	bool isAuxiliary;			//是否辅助线

	int nOriPos[2];				//点击位置坐标
	int nMoveXY[2];				//移动偏移
	int graphAngle;				//外部使用记录的图形方向

	ItemType m_type;
	QPointF getCenter() { return m_center; }
	void setCenter(QPointF p) { m_center = p; }

	QPointF getEdge() { return m_edge; }
	void setEdge(QPointF p) {  
		m_edge = p; }

	ItemType getType() { return m_type; }

protected:
	BGraphicsItem(QPointF center, QPointF edge, ItemType type); 
	virtual void focusInEvent(QFocusEvent* event) override;
	virtual void focusOutEvent(QFocusEvent* event) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
protected:
	QPen m_pen_isSelected;
	QPen m_pen_noSelected;
signals:
	void SceneItemMove(int nUnitType, int nUnitIdx, QPointF pos);
	void SceneMenuClick(int nUnitType, int nUnitIdx);
	void SceneMenuAddClick(int nUnitType, int nUnitIdx);
	void SceneMenuDeleteClick(int nUnitType, int nUnitIdx);
};

//------------------------------------------------------------------------------

// 椭圆
class BEllipse : public BGraphicsItem
{
	Q_OBJECT

public:
	BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;

	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
};

//------------------------------------------------------------------------------

// 圆
class BCircle : public BEllipse
{
public:
	BCircle(qreal x, qreal y, qreal radius, ItemType type);
	void updateRadius(); 
protected:
	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;

	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

protected:
	qreal m_radius;
};


//------------------------------------------------------------------------------


// 矩形
class BRectangle : public BGraphicsItem
{
public:
	BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);
	qreal wallwidth;
protected:
	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;

	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override; 
};

//------------------------------------------------------------------------------

// 正方形
class BSquare : public BRectangle
{
public:
	BSquare(qreal x, qreal y, qreal width, ItemType type);

protected:
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
};

//------------------------------------------------------------------------------

// 多边形
class BPolygon : public BGraphicsItem
{
	Q_OBJECT
public:
	bool is_create_finished;
public:
	BPolygon(ItemType type);

	QPointF getCentroid(QList<QPointF> list);
	void getMaxLength();
	void updatePolygon(QPointF origin, QPointF end);

public slots:
	void pushPoint(QPointF p, QList<QPointF> list, bool isCenter);

protected:
	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;

protected:
	qreal m_radius;
};

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

// 线
class BLine : public BGraphicsItem
{
	Q_OBJECT
public:
	BLine(ItemType type);
	QList<QPointF> point;

protected:
	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;

protected:
	qreal m_radius;
};

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

// 点
class BPoint : public BGraphicsItem
{
	Q_OBJECT
public:
	BPoint(ItemType type);
	QList<QPointF> point;

protected:
	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget) override;

protected:
	qreal m_radius;
};

//------------------------------------------------------------------------------


#endif // BQGRAPHICSITEM_H
