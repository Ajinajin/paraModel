#pragma once

#include <ParaType.h>

class WarheadDataConvert
{
public:
	// 拓扑结构数据转为现实数据 只考虑矩形截面
	int CalPlaneData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vPlaneDraw);
	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);

};