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
	QList<QPointF> vCtrlPts;		// 外壳控制点集

	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);
	// 基于图纸的战斗部各参数
	int GetArmHeadPara(ArmHeadTopo& vArmHeadTopo);
	// 基于战斗部参数计算的战斗部各坐标 基于图纸原点
	int CaArmHeadData(VSHAPE& vOriginShape);
	// 计算居中绘制的战斗部坐标信息
	int CenterArmHeadData(VSHAPE const& vOriginShape, VSHAPE& vDrawShape, QSize oScreenSzie);

};