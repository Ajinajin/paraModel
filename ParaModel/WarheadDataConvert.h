#pragma once

#include <ParaType.h>

class WarheadDataConvert
{
public:
	// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
	int CalPlaneData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vPlaneDraw);
	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);

};