
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
	CArmHeadDesign oArmHeadDesign;	// ս������ƶ���
// ��ʾ��������
public:
    QList<QPointF> lstUpCurveCtrlPts;		// �����������Ƶ��б�
    QStack<int> newPoints;			// ѡ���ջ
    QColor curveColor;				// ��������ɫ
	int iSelectedEdge;				// ��ǰѡ��ı߽������
    int iSelectedPt;				// ��ǰѡ����������Ƶ�����
    bool showHelp;					// ��ʾ����
    bool showControlLines;			// ��ʾ������
	bool bCalCen;					// ���м����־
	bool bCalDesign;				// 
	bool bCalPartShape;				// �������߼����־
	int nScreenWH[2]; 
	int nMoveXY[2];					// ԭʼ���ݺ���ʾ����ƫ��
	float fR[20], fT[10], fH[10]; 
	QSize oPreSize; 
	VSHAPE vDrawShape;				// ����ս�������λ�������ߴ�  ���߼������
	VSHAPE vDesignShape;			// ս��������ԭʼ����ߴ�  ����Ϊ���Ƶ��ʾ
// �������� ��Ļ����
public:
	LSTEDGEDES lstUpEdgeCtrlPts;		// �Ϸ����������/�յ� ���ͼֽ����
	LSTEDGEDES lstUpEdgeDrawCtrlPts;	// �Ϸ����������/�յ� ��Ļ��ʾ����
	VEDGESHAPE vDrawHeadShape;			// ����ս�������λ�������ߴ�  ���߼������
	VEDGESHAPE vDesignHeadShape;		// ս��������ԭʼ����ߴ�  ����Ϊ���Ƶ��ʾ
	LISTPT lstUpCurvePts;			// ������������εĵ�

	VECLSTPT vAllPartDesignPts;			// ������������εĵ� ���ͼֽ���� ���С
	VECLSTPT vAllPartDrawPts;			// ������������εĵ� ���ͼֽ���� ���С
	VECLSTPT vAllPartPtsFor3D;			// ������������εĵ� ��3ά��ʾ�� ���ϴ�

	LISTPT lstUpEdgePts;			// ��������ϵĵ�
	VECLSTPT vlstCurvePt;			// 0-7 ���ݴӴ�С8�������ϵĵ�
	VECLSTPT vlstPartPt;			// 0-4 5�����������㼯
	VECLSTPT vlst3DCurvePt;			// 0-7 ���ݴӴ�С8�������ϵĵ�
	VECLSTPT vlst3DPartPt;			// 0-4 5�����������㼯
	VECLSTPT vlstDrawPartPt;		// 0-4 5�����������㼯

	LISTPT   lstLinkFrontPt;		// ��ǰ�����ӵ�
	LISTPT   lstLinkBackPt;			// �ͺ�����ӵ�

// ���㺯��
public:
    // �������tֵ�����ߵ�����
    QPoint bezier(double t);
	QPoint CalBezier(LISTPT const& vCorner, double t);

    // �жϵ�ǰ����������������/��
    bool belongs(const QPoint &point, int iPoint1, int iPoint2);
	bool belongs(const QPoint &point, QPointF a, QPointF b); 
	bool belongs(const QPoint &point, LSTEDGEDES const& vEdge, int& nSelPt, int& nSelEdge);

	// ��ʼ�����ӵ�
	int CalLinkPt(); 
	int CalMultiPartLinkPt();
	// �����ݴ�� ��ǿ��Ƶ���������
	int CalCurvePt(LISTPT const& lstCtrlPt, LISTPT& lstCurvePt,LISTPT& lst3DDraw, int nDrawStep);
 	// ���������ƽ�Ƶõ��������� ������Ӵ�С����
	int CalAllCurvePt(LISTPT const&  lstCurvePt,VECLSTPT& vlstCurvePt);
	// ���������ƽ�Ƶõ��������� ������Ӵ�С����
	int CalAllPartPt(LISTPT const&  lstLinkFPt, LISTPT const&  lstLinkBPt, VECLSTPT const& vlstCurvePt, VECLSTPT& vlstPartPt);
	// ����ͼֽ���������ͼֽ�е�����
	int CalDesignShape();

	// ����Բ���� ���㶨Բ��
	int CalArcPts(LISTPT const& lstOriPts, LISTPT& lstArcPts, LISTPT& lst3DDraw, int nDrawStep);
	// ����������ǵ�
	int CalDesignEdgePts(int nDrawStep);

	int MergeLoopShape(LISTPT const& lpt1, LISTPT const& lpt2, LISTPT const& lFrontPt, LISTPT const & lBackPt, int nIdx, LISTPT& lShapePt);
	void TransPtToPixel(QList<QPointF> lstPt, VPIXELPOS& vPt); 
   // �������
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
