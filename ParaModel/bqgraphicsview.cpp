#include "bqgraphicsview.h"

#include "ParaType.h"

BQGraphicsView::BQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	// 隐藏水平/竖直滚动条
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// 设置场景范围
	//setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
	setSceneRect(0, 0, 2000, 2000);
	// 反锯齿
	setRenderHints(QPainter::Antialiasing);
}

void BQGraphicsView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
	emit GraphicsViewFocus(true);
}

// 平面图处理
// 基本构件的 增删改查 
// 平面图中可选择 墙门窗 立面图中可选择柱门窗

// 在墙上插入门窗
int AddDoorWndInWall(BasicUnit oDoorWnd, int nInsWallIdx, PixelPos oInsPos, VTOPOTABLE& vLayerTopo)
{
	// 根据插入信息完成插入

	// 更新拓扑结构

	return 0;
}
// 在墙上插入柱子
int AddColInWall(BasicUnit oDoorWnd, int nInsWallIdx, PixelPos oInsPos, VTOPOTABLE& vLayerTopo)
{
	// 根据插入信息完成插入

	// 更新拓扑结构

	return 0;
}

// 在两个柱子之间插入墙 两个柱子之间必须为空 
int AddWallInCols(BasicUnit oAddUnit, int nAdjCol[2], VTOPOTABLE& vLayerTopo)
{
	// 根据插入信息完成插入

	// 更新拓扑结构

	return 0;
}


//void BQGraphicsView::keyPressEvent(QKeyEvent* event)
//{
//    QGraphicsView::keyPressEvent(event);
//    emit GraphicsViewFocus(true);
//}
//
//void BQGraphicsView::keyReleaseEvent(QKeyEvent* event)
//{
//    QGraphicsView::keyReleaseEvent(event);
//    emit GraphicsViewFocus(true);
//}

void BQGraphicsView::wheelEvent(QWheelEvent* event)
{
	double factor = event->delta() > 0 ? 1.1 : 1 / 1.1;

	this->viewport()->update();

}
// 在当前拓扑图中增加构件
int AddBaseUnit(BasicUnit oAddUnit, PixelPos oInsPos, VTOPOTABLE& vLayerTopo)
{
	int nRe = 0;
	// 判断插入点是否在墙线范围内 返回墙在拓扑图的Idx 或者 虚墙区域关联的两个柱子
	int nInsWallIdx = 0;
	int nAdjCol[2];		// 两个中有一个为空则置 [0]为-1
	// 计算 待完善

	if (nInsWallIdx < 0 && nAdjCol[0] < 0)
		return 1;
	// 判断待插入构件是否是门窗
	if (oAddUnit.nUnitType > 3)
		nRe = AddDoorWndInWall(oAddUnit, nInsWallIdx, oInsPos, vLayerTopo);

	// 插入柱子
	else if (oAddUnit.nUnitType == 0)
		nRe = AddColInWall(oAddUnit, nInsWallIdx, oInsPos, vLayerTopo);

	// 插入墙
	else if (oAddUnit.nUnitType == 3 && nAdjCol[0] >= 0)
		nRe = AddWallInCols(oAddUnit, nAdjCol, vLayerTopo);


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
int DelBaseUnit(PixelPos oInsPos, VTOPOTABLE& vLayerTopp, int nRealDel = 1)
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
int MovDoorWndInWall(int nMoveUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo)
{
	// 门窗只能在墙范围内沿墙方向移动
	// 得到包含门窗的墙

	// 计算门窗移动的合规范围

	// 按墙方向更改门窗位置

	return 0;
}

// 移动柱子 由PreMoveWall带动的移动
int MovCol(int nMoveUnitIdx, int nMoveXY[2], int nPreMoveWallIdx, VTOPOTABLE& vLayerTopo)
{
	// 得到柱子关联的其他墙

	return 0;
}
// 移动墙
int MovWall(int nMoveUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo)
{
	// 墙只能沿垂直墙方向移动
	// 得到墙两端的柱子索引

	// 得到柱子关联的其他墙

	// 得到墙中包含的门窗

	// 计算墙的合规范围

	// 按墙垂直方向移动墙

	// 移动两端柱子

	// 更改墙中门窗位置

	return 0;
}
// 平面图中移动选中单元构件
int MoveBaseUnit(int nSelUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo)
{
	int nRe = 0;
	// 平面图中 墙门窗可动 判断是否是墙门窗
	int nUnitType = vLayerTopo[nSelUnitIdx].nUnitType;
	// 判断待插入构件是否是门窗
	if (nUnitType > 3)
		nRe = MovDoorWndInWall(nSelUnitIdx, nMoveXY, vLayerTopo);

	// 移动墙
	else if (nUnitType == 3)
		nRe = MovWall(nSelUnitIdx, nMoveXY, vLayerTopo);

	return 0;
}

// 生成柱子数据
int GenColData(TopoUnit oColUnit, UnitData& oData)
{
	return 0;
}
// 楼板单独处理 较复杂
int GenPlateData(int nUnitIdx, VTOPOTABLE const& vLayerTopo, UnitData& oCurData)
{
	return 0;
}
// 生成梁数据
int GenBeamData(int nUnitIdx, VTOPOTABLE const& vLayerTopo, UnitData& oCurData)
{
	return 0;
}
// 生成墙数据
int GenWallData(int nUnitIdx, VTOPOTABLE const& vLayerTopo, UnitData& oCurData)
{
	return 0;
}
// 生成墙数据
int GenDoorWndData(int nUnitIdx, VTOPOTABLE const& vLayerTopo, UnitData& oCurData)
{
	return 0;
}
// 拓扑结构数据转为现实数据 只考虑矩形截面
int CalRealData(VTOPOTABLE const& vLayerTopo, VUNITDATA& vRealData)
{
	int nUnitNum = vLayerTopo.size();
	int i = 0;
	TopoUnit oUnit;

	vRealData.clear();
	UnitData oCurData;
	for (i = 0; i < nUnitNum; i++)
	{
		oUnit = vLayerTopo[i];
		// 柱 直接生成
		if (oUnit.nUnitType == 0)
			GenColData(oUnit, oCurData);
		// 门窗 直接忽略 墙生成后再生成门窗数据
		else if (oUnit.nUnitType > 4)
			continue;
		// 楼板单独处理 较复杂
		else if (oUnit.nUnitType == 2)
			GenPlateData(i, vLayerTopo, oCurData);
		// 梁 墙一起处理
		else if (oUnit.nUnitType == 1 || oUnit.nUnitType == 2)
			GenBeamData(i, vLayerTopo, oCurData);
		else if (oUnit.nUnitType == 1 || oUnit.nUnitType == 2)
			GenWallData(i, vLayerTopo, oCurData);

		vRealData.push_back(oCurData);
	}

	// 生成门窗数据
	for (i = 0; i < nUnitNum; i++)
	{
		oUnit = vLayerTopo[i];
		// 门窗 依附墙数据生成
		if (oUnit.nUnitType > 4)
			GenDoorWndData(i, vLayerTopo, oCurData);
		vRealData.push_back(oCurData);
	}

	return 0;

}