
#include <ParaType.h>
#include <DimDataConvert.h>
#include <math.h>

//ͨ��ǽ�Ĺ���������õ��������ӵ�������������
void calColumnsIdxFromWallIdx(int wallIdx, int *columnArrayIdx, VTOPOTABLE oglTopTable)
{
	//����ǽ���ӵ��������������
	
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

// �������ĵ�ͳ������ɾ�����״��Ϣ
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
// ���ƽ��ͼ�� �������������꼯
int DimDataConvert::CalPlaneColShape(TopoUnit oUnit, SimpleShape& oShape, VUNITTABLE table)
{
	// ��ϵͳ�������в������ӽ�����״���ߴ� Ŀǰֻ������������
	int nWH[2];
	nWH[0] = table[oUnit.nCenUnitIdx].oShape.nShapeRange[2];
	nWH[1] = table[oUnit.nCenUnitIdx].oShape.nShapeRange[3];
	int nCen[2];
	nCen[0] = oUnit.nCenPos[0];
	nCen[1] = oUnit.nCenPos[2];
	GenRectShape(nCen, nWH, oShape);

	return 0;
}

// �������ӻ����ϼ���ǽ
int DimDataConvert::CalWallShape(int nUnitIdx, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// �õ�ǽ�Ĺ�����
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

	// �õ��������ӵķ�Χ
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
	// ˮƽǽ
	SimpleShape oWallShape;
	int nWH[2], nCen[2];
	if (oUnit.nUnitAngle == 0)
	{
		nWH[0] = abs(oColShape0.nCen[0] - oColShape1.nCen[0]);
		nWH[1] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // ��� ����õ�

		if (oColShape0.nCen[0] > oColShape1.nCen[0])
		{
			nWH[0] -= (oColShape0.nWH[0] + oColShape1.nWH[0]) / 2;
			nCen[1] = oColShape0.nCen[1];
			nCen[0] = oColShape0.nCen[0] - oColShape0.nWH[0] / 2 - nWH[0] / 2;
		}
		else
		{
			nWH[0] = nWH[0]- (oColShape0.nWH[0] + oColShape1.nWH[0])/2;
			nCen[1] = oColShape0.nCen[1];
			nCen[0] = oColShape1.nCen[0] - oColShape1.nWH[0] / 2 - abs(nWH[0] / 2);
		}
	}
	// ��ֱǽ
	else if (oUnit.nUnitAngle == 90)
	{
		nWH[0] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // ��� ����õ�
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
		nWH[0] = table[oUnit.nCenUnitIdx].oShape.nThickNess; // ��� ����õ�
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
	// ����ǽ��״��Ϣ
	vPlaneDraw[nUnitIdx] = oWallShape;

	return 0;
}

// ����ǽ�����ϼ����Ŵ�
int DimDataConvert::CalDoorWndShape(int nUnitIdx, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// �õ��Ŵ��Ĺ���ǽ
	int nWallIdx = vLayerTopo[nUnitIdx].nAdjUnitIdx[0];
	if (nWallIdx < 0)
		return 1;

	vLayerTopo[nUnitIdx].nUnitAngle = vLayerTopo[nWallIdx].nUnitAngle;
	// ˮƽǽ 
	int nCen[2];
	int nWH[2];
	if (vLayerTopo[nWallIdx].nUnitAngle == 0)
	{
		nWH[0] = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; // �������� ��ϵͳ���õ�
		nWH[1] = vPlaneDraw[nWallIdx].nWH[1] + 10; // ������� ֮ǰû���� ����Ϊǽ��+10
		//ǽ�����ĵ�
		//ǰһ�����ӵ����ĵ�+����/2 �õ�ǽ�����½� + �ŵ����λ�� + ��Ԫ�ص����ĵ�
		int nPillarCen = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[0]].nCen[0];//ǰһ�����ӵ����ĵ�
		int nPillarWH = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[0]].nWH[0] / 2;///ǰһ�����ӿ���/2
		int doorW = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2; //��Ԫ�ص����ĵ�
		int doorPosition = vLayerTopo[nUnitIdx].nCenPos[0]; //��׼��ķ���λ��
		nCen[0] = nPillarCen + nPillarWH + doorPosition + doorW;
		nCen[1] = vPlaneDraw[nWallIdx].nCen[1];
	}
	// ��ֱǽ
	else
	{
		nWH[0] = vPlaneDraw[nWallIdx].nWH[0] + 10; // ������� ֮ǰû���� ����Ϊǽ��+10
		nWH[1] = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0]; // �������� ��ϵͳ���õ�

		//ǽ�����ĵ�λ��
		//�ڶ������ӵ����ĵ�Y-�߶�/2 �õ�ǽ����λ�� - �ŵ����λ�� - ��Ԫ�ص����ĵ�
		nCen[0] = vPlaneDraw[nWallIdx].nCen[0];

		int nPillarCen = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[1]].nCen[1];//�ڶ�һ�����ӵ����ĵ�Y
		int nPillarWH = vPlaneDraw[vLayerTopo[nWallIdx].nAdjUnitIdx[1]].nWH[1] / 2;///�ڶ���һ�����Ӹ߶�/2
		int doorPosition = vLayerTopo[nUnitIdx].nCenPos[0];//�ŵ����λ��
		int doorW = table[vLayerTopo[nUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2;//��Ԫ�ص����ĵ�

		nCen[1] = nPillarCen - nPillarWH - doorPosition - doorW;
	}
	SimpleShape oDoorWndShape;
	GenRectShape(nCen, nWH, oDoorWndShape);

	oDoorWndShape.unitType = vLayerTopo[nUnitIdx].nUnitType;
	oDoorWndShape.unitIdx = nUnitIdx;
	// �����Ŵ���״��Ϣ
	vPlaneDraw[nUnitIdx] = oDoorWndShape;

	return 0;
}

