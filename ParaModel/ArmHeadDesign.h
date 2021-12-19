#pragma once

#include "ParaType.h"

#include <QObject>

class CArmHeadDesign : public QObject
{
public:
	CArmHeadDesign();
	CArmHeadDesign(ArmHeadTopo oData);
	~CArmHeadDesign();
// ս������������ �����±�1��ʼ���� ��Ӧͼֽ����سߴ� ��λΪmm
private:
	float fR[20];						// ���ͼֽ��fR����
	float fT[10];						// ���ͼֽ��fT����
	float fH[10]; 						// ���ͼֽ��fH����
// ������� �������
public:
	LISTPT lstUpDesignCtrlPts;			// �Ϸ����������/�յ� ��Ļ��ʾ����
	ArmHeadTopo oDesignData;			// ���ͼֽ����
// �������� ��Ļ����
public:
	QList<QPointF> lstUpDrawCtrlPts;	// �Ϸ����������/�յ� ��Ļ��ʾ����
	VEDGESHAPE vDrawShape;			// ����ս�������λ�������ߴ�  ���߼������
	VEDGESHAPE vDesignShape;		// ս��������ԭʼ����ߴ�  ����Ϊ���Ƶ��ʾ
	LISTPT lstUpCurvePts;			// ��������ϵĵ�
	VECLSTPT vlstCurvePt;			// 0-7 ���ݴӴ�С8�������ϵĵ�
	VECLSTPT vlstPartPt;			// 0-4 5�����������㼯
	VECLSTPT vlstDrawPartPt;		// 0-4 5�����������㼯
	LISTPT   lstLinkFrontPt;		// ��ǰ�����ӵ�
	LISTPT   lstLinkBackPt;			// �ͺ�����ӵ�

// ���ݶ�д����
public:
	PARADES FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code);
	// ����ͼֽ��ս����������
	int GetArmHeadPara(ArmHeadTopo& vArmHeadTopo);

	int ReadArmHeadInfo(QString sFile); 
	int WriteArmHeadInfo(QString sFile); 
// ���ݱ༭���� RTH�����༭
public:
	bool bParaCanEdit;							// �����Ƿ�ɱ༭
	int ModifyRPara(float fNewR,int nRIdx);		// �༭R����
	int ModifyHPara(float fNewT,int nTIdx);		// �༭T����
	int ModifyTPara(float fNewH,int nHIdx);		// �༭H����
// ��Ǳ༭����
public:
	bool bEdgeCanEdit;					// ����Ƿ�ɱ༭
	VINT vCanEditFlag;					// ��ǿ��Ƶ�ɱ༭��־ 0 ���ɱ༭ 1 �ɱ༭
	// Ĭ�����ߺ�Բ�����ɱ༭ ֻ�ɽ���ʽ�޸����߶�
	int AddCtrlPts(QPoint oAddPt, int nInsIdx); 
	int DelCtrlPts(int nDelIdx); 
private:

};

