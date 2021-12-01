#pragma once

#include <ParaType.h>

class DimDataConvert
{
public:
	// 根据中心点和长宽生成矩形形状信息
	int GenRectShape(int nCen[2], int nWH[2], SimpleShape& oRectShape);

	// 求解平面图中 柱构件绝对坐标集
	int CalPlaneColShape(TopoUnit oUnit, SimpleShape& oShape);

	// 基于柱子基础上计算墙
	int CalWallShape(int nUnitIdx, VTOPOTABLE const& vLayerTopo, VSHAPE& vPlaneDraw);

	// 基于墙基础上计算门窗
	int CalDoorWndShape(int nUnitIdx, VTOPOTABLE const& vLayerTopo, VSHAPE& vPlaneDraw);

	// 拓扑结构数据转为现实数据 只考虑矩形截面
	int CalPlaneData(VTOPOTABLE const& vLayerTopo, VSHAPE& vPlaneDraw);
};