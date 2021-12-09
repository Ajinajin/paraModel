
#include "ParaModel.h"
#include <QtWidgets/QApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <qtreewidget.h>
#include <windows.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	ParaModel w;
	w.show();
	//计时开始
	LARGE_INTEGER litmp;
	LONGLONG Qpart1, Qpart2, Useingtime;
	double dfMinus, dfFreq, dfTime;
	//获得CPU计时器的时钟频率
	QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	Qpart1 = litmp.QuadPart; //开始计时
	//计时结束
	QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	Qpart2 = litmp.QuadPart; //终止计时
	dfMinus = (double)(Qpart2 - Qpart1);//计算计数器值
	dfTime = dfMinus / dfFreq;//获得对应时间，云锡摇床。单位为秒,可以乘精确到微秒级（us）
	Useingtime = dfTime * 1000000;

	qDebug() << Useingtime << endl;

    
    return a.exec();
}
