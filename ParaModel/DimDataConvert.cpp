
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
int DimDataConvert::CalWallShape(int nUnitIdx, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
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
		nCen[0] = oColShape0.nCen[0];
		if (oColShape0.nCen[1] > oColShape1.nCen[1])
		{
			nWH[1] = abs(nWH[1]);
			nCen[1] = oColShape1.nCen[1] + abs(oColShape0.nCen[1] - oColShape1.nCen[1]) / 2;
		}
		else
		{
			nWH[1] = abs(nWH[1]) - (oColShape0.nWH[1] + oColShape1.nWH[1]);
			nCen[1] = oColShape0.nCen[1] + abs(oColShape0.nCen[1] - oColShape1.nCen[1]) / 2;
		}
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
int DimDataConvert::CalDoorWndShape(int nUnitIdx, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
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
		nWH[0] = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; // 窗户宽度 查系统表得到
		nWH[1] = vPlaneDraw[nWallIdx].nWH[1] + 10; // 窗户厚度 之前没给定 设置为墙厚+10
		//墙的中心点
		//前一个柱子的中心点+宽度/2 得到墙的左下角 + 门的相对位置 + 门元素的中心点
		int nPillarCen = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[0]].nCen[0];//前一个柱子的中心点
		int nPillarWH = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[0]].nWH[0] / 2;///前一个柱子宽度/2
		int doorW = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2; //门元素的中心点
		int doorPosition = vLayerTopo[nUnitIdx].nCenPos[0]; //基准点的放置位置
		nCen[0] = nPillarCen + nPillarWH + doorPosition + doorW;
		nCen[1] = vPlaneDraw[nWallIdx].nCen[1];
	}
	// 垂直墙
	else
	{
		nWH[0] = vPlaneDraw[nWallIdx].nWH[0] + 10; // 窗户厚度 之前没给定 设置为墙厚+10
		nWH[1] = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; // 窗户宽度 查系统表得到

		//墙的中心点位置
		//第二个柱子的中心点Y-高度/2 得到墙的上位置 - 门的相对位置 - 门元素的中心点
		nCen[0] = vPlaneDraw[nWallIdx].nCen[0];

		int nPillarCen = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[1]].nCen[1];//第二一个柱子的中心点Y
		int nPillarWH = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[1]].nWH[1] / 2;///第二个一个柱子高度/2
		int doorPosition = vLayerTopo[nUnitIdx].nCenPos[0];//门的相对位置
		int doorW = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2;//门元素的中心点

		nCen[1] = nPillarCen - nPillarWH - doorPosition - doorW;
	}
	SimpleShape oDoorWndShape;
	GenRectShape(nCen, nWH, oDoorWndShape);

	oDoorWndShape.unitType = vLayerTopo[nUnitIdx].nUnitType;
	oDoorWndShape.unitIdx = nUnitIdx;
	// 保存门窗形状信息
	vPlaneDraw[nUnitIdx] = oDoorWndShape;

	return 0;
}

