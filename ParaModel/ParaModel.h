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
	//״̬��
	QStatusBar* winStatus;
	QLabel* pTipBar;								// ״̬����������ʾ
	QLineEdit* pModelEdit[7];						// ս�������Ա༭�� ���� ��Ƭ���� ��Ƭ���� �ֲ��Ƕ�1 �ֲ��Ƕ�2
	QLineEdit* pArmHeadEdit[5];						// �㷨��Ϣ
	QTreeWidget* pModelTreeWidget;					// ϵͳģ����
	QDockWidget* MainDockWidget;					// ������ʾ����
	BQGraphicsView* graphicsViewX;					// ��άģ��X��ͼ
	BQGraphicsView* graphicsViewY;					// ��άģ��Y��ͼ
	BQGraphicsView* graphicsViewZ;					// ��άģ��Z��ͼ
	BQGraphicsView* graphicsViewMain;				// ��άģ��Z��ͼ
	BQGraphicsView* graphicsViewOgl;				//��λģ��չʾ��ͼ

	BQGraphicsScene pSceneX;						// ��άģ��X��ͼ����
	BQGraphicsScene pSceneY;						// ��άģ��Y��ͼ����
	BQGraphicsScene pSceneZ;						// ��άģ��Z��ͼ����
	BQGraphicsScene pSceneMain;						// ��άģ��Z��ͼ����Ļ������

	ParaOGLManager* oglmanager;							// ��ά��ʾ������
	ParaOGLManager* oglmanagerMain;							// ��ά��ʾ���ڴ���Ļ����

	QTextEdit* myLogOutLabel;						// ��־����������ı�
    // ͼ��Ŀ�
    int mainLableWidth, mainLableHeight; 
	int MainDockState;								// 0��X��ͼ 1��Y��ͼ 2��Z��ͼ 3����ά��ͼ
    // mainLabel��ʾ��ͼ��
    QImage* img = nullptr;
    QMenu* popMenu_In_ListWidget_; /*�����˵�*/
    QAction * action_Replace_ListWidget_;/*�˵��ϵ�Action*/
    //QGraphicsScene* m_scene = nullptr;


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

public slots:
	void MyLogOutput(const char* myLogout);         //�����־
	void ApplyDataAction();		//�����������������
    void openAndLoadPic(); // ��
    void getPara(); // ���Ĳ���
    void resetPara(); // ȡ������
    void loadViews(); // �����Բ�ͼ
    void showImageList(QStringList& fileNames); // ��ʾ�Ҳ�ͼ���б�
    void replaceSlot(); // �һ��滻
    void drawWall(const std::vector<float>& points);

	void GraphicsViewXFocus(bool b); // �����Բ�ͼ
	void GraphicsViewYFocus(bool b); // �����Բ�ͼ
	void GraphicsViewZFocus(bool b); // �����Բ�ͼ
	void GraphicsViewOgl(bool b);	//opengl�Բ�ͼ

private slots:
	void onCustomContextMenuRequested(const QPoint& pos);
	void on_polygonBtn_clicked();
	void on_clearBtn_clicked();
	void my_save();
	void on_loadBtn_clicked();
};
