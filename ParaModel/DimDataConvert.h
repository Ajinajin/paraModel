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
	int CalWallShape(int nUnitIdx, VTOPOTABLE const& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table);

	// ����ǽ�����ϼ����Ŵ�
	int CalDoorWndShape(int nUnitIdx, VTOPOTABLE const& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table);

	// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
	int CalPlaneData(VTOPOTABLE const& vLayerTopo, VSHAPE& vPlaneDraw,  VUNITTABLE table);
};