// ���˽ṹ����תΪ��ʵ���� ֻ���Ǿ��ν���
int DimDataConvert::CalPlaneData(VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	int nRe = 0;
	// ֻ������ ǽ �Ŵ�
	int nUnitNum = vLayerTopo.size();
	int i = 0;
	SimpleShape oShape;
	// �ȴ�����
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
	// �������ӻ����ϼ���ǽ
	for (i = 0; i < nUnitNum; i++)
	{
		oCurUnit = vLayerTopo[i];
		if (oCurUnit.nUnitType == 4)
		{
			CalWallShape(i, vLayerTopo, vPlaneDraw, table);
		}
	}

	// ����ǽ�����ϼ����Ŵ�
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

// ƽ��ͼ����
// ���������� ��ɾ�Ĳ� 
// ƽ��ͼ�п�ѡ�� ǽ�Ŵ� ����ͼ�п�ѡ�����Ŵ�

// ��ǽ�ϲ����Ŵ�
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
	// ���ݲ�����Ϣ��ɲ���
	if (oInsUnit.nUnitAngle == 0)
	{
		//oInsUnit.nCenPos[2] = vPlaneDraw[nInsWallIdx].nCen[1];
		int nWallLeft = vPlaneDraw[nInsWallIdx].nCen[0] - vPlaneDraw[nInsWallIdx].nWH[0] / 2;
		oInsUnit.nCenPos[0] = abs(oInsPos.nXY[0] - nWallLeft);

		//�Ŵ�����ʼ��߶���д��
		if (oInsUnit.nUnitType == 5){oInsUnit.nCenPos[1] = 0;}
		if (oInsUnit.nUnitType == 6) { oInsUnit.nCenPos[1] = 80; }
		
	}
	else if (oInsUnit.nUnitAngle == 90)
	{
		//oInsUnit.nCenPos[0] = vPlaneDraw[nInsWallIdx].nCen[0];
		int nWallBottom = vPlaneDraw[nInsWallIdx].nCen[1] + vPlaneDraw[nInsWallIdx].nWH[1] / 2;
		//oInsUnit.nCenPos[2] = oInsPos.nXY[0] - nWallBottom;
		oInsUnit.nCenPos[0] = abs(oInsPos.nXY[1] - nWallBottom);
		//�Ŵ�����ʼ��߶���д��
		if (oInsUnit.nUnitType == 5) { oInsUnit.nCenPos[1] = 0; }
		if (oInsUnit.nUnitType == 6) { oInsUnit.nCenPos[1] = 80; }

	}
	// �������˽ṹ
	oInsUnit.nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo.push_back(oInsUnit);
	return 0;
}

