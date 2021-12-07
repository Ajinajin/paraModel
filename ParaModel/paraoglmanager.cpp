#include "paraoglmanager.h"
#include <QPalette>

#include <QDebug>

#include <fstream>
#include <set>
#include <malloc.h>
#include <iomanip>



// ����ȫ�ֱ��� �����޸�
const QVector3D CAMERA_POSITION(300, 130, 1000.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;

//�ռ�������ά��ľ���
float Distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dis = 0;
	dis += pow(x2 - x1, 2);
	dis += pow(y2 - y1, 2);
	dis += pow(z2 - z1, 2);

	return sqrt(dis);
}

//��solid��8���������
int findIndex(vPoint points, vPoint allNodes, int idx[2][4])
{
	//�������нڵ���Ѱ�Ҵ�solid��8���� ����
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < allNodes.size(); j++)
		{
			if ((points[i].x == allNodes[j].x) && (points[i].y == allNodes[j].y) && (points[i].z == allNodes[j].z))
			{
				idx[i / 4][i % 4] = j + 1;
				break;
			}
		}
	}
	return 0;
}

//��ͬ���Ͳ��Ҳ�ͬ��ɫ
QColor ColorHelper(int nUnitType)
{
	if (nUnitType == 1)
	{
		//return QColor(47, 65, 80);
		return QColor(0, 0, 255);
	}
	else if (nUnitType == 2)
	{
		//return QColor(69, 173, 206);
		return QColor(0, 255, 127);
	}
	else if (nUnitType == 3)
	{
		//return QColor(62, 179, 203);
		return QColor(163, 143, 128);
	}
	else if (nUnitType == 4)
	{
		//return QColor(64, 135, 163);
		return QColor(192, 192, 192);
	}
	else if (nUnitType == 5)
	{
		return QColor(232, 220, 102);
	}
	else if (nUnitType == 6)
	{
		return QColor(170, 101, 96);
	}
	return QColor(72, 104, 146);
}

//void calColumnsIdxFromWallIdx(int wallIdx,int& columnIdx, VTOPOTABLE oglTopTable)
//{
//	//����ǽ���ӵ��������������
//	int Idx[2];
//	int index = 0;
//
//	int j = 0;
//	while (oglTopTable.at(wallIdx).nAdjUnitIdx[j] != -1)
//	{
//		if (oglTopTable.at(oglTopTable.at(wallIdx).nAdjUnitIdx[j]).nUnitType == 1)
//		{
//			Idx[index++] = oglTopTable.at(wallIdx).nAdjUnitIdx[j];
//		}
//		j++;
//	}
//}

ParaOGLManager::ParaOGLManager(QWidget* parent) : QOpenGLWidget(parent)
{
	this->setGeometry(10, 0, OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT);
}

ParaOGLManager::~ParaOGLManager() {
	//if (camera)
		//delete camera;

}

void ParaOGLManager::handleKeyPressEvent(QKeyEvent* event)
{
	GLuint key = event->key();
	if (key >= 0 && key <= 1024)
	{
		this->keys[key] = GL_TRUE;
	}

}

void ParaOGLManager::handleKeyReleaseEvent(QKeyEvent* event)
{
	GLuint key = event->key();
	if (key >= 0 && key <= 1024)
	{
		this->keys[key] = GL_FALSE;
	}
}


