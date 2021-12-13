
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <QList>
#include <QStack>

#include "ParaType.h"

class BezierWidget : public QWidget
{
    Q_OBJECT

public:
	BezierWidget(QWidget *parent = 0);
signals:
	void OutLog(QString sInfo); 
protected:
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *event) ;
private:
public:
    QList<QPoint> lstUpCtrlPts;		// 曲线样条控制点列表
    QStack<int> newPoints;			// Pila de puntos nuevos
    QColor curveColor;				// 画曲线颜色
    int iSelected;					// 当前选择的样条控制点索引
    bool showHelp;					// 显示帮助
    bool showControlLines;			// 显示控制线
	bool bCalCen; 
	int nScreenWH[2]; 
	QSize oPreSize; 
	VSHAPE vDrawShape;				// 整个战斗部外形绘制坐标尺寸
	VSHAPE vOriginShape;			// 战斗部外形原始坐标尺寸
    // 计算给定t值的曲线点坐标
    QPoint bezier(double t);
	QPoint CalBezier(VPIXELPOS vCorner, double t); 

    // 判断当前点属于那条控制线/点
    bool belongs(const QPoint &point, int iPoint1, int iPoint2);

    // 输出曲线
    void exportCurve();
	void TransPos(VSHAPE & vShape, int nXY[2]); 
	void TransPos(VPIXELPOS & vPt, int nXY[2]);
	void ReflectPos(VPIXELPOS & vPt, int nXY[2]);
	QColor ColorHelper(int unitIdx);
	void DrawArmHead(QPainter * pPaint, VSHAPE const& vShape); 
};

#endif // WIDGET_H
