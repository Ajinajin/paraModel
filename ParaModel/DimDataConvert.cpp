
#include <ParaType.h>
#include <DimDataConvert.h>
#include <math.h>
#include <vector>
//通过墙的构建序号来得到与其连接的两个柱子索引

void calColumnsIdxFromWallIdx(int wallIdx, int* columnArrayIdx, VTOPOTABLE oglTopTable)
{
	//查找墙连接的两个柱构件序号

	int index = 0;

	int j = 0;
	while (oglTopTable.at(wallIdx).nAdjUnitIdx[j] != -1)
	{
		if (oglTopTable.at(oglTopTable.at(wallIdx).nAdjUnitIdx[j]).nUnitType == 1)
		{
			columnArrayIdx[index++] = oglTopTable.at(wallIdx).nAdjUnitIdx[j];
		}
		j++;
	}

}

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
	nCen[1] = oUnit.nCenPos[2];
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
	//if (nXDis > nYDis)
	//	oUnit.nUnitAngle = 0;
	//else
	//	oUnit.nUnitAngle = 90;
	if (oColShape0.nCen[1] == oColShape1.nCen[1])
		oUnit.nUnitAngle = 0;
	if (oColShape0.nCen[0] == oColShape1.nCen[0])
		oUnit.nUnitAngle = 90;

	vLayerTopo[nUnitIdx].nUnitAngle = oUnit.nUnitAngle;
	// 水平墙
	SimpleShape oWallShape;
	int nWH[2], nCen[2];
	if (oUnit.nUnitAngle == 0)
	{
		nWH[0] = abs(oColShape0.nCen[0] - oColShape1.nCen[0]);
		nWH[1] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // 厚度 查表得到

		if (oColShape0.nCen[0] > oColShape1.nCen[0])
		{
			nWH[0] -= (oColShape0.nWH[0] + oColShape1.nWH[0]) / 2;
			nCen[1] = oColShape0.nCen[1];
			nCen[0] = oColShape0.nCen[0] - oColShape0.nWH[0] / 2 - nWH[0] / 2;
		}
		else
		{
			nWH[0] = nWH[0] - (oColShape0.nWH[0] + oColShape1.nWH[0]) / 2;
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

		int col0X = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[0]].nCen[0];//前一个柱子的中心点
		int col1X = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[1]].nCen[0];//前一个柱子的中心点
		int nPillarCen;
		if (col0X < col1X) { nPillarCen = col0X; }
		else { nPillarCen = col1X; }


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

		int col0Y = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[0]].nCen[1];
		int col1Y = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[1]].nCen[1];

		int nPillarCen;//靠下柱子的中心点Y
		if (col0Y < col1Y) { nPillarCen = col1Y; }
		else { nPillarCen = col0Y; }

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

// 拓扑结构数据转为现实数据 只考虑矩形截面0.

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
	//vTable.push_back(oDoorWnd);

	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oDoorWnd.nUnitType;
	oInsUnit.nCenUnitIdx = oDoorWnd.nUnitIdx;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	oInsUnit.nTopoIdx = vLayerTopo.size();
	for (int i = 0; i < 12; i++) { oInsUnit.nAdjUnitIdx[i] = -1; }
	for (int i = 0; i < 4; i++) { oInsUnit.nCenPos[i] = 0; }
	// 根据插入信息完成插入
	if (oInsUnit.nUnitAngle == 0)
	{
		//oInsUnit.nCenPos[2] = vPlaneDraw[nInsWallIdx].nCen[1];
		int nWallLeft = vPlaneDraw[nInsWallIdx].nCen[0] - vPlaneDraw[nInsWallIdx].nWH[0] / 2;
		oInsUnit.nCenPos[0] = abs(oInsPos.nXY[0] - nWallLeft);

		//门窗的起始点高度先写死
		if (oInsUnit.nUnitType == 5) { oInsUnit.nCenPos[1] = 0; }
		if (oInsUnit.nUnitType == 6) { oInsUnit.nCenPos[1] = 80; }

	}
	else if (oInsUnit.nUnitAngle == 90)
	{
		//oInsUnit.nCenPos[0] = vPlaneDraw[nInsWallIdx].nCen[0];
		int nWallBottom = vPlaneDraw[nInsWallIdx].nCen[1] + vPlaneDraw[nInsWallIdx].nWH[1] / 2;
		//oInsUnit.nCenPos[2] = oInsPos.nXY[0] - nWallBottom;
		oInsUnit.nCenPos[0] = abs(oInsPos.nXY[1] - nWallBottom);
		//门窗的起始点高度先写死
		if (oInsUnit.nUnitType == 5) { oInsUnit.nCenPos[1] = 0; }
		if (oInsUnit.nUnitType == 6) { oInsUnit.nCenPos[1] = 80; }

	}
	// 更新拓扑结构
	oInsUnit.nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo.push_back(oInsUnit);
	return 0;
}