void ParaOGLManager::initializeGL()
{
	/*********** OGL���� ***********/
	pCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	pCore->glEnable(GL_DEPTH_TEST);
	pCore->glEnable(GL_PROGRAM_POINT_SIZE);


	/*********** ������Ӧ��ʱ��֡������  *************/
	for (GLuint i = 0; i != 1024; ++i) //��ʼ�����̰���
	{
		keys[i] = GL_FALSE;
	}

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	isFirstMouse = GL_TRUE;
	isLeftMousePress = GL_FALSE;
	//lastX = width() / 2.0f;
	//lastY = height() / 2.0f;

	lastX = 0;
	lastY = 0;

	rotateRaw = 0.0f;
	rotatePitch = 0.0f;

	time.start();


	/************ ����� ***********/
	camera = new Camera(CAMERA_POSITION);


	//����ӽ�
	switchView = NONE;

	VTOPOTABLE *tmp = new VTOPOTABLE;
	oglTopTable = *tmp;

	VUNITTABLE* temp = new VUNITTABLE;
	oglUnitTable = *temp;

	targetModel.setToIdentity();
	targetModelsave.setToIdentity();
	targetModeluse.setToIdentity();


	//
	allNodes.resize(0);
	allSolids.resize(0);
	//
	if_change_view = GL_FALSE;
	/************ ����shader ***********/

	ResourceManager::loadShader("coordinate", ":/shaders/res/shaders/coordinate.vert", ":/shaders/res/shaders/coordinate.frag");
	ResourceManager::loadShader("column", ":/shaders/res/shaders/column.vert", ":/shaders/res/shaders/column.frag");
	ResourceManager::loadShader("beam", ":/shaders/res/shaders/beam.vert", ":/shaders/res/shaders/beam.frag");
	ResourceManager::loadShader("board", ":/shaders/res/shaders/board.vert", ":/shaders/res/shaders/board.frag");
	ResourceManager::loadShader("wall", ":/shaders/res/shaders/wall.vert", ":/shaders/res/shaders/wall.frag");
	ResourceManager::loadShader("door", ":/shaders/res/shaders/door.vert", ":/shaders/res/shaders/door.frag");
	ResourceManager::loadShader("window", ":/shaders/res/shaders/window.vert", ":/shaders/res/shaders/window.frag");

	/************ ����Texture ***********/
	//ResourceManager::loadTexture("brickwall", ":/textures/res/textures/brickwall.jpg");


	/***********  cube shader���� **************/
	QMatrix4x4 model;
	QMatrix4x4 projection, view;
	//����ɫ��������ֵ
	ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);
	ResourceManager::getShader("column").use().setMatrix4f("model", model);
	ResourceManager::getShader("beam").use().setMatrix4f("model", model);
	ResourceManager::getShader("board").use().setMatrix4f("model", model);
	ResourceManager::getShader("wall").use().setMatrix4f("model", model);
	ResourceManager::getShader("door").use().setMatrix4f("model", model);
	ResourceManager::getShader("window").use().setMatrix4f("model", model);

	/***********  ����Uniform Buffer��ز��� **************/
	/*
	 * ��version 420���ϰ汾 ֱ������ɫ���н���uniform Block indices��Binding Points�󶨣�������һЩ
	 * �����330�汾������Ҫ�����ֶ��󶨹��̡�
	*/
	/*GLuint uniformBlockIndexRed = pCore->glGetUniformBlockIndex(ResourceManager::getShader("red").getShaderID(), "Matrices");
	GLuint uniformBlockIndexGreen = pCore->glGetUniformBlockIndex(ResourceManager::getShader("green").getShaderID(), "Matrices");
	GLuint uniformBlockIndexBlue = pCore->glGetUniformBlockIndex(ResourceManager::getShader("blue").getShaderID(), "Matrices");
	GLuint uniformBlockIndexYellow = pCore->glGetUniformBlockIndex(ResourceManager::getShader("yellow").getShaderID(), "Matrices");

	pCore->glUniformBlockBinding(ResourceManager::getShader("red").getShaderID(), uniformBlockIndexRed, 0);
	pCore->glUniformBlockBinding(ResourceManager::getShader("green").getShaderID(), uniformBlockIndexGreen, 0);
	pCore->glUniformBlockBinding(ResourceManager::getShader("blue").getShaderID(), uniformBlockIndexBlue, 0);
	pCore->glUniformBlockBinding(ResourceManager::getShader("yellow").getShaderID(), uniformBlockIndexYellow, 0);*/

	pCore->glGenBuffers(1, &uboMatrices);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferData(GL_UNIFORM_BUFFER, 2 * 4 * sizeof(QVector4D), NULL, GL_STATIC_DRAW);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);

	pCore->glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * 4 * sizeof(QVector4D));

	///************ ������ɫ�������� ***********/
	pCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCore->glClearColor(284.0/255.0, 284.0 / 255.0, 255.0/255.0f, 1.0f);
	//pCore->glClearColor(1.0f, 1.0f, 1.0, 1.0f);
}

void ParaOGLManager::resizeGL(int w, int h)
{
	pCore->glViewport(0, 0, w, h);
}


