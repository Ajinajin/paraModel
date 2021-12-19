
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <QList>
#include <QVector>
#include <QStack>

#include "ParaType.h"

typedef QList<QPointF> LISTPT; 
typedef QVector<LISTPT> VECLSTPT; 

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
    QList<QPointF> lstUpCtrlPts;		// 曲线样条控制点列表
    QStack<int> newPoints;			// Pila de puntos nuevos
    QColor curveColor;				// 画曲线颜色
    int iSelected;					// 当前选择的样条控制点索引
    bool showHelp;					// 显示帮助
    bool showControlLines;			// 显示控制线
	bool bCalCen;					// 居中计算标志
	bool bCalPartShape;				// 轮廓曲线计算标志
	int nScreenWH[2]; 
	int nMoveXY[2];					// 原始数据和显示数据偏移
	float fR[10], fT[10], fH[10]; 
	QSize oPreSize; 
	VSHAPE vDrawShape;				// 整个战斗部外形绘制坐标尺寸  曲线计算完成
	VSHAPE vDesignShape;			// 战斗部外形原始坐标尺寸  曲线为控制点表示
	LISTPT lstUpCurvePts;			// 外壳曲线上的点
	VECLSTPT vlstCurvePt;			// 0-7 数据从大到小8条曲线上的点
	VECLSTPT vlstPartPt;			// 0-4 5个区域轮廓点集
	VECLSTPT vlstDrawPartPt;		// 0-4 5个区域轮廓点集
	LISTPT   lstLinkFrontPt;		// 和前盖连接点
	LISTPT   lstLinkBackPt;			// 和后盖连接点

    // 计算给定t值的曲线点坐标
    QPoint bezier(double t);
	QPoint CalBezier(LISTPT const& vCorner, double t);

    // 判断当前点属于那条控制线/点
    bool belongs(const QPoint &point, int iPoint1, int iPoint2);

	// 初始化连接点
	int CalLinkPt(); 
	// 由数据大边 外壳控制点生成曲线
	int CalCurvePt(LISTPT const& lstCtrlPt, LISTPT& lstCurvePt);
 	// 由外壳曲线平移得到其他曲线 按坐标从大到小排列
	int CalAllCurvePt(LISTPT const&  lstCurvePt,VECLSTPT& vlstCurvePt);
	// 由外壳曲线平移得到其他曲线 按坐标从大到小排列
	int CalAllPartPt(LISTPT const&  lstLinkFPt, LISTPT const&  lstLinkBPt, VECLSTPT const& vlstCurvePt, VECLSTPT& vlstPartPt);
	// 基于图纸坐标计算在图纸中的坐标
	int CalDesignShape();
   // 输出曲线
    void exportCurve();
	void TransPos(VSHAPE & vShape, int nXY[2]); 
	void TransPos(VPIXELPOS & vPt, int nXY[2]);

	void ReflectPos(VPIXELPOS & vPt, int nXY[2]);
	QColor ColorHelper(int unitIdx);
	void DrawSubPart(QPainter * pPaint, SimpleShape const& oCurShape);
	void DrawArmHead(QPainter * pPaint, VSHAPE const& vShape); 
};

#endif // WIDGET_H