//在墙上添加梁
int AddBeamOnWall(BasicUnit oBeam, int nInsWallIdx, VUNITTABLE vTable, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	// 待增加梁的拓扑单元
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oBeam.nUnitType;
	//oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nCenUnitIdx = oBeam.nUnitIdx;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	oInsUnit.nTopoIdx = vLayerTopo.size();
	for (int i = 0; i < 12; i++) { oInsUnit.nAdjUnitIdx[i] = -1; }

	for (int i = 0; i < 4; i++) { oInsUnit.nCenPos[i] = 0; }
	oInsUnit.nCenPos[0] = vLayerTopo[nInsWallIdx].nCenPos[0];
	oInsUnit.nCenPos[1] = vLayerTopo[nInsWallIdx].nCenPos[1] * 2 + (vTable[oInsUnit.nCenUnitIdx].oShape.nShapeRange[3] - vTable[oInsUnit.nCenUnitIdx].oShape.nShapeRange[1]) / 2;
	oInsUnit.nCenPos[2] = vLayerTopo[nInsWallIdx].nCenPos[2];

	//更新新梁的拓扑关系
	int nColIdx[2];
	calColumnsIdxFromWallIdx(nInsWallIdx, nColIdx, vLayerTopo);
	oInsUnit.nAdjUnitIdx[0] = nColIdx[0];
	oInsUnit.nAdjUnitIdx[1] = nColIdx[1];
	oInsUnit.nAdjUnitIdx[2] = nInsWallIdx;

	//更新旧墙的拓扑关系
	for (int i = 0; i < 12; i++)
	{
		if (vLayerTopo[nInsWallIdx].nAdjUnitIdx[i] == -1)
		{
			vLayerTopo[nInsWallIdx].nAdjUnitIdx[i] = oInsUnit.nTopoIdx;
			break;
		}
	}

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
		if (nAllOldAdj[i] != -1 && nAllNewAdj[i] != -1)
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

	}

	return 0;
}
// 更新添加新柱后梁 墙之间的邻接关系
int UpdateNewAdj(int nNewAdj, int nAllOldAdj[12], VTOPOTABLE const& vLayerTopo, int nNewType)
{
	int i = 0;
	for (i = 0; i < 12; i++)
	{
		if (nAllOldAdj[i] != -1)
		{
			if (vLayerTopo[nAllOldAdj[i]].nUnitType == nNewType)
			{
				nAllOldAdj[i] = nNewAdj;
				break;
			}
		}

	}

	return 0;
}
// 在墙上插入柱子 待完善
int AddColInWall(BasicUnit oCol, int nInsWallIdx, PixelPos oInsPos, VUNITTABLE vTable, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	//vTable.push_back(oCol);

	// 待增加柱子的拓扑单元
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oCol.nUnitType;
	//oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nCenUnitIdx = oCol.nUnitIdx;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	oInsUnit.nTopoIdx = vLayerTopo.size();
	//柱子的中心点下点信息 待完善，一层的y值为0
	oInsUnit.nCenPos[0] = 0, oInsUnit.nCenPos[1] = 0, oInsUnit.nCenPos[2] = 0, oInsUnit.nCenPos[3] = 0;

	for (int i = 0; i < 12; i++) { oInsUnit.nAdjUnitIdx[i] = -1; }
	// 根据插入信息完成插入
	if (oInsUnit.nUnitAngle == 0)
	{
		oInsUnit.nCenPos[2] = vPlaneDraw[nInsWallIdx].nCen[1];
		//int nWallLeft = vPlaneDraw[nInsWallIdx].nCen[0] - vPlaneDraw[nInsWallIdx].nWH[0] / 2;
		//oInsUnit.nCenPos[0] = oInsPos.nXY[0] - nWallLeft;
		oInsUnit.nCenPos[0] = oInsPos.nXY[0];
	}
	else if (oInsUnit.nUnitAngle == 90)
	{
		oInsUnit.nCenPos[0] = vPlaneDraw[nInsWallIdx].nCen[0];
		//int nWallBottom = vPlaneDraw[nInsWallIdx].nCen[1] - vPlaneDraw[nInsWallIdx].nWH[1] / 2;
		//oInsUnit.nCenPos[2] = oInsPos.nXY[1] - nWallBottom;
		oInsUnit.nCenPos[2] = oInsPos.nXY[1];
	}
	// 添加柱 更新拓扑结构
	oInsUnit.nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo.push_back(oInsUnit);
	int nNewColIdx = vLayerTopo.size() - 1;
	// 墙分裂为两个墙
	// 先复制一份
	TopoUnit oAddWall = vLayerTopo[nInsWallIdx];
	int nNewWallIdx = vLayerTopo.size();
	oAddWall.nTopoIdx = nNewWallIdx;
	vLayerTopo.push_back(oAddWall);

	// 梁分裂为两个梁
	int i = 0;
	int nBeamIdx = -1;
	int nAdjIdx = 0;
	for (i = 0; i < 12; i++)
	{
		nAdjIdx = vLayerTopo[nInsWallIdx].nAdjUnitIdx[i];
			if (vLayerTopo[nAdjIdx].nUnitType == 2)
			{
				nBeamIdx = nAdjIdx;
				break;
			}



	}
	TopoUnit oBeamUnit;
	for (int j = 0; j < 12; j++) { oBeamUnit.nAdjUnitIdx[j] = -1; }
	int nNewBeamIdx = -1;
	if (nBeamIdx != -1)
	{
		oBeamUnit = vLayerTopo[nBeamIdx];
		nNewBeamIdx = vLayerTopo.size();
		oBeamUnit.nTopoIdx = nNewBeamIdx;
		vLayerTopo.push_back(oBeamUnit);

	}
	// 更新柱拓扑结构
	vLayerTopo[nNewColIdx].nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[1] = nNewWallIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[2] = nBeamIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[3] = nNewBeamIdx;




	// 更新墙拓扑结构
	UpdateNewAdj(nNewColIdx, vLayerTopo[nInsWallIdx].nAdjUnitIdx, vLayerTopo[nNewWallIdx].nAdjUnitIdx, vLayerTopo, 1);
	UpdateNewAdj(nNewBeamIdx, vLayerTopo[nNewWallIdx].nAdjUnitIdx, vLayerTopo, 2);

	// 更新梁拓扑结构
	UpdateNewAdj(nNewColIdx, vLayerTopo[nBeamIdx].nAdjUnitIdx, vLayerTopo[nNewBeamIdx].nAdjUnitIdx, vLayerTopo, 1);
	UpdateNewAdj(nNewWallIdx, vLayerTopo[nNewBeamIdx].nAdjUnitIdx, vLayerTopo, 4);



	//更新新墙关联柱的拓扑结构
	int nNewWallLinkColId;
	for (int i = 0; i < 12; i++)
	{
		if (vLayerTopo[nNewWallIdx].nAdjUnitIdx[i] != -1)
		{
			if (vLayerTopo[vLayerTopo[nNewWallIdx].nAdjUnitIdx[i]].nUnitType == 1 && vLayerTopo[nNewWallIdx].nAdjUnitIdx[i] != nNewColIdx)
			{
				nNewWallLinkColId = vLayerTopo[nNewWallIdx].nAdjUnitIdx[i];
			}
		}
	}
	for (int j = 0; j < 12; j++)
	{
		if (vLayerTopo[nNewWallLinkColId].nAdjUnitIdx[j] != -1 && vLayerTopo[nNewWallLinkColId].nAdjUnitIdx[j] == nInsWallIdx)
		{
			vLayerTopo[nNewWallLinkColId].nAdjUnitIdx[j] = nNewWallIdx;
		}
	}



	return 0;
}