void ParaOGLManager::paintGL()
{
	pCore->glClear(GL_COLOR_BUFFER_BIT);
	pCore->glClear(GL_DEPTH_BUFFER_BIT);
	
	
	/*********** ��������֡��֮���ʱ����  ***************/
	GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	this->processInput(deltaTime);
	//update
	this->updateGL();

	int layerHeight = 0;
	int boardHeight;
	int columnHeight = 250;

	
	//���ֹ�����͸����
	float transOfColumn = 1.0;
	float transOfBeam = 1.0;
	float transOfBoard = 0.75;
	float transOfWall = 0.5;
	float transOfDoor = 1.0;
	float transOfWindow = 0.25;

	
	//������άģ��
	if (!oglUnitTable.empty() && !oglTopTable.empty())
	{

		for (int i = 0; i < oglTopTable.size(); i++)
		{
			//**�жϴ˹�������**

			//��
			if (oglTopTable.at(i).nUnitType == 1 && oglTopTable.at(i).nStatusFlag!=1)
			{
				QColor color = ColorHelper(1);
				ResourceManager::getShader("column").use();
				ResourceManager::getShader("column").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("column").use().setInteger("R", color.red());
				ResourceManager::getShader("column").use().setInteger("G", color.green());
				ResourceManager::getShader("column").use().setInteger("B", color.blue());
				ResourceManager::getShader("column").use().setFloat("tranS", transOfColumn);

				//���Ҷ�Ӧ���������Ϣ
				BasicUnit info = findUnit(oglTopTable.at(i).nCenUnitIdx, oglUnitTable);

				//����
				if (info.oShape.nShapeType == 1)
				{
					float x, y, z, length, thickness, height;

					height = columnHeight;
					length = info.oShape.nShapeRange[2] - info.oShape.nShapeRange[0];
					thickness = info.oShape.nShapeRange[3] - info.oShape.nShapeRange[1];

					x = oglTopTable.at(i).nCenPos[0];
					y = columnHeight / 2 + layerHeight;
					z = oglTopTable.at(i).nCenPos[2];

					InitAndDrawCuboid(x, y, z, length, thickness, height,1);

					
				}

				//Բ��
				if (info.oShape.nShapeType == 2)
				{
					float x, y, z, radius, height;
					x = oglTopTable.at(i).nCenPos[0];
					y = columnHeight / 2 + layerHeight;
					
					z = oglTopTable.at(i).nCenPos[2];
					height = oglTopTable.at(i).nCenPos[3];
					
					radius = info.oShape.nNumOrRadius;
					//radius = 1;
					InitAndDrawColumn(x, y, z, radius, height);
				}

				//�������
				if (info.oShape.nShapeType == 3)
				{
					float begin_y=0;
					InitAndDrawPolygonPortrait(info.oShape.vPolyPt, begin_y, columnHeight);
				}
			}

			//��
			if (oglTopTable.at(i).nUnitType == 2 && oglTopTable.at(i).nStatusFlag != 1)
			{
				QColor color = ColorHelper(2);
				ResourceManager::getShader("beam").use();
				ResourceManager::getShader("beam").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("beam").use().setInteger("R", color.red());
				ResourceManager::getShader("beam").use().setInteger("G", color.green());
				ResourceManager::getShader("beam").use().setInteger("B", color.blue());
				ResourceManager::getShader("beam").use().setFloat("tranS", transOfBeam);

				BasicUnit info = findUnit(oglTopTable.at(i).nCenUnitIdx, oglUnitTable);

				//����
				if (info.oShape.nShapeType == 1)
				{
					BasicUnit info = findUnit(oglTopTable.at(i).nCenUnitIdx, oglUnitTable);

					float x, y, z, length, thickness, height;


					//���ĺ��
					thickness = info.oShape.nShapeRange[3]- info.oShape.nShapeRange[1];
					//���ĸ߶�
					height = info.oShape.nShapeRange[2] - info.oShape.nShapeRange[0];
					
					//���������ӵ�������֮��ľ���
					vPoint columnPoints;		//��¼�����������ĵ�
					vector<BasicUnit> Columns;	//ǽ���ӵ�����
					int j = 0;
					while (oglTopTable.at(i).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nUnitType == 1)
						{

							BasicUnit tmpinfo = findUnit(oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenUnitIdx, oglUnitTable);
							Columns.push_back(tmpinfo);

							Point info;
							info.x = oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenPos[0];
							info.y = 0;
							info.z = oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenPos[2];
							columnPoints.push_back(info);
						}
						
						j++;
					}
					//���ȼ�ȥ�������Ŀ�ȵ�һ��
					length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
					length -= (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2;
					length -= (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;


					//�Ƕȼ���
					if (columnPoints[0].z == columnPoints[1].z) { oglTopTable.at(i).nUnitAngle = 0; }
					if (columnPoints[0].x == columnPoints[1].x) { oglTopTable.at(i).nUnitAngle = 90; }

					if (oglTopTable.at(i).nUnitAngle == 0)
					{
						//���������˹�ϵ����
						if (columnPoints[0].x < columnPoints[1].x)
						{
							x = columnPoints[0].x + (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2 + length / 2;
							y = columnHeight - height / 2 + layerHeight;
							z = columnPoints[0].z;
							InitAndDrawCuboid(x, y, z, length, thickness, height,2);
						}
						else
						{
							x = columnPoints[1].x + (Columns[1].oShape.nShapeRange[2] - Columns[1].oShape.nShapeRange[0]) / 2 + length / 2;
							y = columnHeight - height / 2 + layerHeight;
							z = columnPoints[1].z;
							InitAndDrawCuboid(x, y, z, length, thickness, height,2);
						}

					}
					if (oglTopTable.at(i).nUnitAngle == 90)
					{
						if (columnPoints[0].z < columnPoints[1].z)
						{
							x = columnPoints[0].x;
							y = columnHeight - height / 2 + layerHeight;
							z = columnPoints[1].z - (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2 - length / 2;
							InitAndDrawCuboid(x, y, z, thickness, length, height,2);
						}
						else
						{
							x = columnPoints[1].x;
							y = columnHeight - height / 2 + layerHeight;
							z = columnPoints[0].z - (Columns[0].oShape.nShapeRange[3] - Columns[0].oShape.nShapeRange[1]) / 2 - length / 2;
							InitAndDrawCuboid(x, y, z, thickness, length, height,2);
						}
					}



				}



				//�����
				if (info.oShape.nShapeType == 3)
				{
					float height = oglTopTable.at(i).nCenPos[3];
					//InitAndDrawPolygonColumnHorizontal(info.oShape.vPolyPt, height);
				}
			}

			//��  ps:Ŀǰ������˳ʱ����ȡ ������Ϣ��
			if (oglTopTable.at(i).nUnitType == 3 && oglTopTable.at(i).nStatusFlag != 1)
			{
				BasicUnit info = findUnit(oglTopTable.at(i).nCenUnitIdx, oglUnitTable);

				float x, y, z, length, width, thickness;
				//��ĺ��
				thickness = info.oShape.nThickNess;

				QColor color = ColorHelper(3);
				ResourceManager::getShader("board").use();
				ResourceManager::getShader("board").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("board").use().setInteger("R", color.red());
				ResourceManager::getShader("board").use().setInteger("G", color.green());
				ResourceManager::getShader("board").use().setInteger("B", color.blue());
				ResourceManager::getShader("board").use().setFloat("tranS", transOfBoard);

				//�洢�����ӵ���֮�����Ϣ
				vPoint columnPoints;
				vector<BasicUnit> columns;
				int j = 0;
				while (oglTopTable.at(i).nAdjUnitIdx[j] != -1)
				{
					if (oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nUnitType == 1)
					{
						BasicUnit tmpinfo = findUnit(oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenUnitIdx, oglUnitTable);
						columns.push_back(tmpinfo);

						Point info;
						info.x = oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenPos[0];
						info.y = 0;
						info.z = oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenPos[2];
						columnPoints.push_back(info);
					}
					j++;
				}

				//��ͨ������֧�ž��ΰ�
				if (columns.size() == 4)
				{
					//���ȼ�ȥ�������Ŀ�ȵ�һ��
					length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
					length += (columns[0].oShape.nShapeRange[2] - columns[0].oShape.nShapeRange[0]) / 2;
					length += (columns[1].oShape.nShapeRange[2] - columns[1].oShape.nShapeRange[0]) / 2;
					//���
					width = Distance(columnPoints[1].x, columnPoints[1].y, columnPoints[1].z, columnPoints[2].x, columnPoints[2].y, columnPoints[2].z);
					width += (columns[1].oShape.nShapeRange[3] - columns[1].oShape.nShapeRange[1]) / 2;
					width += (columns[2].oShape.nShapeRange[3] - columns[2].oShape.nShapeRange[1]) / 2;

					x = columnPoints[3].x - (columns[3].oShape.nShapeRange[2] - columns[3].oShape.nShapeRange[0]) / 2 + length / 2;
					y = columnHeight + thickness / 2 + layerHeight;
					z = columnPoints[3].z + (columns[3].oShape.nShapeRange[3] - columns[3].oShape.nShapeRange[1]) / 2 - width / 2;

					InitAndDrawCuboid(x, y, z, length, width, thickness, 3);
				}
				//����ΰ�
				if (columns.size() > 4)
				{
					VINT polyData;
					for (int i = 0; i < columnPoints.size(); i++)
					{
						polyData.push_back(columnPoints[i].x);
						polyData.push_back(columnPoints[i].z);
					}
					InitAndDrawPolygonPortrait(polyData,columnHeight,thickness);
				}
				
			}

			//ǽ
			if (oglTopTable.at(i).nUnitType == 4 && oglTopTable.at(i).nStatusFlag != 1)
			{
				QColor color = ColorHelper(4);
				ResourceManager::getShader("wall").use();
				ResourceManager::getShader("wall").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("wall").use().setInteger("R", color.red());
				ResourceManager::getShader("wall").use().setInteger("G", color.green());
				ResourceManager::getShader("wall").use().setInteger("B", color.blue());
				ResourceManager::getShader("wall").use().setFloat("tranS", transOfWall);

				BasicUnit info = findUnit(oglTopTable.at(i).nCenUnitIdx, oglUnitTable);
				
				float x, y, z, length, thickness, height;
				

				//ǽ�ĺ��
				thickness = info.oShape.nThickNess;

				//ǽ�ĸ߶���ʱ����һ��
				height = columnHeight;

				//����ǽ���ӵ�������֮��ľ���
				vPoint columnPoints;		//��¼�����������ĵ��µ�
				vector<BasicUnit> Columns;	//ǽ���ӵ�����
				int j = 0;
				while (oglTopTable.at(i).nAdjUnitIdx[j] != -1)
				{
					if (oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nUnitType == 1)
					{
						
						BasicUnit tmpinfo = findUnit(oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenUnitIdx, oglUnitTable);
						Columns.push_back(tmpinfo);

						Point info;
						info.x = oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenPos[0];
						info.y = 0;
						info.z = oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenPos[2];
						columnPoints.push_back(info);
					}
					//if (oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nUnitType == 2)//��
					//{
					//	BasicUnit tmpinfo = findUnit(oglTopTable.at(oglTopTable.at(i).nAdjUnitIdx[j]).nCenUnitIdx, oglUnitTable);
					//	//ǽ�߶�
					//	//height = columnHeight - (tmpinfo.oShape.nShapeRange[2]- tmpinfo.oShape.nShapeRange[0]);//��ȥ���ĸ߶�
					//	
					//}
					j++;
				}
				//���ȼ�ȥ�������Ŀ�ȵ�һ��
				length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
				length -= (Columns[0].oShape.nShapeRange[2]- Columns[0].oShape.nShapeRange[0])/2;
				length -= (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;
				

				//����ǽ�Ĳ�ͬ�Ƕ�������
				if (oglTopTable.at(i).nUnitAngle == 0)
				{
					//����ǽ���˹�ϵ����
					if (columnPoints[0].x < columnPoints[1].x)
					{
						x = columnPoints[0].x + (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2 + length / 2;
						y = columnHeight / 2 + layerHeight;
						z = columnPoints[0].z;
						InitAndDrawCuboid(x, y, z, length, thickness, height,4);
					}
					else
					{
						x = columnPoints[1].x + (Columns[1].oShape.nShapeRange[2] - Columns[1].oShape.nShapeRange[0]) / 2 + length / 2;
						y = columnHeight / 2 + layerHeight;
						z = columnPoints[1].z;
						InitAndDrawCuboid(x, y, z, length, thickness, height,4);
					}
					
				}
				if (oglTopTable.at(i).nUnitAngle == 90)
				{
					if (columnPoints[0].z < columnPoints[1].z)
					{
						x = columnPoints[0].x;
						y = columnHeight / 2 + layerHeight;
						z = columnPoints[1].z - (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2 - length / 2;
						InitAndDrawCuboid(x, y, z, thickness, length, height,4);
					}
					else
					{
						x = columnPoints[1].x;
						y = columnHeight / 2 + layerHeight;
						z = columnPoints[0].z - (Columns[0].oShape.nShapeRange[3] - Columns[0].oShape.nShapeRange[1]) / 2 - length / 2;
						InitAndDrawCuboid(x, y, z, thickness, length, height,4);
					}
				}
				

				
			}
			
			//���봰
			if ((oglTopTable.at(i).nUnitType == 5 || oglTopTable.at(i).nUnitType == 6) && oglTopTable.at(i).nStatusFlag != 1)
			{
				QColor color = ColorHelper(oglTopTable.at(i).nUnitType);
				ResourceManager::getShader("door").use();
				ResourceManager::getShader("door").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("door").use().setInteger("R", color.red());
				ResourceManager::getShader("door").use().setInteger("G", color.green());
				ResourceManager::getShader("door").use().setInteger("B", color.blue());
				ResourceManager::getShader("door").use().setFloat("tranS", transOfDoor);

				BasicUnit info = findUnit(oglTopTable.at(i).nCenUnitIdx, oglUnitTable);
				float x, y, z, length, thickness, height;
				length = info.oShape.nShapeRange[0];
				height = info.oShape.nShapeRange[1];


				//�õ���Ӧ��ǽid��ǽ��Ϣ  ȥ�������Ӧ���ŵ�����
				{
					
					int wallId = oglTopTable.at(i).nAdjUnitIdx[0];
					//����ǽ���ӵ�������֮��ľ���
					vPoint columnPoints;		//��¼�����������ĵ�
					vector<BasicUnit> Columns;	//ǽ���ӵ�����
					int j = 0;
					while (oglTopTable.at(wallId).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable.at(oglTopTable.at(wallId).nAdjUnitIdx[j]).nUnitType == 1)
						{

							BasicUnit tmpinfo = findUnit(oglTopTable.at(oglTopTable.at(wallId).nAdjUnitIdx[j]).nCenUnitIdx, oglUnitTable);
							Columns.push_back(tmpinfo);

							Point info;
							info.x = oglTopTable.at(oglTopTable.at(wallId).nAdjUnitIdx[j]).nCenPos[0];
							info.y = 0;
							info.z = oglTopTable.at(oglTopTable.at(wallId).nAdjUnitIdx[j]).nCenPos[2];
							columnPoints.push_back(info);
						}

						j++;
					}

					//��Ӧǽ����Ϣ
					BasicUnit tmpWallInfo = findUnit(oglTopTable.at(wallId).nCenUnitIdx, oglUnitTable);
					
					thickness = tmpWallInfo.oShape.nThickNess;//�ź����ǽһ��

					//ǽ��Ӧ�ǶȲ�ͬ����Ӧ��ͬ���
					int wallX, wallY, wallZ;
					if (oglTopTable.at(wallId).nUnitAngle == 0)
					{
						//����ǽ���˹�ϵ����
						if (columnPoints[0].x < columnPoints[1].x)
						{
							wallX = columnPoints[0].x + (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2;
							wallY = 0;
							wallZ = columnPoints[0].z + thickness / 2;

						}
						else
						{
							wallX = columnPoints[1].x + (Columns[1].oShape.nShapeRange[2] - Columns[1].oShape.nShapeRange[0]) / 2;
							wallY = 0;
							wallZ = columnPoints[1].z + thickness / 2;

						}

						//�Ŵ�������
						x = wallX + oglTopTable.at(i).nCenPos[0] + length / 2;
						y = oglTopTable.at(i).nCenPos[1] + height / 2 + layerHeight;
						z = wallZ - thickness / 2;

						InitAndDrawCuboid(x, y, z, length, thickness, height, oglTopTable.at(i).nUnitType);
					}
					if (oglTopTable.at(wallId).nUnitAngle == 90)
					{
						if (columnPoints[0].z < columnPoints[1].z)
						{
							wallX = columnPoints[0].x - thickness / 2;
							wallY = 0;
							wallZ = columnPoints[1].z - (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;

						}
						else
						{
							wallX = columnPoints[1].x - thickness / 2;
							wallY = 0;
							wallZ = columnPoints[0].z - (Columns[0].oShape.nShapeRange[3] - Columns[0].oShape.nShapeRange[1]) / 2;

						}

						//�Ŵ�������
						x = wallX + thickness / 2;
						y = oglTopTable.at(i).nCenPos[1] + height / 2 + layerHeight;
						z = wallZ - oglTopTable.at(i).nCenPos[0] - length / 2;

						InitAndDrawCuboid(x, y, z, thickness, length, height, oglTopTable.at(i).nUnitType);
					}

				}
				
			}

		
		}


		if (outFlag != -1) { outFlag++; }
	}
	

	//��ʱ0 1 2����ʱ�ģ�1˵�����еĵ���Ϣ�Ѵ洢������2˵�����е�solid��Ϣ�Ѵ洢(��ʱ�������K�ļ���)

	//������άģ�͵����������K�ļ���
	if (outFlag == 2)
	{
		outFlag = -1;
	}
}

void ParaOGLManager::processInput(GLfloat dt)
{
	if (keys[Qt::Key_W])
		camera->processKeyboard(FORWARD, dt);
	if (keys[Qt::Key_S])
		camera->processKeyboard(BACKWARD, dt);
	if (keys[Qt::Key_A])
		camera->processKeyboard(LEFT, dt);
	if (keys[Qt::Key_D])
		camera->processKeyboard(RIGHT, dt);
	if (keys[Qt::Key_E])
		camera->processKeyboard(UP, dt);
	if (keys[Qt::Key_Q])
		camera->processKeyboard(DOWN, dt);

}



void ParaOGLManager::updateGL()
{

	QMatrix4x4 projection;
	GLfloat a = width();
	GLfloat b = height();
	projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 2000.f);
	

	//switch (switchView)
	//{
	//case FRONT_VIEW:
	//	view.lookAt(QVector3D(0, 0, 5), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//ǰ��ͼ
	//	break;
	//case BACK_VIEW:
	//	view.lookAt(QVector3D(0, 0, -5), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//����ͼ
	//	break;
	//case LEFT_VIEW:
	//	view.lookAt(QVector3D(-5, 0, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//����ͼ
	//	break;
	//case RIGHT_VIEW:
	//	view.lookAt(QVector3D(5, 0, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//����ͼ
	//	break;
	//case TOP_VIEW:
	//	view.lookAt(QVector3D(0, 5, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, -90.0f));					//����ͼ
	//	break;
	//case BOTTOM_VIEW:
	//	view.lookAt(QVector3D(0, -5, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 90.0f));					//����ͼ
	//	break;
	//default:
	//	view = camera->getViewMatrix();
	//	break;
	//}

	if (!if_change_view)
	{
		view = camera->getViewMatrix();
	}

	ResourceManager::getShader("coordinate").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("coordinate").use().setMatrix4f("view", view);

	ResourceManager::getShader("column").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("column").use().setMatrix4f("view", view);

	ResourceManager::getShader("beam").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("beam").use().setMatrix4f("view", view);

	ResourceManager::getShader("board").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("board").use().setMatrix4f("view", view);

	ResourceManager::getShader("wall").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("wall").use().setMatrix4f("view", view);

	ResourceManager::getShader("door").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("door").use().setMatrix4f("view", view);

	ResourceManager::getShader("window").use().setMatrix4f("projection", projection);
	ResourceManager::getShader("window").use().setMatrix4f("view", view);



	/***********  ����Uniform Buffer��ز��� **************/
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(QVector4D), &projection);
	pCore->glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(QVector4D), 4 * sizeof(QVector4D), &view);
	pCore->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ParaOGLManager::mouseMoveEvent(QMouseEvent* event)
{

	if (isLeftMousePress)//�������תģ��
	{
		targetModeluse = targetModelsave;

		GLint xpos = event->pos().x();
		GLint ypos = event->pos().y();


		if (isFirstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			isFirstMouse = GL_FALSE;
		}

		GLint xoffset = xpos - lastX;
		GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;


		rotateRaw += xoffset * 0.1;
		rotatePitch += yoffset * 0.1;

		//������������תģ�;���
		targetModel.translate(200, 0, 300);

		targetModel.setToIdentity();
		GLfloat angle_now = qSqrt(qPow(xoffset, 2) + qPow(yoffset, 2)) / 5;
		targetModel.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModel *= targetModeluse;

		targetModelsave.setToIdentity();
		targetModelsave.rotate(angle_now, -yoffset, xoffset, 0.0);
		targetModelsave *= targetModeluse;

		targetModel.translate(-200, 0, -300);
	}

	//if (isRightMousePress)//�Ҽ�����ת���
	//{
	//	GLint xpos = event->pos().x();
	//	GLint ypos = event->pos().y();

	//	if (isFirstMouse)
	//	{
	//		lastX = xpos;
	//		lastY = ypos;
	//		isFirstMouse = GL_FALSE;
	//	}

	//	GLint xoffset = xpos - lastX;
	//	GLint yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	//	lastX = xpos;
	//	lastY = ypos;
	//	camera->processMouseMovement(xoffset, yoffset);
	//		
	//}
}

void ParaOGLManager::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)//ע����button��������buttons������
		isLeftMousePress = GL_TRUE;

	if (event->button() == Qt::RightButton)//ע����button��������buttons������
		isRightMousePress = GL_TRUE;
}

void ParaOGLManager::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) { //ע����button��������buttons������
		isLeftMousePress = GL_FALSE;
		isFirstMouse = GL_TRUE;
	}
}

void ParaOGLManager::wheelEvent(QWheelEvent* event)
{
	QPoint offset = event->angleDelta();
	camera->processMouseScroll(offset.y() / 20.0f);
}


void ParaOGLManager::InitAndDrawColumn(int x, int y, int z, int radius, int height)
{
	//Բ��ͨ������Բ��������ʵ��

	int pointNum = 4320;//ÿ��Բ�ĵ���������ȣ�

	int* vertices = new int[pointNum * 6];

	for (int i = 0; i < pointNum; i++)
	{
		vertices[i * 6 + 0] = radius * cos(2 * 3.14 * i / pointNum) + x;
		vertices[i * 6 + 1] = y;
		vertices[i * 6 + 2] = radius * sin(2 * 3.14 * i / pointNum) + z;

		vertices[i * 6 + 3] = vertices[i * 6];
		vertices[i * 6 + 4] = vertices[i * 6 + 1] + height;
		vertices[i * 6 + 5] = vertices[i * 6 + 2];
	}


	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, pointNum * 6 * sizeof(int), vertices, GL_STATIC_DRAW);


	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);


	pCore->glDepthMask(GL_FALSE);//ȡ����Ȼ���
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	pCore->glDrawArrays(GL_LINES, 0, pointNum * 2);

	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);







	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] vertices;
}