// 拓扑结构数据转为现实数据 只考虑矩形截面
int DimDataConvert::CalPlaneData(VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
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
			oShape.unitIdx = oCurUnit.nTopoIdx;
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

// 平面图处理
// 基本构件的 增删改查 
// 平面图中可选择 墙门窗 立面图中可选择柱门窗

// 在墙上插入门窗
int AddDoorWndInWall(BasicUnit oDoorWnd, int nInsWallIdx, PixelPos oInsPos, VUNITTABLE& vTable, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	vTable.push_back(oDoorWnd);

	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oDoorWnd.nUnitType;
	oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	// 根据插入信息完成插入
	if (oInsUnit.nUnitAngle == 0)
	{
		oInsUnit.nCenPos[2] = vPlaneDraw[nInsWallIdx].nCen[1];
		int nWallLeft = vPlaneDraw[nInsWallIdx].nCen[0] - vPlaneDraw[nInsWallIdx].nWH[0] / 2;
		oInsUnit.nCenPos[0] = oInsPos.nXY[0] - nWallLeft;
	}
	else if (oInsUnit.nUnitAngle == 90)
	{
		oInsUnit.nCenPos[0] = vPlaneDraw[nInsWallIdx].nCen[1];
		int nWallBottom = vPlaneDraw[nInsWallIdx].nCen[1] - vPlaneDraw[nInsWallIdx].nWH[1] / 2;
		oInsUnit.nCenPos[2] = oInsPos.nXY[0] - nWallBottom;
	}
	// 更新拓扑结构
	oInsUnit.nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo.push_back(oInsUnit);
	return 0;
}

// 更新添加新柱后梁 墙 和 柱的邻接关系
int UpdateNewAdj(int nNewAdj, int nAllOldAdj[12], int nAllNewAdj[12], VTOPOTABLE const& vLayerTopo, int nNewType)
{
	int i = 0;
	bool bfirst = true;
	for (i = 0; i < 12; i++)
	{
		if (bfirst && vLayerTopo[nAllOldAdj[i]].nUnitType == nNewType)
		{
			nAllOldAdj[i] = nNewAdj;
			bfirst = false;
		}
		else if (!bfirst && vLayerTopo[nAllNewAdj[i]].nUnitType == nNewType)
		{
			nAllNewAdj[i] = nNewAdj;
			break;
		}
	}

	return 0;
}
// 更新添加新柱后梁 墙之间的邻接关系
int UpdateNewAdj(int nNewAdj, int nAllOldAdj[12], VTOPOTABLE const& vLayerTopo, int nNewType)
{
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		if (vLayerTopo[nAllOldAdj[i]].nUnitType == nNewType)
		{
			nAllOldAdj[i] = nNewAdj;
			break;
		}
	}

	return 0;
}
// 在墙上插入柱子 待完善
int AddColInWall(BasicUnit oCol, int nInsWallIdx, PixelPos oInsPos, VUNITTABLE& vTable, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	vTable.push_back(oCol);

	// 待增加柱子的拓扑单元
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oCol.nUnitType;
	oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	// 根据插入信息完成插入
	if (oInsUnit.nUnitAngle == 0)
	{
		oInsUnit.nCenPos[2] = vPlaneDraw[nInsWallIdx].nCen[1];
		int nWallLeft = vPlaneDraw[nInsWallIdx].nCen[0] - vPlaneDraw[nInsWallIdx].nWH[0] / 2;
		oInsUnit.nCenPos[0] = oInsPos.nXY[0] - nWallLeft;
	}
	else if (oInsUnit.nUnitAngle == 90)
	{
		oInsUnit.nCenPos[0] = vPlaneDraw[nInsWallIdx].nCen[1];
		int nWallBottom = vPlaneDraw[nInsWallIdx].nCen[1] - vPlaneDraw[nInsWallIdx].nWH[1] / 2;
		oInsUnit.nCenPos[2] = oInsPos.nXY[0] - nWallBottom;
	}
	// 添加柱 更新拓扑结构
	oInsUnit.nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo.push_back(oInsUnit);
	int nNewColIdx = vLayerTopo.size() - 1;
	// 墙分裂为两个墙
	// 先复制一份
	TopoUnit oAddWall = vLayerTopo[nInsWallIdx];
	vLayerTopo.push_back(oAddWall);
	int nNewWallIdx = vLayerTopo.size() - 1;
	// 梁分裂为两个梁
	int i = 0;
	int nBeamIdx = -1;
	int nAdjIdx = 0;
	for (i = 0; i < 12; i++)
	{
		nAdjIdx = vLayerTopo[nInsWallIdx].nAdjUnitIdx[i];
		if (vLayerTopo[nAdjIdx].nUnitType == 1)
		{
			nBeamIdx = nAdjIdx;
			break;
		}
	}
	TopoUnit oBeamUnit;
	int nNewBeamIdx = -1;
	if (nBeamIdx != -1)
	{
		oBeamUnit = vLayerTopo[nBeamIdx];
		vLayerTopo.push_back(oBeamUnit);
		nNewBeamIdx = vLayerTopo.size() - 1;
	}
	// 更新柱拓扑结构
	vLayerTopo[nNewColIdx].nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[1] = nNewWallIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[2] = nBeamIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[3] = nNewBeamIdx;
	// 更新墙拓扑结构
	UpdateNewAdj(nNewColIdx, vLayerTopo[nInsWallIdx].nAdjUnitIdx, vLayerTopo[nNewWallIdx].nAdjUnitIdx, vLayerTopo, 0);
	UpdateNewAdj(nNewBeamIdx, vLayerTopo[nNewWallIdx].nAdjUnitIdx, vLayerTopo, 1);

	// 更新梁拓扑结构
	UpdateNewAdj(nNewColIdx, vLayerTopo[nBeamIdx].nAdjUnitIdx, vLayerTopo[nNewBeamIdx].nAdjUnitIdx, vLayerTopo, 0);
	UpdateNewAdj(nNewWallIdx, vLayerTopo[nNewBeamIdx].nAdjUnitIdx, vLayerTopo, 1);

	return 0;
}

