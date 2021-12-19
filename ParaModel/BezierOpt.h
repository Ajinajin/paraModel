#ifndef CURVE_OPT_H
#define CURVE_OPT_H

// Bernstein递推公式
double bernstein(int i, int n, double t);

// Factorial控制函数
double factorial(double n);

#define MINIMUM_SIZE 640, 480									// 区域尺寸

#define FIRST_POINT QPoint(width() / 8, height() / 8)           // 起始点
#define LAST_POINT QPoint(width() * 7 / 8, height() * 7 / 8)    // 终止点

#define NPOINTS_BEZIER  1000								// 整条Bezier曲线的点数
#define NPOINTS_WEIGHTS 2000								// 控制曲线点数
#define WEIGHTS_HEIGHT  200									// 控制曲线范围

#define HELPTEXT_PEN QPen(Qt::black, 1.0)                   // 文本颜色
#define DRAWPOINTS_PEN QPen(Qt::black, 5.0)                 // 控制点画笔
#define DRAWCONTROL_PEN QPen(Qt::gray, 1.0, Qt::DashLine)   // 控制线画笔
#define DRAWWEIGHTS_PEN QPen(Qt::green, 1.0)                // 权重线画笔
#define DRAWSELWEIGHT_PEN QPen(Qt::red, 2.0)                // 当前权重线画笔
#define DRAWBEZIER_COLOR Qt::blue                           // 曲线画笔
#define DRAWBEZIER_TICK 2.0                                 // Grosor

#define MAX_POINTS 80										// 最大控制点数
#define POINT_TOLLERANCE 5									// 选择点的误差范围 像素单位
#define PARAM_TOLLERANCE 0.2								// 参数误差

#endif // CURVE_OPT_H
