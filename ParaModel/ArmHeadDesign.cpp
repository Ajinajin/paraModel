#include <QFile>
#include <QTextStream>

#include "ArmHeadDesign.h"

CArmHeadDesign::CArmHeadDesign()
{

}

CArmHeadDesign::CArmHeadDesign(ArmHeadTopo oData)
{
	// 深度拷贝
	oDesignData.nArmHeadIdx = oData.nArmHeadIdx; 
	oDesignData.nArrangeType = oData.nArrangeType; 
	oDesignData.lstEdgeDes = oData.lstEdgeDes; 
	oDesignData.mapArmHead = oData.mapArmHead; 
	oDesignData.oFragInfo = oData.oFragInfo; 
	oDesignData.sArmHeadName = oData.sArmHeadName; 
	oDesignData.sArmHeadVersion = oData.sArmHeadVersion; 
	oDesignData.vFragCenPos = oData.vFragCenPos; 
	oDesignData.vFragGesture = oData.vFragGesture; 
	oDesignData.vCtrlPts = oData.vCtrlPts;
}

CArmHeadDesign::~CArmHeadDesign()
{

}

////////////////////////////////////////////////////////////////////
// IO操作
////////////////////////////////////////////////////////////////////

PARADES CArmHeadDesign::FindMapArmHead(ArmHeadTopo& vArmHeadTopo, QString Code)
{
	for (size_t i = 0; i < vArmHeadTopo.mapArmHead.size(); i++)
	{
		if (vArmHeadTopo.mapArmHead[i].sUnitName.contains(Code, Qt::CaseSensitive))
		{
			return vArmHeadTopo.mapArmHead[i];
		}
	}

	PARADES oRe;
	memset(&oRe, 0, sizeof(oRe));
	return oRe;
}
// 基于图纸的战斗部各参数
int CArmHeadDesign::GetArmHeadPara(ArmHeadTopo& vArmHeadTopo)
{
	memset(fR, 0, 10 * sizeof(float));
	memset(fT, 0, 10 * sizeof(float));
	memset(fH, 0, 10 * sizeof(float));
	fR[1] = FindMapArmHead(vArmHeadTopo, "R1").nUnitPropty;
	fR[2] = FindMapArmHead(vArmHeadTopo, "R2").nUnitPropty;
	fR[4] = FindMapArmHead(vArmHeadTopo, "R4").nUnitPropty;
	fR[5] = FindMapArmHead(vArmHeadTopo, "R5").nUnitPropty;
	fR[6] = FindMapArmHead(vArmHeadTopo, "R6").nUnitPropty;
	fR[7] = FindMapArmHead(vArmHeadTopo, "R7").nUnitPropty;
	fR[8] = FindMapArmHead(vArmHeadTopo, "R8").nUnitPropty;
	fR[9] = FindMapArmHead(vArmHeadTopo, "R9").nUnitPropty;
	fT[1] = FindMapArmHead(vArmHeadTopo, "T1").nUnitPropty;
	fT[2] = FindMapArmHead(vArmHeadTopo, "T2").nUnitPropty;
	fT[4] = FindMapArmHead(vArmHeadTopo, "T4").nUnitPropty;
	fT[5] = FindMapArmHead(vArmHeadTopo, "T5").nUnitPropty;
	fT[8] = FindMapArmHead(vArmHeadTopo, "T8").nUnitPropty;
	fT[9] = FindMapArmHead(vArmHeadTopo, "T9").nUnitPropty;
	fH[1] = FindMapArmHead(vArmHeadTopo, "H1").nUnitPropty;
	fH[4] = FindMapArmHead(vArmHeadTopo, "H4").nUnitPropty;
	fH[5] = FindMapArmHead(vArmHeadTopo, "H5").nUnitPropty;

	lstUpDrawCtrlPts = vArmHeadTopo.vCtrlPts;

	return 0; 
}

