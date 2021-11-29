#ifndef BQGRAPHICSVIEW_H
#define BQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class BQGraphicsView : public QGraphicsView
{ 
	Q_OBJECT
public:
	BQGraphicsView(QWidget* parent = nullptr);
signals:
	void GraphicsViewFocus(bool b);
protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // BQGRAPHICSVIEW_H
