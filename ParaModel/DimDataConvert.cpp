
#include <ParaType.h>
#include <DimDataConvert.h>
#include <math.h>


// 根据中心点和长宽生成矩形形状信息
int DimDataConvert::GenRectShape(int nCen[2], int nWH[2], SimpleShape& oRectShape)
{
	oRectShape.vCorner.clear();

	oRectShape.nWH[0] = nWH[0];
	oRectShape.nWH[1] = nWH[1];

	oRectShape.nCen[0] = nCen[0];
	oRectShape.nCen[1] = nCen[1];

	PixelPos oCor;
	oCor.nXY[0] = nCen[0] - nWH[0] / 2;
	oCor.nXY[1] = nCen[1] - nWH[1] / 2;
	oRectShape.vCorner.push_back(oCor);
	oCor.nXY[0] = nCen[0] + nWH[0] / 2;
	oCor.nXY[1] = nCen[1] - nWH[1] / 2;
	oRectShape.vCorner.push_back(oCor);
	oCor.nXY[0] = nCen[0] + nWH[0] / 2;
	oCor.nXY[1] = nCen[1] + nWH[1] / 2;
	oRectShape.vCorner.push_back(oCor);
	oCor.nXY[0] = nCen[0] - nWH[0] / 2;
	oCor.nXY[1] = nCen[1] + nWH[1] / 2;
	oRectShape.vCorner.push_back(oCor);

	return 0;
}
// 求解平面图中 柱构件绝对坐标集
int DimDataConvert::CalPlaneColShape(TopoUnit oUnit, SimpleShape& oShape, VUNITTABLE table)
{
	// 在系统构件库中查找柱子截面形状及尺寸 目前只处理方形柱子
	int nWH[2];
	nWH[0] = table[oUnit.nCenUnitIdx].oShape.nShapeRange[2];
	nWH[1] = table[oUnit.nCenUnitIdx].oShape.nShapeRange[3];
	int nCen[2];
	nCen[0] = oUnit.nCenPos[0];
	nCen[1] = oUnit.nCenPos[1];
	GenRectShape(nCen, nWH, oShape);

	return 0;
}

// 基于柱子基础上计算墙
int DimDataConvert::CalWallShape(int nUnitIdx, VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// 得到墙的关联柱
	int nAdjNum = 10;
	int i = 0;
	int nCurAdj;
	int nAdjColIdx[2];
	int nColIdx = 0;
	TopoUnit oUnit = vLayerTopo[nUnitIdx];
	for (i = 0; i < nAdjNum; i++)
	{
		nCurAdj = oUnit.nAdjUnitIdx[i];
		if (nCurAdj >= 0 && vLayerTopo[nCurAdj].nUnitType == 1)
		{
			nAdjColIdx[nColIdx] = nCurAdj;
			nColIdx += 1;
			if (nColIdx == 2)
				break;
		}
	}
	if (nColIdx < 2)
		return 1;

	// 得到两个柱子的范围
	SimpleShape oColShape0 = vPlaneDraw[nAdjColIdx[0]];
	SimpleShape oColShape1 = vPlaneDraw[nAdjColIdx[1]];
	int nXDis = oColShape0.nCen[0] - oColShape1.nCen[0];
	int nYDis = oColShape0.nCen[1] - oColShape1.nCen[1];
	nXDis *= nXDis; 
	nYDis *= nYDis; 
	if (nXDis > nYDis)
		oUnit.nUnitAngle = 0;
	else
		oUnit.nUnitAngle = 90; 

	vLayerTopo[nUnitIdx].nUnitAngle = oUnit.nUnitAngle; 
	// 水平墙
	SimpleShape oWallShape;
	int nWH[2], nCen[2];
	if (oUnit.nUnitAngle == 0)
	{
		nWH[0] = oColShape0.nCen[0] - oColShape1.nCen[0];
		nWH[1] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // 厚度 查表得到

		if (oColShape0.nCen[0] > oColShape1.nCen[0])
		{
			nWH[0] -= (oColShape0.nWH[0] + oColShape1.nWH[0]) / 2;
			nCen[1] = oColShape0.nCen[1];
			nCen[0] = oColShape0.nCen[0] - oColShape0.nWH[0] / 2 - nWH[0] / 2;
		}
		else
		{
			nWH[0] += (oColShape0.nWH[0] + oColShape1.nWH[0]) / 4;
			nCen[1] = oColShape0.nCen[1];
			nCen[0] = oColShape1.nCen[0] - oColShape1.nWH[0] / 2 - abs(nWH[0] / 2);
		}
	}
	// 垂直墙
	else if (oUnit.nUnitAngle == 90)
	{
		nWH[0] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // 厚度 查表得到
		nWH[1] = oColShape0.nCen[1] - oColShape1.nCen[1];
		nWH[1] -= (oColShape0.nWH[1] + oColShape1.nWH[1]) / 2;
		nWH[1] = abs(nWH[1]) - (oColShape0.nWH[1] + oColShape1.nWH[1]) ;
		nCen[0] = oColShape0.nCen[0];
		nCen[1] = abs(oColShape0.nCen[1] - oColShape1.nCen[1]) / 2;
	}
	else
	{
		nWH[0] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // 厚度 查表得到
		nWH[1] = oColShape0.nCen[0] - oColShape1.nCen[0];
		nWH[1] -= (oColShape0.nWH[0] + oColShape1.nWH[0]) / 2;
		nCen[0] = oColShape0.nCen[0];
		if (oColShape0.nCen[1] > oColShape1.nCen[1])
		{
			nCen[1] = oColShape0.nCen[1] - oColShape0.nWH[1] / 2 - nWH[1] / 2;
		}
		else
		{
			nCen[1] = oColShape1.nCen[1] - oColShape1.nWH[1] / 2 - nWH[1] / 2;
		}
	}

	GenRectShape(nCen, nWH, oWallShape);

	oWallShape.unitType = 4;
	oWallShape.unitIdx = nUnitIdx;
	// 保存墙形状信息
	vPlaneDraw[nUnitIdx] = oWallShape;

	return 0;
}

