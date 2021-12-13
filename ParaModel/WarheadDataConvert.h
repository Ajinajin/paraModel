#pragma once

#include <ParaType.h>
#include <QSize>

class WarheadDataConvert
{
public:
	bool bFirst; 
	
	// 拓扑结构数据转为现实数据 只考虑矩形截面
	int CaArmHeadData(ArmHeadTopo& vArmHeadTopo, VSHAPE& vOriginShape);
	int CenterArmHeadData(VSHAPE const& vOriginShape, VSHAPE& vDrawShape, QSize oScreenSzie);
	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);

};