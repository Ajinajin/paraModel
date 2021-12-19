
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
    QList<QPointF> lstUpCtrlPts;		// �����������Ƶ��б�
    QStack<int> newPoints;			// Pila de puntos nuevos
    QColor curveColor;				// ��������ɫ
    int iSelected;					// ��ǰѡ����������Ƶ�����
    bool showHelp;					// ��ʾ����
    bool showControlLines;			// ��ʾ������
	bool bCalCen;					// ���м����־
	bool bCalPartShape;				// �������߼����־
	int nScreenWH[2]; 
	int nMoveXY[2];					// ԭʼ���ݺ���ʾ����ƫ��
	float fR[10], fT[10], fH[10]; 
	QSize oPreSize; 
	VSHAPE vDrawShape;				// ����ս�������λ�������ߴ�  ���߼������
	VSHAPE vDesignShape;			// ս��������ԭʼ����ߴ�  ����Ϊ���Ƶ��ʾ
	LISTPT lstUpCurvePts;			// ��������ϵĵ�
	VECLSTPT vlstCurvePt;			// 0-7 ���ݴӴ�С8�������ϵĵ�
	VECLSTPT vlstPartPt;			// 0-4 5�����������㼯
	VECLSTPT vlstDrawPartPt;		// 0-4 5�����������㼯
	LISTPT   lstLinkFrontPt;		// ��ǰ�����ӵ�
	LISTPT   lstLinkBackPt;			// �ͺ�����ӵ�

    // �������tֵ�����ߵ�����
    QPoint bezier(double t);
	QPoint CalBezier(LISTPT const& vCorner, double t);

    // �жϵ�ǰ����������������/��
    bool belongs(const QPoint &point, int iPoint1, int iPoint2);

	// ��ʼ�����ӵ�
	int CalLinkPt(); 
	// �����ݴ�� ��ǿ��Ƶ���������
	int CalCurvePt(LISTPT const& lstCtrlPt, LISTPT& lstCurvePt);
 	// ���������ƽ�Ƶõ��������� ������Ӵ�С����
	int CalAllCurvePt(LISTPT const&  lstCurvePt,VECLSTPT& vlstCurvePt);
	// ���������ƽ�Ƶõ��������� ������Ӵ�С����
	int CalAllPartPt(LISTPT const&  lstLinkFPt, LISTPT const&  lstLinkBPt, VECLSTPT const& vlstCurvePt, VECLSTPT& vlstPartPt);
	// ����ͼֽ���������ͼֽ�е�����
	int CalDesignShape();
   // �������
    void exportCurve();
	void TransPos(VSHAPE & vShape, int nXY[2]); 
	void TransPos(VPIXELPOS & vPt, int nXY[2]);

	void ReflectPos(VPIXELPOS & vPt, int nXY[2]);
	QColor ColorHelper(int unitIdx);
	void DrawSubPart(QPainter * pPaint, SimpleShape const& oCurShape);
	void DrawArmHead(QPainter * pPaint, VSHAPE const& vShape); 
};

#endif // WIDGET_H