void ParaOGLManager::InitAndDrawCuboid(int x, int y, int z, int length, int thickness, int height,int type)
{
	//init
	int* vertices = new int[72];

	vPoint points;
	Point tmp;
	tmp.x = x - length / 2, tmp.y = y - height / 2, tmp.z = z - thickness / 2; points.push_back(tmp);
	tmp.x = x + length / 2, tmp.y = y - height / 2, tmp.z = z - thickness / 2; points.push_back(tmp);
	tmp.x = x + length / 2, tmp.y = y - height / 2, tmp.z = z + thickness / 2; points.push_back(tmp);
	tmp.x = x - length / 2, tmp.y = y - height / 2, tmp.z = z + thickness / 2; points.push_back(tmp);
	tmp.x = x - length / 2, tmp.y = y + height / 2, tmp.z = z - thickness / 2; points.push_back(tmp);
	tmp.x = x + length / 2, tmp.y = y + height / 2, tmp.z = z - thickness / 2; points.push_back(tmp);
	tmp.x = x + length / 2, tmp.y = y + height / 2, tmp.z = z + thickness / 2; points.push_back(tmp);
	tmp.x = x - length / 2, tmp.y = y + height / 2, tmp.z = z + thickness / 2; points.push_back(tmp);


	int solidToFaceOrder[] = { 0,1,2,3,0,1,5,4,1,2,6,5,3,2,6,7,0,3,7,4,4,5,6,7 };



	//���뵽�㼯����
	if (outFlag == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			allNodes.push_back(points[i]);
		}
	}
	//���Ѵ�����ϣ���ʱ��ʼ��ÿ��solid����ĵ����� ��ֵ
	if (outFlag == 1)
	{
		Solid tmp;
		findIndex(points,allNodes,tmp.idx);

		allSolids.push_back(tmp);
	}


	//6��������Ƭ 24��������
	for (int k = 0; k < 24; k++)
	{
		vertices[k * 3] = points[solidToFaceOrder[k]].x;
		vertices[k * 3 + 1] = points[solidToFaceOrder[k]].y;
		vertices[k * 3 + 2] = points[solidToFaceOrder[k]].z;
	}


	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(int), vertices, GL_STATIC_DRAW);


	
	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);


	//�����Ŵ��ǲ�͸���ģ����Զ������Ͳ�����Ȼ���
	
	pCore->glEnable(GL_DEPTH_TEST);
	if (type != 1 && type != 2/* && type != 3*/ && type != 5 /*&& type != 6*/) { pCore->glDepthMask(GL_FALSE); }//ȡ����Ȼ���
	
	pCore->glDisable(GL_CULL_FACE);
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	pCore->glDrawArrays(GL_QUADS, 0, 24);

	pCore->glDisable(GL_BLEND);

	if (type != 1 && type != 2 /*&& type != 3*/  && type != 5 /*&& type != 6*/ ) { pCore->glDepthMask(GL_TRUE); }
	pCore->glDisable(GL_DEPTH_TEST);


	
	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] vertices;
}