//��ǽ��������
int AddBeamOnWall(BasicUnit oBeam, int nInsWallIdx, VUNITTABLE vTable, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	// �������������˵�Ԫ
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oBeam.nUnitType;
	//oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nCenUnitIdx = oBeam.nUnitIdx;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	oInsUnit.nTopoIdx = vLayerTopo.size();
	for (int i= 0; i < 12; i++) { oInsUnit.nAdjUnitIdx[i]=-1; }

	for (int i = 0; i < 4; i++) { oInsUnit.nCenPos[i]=0; }
	oInsUnit.nCenPos[0] = vLayerTopo[nInsWallIdx].nCenPos[0];
	oInsUnit.nCenPos[1] = vLayerTopo[nInsWallIdx].nCenPos[1] * 2 + (vTable[oInsUnit.nCenUnitIdx].oShape.nShapeRange[3]- vTable[oInsUnit.nCenUnitIdx].oShape.nShapeRange[1])/2;
	oInsUnit.nCenPos[2] = vLayerTopo[nInsWallIdx].nCenPos[2];

	//�������������˹�ϵ
	int nColIdx[2];
	calColumnsIdxFromWallIdx(nInsWallIdx, nColIdx, vLayerTopo);
	oInsUnit.nAdjUnitIdx[0] = nColIdx[0];
	oInsUnit.nAdjUnitIdx[1] = nColIdx[1];
	oInsUnit.nAdjUnitIdx[2] = nInsWallIdx;

	//���¾�ǽ�����˹�ϵ
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

// ���������������� ǽ �� �����ڽӹ�ϵ
int UpdateNewAdj(int nNewAdj, int nAllOldAdj[12], int nAllNewAdj[12], VTOPOTABLE const& vLayerTopo, int nNewType)
{
	int i = 0;
	bool bfirst = true;
	for (i = 0; i < 12; i++)
	{
		if (nAllOldAdj[i] != -1 && nAllNewAdj[i]!=-1)
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
// ���������������� ǽ֮����ڽӹ�ϵ
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
// ��ǽ�ϲ������� ������
int AddColInWall(BasicUnit oCol, int nInsWallIdx, PixelPos oInsPos, VUNITTABLE vTable, VTOPOTABLE& vLayerTopo, VSHAPE & vPlaneDraw)
{
	//vTable.push_back(oCol);

	// ���������ӵ����˵�Ԫ
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oCol.nUnitType;
	//oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nCenUnitIdx = oCol.nUnitIdx;
	oInsUnit.nUnitAngle = vLayerTopo[nInsWallIdx].nUnitAngle;
	oInsUnit.nTopoIdx = vLayerTopo.size();
	//���ӵ����ĵ��µ���Ϣ �����ƣ�һ���yֵΪ0
	oInsUnit.nCenPos[0] = 0, oInsUnit.nCenPos[1] = 0, oInsUnit.nCenPos[2] = 0, oInsUnit.nCenPos[3] = 0;

	for (int i = 0; i < 12; i++) { oInsUnit.nAdjUnitIdx[i]=-1; }
	// ���ݲ�����Ϣ��ɲ���
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
	// ������ �������˽ṹ
	oInsUnit.nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo.push_back(oInsUnit);
	int nNewColIdx = vLayerTopo.size() - 1;
	// ǽ����Ϊ����ǽ
	// �ȸ���һ��
	TopoUnit oAddWall = vLayerTopo[nInsWallIdx];
	vLayerTopo.push_back(oAddWall);
	int nNewWallIdx = vLayerTopo.size() - 1;
	// ������Ϊ������
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
		vLayerTopo.push_back(oBeamUnit);
		nNewBeamIdx = vLayerTopo.size() - 1;
	}
	// ���������˽ṹ
	vLayerTopo[nNewColIdx].nAdjUnitIdx[0] = nInsWallIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[1] = nNewWallIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[2] = nBeamIdx;
	vLayerTopo[nNewColIdx].nAdjUnitIdx[3] = nNewBeamIdx;
	// ����ǽ���˽ṹ
	UpdateNewAdj(nNewColIdx, vLayerTopo[nInsWallIdx].nAdjUnitIdx, vLayerTopo[nNewWallIdx].nAdjUnitIdx, vLayerTopo, 1);
	UpdateNewAdj(nNewBeamIdx, vLayerTopo[nNewWallIdx].nAdjUnitIdx, vLayerTopo, 2);

	// ���������˽ṹ
	UpdateNewAdj(nNewColIdx, vLayerTopo[nBeamIdx].nAdjUnitIdx, vLayerTopo[nNewBeamIdx].nAdjUnitIdx, vLayerTopo, 1);
	UpdateNewAdj(nNewWallIdx, vLayerTopo[nNewBeamIdx].nAdjUnitIdx, vLayerTopo, 4);

	return 0;
}

// ����������֮�����ǽ ��������֮�����Ϊ�� 
int AddWallInCols(BasicUnit oAddUnit, int nAdjCol[2], VUNITTABLE vTable,VTOPOTABLE& vLayerTopo)
{
	// ���ݲ�����Ϣ��ɲ���
	TopoUnit oInsUnit;
	oInsUnit.nUnitType = oAddUnit.nUnitType;
	oInsUnit.nCenUnitIdx = vTable.size() - 1;
	oInsUnit.nCenUnitIdx = oAddUnit.nUnitIdx;

	if (vLayerTopo[nAdjCol[0]].nCenPos[2] == vLayerTopo[nAdjCol[1]].nCenPos[2])
	{
		oInsUnit.nUnitAngle = 0;
	}
	else
	{
		oInsUnit.nUnitAngle = 90;
	}
	oInsUnit.nTopoIdx = vLayerTopo.size();
	for (int i = 0; i < 12; i++){oInsUnit.nAdjUnitIdx[i] = -1;}
	oInsUnit.nAdjUnitIdx[0] = nAdjCol[0];
	oInsUnit.nAdjUnitIdx[1] = nAdjCol[1];
	

	// �������˽ṹ
	vLayerTopo.push_back(oInsUnit);
	return 0;
}

// �ڵ�ǰ����ͼ�����ӹ���
int DimDataConvert::AddBaseUnit(BasicUnit oAddUnit, PixelPos oInsPos, VUNITTABLE& vTable, VTOPOTABLE& vLayerTopo, VSHAPE vPlaneDraw)
{
	int nRe = 0;
	int nInsWallIdx=-1;//ѡ�񵽵�ǽ��ID
	int nAdjCol[2];		// ��������һ��Ϊ������ [0]Ϊ-1
	
	// ���� ������

	//������ǽ���ӵ�������ID,�����ʱѡ��Ĳ���ǽ����nInsWallIdxΪ-1
	if (vLayerTopo[selectUnitIdx].nUnitType == 4)
	{
		nInsWallIdx = selectUnitIdx;
		calColumnsIdxFromWallIdx(nInsWallIdx, nAdjCol, vLayerTopo);
	}
	


	if (nInsWallIdx < 0 && nAdjCol[0] < 0)
		return 1;
	// �жϴ����빹���Ƿ����Ŵ�
	if (oAddUnit.nUnitType > 4)
	{
		nRe = AddDoorWndInWall(oAddUnit, nInsWallIdx, oInsPos, vTable, vLayerTopo, vPlaneDraw);
	}
		

	// ��������
	else if (oAddUnit.nUnitType == 1)
	{
		nRe = AddColInWall(oAddUnit, nInsWallIdx, oInsPos, vTable, vLayerTopo, vPlaneDraw);
	}
	// ������
	else if (oAddUnit.nUnitType == 2)
	{
		nRe = AddBeamOnWall(oAddUnit, nInsWallIdx, vTable,vLayerTopo, vPlaneDraw);
	}
	// �����
	else if (oAddUnit.nUnitType == 2)
	{
		//nRe = AddBoard(oAddUnit, nInsWallIdx, vTable, vLayerTopo, vPlaneDraw);;
	}

	// ����ǽ
	else if (oAddUnit.nUnitType == 4 && nAdjCol[0] >= 0)
	{
		//��ȡ������ID
		for (int i = 0; i < vLayerTopo.size(); i++)
		{
			for (int j = 0; j < vLayerTopo.size(); j++)
			{
				//����Ϊ���ӣ����жϲ�����Ƿ�����������֮��
				if (j != i && vLayerTopo[i].nUnitType == 1 && vLayerTopo[j].nUnitType == 1)
				{
					//0��
					if (vLayerTopo[i].nCenPos[2] == vLayerTopo[j].nCenPos[2])
					{
						if ((oInsPos.nXY[0]< vLayerTopo[i].nCenPos[0] && oInsPos.nXY[0] > vLayerTopo[j].nCenPos[0])
							|| (oInsPos.nXY[0]< vLayerTopo[j].nCenPos[0] && oInsPos.nXY[0] > vLayerTopo[i].nCenPos[0]))
						{
							nAdjCol[0] = vLayerTopo[i].nTopoIdx, nAdjCol[1] = vLayerTopo[j].nTopoIdx;
							break;
						}
						
					}
					//90��
					if (vLayerTopo[i].nCenPos[0] == vLayerTopo[j].nCenPos[0])
					{
						if ((oInsPos.nXY[1]< vLayerTopo[i].nCenPos[2] && oInsPos.nXY[1] > vLayerTopo[j].nCenPos[2])
							|| (oInsPos.nXY[1]< vLayerTopo[j].nCenPos[2] && oInsPos.nXY[1] > vLayerTopo[i].nCenPos[2]))
						{
							nAdjCol[0] = vLayerTopo[i].nTopoIdx, nAdjCol[1] = vLayerTopo[j].nTopoIdx;
							break;
						}
					}
				}
			}
		}

		nRe = AddWallInCols(oAddUnit, nAdjCol, vTable, vLayerTopo);
	}
		


	// ���������˽ṹ���¼��㼸����ֵ

	return 0;
}

// �������� ����ɾ��
int CleanTopoTable(VTOPOTABLE& vLayerTopp)
{
	return 0;
}

// ���ݵ��λ��ȷ��ѡ��Ĺ��� ������
int GetSelUnitIdx(PixelPos oInsPos, int& nUnitIdx, VTOPOTABLE& vLayerTopp)
{
	return 0;
}
// �ڵ�ǰ����ͼ��ɾ������
int DimDataConvert::DelBaseUnit(int nSelUnitIdx,VTOPOTABLE& vLayerTopp, int nRealDel)
{
	int nRe = 0;
	// ���ݵ��λ��ȷ��ѡ��Ĺ���
	//nRe = GetSelUnitIdx(oInsPos, nSelUnitIdx, vLayerTopp);
	if (nRe != 0 || nSelUnitIdx < 0)
		return nRe;
	// �ӹ����Ĺ�����ɾ��������ϵ ��ɾ�� ���ݻ��� ��������

	// ɾ����־��Ϊ1 
	vLayerTopp[nSelUnitIdx].nStatusFlag = 1;

	// �������� ����ɾ��
	nRe = CleanTopoTable(vLayerTopp);

	return 0;
}

// �ڵ�ǰ����ͼ���޸Ĺ�������
// ����ͬ�๹���Ĳ�ͬ������ ���� ���� 
int ModifyBaseUnitProp(void* pValue, VTOPOTABLE& vLayerTopp)
{
	return 0;
}

// �ƶ��Ŵ�
int MovDoorWndInWall(int nMoveUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	// �Ŵ�ֻ����ǽ��Χ����ǽ�����ƶ�
	// �õ������Ŵ���ǽ
	int nInsWallIdx = vLayerTopo[nMoveUnitIdx].nAdjUnitIdx[0];

	// �����ƶ����� �ƶ�����һ����ֵ��Ϊ0
	if (nMoveX != 0 && vLayerTopo[nInsWallIdx].nUnitAngle == 0)
	{
		// �����Ŵ��ƶ��ĺϹ淶Χ nCenPos[0]������λ��
		//����С��0;
		//���ܴ��ډ��Č���-Ԫ�ص������c��;
		//����������ڼ���ֵ

		int nLeft = 0;


		int doorW = table[vLayerTopo[nMoveUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2; //Ԫ�ص����ĵ�
		int nWallWH = vPlaneDraw[vLayerTopo[nInsWallIdx].nTopoIdx].nWH[0];					//���Č���
		int nRight = vPlaneDraw[vLayerTopo[nInsWallIdx].nTopoIdx].nWH[0] - doorW;


		// ��ǽ��������Ŵ�λ��
		int nNewX = vLayerTopo[nMoveUnitIdx].nCenPos[0] + nMoveX;

		if (nNewX < nLeft)
			nNewX = nLeft;

		else if (nNewX > nRight)
			nNewX = nRight;
		vLayerTopo[nMoveUnitIdx].nCenPos[0] = nNewX;
	}
	else if (nMoveY != 0 && vLayerTopo[nInsWallIdx].nUnitAngle == 90)
	{

		// �����Ŵ��ƶ��ĺϹ淶Χ nCenPos[0]������λ��
		//����С��0;
		//���ܴ��ډ��ĸ߶�-Ԫ�ص������cY - ���ӵĿ���/2;
		//(vPlaneDraw[(vLayerTopo[(vLayerTopo[24]).nAdjUnitIdx[0]]).nAdjUnitIdx[0]]).nWH/2
		int nBottom = 0;
		int doorW = table[vLayerTopo[nMoveUnitIdx].nCenUnitIdx].oShape.nShapeRange[0] / 2; //Ԫ�ص����ĵ�
		int PillarH = (vPlaneDraw[(vLayerTopo[(vLayerTopo[nMoveUnitIdx]).nAdjUnitIdx[0]]).nAdjUnitIdx[0]]).nWH[1] / 2; //���ӵĸ߶� / 2;
		int nUp = vPlaneDraw[nInsWallIdx].nWH[1] - doorW- PillarH;
		// ��ǽ��������Ŵ�λ��
		int nNewY = vLayerTopo[nMoveUnitIdx].nCenPos[1] + (nMoveY * -1) ;
		if (nNewY < nBottom)
			nNewY = nBottom;
		else if (nNewY > nUp)
			nNewY = nUp;

		 
		vLayerTopo[nMoveUnitIdx].nCenPos[0] = nNewY;
	}

	return 0;
}

// �õ�ǽ���������� �Լ� ���ӹ���������ǽ
// ����Ϊ����vector vector�е�һ��Ϊ����Idx ����Ϊ�����ӵĹ���ǽ
int GetColAdjWall(int nSelWallIdx, VTOPOTABLE const& vLayerTopo, VVINT& vvAdjWall)
{
	// �õ�ǽ���˵���������
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

	// �õ����ӹ�����ǽ
	int nColNum = 2;
	VINT vColAndWall;
	vvAdjWall.clear();
	for (i = 0; i < nColNum; i++)
	{
		vColAndWall.clear();
		vColAndWall.push_back(nAdjColIdx[i]);
		TopoUnit oColUnit = vLayerTopo[nAdjColIdx[i]];
		// ÿ���������12���ڽӵ�Ԫ
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
// �ƶ����� ��PreMoveWall�������ƶ�
int MovCol(int nMoveUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo)
{
	// ֻ�ƶ�����
	vLayerTopo[nMoveUnitIdx].nCenPos[0] += nMoveX;
	vLayerTopo[nMoveUnitIdx].nCenPos[2] += nMoveY;

	return 0;
}

// �������ε������ܷ�Χ
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
// ����ǽ�ĺϹ淶Χ
int CalWallMoveRange(int nWallIdx, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, int nAdjCol[2], int nXYMove[2][2])
{
	int nRe = 0;
	// ǽֻ���ش�ֱǽ�����ƶ� ���ƶ������ʼȫΪ0
	memset(nXYMove, 0, 4 * sizeof(int));

	// ǽ���˱���������
	// �õ����ӹ���������ǽ
	VVINT vvAdjColWall;
	nRe = GetColAdjWall(nWallIdx, vLayerTopo, vvAdjColWall);
	nAdjCol[0] = vvAdjColWall[0][0];
	nAdjCol[1] = vvAdjColWall[1][0];
	// �õ����ƶ�ǽ�ķ�Χ
	int nWallRange[2][2];
	CalPolyFrm(vPlaneDraw[nWallIdx].vCorner, nWallRange);

	// �õ����й���ǽ�ķ�Χ
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
	// ˮƽǽ ��ֱ�ƶ���Χ
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
	// ��ֱǽ ˮƽ�ƶ���Χ
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
// �ƶ�ǽ
int MovWall(int nMoveUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw)
{
	int nRe = 0;
	// ǽֻ���ش�ֱǽ�����ƶ�
	// ǽ���˱���������
	// �õ����ӹ���������ǽ
	int nAdjCol[2];
	int nXYMoveRange[2][2];
	// 	// ����ǽ�ĺϹ淶Χ
	nRe = CalWallMoveRange(nMoveUnitIdx, vLayerTopo, vPlaneDraw, nAdjCol, nXYMoveRange);

	// ��ǽ��ֱ��������ƶ�����
	if (nMoveX <= nXYMoveRange[0][0] || nMoveX >= nXYMoveRange[0][1])
		nMoveX = 0;
	if (nMoveY <= nXYMoveRange[1][0] || nMoveY >= nXYMoveRange[1][1])
		nMoveY = 0;
	// �ƶ���������
	int nColNum = 2;
	int i = 0;
	for (i = 0; i < nColNum; i++)
	{
		MovCol(nAdjCol[i], nMoveX, nMoveY, vLayerTopo);
	}
	// ���¼��㼸����ֵ

	return 0;
}
// ƽ��ͼ���ƶ�ѡ�е�Ԫ����
int DimDataConvert::MoveBaseUnit(int nSelUnitIdx, int nMoveX, int nMoveY, VTOPOTABLE& vLayerTopo, VSHAPE& vPlaneDraw, VUNITTABLE table)
{
	int nRe = 0;
	// ƽ��ͼ�� ǽ�Ŵ��ɶ� �ж��Ƿ���ǽ�Ŵ�
	int nUnitType = vLayerTopo[nSelUnitIdx].nUnitType;
	// �жϴ��ƶ������Ƿ����Ŵ�
	if (nUnitType > 4)
		nRe = MovDoorWndInWall(nSelUnitIdx,  nMoveX,  nMoveY, vLayerTopo, vPlaneDraw, table);

	// �ƶ�ǽ
	else if (nUnitType == 4)
		nRe = MovWall(nSelUnitIdx,  nMoveX,  nMoveY, vLayerTopo, vPlaneDraw);

	return 0;
}

