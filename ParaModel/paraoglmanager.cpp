#include "paraoglmanager.h"
#include <QPalette>

#include <QDebug>

#include <set>
#include <malloc.h>




// ����ȫ�ֱ��� �����޸�
const QVector3D CAMERA_POSITION(200.0f, 100.0f, 1000.0f);
const QVector3D LIGHT_POSITION(0.0f, 1.0f, 0.0f);

const int OGLMANAGER_WIDTH = 1200;
const int OGLMANAGER_HEIGHT = 800;

float Distance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dis = 0;
	dis += pow(x2 - x1, 2);
	dis += pow(y2 - y1, 2);
	dis += pow(z2 - z1, 2);

	return sqrt(dis);
}

QColor ColorHelper(int nUnitType)
{
	if (nUnitType == 1)
	{
		return QColor(47, 65, 80);
	}
	else if (nUnitType == 2)
	{
		return QColor(69, 173, 206);
	}
	else if (nUnitType == 3)
	{
		return QColor(62, 179, 203);
	}
	else if (nUnitType == 4)
	{
		return QColor(64, 135, 163);
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


	oglTopTable = new VTOPOTABLE;
	oglUnitTable = new VUNITTABLE;

	targetModel.setToIdentity();
	targetModelsave.setToIdentity();
	targetModeluse.setToIdentity();

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
	pCore->glClearColor(1.0f, 0.9725f, 0.8627f, 1.0f);
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

	int layerHeight;
	int boardHeight;
	int columnHeight = 250;

	
	//���ֹ�����͸����
	float transOfColumn = 1.0;
	float transOfBeam = 1.0;
	float transOfBoard = 0.75;
	float transOfWall = 0.5;
	float transOfDoor = 0.25;
	float transOfWindow = 0.25;

	//������άģ��
	if (!oglUnitTable->empty() && !oglTopTable->empty())
	{
		modelInfos.resize(oglTopTable->size());
		for (int i = 0; i < oglTopTable->size(); i++)
		{
			//**�жϴ˹�������**

			//��
			if (oglTopTable->at(i).nUnitType == 1)
			{
				QColor color = ColorHelper(1);
				ResourceManager::getShader("column").use();
				ResourceManager::getShader("column").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("column").use().setInteger("R", color.red());
				ResourceManager::getShader("column").use().setInteger("G", color.green());
				ResourceManager::getShader("column").use().setInteger("B", color.blue());
				ResourceManager::getShader("column").use().setFloat("tranS", transOfColumn);

				//���Ҷ�Ӧ���������Ϣ
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);

				//����
				if (info.oShape.nShapeType == 1)
				{
					float x, y, z, length, thickness, height;

					height = columnHeight;
					length = info.oShape.nShapeRange[2] - info.oShape.nShapeRange[0];
					thickness = info.oShape.nShapeRange[3] - info.oShape.nShapeRange[1];

					x = oglTopTable->at(i).nCenPos[0] - length / 2;
					y = 0;
					z = oglTopTable->at(i).nCenPos[1] + thickness / 2;

					InitAndDrawCuboid(x, y, z, length, thickness, height,1);

					//�洢
					//modelInfos[i].push_back();
				}

				//Բ��
				if (info.oShape.nShapeType == 2)
				{
					float x, y, z, radius, height;
					x = oglTopTable->at(i).nCenPos[0];
					y = oglTopTable->at(i).nCenPos[1];
					//y = 0.5;
					z = oglTopTable->at(i).nCenPos[1];
					height = oglTopTable->at(i).nCenPos[3];
					//height = 2;
					radius = info.oShape.nNumOrRadius;
					//radius = 1;
					InitAndDrawColumn(x, y, z, radius, height);
				}

				//�������
				if (info.oShape.nShapeType == 3)
				{
					float height = oglTopTable->at(i).nCenPos[3];
					InitAndDrawPolygonPortrait(info.oShape.vPolyPt, height);
				}
			}

			//��
			if (oglTopTable->at(i).nUnitType == 2)
			{
				QColor color = ColorHelper(2);
				ResourceManager::getShader("beam").use();
				ResourceManager::getShader("beam").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("beam").use().setInteger("R", color.red());
				ResourceManager::getShader("beam").use().setInteger("G", color.green());
				ResourceManager::getShader("beam").use().setInteger("B", color.blue());
				ResourceManager::getShader("beam").use().setFloat("tranS", transOfBeam);

				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);

				//����
				if (info.oShape.nShapeType == 1)
				{
					BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);

					float x, y, z, length, thickness, height;


					//���ĺ��
					thickness = info.oShape.nShapeRange[3]- info.oShape.nShapeRange[1];
					//���ĸ߶�
					height = info.oShape.nShapeRange[2] - info.oShape.nShapeRange[0];
					//����ǽ���ӵ�������֮��ľ���
					vPoint columnPoints;		//��¼�����������ĵ�
					vector<BasicUnit> Columns;	//ǽ���ӵ�����
					int j = 0;
					while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
						{

							BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
							Columns.push_back(tmpinfo);

							Point info;
							info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
							info.y = 0;
							info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
							columnPoints.push_back(info);
						}
						
						j++;
					}
					//���ȼ�ȥ�������Ŀ�ȵ�һ��
					length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
					length -= (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2;
					length -= (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;


					//�Ƕȼ���
					if (columnPoints[0].z == columnPoints[1].z) { oglTopTable->at(i).nUnitAngle = 0; }
					if (columnPoints[0].x == columnPoints[1].x) { oglTopTable->at(i).nUnitAngle = 90; }

					if (oglTopTable->at(i).nUnitAngle == 0)
					{
						//���������˹�ϵ����
						if (columnPoints[0].x < columnPoints[1].x)
						{
							x = columnPoints[0].x + (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2;
							y = columnHeight - height;
							z = columnPoints[0].z + (Columns[0].oShape.nShapeRange[3] - Columns[0].oShape.nShapeRange[1]) / 2;
							InitAndDrawCuboid(x, y, z, length, thickness, height,2);
						}
						else
						{
							x = columnPoints[1].x + (Columns[1].oShape.nShapeRange[2] - Columns[1].oShape.nShapeRange[0]) / 2;
							y = columnHeight - height;
							z = columnPoints[1].z + (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;
							InitAndDrawCuboid(x, y, z, length, thickness, height,2);
						}

					}
					if (oglTopTable->at(i).nUnitAngle == 90)
					{
						if (columnPoints[0].z < columnPoints[1].z)
						{
							x = columnPoints[0].x - (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2;
							y = columnHeight - height;
							z = columnPoints[1].z - (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;
							InitAndDrawCuboid(x, y, z, thickness, length, height,2);
						}
						else
						{
							x = columnPoints[1].x - (Columns[1].oShape.nShapeRange[2] - Columns[1].oShape.nShapeRange[0]) / 2;
							y = columnHeight - height;
							z = columnPoints[0].z - (Columns[0].oShape.nShapeRange[3] - Columns[0].oShape.nShapeRange[1]) / 2;
							InitAndDrawCuboid(x, y, z, thickness, length, height,2);
						}
					}



				}



				//�����
				if (info.oShape.nShapeType == 3)
				{
					float height = oglTopTable->at(i).nCenPos[3];
					//InitAndDrawPolygonColumnHorizontal(info.oShape.vPolyPt, height);
				}
			}

			//��  ps:Ŀǰ������˳ʱ����ȡ ������Ϣ��
			if (oglTopTable->at(i).nUnitType == 3)
			{
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);

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
				while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
				{
					if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
					{
						BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
						columns.push_back(tmpinfo);

						Point info;
						info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
						info.y = 0;
						info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
						columnPoints.push_back(info);
					}
					j++;
				}

				//��ͨ���������ΰ�
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

					x = columnPoints[3].x - (columns[3].oShape.nShapeRange[2] - columns[3].oShape.nShapeRange[0]) / 2;
					y = columnHeight;
					z = columnPoints[3].z + (columns[3].oShape.nShapeRange[3] - columns[3].oShape.nShapeRange[1]) / 2;

					InitAndDrawCuboid(x, y, z, length, width, thickness, 3);
				}
				//����ΰ�
				if (columns.size() > 4)
				{

				}
				
			}

			//ǽ
			if (oglTopTable->at(i).nUnitType == 4)
			{
				QColor color = ColorHelper(4);
				ResourceManager::getShader("wall").use();
				ResourceManager::getShader("wall").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("wall").use().setInteger("R", color.red());
				ResourceManager::getShader("wall").use().setInteger("G", color.green());
				ResourceManager::getShader("wall").use().setInteger("B", color.blue());
				ResourceManager::getShader("wall").use().setFloat("tranS", transOfWall);

				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
				
				float x, y, z, length, thickness, height;
				

				//ǽ�ĺ��
				thickness = info.oShape.nThickNess;

				//����ǽ���ӵ�������֮��ľ���
				vPoint columnPoints;		//��¼�����������ĵ�
				vector<BasicUnit> Columns;	//ǽ���ӵ�����
				int j = 0;
				while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
				{
					if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
					{
						
						BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
						Columns.push_back(tmpinfo);

						Point info;
						info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
						info.y = 0;
						info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
						columnPoints.push_back(info);
					}
					if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 2)//��
					{
						BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
						//ǽ�߶�
						//height = columnHeight - (tmpinfo.oShape.nShapeRange[2]- tmpinfo.oShape.nShapeRange[0]);//��ȥ���ĸ߶�
						height = columnHeight;
					}
					j++;
				}
				//���ȼ�ȥ�������Ŀ�ȵ�һ��
				length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
				length -= (Columns[0].oShape.nShapeRange[2]- Columns[0].oShape.nShapeRange[0])/2;
				length -= (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;
				

				//����ǽ�Ĳ�ͬ�Ƕ�������
				if (oglTopTable->at(i).nUnitAngle == 0)
				{
					//����ǽ���˹�ϵ����
					if (columnPoints[0].x < columnPoints[1].x)
					{
						x = columnPoints[0].x + (Columns[0].oShape.nShapeRange[2] - Columns[0].oShape.nShapeRange[0]) / 2;
						y = 0;
						z = columnPoints[0].z + thickness / 2;
						InitAndDrawCuboid(x, y, z, length, thickness, height,4);
					}
					else
					{
						x = columnPoints[1].x + (Columns[1].oShape.nShapeRange[2] - Columns[1].oShape.nShapeRange[0]) / 2;
						y = 0;
						z = columnPoints[1].z + thickness / 2;
						InitAndDrawCuboid(x, y, z, length, thickness, height,4);
					}
					
				}
				if (oglTopTable->at(i).nUnitAngle == 90)
				{
					if (columnPoints[0].z < columnPoints[1].z)
					{
						x = columnPoints[0].x - thickness / 2;
						y = 0;
						z = columnPoints[1].z - (Columns[1].oShape.nShapeRange[3] - Columns[1].oShape.nShapeRange[1]) / 2;
						InitAndDrawCuboid(x, y, z, thickness, length, height,4);
					}
					else
					{
						x = columnPoints[1].x - thickness / 2;
						y = 0;
						z = columnPoints[0].z - (Columns[0].oShape.nShapeRange[3] - Columns[0].oShape.nShapeRange[1]) / 2;
						InitAndDrawCuboid(x, y, z, thickness, length, height,4);
					}
				}
				

				
			}
			
			//��
			if (oglTopTable->at(i).nUnitType == 5)
			{
				QColor color = ColorHelper(5);
				ResourceManager::getShader("door").use();
				ResourceManager::getShader("door").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("door").use().setInteger("R", color.red());
				ResourceManager::getShader("door").use().setInteger("G", color.green());
				ResourceManager::getShader("door").use().setInteger("B", color.blue());
				ResourceManager::getShader("door").use().setFloat("tranS", transOfDoor);

				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
				float x, y, z, length, thickness, height;
				length = info.oShape.nShapeRange[0];
				height = info.oShape.nShapeRange[1];


				//�õ���Ӧ��ǽid��ǽ��Ϣ  ȥ�������Ӧ���ŵ�����
				{
					
					int wallId = oglTopTable->at(i).nAdjUnitIdx[0];
					//����ǽ���ӵ�������֮��ľ���
					vPoint columnPoints;		//��¼�����������ĵ�
					vector<BasicUnit> Columns;	//ǽ���ӵ�����
					int j = 0;
					while (oglTopTable->at(wallId).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nUnitType == 1)
						{

							BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
							Columns.push_back(tmpinfo);

							Point info;
							info.x = oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nCenPos[0];
							info.y = 0;
							info.z = oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nCenPos[1];
							columnPoints.push_back(info);
						}

						j++;
					}

					//��Ӧǽ����Ϣ
					BasicUnit tmpWallInfo = findUnit(oglTopTable->at(wallId).nCenUnitIdx, *oglUnitTable);
					
					thickness = tmpWallInfo.oShape.nThickNess;//�ź����ǽһ��

					//ǽ��Ӧ�ǶȲ�ͬ����Ӧ��ͬ���
					int wallX, wallY, wallZ;
					if (oglTopTable->at(wallId).nUnitAngle == 0)
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

						//�ŵ�����
						x = wallX + oglTopTable->at(i).nCenPos[0];
						y = oglTopTable->at(i).nCenPos[1];
						z = wallZ;

						InitAndDrawCuboid(x, y, z, length, thickness, height,5);
					}
					if (oglTopTable->at(wallId).nUnitAngle == 90)
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

						//�ŵ�����
						x = wallX;
						y = oglTopTable->at(i).nCenPos[1];
						z = wallZ - oglTopTable->at(i).nCenPos[0];

						InitAndDrawCuboid(x, y, z, thickness, length, height,5);
					}

				}
				
			}

			//��
			if (oglTopTable->at(i).nUnitType == 6)
			{
				QColor color = ColorHelper(6);
				ResourceManager::getShader("window").use();
				ResourceManager::getShader("window").use().setMatrix4f("model", targetModel);
				ResourceManager::getShader("window").use().setInteger("R", color.red());
				ResourceManager::getShader("window").use().setInteger("G", color.green());
				ResourceManager::getShader("window").use().setInteger("B", color.blue());
				ResourceManager::getShader("window").use().setFloat("tranS", transOfWindow);

				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
				float x, y, z, length, thickness, height;
				length = info.oShape.nShapeRange[0];
				height = info.oShape.nShapeRange[1];


				//�õ���Ӧ��ǽid��ǽ��Ϣ  ȥ�������Ӧ�Ĵ�������
				{

					int wallId = oglTopTable->at(i).nAdjUnitIdx[0];
					//����ǽ���ӵ�������֮��ľ���
					vPoint columnPoints;		//��¼�����������ĵ�
					vector<BasicUnit> Columns;	//ǽ���ӵ�����
					int j = 0;
					while (oglTopTable->at(wallId).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nUnitType == 1)
						{

							BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
							Columns.push_back(tmpinfo);

							Point info;
							info.x = oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nCenPos[0];
							info.y = 0;
							info.z = oglTopTable->at(oglTopTable->at(wallId).nAdjUnitIdx[j]).nCenPos[1];
							columnPoints.push_back(info);
						}

						j++;
					}

					//��Ӧǽ����Ϣ
					BasicUnit tmpWallInfo = findUnit(oglTopTable->at(wallId).nCenUnitIdx, *oglUnitTable);
					
					thickness = tmpWallInfo.oShape.nThickNess;//�������ǽһ��

					//ǽ��Ӧ�ǶȲ�ͬ����Ӧ��ͬ���
					int wallX, wallY, wallZ;
					if (oglTopTable->at(wallId).nUnitAngle == 0)
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

						//��������
						x = wallX + oglTopTable->at(i).nCenPos[0];
						y = oglTopTable->at(i).nCenPos[1];
						z = wallZ;

						InitAndDrawCuboid(x, y, z, length, thickness, height,6);
					}
					if (oglTopTable->at(wallId).nUnitAngle == 90)
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

						//��������
						x = wallX;
						y = oglTopTable->at(i).nCenPos[1];
						z = wallZ - oglTopTable->at(i).nCenPos[0];

						InitAndDrawCuboid(x, y, z, thickness, length, height,6);
					}

				}
			}
		}

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

	QMatrix4x4 projection, view;
	GLfloat a = width();
	GLfloat b = height();
	projection.perspective(camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 2000.f);
	//projection.frustum(-10000, 10000, -10000, 10000, -10000, 10000);
	//projection.ortho(-1000,1000,-1000,1000,-1000,10000);

	switch (switchView)
	{
	case FRONT_VIEW:
		view.lookAt(QVector3D(0, 0, 5), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//ǰ��ͼ
		break;
	case BACK_VIEW:
		view.lookAt(QVector3D(0, 0, -5), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//����ͼ
		break;
	case LEFT_VIEW:
		view.lookAt(QVector3D(-5, 0, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//����ͼ
		break;
	case RIGHT_VIEW:
		view.lookAt(QVector3D(5, 0, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));						//����ͼ
		break;
	case TOP_VIEW:
		view.lookAt(QVector3D(0, 5, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, -90.0f));					//����ͼ
		break;
	case BOTTOM_VIEW:
		view.lookAt(QVector3D(0, -5, 0), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 90.0f));					//����ͼ
		break;
	default:
		view = camera->getViewMatrix();
		break;
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


void ParaOGLManager::InitAndDrawColumn(float x, float y, float z, float radius, float height)
{
	//Բ��ͨ������Բ��������ʵ��

	int pointNum = 4320;//ÿ��Բ�ĵ���������ȣ�

	float* vertices = new float[pointNum * 6];

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
	pCore->glBufferData(GL_ARRAY_BUFFER, pointNum * 6 * sizeof(float), vertices, GL_STATIC_DRAW);


	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


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

void ParaOGLManager::InitAndDrawCuboid(float x, float y, float z, float length, float thickness, float height,int type)
{
	//init
	float* vertices = new float[72];

	vPoint points;
	Point tmp;
	tmp.x = x, tmp.y = y, tmp.z = z; points.push_back(tmp);
	tmp.x = x + length, tmp.y = y, tmp.z = z; points.push_back(tmp);
	tmp.x = x + length, tmp.y = y, tmp.z = z - thickness; points.push_back(tmp);
	tmp.x = x, tmp.y = y, tmp.z = z - thickness; points.push_back(tmp);
	tmp.x = x, tmp.y = y + height, tmp.z = z; points.push_back(tmp);
	tmp.x = x + length, tmp.y = y + height, tmp.z = z; points.push_back(tmp);
	tmp.x = x + length, tmp.y = y + height, tmp.z = z - thickness; points.push_back(tmp);
	tmp.x = x, tmp.y = y + height, tmp.z = z - thickness; points.push_back(tmp);

	int solidToFaceOrder[] = { 0,1,2,3,0,1,5,4,1,2,6,5,3,2,6,7,0,3,7,4,4,5,6,7 };

	//6��������Ƭ 24��������

	for (int k = 0; k < 24; k++)
	{
		vertices[k * 3] = points[solidToFaceOrder[k]].x;
		vertices[k * 3 + 1] = points[solidToFaceOrder[k]].y;
		vertices[k * 3 + 2] = points[solidToFaceOrder[k]].z;
	}


	pCore->glGenBuffers(1, &VBO);

	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), vertices, GL_STATIC_DRAW);


	
	//draw
	pCore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pCore->glEnableVertexAttribArray(0);
	pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//�����Ŵ��ǲ�͸���ģ����Զ������Ͳ�����Ȼ���
	if (type != 1 && type != 2 /*&& type != 3 && type != 5 && type != 6*/) { pCore->glDepthMask(GL_FALSE); }//ȡ����Ȼ���
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	pCore->glDrawArrays(GL_QUADS, 0, 24);

	pCore->glDisable(GL_BLEND);
	if (type != 1 && type != 2/* && type != 3 && type != 5 && type != 6*/) { pCore->glDepthMask(GL_TRUE); }



	
	//delete
	pCore->glDeleteBuffers(1, &VBO);
	delete[] vertices;
}

BasicUnit ParaOGLManager::findUnit(int idx, VUNITTABLE oglUnitTable)
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

void ParaOGLManager::InitAndDrawPolygonPortrait(VINT data, float height)
{

	float* vertices = new float[data.size() * 1.5 * 2];

	//�Ȼ���������
	for (int i = 0; i < data.size(); i += 2)
	{
		vertices[i * 6] = data[i];
		vertices[i * 6 + 1] = 0;
		vertices[i * 6 + 2] = data[i + 1];

		vertices[i * 6 + 3] = vertices[i * 6];
		vertices[i * 6 + 4] = 0 + height;
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




	//�ٻ�������������
}

void ParaOGLManager::InitAndDrawPolygonHorizontal(VINT data, float length)
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