// 基于墙基础上计算门窗
int DimDataConvert::CalDoorWndShape(int nUnitIdx, VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// 得到门窗的关联墙
	int nWallIdx = vLayerTopo[nUnitIdx].nAdjUnitIdx[0];
	if (nWallIdx < 0)
		return 1;

	vLayerTopo[nUnitIdx].nUnitAngle = vLayerTopo[nWallIdx].nUnitAngle; 
	// 水平墙 
	int nCen[2];
	int nWH[2];
	if (vLayerTopo[nWallIdx].nUnitAngle == 0)
	{
		nCen[0] = vLayerTopo[nUnitIdx].nCenPos[0];
		nCen[1] = vPlaneDraw[nWallIdx].nCen[1];
		nWH[0] = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; // 窗户宽度 查系统表得到
		nWH[1] = vPlaneDraw[nWallIdx].nWH[1] + 10; // 窗户厚度 之前没给定 设置为墙厚+10
	}
	// 垂直墙
	else
	{
		nCen[0] = vPlaneDraw[nWallIdx].nCen[0];
		nCen[1] = vLayerTopo[nUnitIdx].nCenPos[2] + table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[1];
		nWH[0] = vPlaneDraw[nWallIdx].nWH[0] + 10; // 窗户厚度 之前没给定 设置为墙厚+10
		nWH[1] = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; // 窗户宽度 查系统表得到
	}
	SimpleShape oDoorWndShape;
	GenRectShape(nCen, nWH, oDoorWndShape);

	oDoorWndShape.unitType = table[vLayerTopo[nUnitIdx].nCenUnitIdx].nUnitType;
	oDoorWndShape.unitIdx = nUnitIdx;
	// 保存门窗形状信息
	vPlaneDraw[nUnitIdx] = oDoorWndShape;

	return 0;
}

// 拓扑结构数据转为现实数据 只考虑矩形截面
int DimDataConvert::CalPlaneData(VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	int nRe = 0;
	// 只处理柱 墙 门窗
	int nUnitNum = vLayerTopo.size();
	int i = 0;
	SimpleShape oShape;
	// 先处理柱
	TopoUnit oCurUnit;
	vPlaneDraw.clear();
	vPlaneDraw = VSHAPE(nUnitNum);
	for (i = 0; i < nUnitNum; i++)
	{
		oCurUnit = vLayerTopo[i];
		if (oCurUnit.nUnitType == 1)
		{
			oShape.vCorner.clear();
			CalPlaneColShape(oCurUnit, oShape, table);
			oShape.unitType = 1;
			oShape.unitIdx = oCurUnit.nCenUnitIdx;
			vPlaneDraw[i] = oShape;
		}
	}
	// 基于柱子基础上计算墙
	for (i = 0; i < nUnitNum; i++)
	{
		oCurUnit = vLayerTopo[i];
		if (oCurUnit.nUnitType == 4)
		{
			CalWallShape(i, vLayerTopo, vPlaneDraw, table);
		}
	}

	// 基于墙基础上计算门窗
	for (i = 0; i < nUnitNum; i++)
	{
		oCurUnit = vLayerTopo[i];
		if (oCurUnit.nUnitType > 4)
		{
			CalDoorWndShape(i, vLayerTopo, vPlaneDraw, table);
		}
	}
	return 0;
}