// 在两个柱子之间插入墙 两个柱子之间必须为空 
int AddWallInCols(BasicUnit oAddUnit, int nAdjCol[2], VTOPOTABLE& vLayerTopo)
{
	// 根据插入信息完成插入

	// 更新拓扑结构

	return 0;
}

// 在当前拓扑图中增加构件
int DimDataConvert::AddBaseUnit(BasicUnit oAddUnit, PixelPos oInsPos, VUNITTABLE& vTable, VTOPOTABLE& vLayerTopo, VSHAPE vPlaneDraw)
{
	int nRe = 0;
	// 判断插入点是否在墙线范围内 返回墙在拓扑图的Idx 或者 虚墙区域关联的两个柱子
	int nInsWallIdx = 0;
	int nAdjCol[2];		// 两个中有一个为空则置 [0]为-1
	// 计算 待完善

	if (nInsWallIdx < 0 && nAdjCol[0] < 0)
		return 1;
	// 判断待插入构件是否是门窗
	if (oAddUnit.nUnitType > 4)
		nRe = AddDoorWndInWall(oAddUnit, nInsWallIdx, oInsPos, vTable, vLayerTopo, vPlaneDraw);

	// 插入柱子
	else if (oAddUnit.nUnitType == 1)
		nRe = AddColInWall(oAddUnit, nInsWallIdx, oInsPos, vTable, vLayerTopo, vPlaneDraw);

	// 插入墙
	else if (oAddUnit.nUnitType == 4 && nAdjCol[0] >= 0)
		nRe = AddWallInCols(oAddUnit, nAdjCol, vLayerTopo);

	// 基于新拓扑结构重新计算几何数值

	return 0;
}

// 清理数据 真正删除
int CleanTopoTable(VTOPOTABLE& vLayerTopp)
{
	return 0;
}

// 根据点击位置确定选择的构件 待完善
int GetSelUnitIdx(PixelPos oInsPos, int& nUnitIdx, VTOPOTABLE& vLayerTopp)
{
	return 0;
}
// 在当前拓扑图中删除构件
int DimDataConvert::DelBaseUnit(PixelPos oInsPos, VTOPOTABLE& vLayerTopp, int nRealDel)
{
	int nRe = 0;
	// 根据点击位置确定选择的构件
	int nSelUnitIdx;
	nRe = GetSelUnitIdx(oInsPos, nSelUnitIdx, vLayerTopp);
	if (nRe != 0 || nSelUnitIdx < 0)
		return nRe;
	// 从关联的构件中删除关联关系 假删除 数据还在 但不可用

	// 删除标志置为1 
	vLayerTopp[nSelUnitIdx].nStatusFlag = 1;

	// 清理数据 真正删除
	nRe = CleanTopoTable(vLayerTopp);

	return 0;
}

// 在当前拓扑图中修改构件属性
// 更改同类构件的不同子类型 材质 纹理 
int ModifyBaseUnitProp(void* pValue, VTOPOTABLE& vLayerTopp)
{
	return 0;
}

