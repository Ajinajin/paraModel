#ifndef BQGRAPHICSSCENE_H
#define BQGRAPHICSSCENE_H

#include <QList>
#include <QStack>
#include <QGraphicsScene>
#include "bpointitem.h"

class BQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BQGraphicsScene(QObject *parent = nullptr);

    void startCreate();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void updatePoint(QPointF p, QList<QPointF> list, bool isCenter);
    void createFinished();

// 曲线相关数据
public:
	int nArmHeadType; 
	QPointF oScePt;
	int iUpSelected;						// 当前选择的控制点
	int iDnSelected;						// 当前选择的控制点
	QList<QPointF> listUpCurveCtrlPt;		// 上方曲线控制点 按画布坐标保存
	QList<QPointF> listDnCurveCtrlPt;		// 下方曲线控制点 按画布坐标保存
	QStack<int> newUpPoints;				// 控制点索引
	QStack<int> newDnPoints;				// 控制点索引
	bool belongs(const QPoint &point, int iPoint1, int iPoint2, QList<QPointF> const& lstPt);

protected:
    QList<QPointF> m_list;
    bool is_creating_BPolygon;
};

#endif // BQGRAPHICSSCENE_H
