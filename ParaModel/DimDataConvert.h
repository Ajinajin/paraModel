#pragma once

#include <ParaType.h>


class DimDataConvert
{
public:
	// �������ĵ�ͳ������ɾ�����״��Ϣ
	int GenRectShape(int nCen[2], int nWH[2], SimpleShape& oRectShape);

	// ���ƽ��ͼ�� �������������꼯
	int CalPlaneColShape(TopoUnit oUnit, SimpleShape& oShape, VUNITTABLE table);

	// �������ӻ����ϼ���ǽ
	int CalWallShape(int nUnitIdx, VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table);

	// ����ǽ�����ϼ����Ŵ�
	int CalDoorWndShape(int nUnitIdx, VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table);

	// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
	int CalPlaneData(VTOPOTABLE & vLayerTopo, VSHAPE& vPlaneDraw,  VUNITTABLE table);

	// �ڵ�ǰ����ͼ�����ӹ���
	int AddBaseUnit(BasicUnit oAddUnit, PixelPos oInsPos, VUNITTABLE & vTable, VTOPOTABLE& vLayerTopo, VSHAPE vPlaneDraw); 

	// �ڵ�ǰ����ͼ��ɾ������
	int DelBaseUnit(int nSelUnitIdx,VTOPOTABLE& vLayerTopp, int nRealDel = 1);

	// ƽ��ͼ���ƶ�ѡ�е�Ԫ����
	int MoveBaseUnit(int nSelUnitIdx, int nMoveXY[2], VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw); 

};