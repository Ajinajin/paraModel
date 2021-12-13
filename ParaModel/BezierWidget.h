
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
    QList<QPoint> lstUpCtrlPts;		// �����������Ƶ��б�
    QStack<int> newPoints;			// Pila de puntos nuevos
    QColor curveColor;				// ��������ɫ
    int iSelected;					// ��ǰѡ����������Ƶ�����
    bool showHelp;					// ��ʾ����
    bool showControlLines;			// ��ʾ������
	bool bCalCen; 
	int nScreenWH[2]; 
	QSize oPreSize; 
	VSHAPE vDrawShape;				// ����ս�������λ�������ߴ�
	VSHAPE vOriginShape;			// ս��������ԭʼ����ߴ�
    // �������tֵ�����ߵ�����
    QPoint bezier(double t);
	QPoint CalBezier(VPIXELPOS vCorner, double t); 

    // �жϵ�ǰ����������������/��
    bool belongs(const QPoint &point, int iPoint1, int iPoint2);

    // �������
    void exportCurve();
	void TransPos(VSHAPE & vShape, int nXY[2]); 
	void TransPos(VPIXELPOS & vPt, int nXY[2]);
	void ReflectPos(VPIXELPOS & vPt, int nXY[2]);
	QColor ColorHelper(int unitIdx);
	void DrawArmHead(QPainter * pPaint, VSHAPE const& vShape); 
};

#endif // WIDGET_H
