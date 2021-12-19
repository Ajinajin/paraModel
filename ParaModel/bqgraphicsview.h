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
	virtual void wheelEvent(QWheelEvent* event) override;
	/*virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;*/
};

#endif // BQGRAPHICSVIEW_H
