#pragma once

#include <ParaType.h>


class DimDataConvert
{
public:
	// 根据中心点和长宽生成矩形形状信息
	int GenRectShape(int nCen[2], int nWH[2], SimpleShape& oRectShape);

	// 求解平面图中 柱构件绝对坐标集
	int CalPlaneColShape(TopoUnit oUnit, SimpleShape& oShape, VUNITTABLE table);

	// 基于柱子基础上计算墙
	int CalWallShape(int nUnitIdx, VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table);

	// 基于墙基础上计算门窗
	int CalDoorWndShape(int nUnitIdx, VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table);

	// 拓扑结构数据转为现实数据 只考虑矩形截面
	int CalPlaneData(VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw,  VUNITTABLE table);

	// 在当前拓扑图中增加构件
	int AddBaseUnit(BasicUnit oAddUnit, PixelPos oInsPos, VUNITTABLE & vTable, VTOPOTABLE& vLayerTopo, VSHAPE vPlaneDraw); 

	// 在当前拓扑图中删除构件
	int DelBaseUnit(int nSelUnitIdx,VTOPOTABLE& vLayerTopp, int nRealDel = 1);

	// 平面图中移动选中单元构件
	int MoveBaseUnit(int nSelUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw); 

};