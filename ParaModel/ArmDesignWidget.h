
#ifndef ARMDESIGNWIDGET_H
#define ARMDESIGNWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QList>
#include <QVector>
#include <QStack>

#include "ParaType.h"
#include "ArmHeadDesign.h"

class ArmDesignWidget : public QWidget
{
    Q_OBJECT

public:
	ArmDesignWidget(QWidget *parent = 0);
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
	CArmHeadDesign oArmHeadDesign;	// 战斗部设计对象
// 显示控制属性
public:
    QList<QPointF> lstUpCurveCtrlPts;		// 曲线样条控制点列表
    QStack<int> newPoints;			// 选择点栈
    QColor curveColor;				// 画曲线颜色
	int iSelectedEdge;				// 当前选择的边界段索引
    int iSelectedPt;				// 当前选择的样条控制点索引
    bool showHelp;					// 显示帮助
    bool showControlLines;			// 显示控制线
	bool bCalCen;					// 居中计算标志
	bool bCalDesign;				// 
	bool bCalPartShape;				// 轮廓曲线计算标志
	int nScreenWH[2]; 
	int nMoveXY[2];					// 原始数据和显示数据偏移
	float fR[20], fT[10], fH[10]; 
	QSize oPreSize; 
	VSHAPE vDrawShape;				// 整个战斗部外形绘制坐标尺寸  曲线计算完成
	VSHAPE vDesignShape;			// 战斗部外形原始坐标尺寸  曲线为控制点表示
// 绘制数据 屏幕坐标
public:
	LSTEDGEDES lstUpEdgeCtrlPts;		// 上方外壳特征点/拐点 设计图纸坐标
	LSTEDGEDES lstUpEdgeDrawCtrlPts;	// 上方外壳特征点/拐点 屏幕显示坐标
	VEDGESHAPE vDrawHeadShape;			// 整个战斗部外形绘制坐标尺寸  曲线计算完成
	VEDGESHAPE vDesignHeadShape;		// 战斗部外形原始坐标尺寸  曲线为控制点表示
	LISTPT lstUpCurvePts;			// 外壳上曲线区段的点

	VECLSTPT vAllPartDesignPts;			// 外壳上所有区段的点 设计图纸坐标 间距小
	VECLSTPT vAllPartDrawPts;			// 外壳上所有区段的点 设计图纸坐标 间距小
	VECLSTPT vAllPartPtsFor3D;			// 外壳上所有区段的点 给3维显示用 间距较大

	LISTPT lstUpEdgePts;			// 整个外壳上的点
	VECLSTPT vlstCurvePt;			// 0-7 数据从大到小8条曲线上的点
	VECLSTPT vlstPartPt;			// 0-4 5个区域轮廓点集
	VECLSTPT vlst3DCurvePt;			// 0-7 数据从大到小8条曲线上的点
	VECLSTPT vlst3DPartPt;			// 0-4 5个区域轮廓点集
	VECLSTPT vlstDrawPartPt;		// 0-4 5个区域轮廓点集

	LISTPT   lstLinkFrontPt;		// 和前盖连接点
	LISTPT   lstLinkBackPt;			// 和后盖连接点

// 计算函数
public:
    // 计算给定t值的曲线点坐标
    QPoint bezier(double t);
	QPoint CalBezier(LISTPT const& vCorner, double t);

    // 判断当前点属于那条控制线/点
    bool belongs(const QPoint &point, int iPoint1, int iPoint2);
	bool belongs(const QPoint &point, QPointF a, QPointF b); 
	bool belongs(const QPoint &point, LSTEDGEDES const& vEdge, int& nSelPt, int& nSelEdge);

	// 初始化连接点
	int CalLinkPt(); 
	int CalMultiPartLinkPt();
	// 由数据大边 外壳控制点生成曲线
	int CalCurvePt(LISTPT const& lstCtrlPt, LISTPT& lstCurvePt,LISTPT& lst3DDraw, int nDrawStep);
 	// 由外壳曲线平移得到其他曲线 按坐标从大到小排列
	int CalAllCurvePt(LISTPT const&  lstCurvePt,VECLSTPT& vlstCurvePt);
	// 由外壳曲线平移得到其他曲线 按坐标从大到小排列
	int CalAllPartPt(LISTPT const&  lstLinkFPt, LISTPT const&  lstLinkBPt, VECLSTPT const& vlstCurvePt, VECLSTPT& vlstPartPt);
	// 基于图纸坐标计算在图纸中的坐标
	int CalDesignShape();

	// 计算圆弧点 三点定圆弧
	int CalArcPts(LISTPT const& lstOriPts, LISTPT& lstArcPts, LISTPT& lst3DDraw, int nDrawStep);
	// 计算所有外壳点
	int CalDesignEdgePts(int nDrawStep);

	int MergeLoopShape(LISTPT const& lpt1, LISTPT const& lpt2, LISTPT const& lFrontPt, LISTPT const & lBackPt, int nIdx, LISTPT& lShapePt);
	void TransPtToPixel(QList<QPointF> lstPt, VPIXELPOS& vPt); 
   // 输出曲线
    void exportCurve();
	void TransPos(VSHAPE & vShape, int nXY[2]); 
	void TransPos(VPIXELPOS & vPt, int nXY[2]);
	void TransPos(LISTPT & vPt, int nXY[2]);
	void TransPos(VECLSTPT & vvPt, int nXY[2]);
	void TransPos(LSTEDGEDES & vEdge, int nXY[2]);

	void ReflectPos(VPIXELPOS & vPt, int nXY[2]);
	QColor ColorHelper(int unitIdx);
	void DrawSubPart(QPainter * pPaint, SimpleShape const& oCurShape);
// 	void DrawArmHead(QPainter * pPaint, VSHAPE const& vShape); 
	void DrawArmHead(QPainter * pPaint, VECLSTPT const& vAllDraw, VSHAPE const& vShape);
};

#endif // ARMDESIGNWIDGET_H