// 在两个柱子之间插入墙 两个柱子之间必须为空 
int AddWallInCols(BasicUnit oAddUnit, int nAdjCol[2], VUNITTABLE vTable, VTOPOTABLE& vLayerTopo)
{
	// 根据插入信息完成插入
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oAddUnit.nUnitType;
	oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nCenUnitIdx = oAddUnit.nUnitIdx;

	if (abs(vLayerTopo[nAdjCol[0]].nCenPos[2] - vLayerTopo[nAdjCol[1]].nCenPos[2])<10)
	{
		oInsUnit.nUnitAngle = 0;
	}
	else
	{
		oInsUnit.nUnitAngle = 90;
	}
	oInsUnit.nTopoIdx = vLayerTopo.size();
	for (int i = 0; i < 12; i++) { oInsUnit.nAdjUnitIdx[i] = -1; }
	oInsUnit.nAdjUnitIdx[0] = nAdjCol[0];
	oInsUnit.nAdjUnitIdx[1] = nAdjCol[1];


	// 更新拓扑结构
	vLayerTopo.push_back(oInsUnit);

	//更新柱子拓扑结构
	for (int i = 0; i < 12; i++)
	{
		if (vLayerTopo[nAdjCol[0]].nAdjUnitIdx[i] == -1)
		{
			vLayerTopo[nAdjCol[0]].nAdjUnitIdx[i] = vLayerTopo.size() - 1;
			break;
		}
	}
	for (int i = 0; i < 12; i++)
	{
		if (vLayerTopo[nAdjCol[1]].nAdjUnitIdx[i] == -1)
		{
			vLayerTopo[nAdjCol[1]].nAdjUnitIdx[i] = vLayerTopo.size() - 1;
			break;
		}
	}
	

	return 0;
}

