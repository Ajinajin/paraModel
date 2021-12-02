#pragma once
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

//#include "GLWidget.h"
#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include "ui_ParaModel.h"
#include "GLWidget.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <vector>
#include <qlistview.h>
#include <qlistwidget.h>
#include <qmenu.h>
#include <qlayout.h>

#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"

#include "paraoglmanager.h"
#include "SARibbonMainWindow.h"
#include <ParaType.h>


class QMenu;
class QListWidget;


class SARibbonCategory;
class ParaModel : public SARibbonMainWindow
{
	Q_OBJECT

		//���캯��
public:
	ParaModel(QWidget* parent = 0);

private:
	QStatusBar* winStatus;							//״̬��
	QLabel* pTipBar;								// ״̬����������ʾ
	QLineEdit* pModelEdit[7];						// ս�������Ա༭�� ���� ��Ƭ���� ��Ƭ���� �ֲ��Ƕ�1 �ֲ��Ƕ�2
	QLineEdit* pArmHeadEdit[5];						// �㷨��Ϣ
	QTreeWidget* pModelTreeWidget;					// ϵͳģ����
	QDockWidget* MainDockWidget;					// ������ʾ����
	BQGraphicsView* graphicsViewX;					// ��άģ��X��ͼ
	BQGraphicsView* graphicsViewY;					// ��άģ��Y��ͼ
	BQGraphicsView* graphicsViewZ;					// ��άģ��Z��ͼ
	BQGraphicsView* graphicsViewMain;				// ��άģ����ά��ͼ
	BQGraphicsView* graphicsViewOgl;				// ��λģ��չʾ��ͼ

	BQGraphicsScene pSceneX;						// ��άģ��X��ͼ����
	BQGraphicsScene pSceneY;						// ��άģ��Y��ͼ����
	BQGraphicsScene pSceneZ;						// ��άģ��Z��ͼ����
	BQGraphicsScene pSceneMain;						// ��άģ��Z��ͼ����Ļ������
	
	ParaOGLManager* paraOglmanager;					// ��ά��ʾ������
	ParaOGLManager* paraOglmanagerMain;				// ��ά��ʾ���ڴ���Ļ����

	int if_data;									//0��δ�������ݡ�1��������
	SysPath oPath;									//ϵͳ·��
	int pSceneOffset;

	QTextEdit* myLogOutLabel;						// ��־����������ı�
	int MainDockState;								// 0��X��ͼ 1��Y��ͼ 2��Z��ͼ 3����ά��ͼ

    // mainLabel��ʾ��ͼ��
    QMenu* popMenu_In_ListWidget_;					/*�����˵���ʹ���޷�ɾ��*/  

public:
	VUNITTABLE vBaseUnit;					// ϵͳ����������
	VTOPOTABLE vModelTmpl;					// ϵͳƽ��ͼ��
	int InitPath();							// ��ʼ��·��
	int InitUnitLib();						// ��ʼ������������ 
	int InitPlaneDrawLib();					// ��ʼ��ƽ��ͼ��
	int InitParaTmpl();						// ��ʼ������������ģ��


private:
	//��ʼ������
	void InitWindow();
	//��ʼ����������
	void InitCentralWidget();
	//��ʼ���м��������
	void InitOglManagerWidget(QDockWidget* from);
	//��ʼ����־����
	void InitLogWidget(QDockWidget* from);
	//��ʼ���Ѽ��ص�ģ�ʹ���
	void InitLoadModelWidget(QDockWidget* from);
	//��ʼ��ϵͳģ�ʹ���
	void InitSysWidget(QDockWidget* from);
	//��ʼ�����Դ���
	void InitPropertyWidget(QDockWidget* from);

	//��ʼ��״̬��������ʾ
	void InitStatusWidget();

	//��ʼ��Ribbon
	void InitSARibbon();
	//��ʼ��Ribbon�е��ļ��˵�
	void InitCategoryMain(SARibbonCategory* page);

	//��ʼ����������
	void InitTipWindow();

	BasicUnit GetBaseUnit(int idx);

	//���ݻ��ƹ������ʶ����ɫ
	QColor ColorHelper(int nUnitType);
	//���ݹ���������ת���ɶ�Ӧ���ַ�����
	QString GetUnitType(int nUnitType);
	//���ݹ��������ַ�����ת���ɶ�Ӧ��������
	int GetUnitTypeCode(QString unitTypeStr);
	//���ݹ�����״���ַ�����ת���ɶ�Ӧ��������
	int GetShapeTypeCode(QString shapeTypeStr);
	 
public slots:
	void MyLogOutput(QString myLogout);         //�����־
	void ApplyDataAction();		//����������������� 
    void drawWall(const std::vector<float>& points);


	void GraphicsViewXFocus(bool b); // �����Բ�ͼX
	void GraphicsViewYFocus(bool b); // �����Բ�ͼY
	void GraphicsViewZFocus(bool b); // �����Բ�ͼZ

	void GraphicsViewOgl(bool b);	//opengl�Բ�ͼ
	void updateOGL();				//������ά��������
	void updateScene();				//���»�������



private slots: 
	void NewFileAction();
	void OpenFileAction();
	void CloseFileAction();
};
