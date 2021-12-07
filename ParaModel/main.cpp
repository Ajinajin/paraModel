
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
	//��ʱ��ʼ
	LARGE_INTEGER litmp;
	LONGLONG Qpart1, Qpart2, Useingtime;
	double dfMinus, dfFreq, dfTime;
	//���CPU��ʱ����ʱ��Ƶ��
	QueryPerformanceFrequency(&litmp);//ȡ�ø߾������м�������Ƶ��f,��λ��ÿ����ٴΣ�n/s����
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
	Qpart1 = litmp.QuadPart; //��ʼ��ʱ
	//��ʱ����
	QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
	Qpart2 = litmp.QuadPart; //��ֹ��ʱ
	dfMinus = (double)(Qpart2 - Qpart1);//���������ֵ
	dfTime = dfMinus / dfFreq;//��ö�Ӧʱ�䣬����ҡ������λΪ��,���Գ˾�ȷ��΢�뼶��us��
	Useingtime = dfTime * 1000000;

	qDebug() << Useingtime << endl;

    
    return a.exec();
}