// 在当前拓扑图中增加构件
int DimDataConvert::AddBaseUnit(BasicUnit oAddUnit, PixelPos oInsPos, VUNITTABLE& vTable, VTOPOTABLE& vLayerTopo, VSHAPE vPlaneDraw)
{
	int nRe = 0;
	int nInsWallIdx = -1;//选择到的墙的ID
	int nAdjCol[2];		// 两个中有一个为空则置 [0]为-1

	// 计算 待完善

	//计算与墙连接的两个柱ID,如果此时选择的不是墙，则nInsWallIdx为-1
	if (vLayerTopo[selectUnitIdx].nUnitType == 4)
	{
		nInsWallIdx = selectUnitIdx;
		calColumnsIdxFromWallIdx(nInsWallIdx, nAdjCol, vLayerTopo);
	}

	if (oAddUnit.nUnitType != 4)
	{
		if (nInsWallIdx < 0 && nAdjCol[0] < 0)
			return 1;
		// 判断待插入构件是否是门窗
	}

	
	if (oAddUnit.nUnitType > 4)
	{
		nRe = AddDoorWndInWall(oAddUnit, nInsWallIdx, oInsPos, vTable, vLayerTopo, vPlaneDraw);
	}


	// 插入柱子
	else if (oAddUnit.nUnitType == 1)
	{
		nRe = AddColInWall(oAddUnit, nInsWallIdx, oInsPos, vTable, vLayerTopo, vPlaneDraw);
	}
	// 插入梁
	else if (oAddUnit.nUnitType == 2)
	{
		nRe = AddBeamOnWall(oAddUnit, nInsWallIdx, vTable, vLayerTopo, vPlaneDraw);
	}
	// 插入板
	else if (oAddUnit.nUnitType == 3)
	{
		//nRe = AddBoard(oAddUnit, nInsWallIdx, vTable, vLayerTopo, vPlaneDraw);;
	}

	// 插入墙
	else if (oAddUnit.nUnitType == 4 /*&& nAdjCol[0] >= 0*/)
	{
		//存储所有满足条件的一对柱子
		vector<VINT> nAdjCols;
		//获取两柱子ID
		for (int i = 0; i < vLayerTopo.size(); i++)
		{
			for (int j = 0; j < vLayerTopo.size(); j++)
			{
				//两个为柱子，则判断插入点是否在两个柱子之间
				if (j != i && vLayerTopo[i].nUnitType == 1 && vLayerTopo[j].nUnitType == 1)
				{



					VINT allCols;				//找到这些墙的所有柱子	


					//<10为允许鼠标点击误差
					//0度
					if (abs(vLayerTopo[i].nCenPos[2] - vLayerTopo[j].nCenPos[2])<10)
					{
						if ((oInsPos.nXY[0]< vLayerTopo[i].nCenPos[0] && oInsPos.nXY[0] > vLayerTopo[j].nCenPos[0])
							|| (oInsPos.nXY[0]< vLayerTopo[j].nCenPos[0] && oInsPos.nXY[0] > vLayerTopo[i].nCenPos[0]))
						{
							if (abs(oInsPos.nXY[1] - vLayerTopo[i].nCenPos[2]) < (vTable[vLayerTopo[i].nCenUnitIdx].oShape.nShapeRange[3] - vTable[vLayerTopo[i].nCenUnitIdx].oShape.nShapeRange[1]))
							{
								VINT tmp(2);
								tmp[0] = i;
								tmp[1] = j;

									//nAdjCol[0] = vLayerTopo[i].nTopoIdx, nAdjCol[1] = vLayerTopo[j].nTopoIdx;
									//break;
								nAdjCols.push_back(tmp);
							}
						}

					}
					//90度
					if (abs(vLayerTopo[i].nCenPos[0] - vLayerTopo[j].nCenPos[0])<10)
					{
						if ((oInsPos.nXY[1]< vLayerTopo[i].nCenPos[2] && oInsPos.nXY[1] > vLayerTopo[j].nCenPos[2])
							|| (oInsPos.nXY[1]< vLayerTopo[j].nCenPos[2] && oInsPos.nXY[1] > vLayerTopo[i].nCenPos[2]))
						{
							if (abs(oInsPos.nXY[0] - vLayerTopo[i].nCenPos[0]) < (vTable[vLayerTopo[i].nCenUnitIdx].oShape.nShapeRange[2] - vTable[vLayerTopo[i].nCenUnitIdx].oShape.nShapeRange[0]))
							{
									//nAdjCol[0] = vLayerTopo[i].nTopoIdx, nAdjCol[1] = vLayerTopo[j].nTopoIdx;
									//break;
								VINT tmp(2);
								tmp[0] = i;
								tmp[1] = j;

								nAdjCols.push_back(tmp);
							}
							
						}
					}
				}
			}
		}

		//找到距离最短的两个柱子
		int minDis = INT_MAX;
		for (int k = 0; k < nAdjCols.size(); k++)
		{
			int col1 = nAdjCols[k][0];
			int col2 = nAdjCols[k][1];

			float dis = 0;
			dis += (vLayerTopo[col1].nCenPos[0] - vLayerTopo[col2].nCenPos[0]) * (vLayerTopo[col1].nCenPos[0] - vLayerTopo[col2].nCenPos[0]);
			dis += (vLayerTopo[col1].nCenPos[1] - vLayerTopo[col2].nCenPos[1]) * (vLayerTopo[col1].nCenPos[1] - vLayerTopo[col2].nCenPos[1]);
			dis += (vLayerTopo[col1].nCenPos[2] - vLayerTopo[col2].nCenPos[2]) * (vLayerTopo[col1].nCenPos[2] - vLayerTopo[col2].nCenPos[2]);
			dis = sqrt(dis);

			if (dis < minDis)
			{
				minDis = dis;
				nAdjCol[0] = col1;
				nAdjCol[1] = col2;
			}
		}


		nRe = AddWallInCols(oAddUnit, nAdjCol, vTable, vLayerTopo);



	}





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
int DimDataConvert::DelBaseUnit(int nSelUnitIdx, VTOPOTABLE& vLayerTopp, int nRealDel)
{
	int nRe = 0;
	// 根据点击位置确定选择的构件
	//nRe = GetSelUnitIdx(oInsPos, nSelUnitIdx, vLayerTopp);
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
int MovDoorWndInWall(int nMoveUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// 门窗只能在墙范围内沿墙方向移动
	// 得到包含门窗的墙
	int nInsWallIdx = vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0];

	// 符合移动条件 移动方向一致且值不为0
	if (nMoveX != 0 && vLayerTopo[nInsWallIdx].nUnitAngle == 0)
	{
		// 计算门窗移动的合规范围 nCenPos[0]是相對位置
		//不能小于0;
		//不能大于墻的寬度-元素的中心點宽;
		//如果超过等于极限值

		int nLeft = 0;


		int doorW = table[vLayerTopo[nMoveUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; //元素的中心点
		int nWallWH = vPlaneDraw[vLayerTopo[nInsWallIdx].nTopoIdx].nWH[0];					//墻的寬度
		int nRight = nWallWH - doorW;



		// 按墙方向更改门窗位置
		int nNewX = vLayerTopo[nMoveUnitIdx].nCenPos[0] + nMoveX;

		if (nNewX < nLeft)
			nNewX = nLeft;

		else if (nNewX > nRight)
			nNewX = nRight;
		vLayerTopo[nMoveUnitIdx].nCenPos[0] = nNewX;
	}
	else if (nMoveY != 0 && vLayerTopo[nInsWallIdx].nUnitAngle == 90)
	{

		// 计算门窗移动的合规范围 nCenPos[0]是相對位置
		//不能小于0;
		//不能大于墻的高度-元素的中心點Y - 柱子的宽度/2;
		//(vPlaneDraw[(vLayerTopo[(vLayerTopo[24]).nAdjUnitIdx[0]]).nAdjUnitIdx[0]]).nWH/2
		int nBottom = 0;
		int doorW = table[vLayerTopo[nMoveUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; //元素的中心点
		int nUp = vPlaneDraw[nInsWallIdx].nWH[1] - doorW;
		// 按墙方向更改门窗位置
		int nNewY = vLayerTopo[nMoveUnitIdx].nCenPos[0] + (nMoveY * -1);
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
// 移动柱子 由PreMoveWall带动的移动(以及移动相连同角度的墙的柱子)
int MovCol(int nMoveUnitIdx, int moveWallId, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	//水平墙
	// 只移动柱子
	vLayerTopo[nMoveUnitIdx].nCenPos[0] += nMoveX;
	vLayerTopo[nMoveUnitIdx].nCenPos[2] += nMoveY;

	vPlaneDraw[nMoveUnitIdx].nCen[0] += nMoveX;
	vPlaneDraw[nMoveUnitIdx].nCen[1] += nMoveY;

	if (vLayerTopo[moveWallId].nUnitAngle == 0)
	{
		//找与此柱子连接的另一个水平墙
		int wallId = -1;
		for (int i = 0; i < 12; i++)
		{
			if ((vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i] != -1) && (vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i]].nUnitType == 4))
			{
				if (vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i]].nUnitAngle == 0)
				{
					if (vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i]].nTopoIdx != moveWallId)
					{
						wallId = vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i];
					}

				}
			}
		}
		//找到另一个水平墙
		int otherColId;
		if (wallId != -1)
		{
			for (int i = 0; i < 12; i++)
			{
				if (vLayerTopo[wallId].nAdjUnitIdx[i] != -1 && vLayerTopo[vLayerTopo[wallId].nAdjUnitIdx[i]].nUnitType == 1)
				{
					if (vLayerTopo[wallId].nAdjUnitIdx[i] != nMoveUnitIdx) { otherColId = vLayerTopo[wallId].nAdjUnitIdx[i]; }
				}
			}
		}
		else {
			return 0;
		}
		//
		MovCol(otherColId, wallId, 0, nMoveY, vLayerTopo, vPlaneDraw);
	}
	//垂直墙
	// 只移动柱子
	if (vLayerTopo[moveWallId].nUnitAngle == 90)
	{
		//找与此柱子连接的另一个垂直墙
		int wallId = -1;
		for (int i = 0; i < 12; i++)
		{
			if ((vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i] != -1) && (vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i]].nUnitType == 4))
			{
				if (vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i]].nUnitAngle == 90)
				{
					if (vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i]].nTopoIdx != moveWallId)
					{
						wallId = vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[i];
					}

				}
			}
		}
		//找到另一个垂直墙
		int otherColId;
		if (wallId != -1)
		{
			for (int i = 0; i < 12; i++)
			{
				if (vLayerTopo[wallId].nAdjUnitIdx[i] != -1 && vLayerTopo[vLayerTopo[wallId].nAdjUnitIdx[i]].nUnitType == 1)
				{
					if (vLayerTopo[wallId].nAdjUnitIdx[i] != nMoveUnitIdx) { otherColId = vLayerTopo[wallId].nAdjUnitIdx[i]; }
				}
			}
		}
		else {
			return 0;
		}
		//
		MovCol(otherColId, wallId, nMoveX, 0, vLayerTopo, vPlaneDraw);
	}
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
int MovWall(int nMoveUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	//int nRe = 0;
	//// 墙只能沿垂直墙方向移动
	//// 墙两端必须有柱子
	//// 得到柱子关联的其他墙
	//int nAdjCol[2];
	//int nXYMoveRange[2][2];
	//// 	// 计算墙的合规范围
	//nRe = CalWallMoveRange(nMoveUnitIdx, vLayerTopo, vPlaneDraw, nAdjCol, nXYMoveRange);

	//// 按墙垂直方向计算移动距离
	//if (nMoveX <= nXYMoveRange[0][0] || nMoveX >= nXYMoveRange[0][1])
	//	nMoveX = 0;
	//if (nMoveY <= nXYMoveRange[1][0] || nMoveY >= nXYMoveRange[1][1])
	//	nMoveY = 0;



	int nAdjCol[2];
	int nXYMoveRange[2][2];
	int nRe = 0;
	// 计算墙的合规范围
	nRe = CalWallMoveRange(nMoveUnitIdx, vLayerTopo, vPlaneDraw, nAdjCol, nXYMoveRange);
	int nAdjNum = 10;
	int j = 0;
	int nCurAdj;
	int nAdjColIdx[2];
	int nColIdx = 0;
	int nWall = 0;
	TopoUnit oUnit = vLayerTopo[nMoveUnitIdx];
	for (j = 0; j < nAdjNum; j++)
	{
		nCurAdj = oUnit.nAdjUnitIdx[j];
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
	int thickness;
	// 得到两个柱子的范围
	//for (int j = 0; j < 12; j++)
	//{
	//	if (vLayerTopo[nWall].nAdjUnitIdx[j] != -1 && vLayerTopo[vLayerTopo[nWall].nAdjUnitIdx[j]].nUnitType == 1 && vLayerTopo[vLayerTopo[nWall].nAdjUnitIdx[j]].nUnitAngle == 0)
	//	{
	//		int nColIdx = vLayerTopo[vLayerTopo[nWall].nAdjUnitIdx[j]].nCenUnitIdx;
	//		int thickness = table[nColIdx].oShape.nThickNess;


	//	}

	//}






	//SimpleShape oColShape0 = vPlaneDraw[nAdjColIdx[0]];
	//SimpleShape oColShape1 = vPlaneDraw[nAdjColIdx[1]];
	//int nXDis = oColShape0.nCen[0] - oColShape1.nCen[0] - (oColShape0.nWH[0] + oColShape1.nWH[0]) / 2;
	//int nYDis = oColShape0.nCen[1] - oColShape1.nCen[1] - (oColShape0.nWH[1] + oColShape1.nWH[1]) / 2;






	// 根据墙的两个柱子得到另外两个需要判定的墙
	int finalXminDis, finalYminDis;
	int nXDis=0;
	int nYDis=0;
	if (vLayerTopo[nMoveUnitIdx].nUnitAngle == 0)
	{
		VINT allWalls;				//找到所有与此墙同一水平的柱子	

		for (int i = 0; i < vLayerTopo.size(); i++)//找寻与此墙同一水平的其他墙
		{

			if (vLayerTopo[i].nUnitType == 4 && vLayerTopo[i].nUnitAngle == 0)
			{
				int moveWallY = vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[2];					//移动的墙的中心Y值
				int newWallY = vLayerTopo[vLayerTopo[i].nAdjUnitIdx[0]].nCenPos[2];								//判定墙的中心Y值

				if (moveWallY == newWallY)
				{
					allWalls.push_back(i);
				}

			}
		}

		VINT allCols;				//找到这些墙的所有柱子	
		for (int j = 0; j < allWalls.size(); j++)
		{
			if (find(allCols.begin(), allCols.end(), vLayerTopo[allWalls[j]].nAdjUnitIdx[0]) == allCols.end())
			{
				allCols.push_back(vLayerTopo[allWalls[j]].nAdjUnitIdx[0]);
			}
			if (find(allCols.begin(), allCols.end(), vLayerTopo[allWalls[j]].nAdjUnitIdx[1]) == allCols.end())
			{
				allCols.push_back(vLayerTopo[allWalls[j]].nAdjUnitIdx[1]);
			}
		}

		VINT nUpWalls;				//找到这些柱子所连接的竖直墙(分上面与下面，基于最初移动的墙)
		VINT nDownWalls;
		for (int k = 0; k < allCols.size(); k++)
		{
			for (int i = 0; i < 12; i++)
			{
				if (vLayerTopo[allCols[k]].nAdjUnitIdx[i] != -1)
				{
					if (vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nUnitType == 4 && vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nUnitAngle == 90)
					{
						int moveWallY = vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[2];					//移动的墙的中心Y值
						int newWallY;								//判定墙的中心Y值
						{
							int col1 = vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nAdjUnitIdx[0];
							int col2 = vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nAdjUnitIdx[1];
							newWallY = (vLayerTopo[col1].nCenPos[2] + vLayerTopo[col2].nCenPos[2]) / 2;

							//int thickness = table[vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nCenUnitIdx].oShape.nThickNess;
							//int colH = vPlaneDraw[col1].nWH[1];
							//int Dis = (colH - thickness)/2;
							//if (vLayerTopo[col1].nCenPos[2] > vLayerTopo[col2].nCenPos[2])
							//{
							//	nYDis = vLayerTopo[col1].nCenPos[2] - vLayerTopo[col2].nCenPos[2]/*-Dis*/ - colH;//新增
							//}
							//else
							//{
							//	nYDis = vLayerTopo[col2].nCenPos[2] - vLayerTopo[col2].nCenPos[1]/*-Dis*/ - colH;//新增
							//}
							}

						if (newWallY < moveWallY)
						{
							nUpWalls.push_back(vLayerTopo[allCols[k]].nAdjUnitIdx[i]);
						}
						else {
							nDownWalls.push_back(vLayerTopo[allCols[k]].nAdjUnitIdx[i]);
						}
					}
				}
			}
		}


		//找到能移动的最大距离
		{
			if (nMoveY < 0)	//如果墙是往上挪的
			{
				VINT nDoorAndWinIds;		//找到这些竖直的墙的对应门窗ID
				for (int i = 0; i < nUpWalls.size(); i++)		//原墙以上的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nUpWalls[i]].nAdjUnitIdx[j] != -1)
						{
							if (vLayerTopo[vLayerTopo[nUpWalls[i]].nAdjUnitIdx[j]].nUnitType > 4)
							{
								nDoorAndWinIds.push_back(vLayerTopo[nUpWalls[i]].nAdjUnitIdx[j]);
							}
						}
					}
				}

				for (int i = 0; i < nUpWalls.size(); i++)		//原墙以上的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nUpWalls[i]].nAdjUnitIdx[j] != -1)
						{
							int col1 = vLayerTopo[nUpWalls[i]].nAdjUnitIdx[0];
							int col2 = vLayerTopo[nUpWalls[i]].nAdjUnitIdx[1];
							int colH = vPlaneDraw[col1].nWH[1];
							int thickness = table[vLayerTopo[nUpWalls[i]].nCenUnitIdx].oShape.nThickNess;
							int addY = (colH - thickness) / 2;
							if (vLayerTopo[col1].nCenPos[2] > vLayerTopo[col2].nCenPos[2])
							{
								nYDis = vLayerTopo[col1].nCenPos[2] - vLayerTopo[col2].nCenPos[2] - colH- addY;//新增
							}
							else
							{
								nYDis = vLayerTopo[col2].nCenPos[2] - vLayerTopo[col2].nCenPos[1] - colH- addY;//新增
							}
						}
						break;
					}
					break;
				}



				if (nDoorAndWinIds.size() != 0)
				{
					int maxDisY = 0;
					for (int tmp = 0; tmp < nDoorAndWinIds.size(); tmp++)
					{

						int dis = vLayerTopo[nDoorAndWinIds[tmp]].nCenPos[0] + vPlaneDraw[nDoorAndWinIds[tmp]].nWH[0];
						if (dis > maxDisY) { maxDisY = dis; }
					}
					//nYDis += maxDisY;
					// 
					// 
					//再减去另一个墙的厚度

					{
						int wallId = vLayerTopo[nDoorAndWinIds[0]].nAdjUnitIdx[0];
						int colId;
						for (int i = 0; i < 2; i++)
						{
							if (vLayerTopo[vLayerTopo[wallId].nAdjUnitIdx[i]].nCenPos[2] < vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[2])
							{
								colId = vLayerTopo[wallId].nAdjUnitIdx[i];
							}
						}
						//通过此柱子找到移动墙对面的墙
						for (int j = 0; j < 12; j++)
						{
							if (vLayerTopo[colId].nAdjUnitIdx[j] != -1 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitType == 4 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitAngle == 0)
							{
								int walUnitId = vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nCenUnitIdx;
								maxDisY += thickness;

								//柱子与墙厚度有一点差异
								int colH = vPlaneDraw[colId].nWH[1];
								maxDisY += (colH - thickness) / 2;

								break;
							}
						}


					}



					//nYDis += maxDisY;
					nXYMoveRange[1][0] += maxDisY;
				}

			}
			else
			{
				VINT nDoorAndWinIds;		//找到这些竖直的墙的对应门窗ID
				for (int i = 0; i < nDownWalls.size(); i++)		//原墙以下的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nDownWalls[i]].nAdjUnitIdx[j] != -1)
						{
							if (vLayerTopo[vLayerTopo[nDownWalls[i]].nAdjUnitIdx[j]].nUnitType > 4)
							{
								nDoorAndWinIds.push_back(vLayerTopo[nDownWalls[i]].nAdjUnitIdx[j]);
							}
						}
					}
				}


				for (int i = 0; i < nDownWalls.size(); i++)		//原墙以下的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nDownWalls[i]].nAdjUnitIdx[j] != -1)
						{
							int col1 = vLayerTopo[nDownWalls[i]].nAdjUnitIdx[0];
							int col2 = vLayerTopo[nDownWalls[i]].nAdjUnitIdx[1];
							int colH = vPlaneDraw[col1].nWH[1];
							if (vLayerTopo[col1].nCenPos[2] > vLayerTopo[col2].nCenPos[2])
							{
								nYDis = vLayerTopo[col1].nCenPos[2] - vLayerTopo[col2].nCenPos[2] - colH;//新增
							}
							else
							{
								nYDis = vLayerTopo[col2].nCenPos[2] - vLayerTopo[col2].nCenPos[1] - colH;//新增
							}
						}
						break;
					}
					break;
				}



				if (nDoorAndWinIds.size() != 0)
				{
					int maxDisY = 0;
					for (int tmp = 0; tmp < nDoorAndWinIds.size(); tmp++)
					{

						int dis = vLayerTopo[nDoorAndWinIds[tmp]].nCenPos[0] + vPlaneDraw[nDoorAndWinIds[tmp]].nWH[0];
						if (dis > maxDisY) { maxDisY = dis; }
					}
					//nYDis -= maxDisY;

					//再减去另一个墙的厚度
					{
						int wallId = vLayerTopo[nDoorAndWinIds[0]].nAdjUnitIdx[0];
						int colId;
						for (int i = 0; i < 2; i++)
						{
							if (vLayerTopo[vLayerTopo[wallId].nAdjUnitIdx[i]].nCenPos[2] > vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[2])
							{
								colId = vLayerTopo[wallId].nAdjUnitIdx[i];
							}
						}
						//通过此柱子找到移动墙对面的墙
						for (int j = 0; j < 12; j++)
						{
							if (vLayerTopo[colId].nAdjUnitIdx[j] != -1 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitType == 4 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitAngle == 0)
							{
								int walUnitId = vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nCenUnitIdx;
								int thickness = table[walUnitId].oShape.nThickNess;
								maxDisY += thickness;

								//柱子与墙厚度有一点差异
								int colH = vPlaneDraw[colId].nWH[1];
								maxDisY += (colH - thickness) / 2;

								break;
							}
						}


					}
					//nYDis += maxDisY;
					nXYMoveRange[1][1] -= maxDisY;
				}

			}




		}

	}









	// 根据墙的两个柱子得到另外两个需要判定的墙

	if (vLayerTopo[nMoveUnitIdx].nUnitAngle == 90)
	{
		VINT allWalls;				//找到所有与此墙同垂直的柱子	

		for (int i = 0; i < vLayerTopo.size(); i++)//找寻与此墙垂直的其他墙
		{

			if (vLayerTopo[i].nUnitType == 4 && vLayerTopo[i].nUnitAngle == 90)
			{
				int moveWallX = vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[0];					//移动的墙的中心X值
				int newWallX = vLayerTopo[vLayerTopo[i].nAdjUnitIdx[0]].nCenPos[0];								//判定墙的中心X值

				if (moveWallX == newWallX)
				{
					allWalls.push_back(i);
				}

			}
		}

		VINT allCols;				//找到这些墙的所有柱子	
		for (int j = 0; j < allWalls.size(); j++)
		{
			if (find(allCols.begin(), allCols.end(), vLayerTopo[allWalls[j]].nAdjUnitIdx[0]) == allCols.end())
			{
				allCols.push_back(vLayerTopo[allWalls[j]].nAdjUnitIdx[0]);
			}
			if (find(allCols.begin(), allCols.end(), vLayerTopo[allWalls[j]].nAdjUnitIdx[1]) == allCols.end())
			{
				allCols.push_back(vLayerTopo[allWalls[j]].nAdjUnitIdx[1]);
			}
		}


		VINT nLeftWalls;				//找到这些柱子所连接的竖直墙(左边与右边，基于最初移动的墙)
		VINT nRightWalls;
		for (int k = 0; k < allCols.size(); k++)
		{
			for (int i = 0; i < 12; i++)
			{
				if (vLayerTopo[allCols[k]].nAdjUnitIdx[i] != -1)
				{
					if (vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nUnitType == 4 && vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nUnitAngle == 0)
					{
						int moveWallX = vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[0];					//移动的墙的中心X值
						int newWallX;								//判定墙的中心X值
						{
							int col1 = vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nAdjUnitIdx[0];
							int col2 = vLayerTopo[vLayerTopo[allCols[k]].nAdjUnitIdx[i]].nAdjUnitIdx[1];
							newWallX = (vLayerTopo[col1].nCenPos[0] + vLayerTopo[col2].nCenPos[0]) / 2;


							//int colH = vPlaneDraw[col1].nWH[0];
							//nXDis = abs(vLayerTopo[col1].nCenPos[0] - vLayerTopo[col2].nCenPos[0])/*-Dis*/ - colH;//新增
						


						}

						if (newWallX < moveWallX)
						{
							nLeftWalls.push_back(vLayerTopo[allCols[k]].nAdjUnitIdx[i]);
						}
						else {
							nRightWalls.push_back(vLayerTopo[allCols[k]].nAdjUnitIdx[i]);
						}
					}
				}
			}
		}



		//找到能移动的最大距离
		{
			if (nMoveX < 0)	//如果墙是往左挪的
			{
				VINT nDoorAndWinIds;		//找到这些竖直的墙的对应门窗ID
				for (int i = 0; i < nLeftWalls.size(); i++)		//原墙左边的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nLeftWalls[i]].nAdjUnitIdx[j] != -1)
						{
							if (vLayerTopo[vLayerTopo[nLeftWalls[i]].nAdjUnitIdx[j]].nUnitType > 4)
							{
								nDoorAndWinIds.push_back(vLayerTopo[nLeftWalls[i]].nAdjUnitIdx[j]);
							}
						}
					}
				}


				for (int i = 0; i < nLeftWalls.size(); i++)		//原墙以左的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nLeftWalls[i]].nAdjUnitIdx[j] != -1)
						{
							int col1 = vLayerTopo[nLeftWalls[i]].nAdjUnitIdx[0];
							int col2 = vLayerTopo[nLeftWalls[i]].nAdjUnitIdx[1];
							int colH = vPlaneDraw[col1].nWH[0];
							int thickness = table[vLayerTopo[nLeftWalls[i]].nCenUnitIdx].oShape.nThickNess;
							int addX = (colH - thickness) / 2;

							if (vLayerTopo[col1].nCenPos[0] > vLayerTopo[col2].nCenPos[0])
							{
								nXDis = vLayerTopo[col1].nCenPos[0] - vLayerTopo[col2].nCenPos[0] - colH- addX;//新增
							}
							else
							{
								nXDis = vLayerTopo[col2].nCenPos[0] - vLayerTopo[col1].nCenPos[0] - colH- addX;//新增
							}
						}
						break;
					}
					break;
				}



				if (nDoorAndWinIds.size() != 0)
				{
					int maxDisX = 0;
					for (int tmp = 0; tmp < nDoorAndWinIds.size(); tmp++)
					{

						int dis = vLayerTopo[nDoorAndWinIds[tmp]].nCenPos[2] + vPlaneDraw[nDoorAndWinIds[tmp]].nWH[0];
						if (dis > maxDisX) { maxDisX = dis; }
					}
					//nYDis += maxDisY;
					// 
					// 
					//再减去另一个墙的厚度

					{
						int wallId = vLayerTopo[nDoorAndWinIds[0]].nAdjUnitIdx[0];
						int colId;
						for (int i = 0; i < 2; i++)
						{
							if (vLayerTopo[vLayerTopo[wallId].nAdjUnitIdx[i]].nCenPos[2] < vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[2])
							{
								colId = vLayerTopo[wallId].nAdjUnitIdx[i];
							}
						}
						//通过此柱子找到移动墙对面的墙
						for (int j = 0; j < 12; j++)
						{
							if (vLayerTopo[colId].nAdjUnitIdx[j] != -1 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitType == 4 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitAngle == 90)
							{
								int walUnitId = vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nCenUnitIdx;
								int thickness = table[walUnitId].oShape.nThickNess;
								maxDisX += thickness;

								//柱子与墙厚度有一点差异
								int colH = vPlaneDraw[colId].nWH[1];
								maxDisX += (colH - thickness)/* / 2*/;

								break;
							}
						}


					}




					nXYMoveRange[0][0] += maxDisX;
				}

			}
			else
			{
				VINT nDoorAndWinIds;		//找到这些竖直的墙的对应门窗ID
				for (int i = 0; i < nRightWalls.size(); i++)		//原墙右边的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nRightWalls[i]].nAdjUnitIdx[j] != -1)
						{
							if (vLayerTopo[vLayerTopo[nRightWalls[i]].nAdjUnitIdx[j]].nUnitType > 4)
							{
								nDoorAndWinIds.push_back(vLayerTopo[nRightWalls[i]].nAdjUnitIdx[j]);
							}
						}
					}
				}

				for (int i = 0; i < nRightWalls.size(); i++)		//原墙以右的墙
				{
					for (int j = 0; j < 12; j++)
					{
						if (vLayerTopo[nRightWalls[i]].nAdjUnitIdx[j] != -1)
						{
							int col1 = vLayerTopo[nRightWalls[i]].nAdjUnitIdx[0];
							int col2 = vLayerTopo[nRightWalls[i]].nAdjUnitIdx[1];
							int colH = vPlaneDraw[col1].nWH[0];
							if (vLayerTopo[col1].nCenPos[0] > vLayerTopo[col2].nCenPos[0])
							{
								nXDis = vLayerTopo[col1].nCenPos[0] - vLayerTopo[col2].nCenPos[0] - colH;//新增
							}
							else
							{
								nXDis = vLayerTopo[col2].nCenPos[0] - vLayerTopo[col1].nCenPos[0] - colH;//新增
							}
						}
						break;
					}
					break;
				}




				if (nDoorAndWinIds.size() != 0)
				{
					int maxDisX = 0;
					for (int tmp = 0; tmp < nDoorAndWinIds.size(); tmp++)
					{

						int dis = vLayerTopo[nDoorAndWinIds[tmp]].nCenPos[2] + vPlaneDraw[nDoorAndWinIds[tmp]].nWH[0];
						if (dis > maxDisX) { maxDisX = dis; }
					}
					//nYDis -= maxDisY;

					//再减去另一个墙的厚度
					{
						int wallId = vLayerTopo[nDoorAndWinIds[0]].nAdjUnitIdx[0];
						int colId;
						for (int i = 0; i < 2; i++)
						{
							if (vLayerTopo[vLayerTopo[wallId].nAdjUnitIdx[i]].nCenPos[0] > vLayerTopo[vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0]].nCenPos[0])
							{
								colId = vLayerTopo[wallId].nAdjUnitIdx[i];
							}
						}
						//通过此柱子找到移动墙对面的墙
						for (int j = 0; j < 12; j++)
						{
							if (vLayerTopo[colId].nAdjUnitIdx[j] != -1 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitType == 4 && vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nUnitAngle == 90)
							{
								int walUnitId = vLayerTopo[vLayerTopo[colId].nAdjUnitIdx[j]].nCenUnitIdx;
								int thickness = table[walUnitId].oShape.nThickNess;
								maxDisX += thickness;

								//柱子与墙厚度有一点差异
								int colH = vPlaneDraw[colId].nWH[1];
								maxDisX += (colH - thickness)/* / 2*/;

								break;
							}
						}


					}

					nXYMoveRange[0][1] -= maxDisX;
				}

			}




		}

	}














	//// 按墙垂直方向计算移动距离

	//if (nMoveX <= nXYMoveRange[0][0] || nMoveX >= nXYMoveRange[0][1])
	//{

	//	if (abs(nMoveX) < nYDis)
	//	{
	//		nMoveX = 0;
	//	}
	//}

	//if (nMoveY <= nXYMoveRange[1][0] || nMoveY >= nXYMoveRange[1][1])
	//{
	//	if (abs(nMoveY) < nXDis)
	//	{
	//		nMoveY = 0;
	//	}
	//}




		// 按墙垂直方向计算移动距离

	if (nMoveX <= nXYMoveRange[0][0] || nMoveX >= nXYMoveRange[0][1])
	{
		nMoveX = 0;

	}
	if (nMoveX > nXDis)
	{
		nMoveX = 0;
	}

	if (nMoveY <= nXYMoveRange[1][0] || nMoveY >= nXYMoveRange[1][1])
	{
		nMoveY = 0;
	}

	if (nMoveY > nYDis)
	{
		nMoveY = 0;
	}






	// 移动两端柱子
	int nColNum = 2;
	int i = 0;



	for (i = 0; i < nColNum; i++)
	{
		MovCol(nAdjCol[i], nMoveUnitIdx, nMoveX, nMoveY, vLayerTopo, vPlaneDraw);
	}
	// 重新计算几何数值

	return 0;
}
// 平面图中移动选中单元构件
int DimDataConvert::MoveBaseUnit(int nSelUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	int nRe = 0;
	// 平面图中 墙门窗可动 判断是否是墙门窗
	int nUnitType = vLayerTopo[nSelUnitIdx].nUnitType;
	// 判断待移动构件是否是门窗
	if (nUnitType > 4)
		nRe = MovDoorWndInWall(nSelUnitIdx, nMoveX, nMoveY, vLayerTopo, vPlaneDraw, table);

	// 移动墙
	else if (nUnitType == 4)
		nRe = MovWall(nSelUnitIdx, nMoveX, nMoveY, vLayerTopo, vPlaneDraw, table);

	return 0;
}


