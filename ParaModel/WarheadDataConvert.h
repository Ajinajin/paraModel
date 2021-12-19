#pragma once

#include <ParaType.h>
#include <QSize>

class WarheadDataConvert
{
public:
	bool bFirst; 
	float fR[20];
	float fT[10];
	float fH[10];
	QList<QPointF> vCtrlPts;		// ��ǿ��Ƶ㼯

	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);
	// ����ͼֽ��ս����������
	int GetArmHeadPara(ArmHeadTopo& vArmHeadTopo);
	// ����ս�������������ս���������� ����ͼֽԭ��
	int CaArmHeadData(VSHAPE& vOriginShape);
	// ������л��Ƶ�ս����������Ϣ
	int CenterArmHeadData(VSHAPE const& vOriginShape, VSHAPE& vDrawShape, QSize oScreenSzie);

};