// 移动门窗
int MovDoorWndInWall(int nMoveUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// 门窗只能在墙范围内沿墙方向移动
	// 得到包含门窗的墙
	int nInsWallIdx = vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0];

	// 符合移动条件 移动方向一致且值不为0
	if (nMoveXY[0] != 0 && vLayerTopo[nInsWallIdx].nUnitAngle == 0)
	{
		// 计算门窗移动的合规范围 nCenPos[0]是相對位置
		//不能小于0;
		//不能大于墻的寬度-元素的中心點;
		//如果超过等于极限值
		 
		int nLeft= 0;
		 

		int doorW = table[vLayerTopo[nMoveUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2; //元素的中心点
		int nWallWH = vPlaneDraw[vLayerTopo[nInsWallIdx].nTopoIdx].nWH[0] ;					//墻的寬度
		int nRight = vPlaneDraw[vLayerTopo[nInsWallIdx].nTopoIdx].nWH[0] - doorW;
		
		
		// 按墙方向更改门窗位置
		int nNewX = vLayerTopo[nMoveUnitIdx].nCenPos[0] + nMoveXY[0];
		
		if (nNewX < nLeft)
			nNewX = nLeft;

		else if (nNewX > nRight)
			nNewX = nRight;
		vLayerTopo[nMoveUnitIdx].nCenPos[0] = nNewX;
	}
	else if (nMoveXY[1] != 0 && vLayerTopo[nInsWallIdx].nUnitAngle == 90)
	{
		// 计算门窗移动的合规范围
		int nBottom;
		nBottom = vPlaneDraw[nMoveUnitIdx].nWH[1] / 2;
		int nUp = vPlaneDraw[nInsWallIdx].nWH[1] - nBottom;
		// 按墙方向更改门窗位置
		int nNewY = vLayerTopo[nMoveUnitIdx].nCenPos[1] + nMoveXY[0];
		if (nNewY < nBottom)
			nNewY = nBottom;
		else if (nNewY > nUp)
			nNewY = nUp;
		vLayerTopo[nMoveUnitIdx].nCenPos[0] = nNewY;
	}

	return 0;
}

// 得到墙关联的柱子 以及 柱子关联的其他墙
// 正常为两个vector vector中第一个为柱子Idx 后面为该柱子的关联墙
int GetColAdjWall(int nSelWallIdx, VTOPOTABLE const& vLayerTopo, VVINT& vvAdjWall)
{
	// 得到墙两端的柱子索引
	int nAdjNum = 10;
	int i = 0;
	TopoUnit oWallUnit = vLayerTopo[nSelWallIdx];
	int nCurAdj;
	int nAdjColIdx[2];
	int nColIdx = 0;
	for (i = 0; i < nAdjNum; i++)
	{
		nCurAdj = oWallUnit.nAdjUnitIdx[i];
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

	// 得到柱子关联的墙
	int nColNum = 2;
	VINT vColAndWall;
	vvAdjWall.clear();
	for (i = 0; i < nColNum; i++)
	{
		vColAndWall.clear();
		vColAndWall.push_back(nAdjColIdx[i]);
		TopoUnit oColUnit = vLayerTopo[nAdjColIdx[i]];
		// 每个柱子最多12个邻接单元
		for (int j = 0; j < 12; j++)
		{
			nCurAdj = oColUnit.nAdjUnitIdx[j];
			if (nCurAdj > 0)
			{
				if (nCurAdj != nSelWallIdx && vLayerTopo[nCurAdj].nUnitType == 4)
					vColAndWall.push_back(nCurAdj);
			}
		}
		vvAdjWall.push_back(vColAndWall);
	}

	return 0;
}
// 移动柱子 由PreMoveWall带动的移动
int MovCol(int nMoveUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo)
{
	// 只移动柱子
	vLayerTopo[nMoveUnitIdx].nCenPos[0] += nMoveXY[0];
	vLayerTopo[nMoveUnitIdx].nCenPos[2] += nMoveXY[1];

	return 0;
}

// 计算多边形的外包框架范围
int CalPolyFrm(VPIXELPOS const& vCorner, int nRangeXY[2][2])
{
	int nCorNum = vCorner.size();
	int i = 0;
	VINT vx, vy;
	for (i = 0; i < nCorNum; i++)
	{
		vx.push_back(vCorner[i].nXY[0]);
		vy.push_back(vCorner[i].nXY[1]);
	}
	sort(vx.begin(), vx.end());
	sort(vy.begin(), vy.end());
	nRangeXY[0][0] = vx[0];
	nRangeXY[0][1] = vx[nCorNum - 1];
	nRangeXY[1][0] = vy[0];
	nRangeXY[1][1] = vy[nCorNum - 1];

	return 0;
}
// 计算墙的合规范围
int CalWallMoveRange(int nWallIdx, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, int nAdjCol[2], int nXYMove[2][2])
{
	int nRe = 0;
	// 墙只能沿垂直墙方向移动 可移动距离初始全为0
	memset(nXYMove, 0, 4 * sizeof(int));

	// 墙两端必须有柱子
	// 得到柱子关联的其他墙
	VVINT vvAdjColWall;
	nRe = GetColAdjWall(nWallIdx, vLayerTopo, vvAdjColWall);
	nAdjCol[0] = vvAdjColWall[0][0];
	nAdjCol[1] = vvAdjColWall[1][0];
	// 得到待移动墙的范围
	int nWallRange[2][2];
	CalPolyFrm(vPlaneDraw[nWallIdx].vCorner, nWallRange);

	// 得到所有关联墙的范围
	VPIXELPOS vAllCor;
	int nVVNum = vvAdjColWall.size();
	int i = 0;
	int j = 0;
	int nVAdjNum = 0;
	int nAllRange[2][2];
	for (i = 0; i < nVVNum; i++)
	{
		VINT vCurAdjIdx = vvAdjColWall[i];
		nVAdjNum = vCurAdjIdx.size();
		for (j = 0; j < nVAdjNum; j++)
		{
			int nUnitIdx = vCurAdjIdx[j];
			if (vLayerTopo[nUnitIdx].nUnitType == 4)
				vAllCor.insert(vAllCor.end(), vPlaneDraw[nUnitIdx].vCorner.begin(), vPlaneDraw[nUnitIdx].vCorner.end());
		}
	}
	CalPolyFrm(vAllCor, nAllRange);
	// 水平墙 垂直移动范围
	TopoUnit oUnit = vLayerTopo[nWallIdx];
	if (oUnit.nUnitAngle == 0)
	{
		nXYMove[1][0] = nAllRange[1][0] - nWallRange[1][0];
		if (nWallRange[1][0] == nAllRange[1][0])
			nXYMove[1][0] = -900000;

		nXYMove[1][1] = nAllRange[1][1] - nWallRange[1][1];
		if (nWallRange[1][1] == nAllRange[1][1])
			nXYMove[1][1] = 900000;
	}
	// 垂直墙 水平移动范围
	else if (oUnit.nUnitAngle == 90)
	{
		nXYMove[0][0] = nAllRange[0][0] - nWallRange[0][0];
		if (nWallRange[0][0] <= nAllRange[0][0])
			nXYMove[0][0] = -900000;

		nXYMove[0][1] = nAllRange[0][1] - nWallRange[0][1];
		if (nWallRange[0][1] >= nAllRange[0][1])
			nXYMove[0][1] = 900000;
	}

	return 0;
}
// 移动墙
int MovWall(int nMoveUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	int nRe = 0;
	// 墙只能沿垂直墙方向移动
	// 墙两端必须有柱子
	// 得到柱子关联的其他墙
	int nAdjCol[2];
	int nXYMoveRange[2][2];
	// 	// 计算墙的合规范围
	nRe = CalWallMoveRange(nMoveUnitIdx, vLayerTopo, vPlaneDraw, nAdjCol, nXYMoveRange);

	// 按墙垂直方向计算移动距离
	if (nMoveXY[0] <= nXYMoveRange[0][0] || nMoveXY[0] >= nXYMoveRange[0][1])
		nMoveXY[0] = 0;
	if (nMoveXY[1] <= nXYMoveRange[1][0] || nMoveXY[1] >= nXYMoveRange[1][1])
		nMoveXY[1] = 0;
	// 移动两端柱子
	int nColNum = 2;
	int i = 0;
	for (i = 0; i < nColNum; i++)
	{
		MovCol(nAdjCol[i], nMoveXY, vLayerTopo);
	}
	// 重新计算几何数值

	return 0;
}
// 平面图中移动选中单元构件
int DimDataConvert::MoveBaseUnit(int nSelUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	int nRe = 0;
	// 平面图中 墙门窗可动 判断是否是墙门窗
	int nUnitType = vLayerTopo[nSelUnitIdx].nUnitType;
	// 判断待移动构件是否是门窗
	if (nUnitType > 4)
		nRe = MovDoorWndInWall(nSelUnitIdx, nMoveXY, vLayerTopo, vPlaneDraw, table);

	// 移动墙
	else if (nUnitType == 4)
		nRe = MovWall(nSelUnitIdx, nMoveXY, vLayerTopo, vPlaneDraw);

	return 0;
}
