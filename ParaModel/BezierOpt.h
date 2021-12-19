#ifndef CURVE_OPT_H
#define CURVE_OPT_H

// Bernstein���ƹ�ʽ
double bernstein(int i, int n, double t);

// Factorial���ƺ���
double factorial(double n);

#define MINIMUM_SIZE 640, 480									// ����ߴ�

#define FIRST_POINT QPoint(width() / 8, height() / 8)           // ��ʼ��
#define LAST_POINT QPoint(width() * 7 / 8, height() * 7 / 8)    // ��ֹ��

#define NPOINTS_BEZIER  1000								// ����Bezier���ߵĵ���
#define NPOINTS_WEIGHTS 2000								// �������ߵ���
#define WEIGHTS_HEIGHT  200									// �������߷�Χ

#define HELPTEXT_PEN QPen(Qt::black, 1.0)                   // �ı���ɫ
#define DRAWPOINTS_PEN QPen(Qt::black, 5.0)                 // ���Ƶ㻭��
#define DRAWCONTROL_PEN QPen(Qt::gray, 1.0, Qt::DashLine)   // �����߻���
#define DRAWWEIGHTS_PEN QPen(Qt::green, 1.0)                // Ȩ���߻���
#define DRAWSELWEIGHT_PEN QPen(Qt::red, 2.0)                // ��ǰȨ���߻���
#define DRAWBEZIER_COLOR Qt::blue                           // ���߻���
#define DRAWBEZIER_TICK 2.0                                 // Grosor

#define MAX_POINTS 80										// �����Ƶ���
#define POINT_TOLLERANCE 5									// ѡ������Χ ���ص�λ
#define PARAM_TOLLERANCE 0.2								// �������

#endif // CURVE_OPT_H