////////////////////////////////////////////////////////////////////
// 读入文件数据
////////////////////////////////////////////////////////////////////
int CArmHeadDesign::ReadArmHeadInfo(QString sFile)
{
	// 暂不考虑中文
	QString Path = QString("d:/B.txt");
	QFile file(Path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
// 		QMessageBox::information(NULL, "信息提示", "系统战斗部文件解析失败！");
// 		MyLogOutput("系统战斗部文件解析失败！");
		return 0;
	}
	// 清理原来数据
	oDesignData.lstEdgeDes.clear(); 
	oDesignData.mapArmHead.clear(); 
	oDesignData.vCtrlPts.clear(); 
	oDesignData.vFragCenPos.clear(); 
	oDesignData.vFragGesture.clear(); 

	QList<QPointF> lstPoint;
	LSTEDGEDES lstEdges; 
	QTextStream readStream(&file);
	QString versionStr = "";
	QString sName = "";
	vector<PARADES> plist;
	QPointF oAxis[2]; 
	int nFragNum; 
	bool bName = false; 
	bool bPart = false; 
	bool bCorner = false;
	bool bEdge = false; 
	bool bFragNum = false; 
	bool bFragType = false; 
	int nAxis = 2; 
	FragDes oFragDes; 
	memset(&oFragDes, 0, sizeof(oFragDes)); 

	while (!readStream.atEnd()) 
	{
		QString content = readStream.readLine();
		// 去除字符串前后空格
		content = content.trimmed(); 
		QStringList list = content.split(' ');
		bool verify;
		QString str = list[0];
		if (list[0].toLower() == "*version")
		{
			versionStr = list[1];
			continue;
		}
		if (nAxis < 2)
		{
			oAxis[nAxis] = QPointF(list[1].toFloat(), list[2].toFloat());
			nAxis += 1; 
			continue;
		}

		if( bPart )
		{
			int  locationX = str.toInt(&verify);
			if (!verify)
			{
				bPart = false;
				continue;
			}
			PARADES p;
			p.sUnitName = QString(list[1]);
			p.nUnitPropty = list[2].toFloat();
			plist.push_back(p);
			continue;
		}
		if (bCorner)
		{
			int  locationX = str.toInt(&verify);
			if (!verify)
			{
				bCorner = false;
				continue;
			}
			QPointF oPt(list[1].toFloat()-oAxis[0].x(),list[2].toFloat()-oAxis[0].y());
			lstPoint.push_back(oPt); 
			continue;
		}
		if (bEdge)
		{
			int  locationX = str.toInt(&verify);
			if (!verify)
			{
				bEdge = false;
				continue;
			}
			EdgeDes oEdge; 
			oEdge.nEdgeType = list[1].toInt(); 
			for (size_t j = 2; j < list.size() ; j++)
			{
				int nPtIdx = list[j].toInt(); 
				oEdge.vCtrlPtIdx.push_back(nPtIdx); 
				oEdge.lstCtrlPts.push_back(lstPoint[nPtIdx]); 
			}
			lstEdges.push_back(oEdge);
			continue;
		}
		if (bFragNum)
		{
			nFragNum = list[1].toInt();
			bFragNum = false;
			continue;
		}
		if (bFragType)
		{
			oFragDes.nFragType = list[1].toInt();
			oFragDes.fFragPara[0] = list[2].toFloat();
			if (oFragDes.nFragType >= 1)
				oFragDes.fFragPara[1] = list[3].toFloat(); 
			if( oFragDes.nFragType >= 2 )
				oFragDes.fFragPara[2] = list[4].toFloat(); 
			bFragType = false;
			continue;
		}

		QString sTmp = QString::fromLocal8Bit("坐标轴线"); 
		if (list[0].contains(sTmp))
		{
			nAxis = 0;
			continue;
		}
		sTmp = QString::fromLocal8Bit("名称");
		if ( list[0].contains(sTmp) )
		{
			sName = list[1]; 
			continue;
		}
		sTmp = QString::fromLocal8Bit("构件序号");
		if (list[0].contains(sTmp))
		{
			bPart = true;
			continue;
		}
		sTmp = QString::fromLocal8Bit("拐点序号");
		if (list[0].contains(sTmp))
		{
			bCorner = true;
			continue;
		}
		sTmp = QString::fromLocal8Bit("区段序号");
		if (list[0].contains(sTmp))
		{
			bEdge = true;
			continue;
		}
		sTmp = QString::fromLocal8Bit("破片数量");
		if (list[0].contains(sTmp))
		{
			bFragNum = true;
			continue;
		}
		sTmp = QString::fromLocal8Bit("破片类型");
		if (list[0].contains(sTmp))
		{
			bFragType = true;
			continue;
		}

	}
	oDesignData.nArmHeadIdx = 0;
	oDesignData.nArrangeType = 0;
	oDesignData.nFragNum = nFragNum;
	oDesignData.sArmHeadName = sName;
	oDesignData.oFragInfo = oFragDes;
	oDesignData.sArmHeadVersion = versionStr;
	oDesignData.vCtrlPts = lstPoint;
	oDesignData.lstEdgeDes = lstEdges; 
	oDesignData.mapArmHead = plist;

	return 0; 
}

////////////////////////////////////////////////////////////////////
// 保存数据到文件
////////////////////////////////////////////////////////////////////
int CArmHeadDesign::WriteArmHeadInfo(QString sFile)
{
	return 0; 
}

////////////////////////////////////////////////////////////////////
// 编辑参数
////////////////////////////////////////////////////////////////////

// 编辑R参数
int CArmHeadDesign::ModifyRPara(float fNewR, int nRIdx)
{
	return 0 ;
}
// 编辑T参数
int CArmHeadDesign::ModifyHPara(float fNewT, int nTIdx)
{
	return 0 ;
}
// 编辑H参数
int CArmHeadDesign::ModifyTPara(float fNewH, int nHIdx)
{
	return 0;
}

////////////////////////////////////////////////////////////////////
// 编辑外壳控制点
////////////////////////////////////////////////////////////////////
int CArmHeadDesign::AddCtrlPts(QPoint oAddPt, int nInsIdx)
{
	// 添加控制点

	// 更新点所属区域关系 
	return 0;
}
int CArmHeadDesign::DelCtrlPts(int nDelIdx)
{
	return 0;
}