BasicUnit findUnit(int idx, VUNITTABLE oglUnitTable)
{
	for (int i = 0; i < oglUnitTable.size(); i++)
	{
		if (oglUnitTable[i].nUnitIdx == idx)
		{
			return oglUnitTable[i];
		}
	}
	//return;
}


void ParaOGLManager::InitAndDrawPolygonPortrait(VINT data, int begin_y, int height)
{

	//�Ȼ���������(�������data.size()/2)(ģ��yֵ��ʼ����Ϊ0)
	int* vertices_1 = new int[data.size() * 6];
	int pointNum = data.size() / 2;

	for (int i = 0; i < data.size()/2 -1; i++)//���δ�ÿ���� �ĸ�����Ϣ
	{
		vertices_1[i * 12] = data[i * 2];
		vertices_1[i * 12 + 1] = begin_y;
		vertices_1[i * 12 + 2] = data[i * 2 + 1];

		vertices_1[i * 12 + 3] = data[(i + 1) * 2];
		vertices_1[i * 12 + 4] = begin_y;
		vertices_1[i * 12 + 5] = data[(i + 1) * 2 + 1];

		vertices_1[i * 12 + 6] = data[(i + 1) * 2];
		vertices_1[i * 12 + 7] = begin_y + height;
		vertices_1[i * 12 + 8] = data[(i + 1) * 2 + 1];

		vertices_1[i * 12 + 9] = data[(i) * 2];
		vertices_1[i * 12 + 10] = begin_y + height;
		vertices_1[i * 12 + 11] = data[(i) * 2 + 1];
	}
	{//���һ������
		vertices_1[(data.size() / 2 - 1) * 12] = data[(data.size() / 2 - 1) * 2];
		vertices_1[(data.size() / 2 - 1) * 12 + 1] = begin_y;
		vertices_1[(data.size() / 2 - 1) * 12 + 2] = data[(data.size() / 2 - 1) * 2 + 1];

		vertices_1[(data.size() / 2 - 1) * 12 + 3] = data[(0) * 2];
		vertices_1[(data.size() / 2 - 1) * 12 + 4] = begin_y;
		vertices_1[(data.size() / 2 - 1) * 12 + 5] = data[(0) * 2 + 1];

		vertices_1[(data.size() / 2 - 1) * 12 + 6] = data[(0) * 2];
		vertices_1[(data.size() / 2 - 1) * 12 + 7] = begin_y + height;
		vertices_1[(data.size() / 2 - 1) * 12 + 8] = data[(0) * 2 + 1];

		vertices_1[(data.size() / 2 - 1) * 12 + 9] = data[(data.size() / 2 - 1) * 2];
		vertices_1[(data.size() / 2 - 1) * 12 + 10] = begin_y + height;
		vertices_1[(data.size() / 2 - 1) * 12 + 11] = data[(data.size() / 2 - 1) * 2 + 1];
	}
	

	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, data.size() * 6 * sizeof(int), vertices_1, GL_STATIC_DRAW);


	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);


	pCore->glDepthMask(GL_FALSE);//ȡ����Ȼ���
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	pCore->glDrawArrays(GL_QUADS, 0, data.size() * 2);

	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);



	//�ٻ�����������
	int* vertices_2 = new int[(data.size() / 2)*6];
	for (int i = 0; i < data.size() / 2; i++)
	{
		
		vertices_2[i * 3] = data[i * 2];
		vertices_2[i * 3 + 1] = begin_y;
		vertices_2[i * 3 + 2] = data[i * 2 + 1];

		vertices_2[(i + data.size() / 2) * 3] = data[i * 2];
		vertices_2[(i + data.size() / 2) * 3 + 1] = begin_y + height;
		vertices_2[(i + data.size() / 2) * 3 + 2] = data[i * 2 + 1];
		
	}
	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, (data.size() / 2) * 6 * sizeof(int), vertices_2, GL_STATIC_DRAW);


	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);

	
	//����Ϊ����λ���
	pCore->glPolygonMode(GL_FRONT,GL_FILL);
	pCore->glPolygonMode(GL_BACK, GL_FILL);

	pCore->glDepthMask(GL_FALSE);//ȡ����Ȼ���
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	
	pCore->glDrawArrays(GL_POLYGON, 0, data.size() / 2);
	pCore->glDrawArrays(GL_POLYGON, data.size() / 2, data.size() / 2);

	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);


	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] vertices_1;
	delete[] vertices_2;
}

void ParaOGLManager::InitAndDrawPolygonHorizontal(VINT data, int length)
{
	float* vertices = new float[data.size() * 1.5 * 2];

	for (int i = 0; i < data.size(); i += 2)
	{
		vertices[i * 6] = data[i];
		vertices[i * 6 + 1] = 0;
		vertices[i * 6 + 2] = data[i + 1];

		vertices[i * 6 + 3] = vertices[i * 6] + length;
		vertices[i * 6 + 4] = 0;
		vertices[i * 6 + 5] = vertices[i * 6 + 2];
	}


	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, data.size() * 3 * sizeof(float), vertices, GL_STATIC_DRAW);


	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	pCore->glDepthMask(GL_FALSE);//ȡ����Ȼ���
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	pCore->glDrawArrays(GL_LINES, 0, data.size());

	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);




	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] vertices;
}










