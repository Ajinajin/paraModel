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

ParaOGLManager::ParaOGLManager(QWidget* parent) : QOpenGLWidget(parent)
{
	this->setGeometry(10, 0, OGLMANAGER_WIDTH, OGLMANAGER_HEIGHT);
}

ParaOGLManager::~ParaOGLManager() {
	if (camera)
		delete camera;

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

	/*********** ��������֡��֮���ʱ����  ***************/
	GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	this->processInput(deltaTime);
	//update
	this->updateGL();


	ResourceManager::getShader("coordinate").use();
	ResourceManager::getShader("coordinate").use().setMatrix4f("model", targetModel);


	//������άģ��
	if (!oglUnitTable->empty() && !oglTopTable->empty())
	{
		for (int i = 0; i < oglTopTable->size(); i++)
		{
			//**�жϴ˹�������**

			//��
			if (oglTopTable->at(i).nUnitType == 1)
			{
				//���Ҷ�Ӧ���������Ϣ
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
				
				//����
				if (info.oShape.nShapeType == 1)
				{
					float x, y, z, length, thickness, height;

					height = oglTopTable->at(i).nCenPos[3];
					length = info.oShape.nShapeRange[2] - info.oShape.nShapeRange[0];
					thickness = info.oShape.nShapeRange[3] - info.oShape.nShapeRange[1];

					x = oglTopTable->at(i).nCenPos[0];
					y = oglTopTable->at(i).nCenPos[1];
					z = oglTopTable->at(i).nCenPos[2];

					InitAndDrawCuboid(x, y, z, length, thickness, height);
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
					InitAndDrawPolygonColumnPortrait(info.oShape.vPolyPt,height);
				}
			}
			
			//��
			if (oglTopTable->at(i).nUnitType == 2)
			{
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
				
				//����
				if (info.oShape.nShapeType == 1)
				{
					float x, y, z, length, thickness, height;
					
					height = oglTopTable->at(i).nCenPos[3];

					//���ĺ��
					//thickness = info.oShape.nThickNess;
					thickness = 1;

					//���������ӵ�������֮��ľ���
					vPoint columnPoints;
					int j = 0;
					while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
						{
							Point info;
							info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
							info.y = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
							info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[2];
							columnPoints.push_back(info);
						}
						j++;
					}


					x = oglTopTable->at(i).nCenPos[0];
					y = oglTopTable->at(i).nCenPos[1];
					z = oglTopTable->at(i).nCenPos[2];

					length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);

					//InitAndDrawCuboid(x, y, z, length, thickness, height);
				}
				
				
				
				//�����
				if (info.oShape.nShapeType == 3)
				{
					float height = oglTopTable->at(i).nCenPos[3];
					//InitAndDrawPolygonColumnHorizontal(info.oShape.vPolyPt, height);
				}
			}

			//��
			if (oglTopTable->at(i).nUnitType == 3)
			{
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
			
				float x, y, z, length, width, thickness;
				
				//height = oglTopTable->at(i).nCenPos[3];


				//�洢�����ӵ��ĸ���֮�����Ϣ
				vPoint columnPoints;
				int j = 0;
				while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
				{
					if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
					{
						Point info;
						info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
						info.y = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
						info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[2];
						columnPoints.push_back(info);
					}
					j++;
				}

				//��ĺ��
				thickness = info.oShape.nThickNess;

				length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
				width = Distance(columnPoints[1].x, columnPoints[1].y, columnPoints[1].z, columnPoints[2].x, columnPoints[2].y, columnPoints[2].z);
				
				x = oglTopTable->at(i).nCenPos[0];
				y = oglTopTable->at(i).nCenPos[1];
				z = oglTopTable->at(i).nCenPos[2];
				
				//InitAndDrawCuboid(x, y, z, length, width, thickness);
			}

			//ǽ
			if (oglTopTable->at(i).nUnitType == 4)
			{
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);
				//��ǽ   0��
				if (oglTopTable->at(i).nUnitAngle == 0)
				{
					float x, y, z, length, thickness, height;

					height = oglTopTable->at(i).nCenPos[3];

					//ǽ�ĺ��

					thickness = info.oShape.nThickNess;

					//����ǽ���ӵ�������֮��ľ���
					vPoint columnPoints;
					int j = 0, temp_length;
					while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
						{
							BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
							//ǽ�������ټ�ȥһ�������
							temp_length = tmpinfo.oShape.nShapeRange[2] - tmpinfo.oShape.nShapeRange[0];

							Point info;
							info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
							info.y = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
							info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[2];
							columnPoints.push_back(info);
						}
						j++;
					}

					length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
					length -= temp_length;

					x = oglTopTable->at(i).nCenPos[0];
					y = oglTopTable->at(i).nCenPos[1];
					z = oglTopTable->at(i).nCenPos[2];
					InitAndDrawCuboid(x, y, z, length, thickness, height);

				}
				//��ǽ   90��
				if (oglTopTable->at(i).nUnitAngle == 90)
				{
					float x, y, z, length, thickness, height;

					height = oglTopTable->at(i).nCenPos[3];

					//ǽ�ĺ��

					thickness = info.oShape.nThickNess;

					//����ǽ���ӵ�������֮��ľ���
					vPoint columnPoints;
					int j = 0, temp_length;
					while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
					{
						if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
						{
							BasicUnit tmpinfo = findUnit(oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenUnitIdx, *oglUnitTable);
							//ǽ�������ټ�ȥһ�������
							temp_length = tmpinfo.oShape.nShapeRange[2] - tmpinfo.oShape.nShapeRange[0];

							Point info;
							info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
							info.y = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
							info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[2];
							columnPoints.push_back(info);
						}
						j++;
					}

					length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);
					length -= temp_length;

					x = oglTopTable->at(i).nCenPos[0];
					y = oglTopTable->at(i).nCenPos[1];
					z = oglTopTable->at(i).nCenPos[2];
					InitAndDrawCuboid(x, y, z, thickness, length, height);

				}
				
			}

			//��
			if (oglTopTable->at(i).nUnitType == 5)
			{
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);

				float x, y, z, length, thickness, height;

				//length = oglTopTable->at(i).nCenPos[3];
				length = 5;

				//�ŵĺ��
				//thickness = info.oShape.nThickNess;
				thickness = 0.5;
				
				//vPoint columnPoints;
				//int j = 0;
				//while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
				//{
				//	if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
				//	{
				//		Point info;
				//		info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
				//		info.y = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
				//		info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[2];
				//		columnPoints.push_back(info);
				//	}
				//	j++;
				//}

				//length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);

				height = 10;
				x = oglTopTable->at(i).nCenPos[0];
				y = oglTopTable->at(i).nCenPos[1];
				z = oglTopTable->at(i).nCenPos[2];
				//InitAndDrawCuboid(x, y, z, length, thickness, height);
				//InitAndDrawCuboid(x, y, z, length, thickness, thickness);
			}

			//��
			if (oglTopTable->at(i).nUnitType == 6)
			{
				BasicUnit info = findUnit(oglTopTable->at(i).nCenUnitIdx, *oglUnitTable);

				float x, y, z, length, thickness, height;

				//length = oglTopTable->at(i).nCenPos[3];
				length = 5;

				//���ĺ��

				//thickness = info.oShape.nThickNess;
				thickness = 0.5;
				
				//vPoint columnPoints;
				//int j = 0;
				//while (oglTopTable->at(i).nAdjUnitIdx[j] != -1)
				//{
				//	if (oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nUnitType == 1)
				//	{
				//		Point info;
				//		info.x = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[0];
				//		info.y = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[1];
				//		info.z = oglTopTable->at(oglTopTable->at(i).nAdjUnitIdx[j]).nCenPos[2];
				//		columnPoints.push_back(info);
				//	}
				//	j++;
				//}

				//length = Distance(columnPoints[0].x, columnPoints[0].y, columnPoints[0].z, columnPoints[1].x, columnPoints[1].y, columnPoints[1].z);

				height = 10;
				x = oglTopTable->at(i).nCenPos[0];
				y = oglTopTable->at(i).nCenPos[1] - height / 2;
				z = oglTopTable->at(i).nCenPos[2] + thickness / 2;
				//InitAndDrawCuboid(x, y, z, length, thickness, height);
				//InitAndDrawCuboid(x, y, z, height, thickness, height);
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
	

	
	//GLfloat currentFrame = (GLfloat)time.elapsed() / 100;
	//QMatrix4x4 model;
	//model.setToIdentity();
	
	//model.rotate(currentFrame * 5, QVector3D(0.0f, 0.0f, 1.0f));
	//ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);
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

		
		rotateRaw += xoffset*0.1;
		rotatePitch += yoffset*0.1;

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


void ParaOGLManager::InitAndDrawColumn(float x,float y, float z, float radius, float height)
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

void ParaOGLManager::InitAndDrawCuboid(float x, float y, float z, float length, float thickness, float height)
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

	int solidToFaceOrder[] = {0,1,2,3,0,1,5,4,1,2,6,5,3,2,6,7,0,3,7,4,4,5,6,7};

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

	
	pCore->glDepthMask(GL_FALSE);//ȡ����Ȼ���
	pCore->glEnable(GL_BLEND);//������ɫ���
	pCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//alphaֵ����

	pCore->glDrawArrays(GL_QUADS, 0, 24);

	pCore->glDisable(GL_BLEND);
	pCore->glDepthMask(GL_TRUE);
		

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

void ParaOGLManager::InitAndDrawPolygonColumnPortrait(VINT data, float height)
{

	float* vertices = new float[data.size() * 1.5 * 2];

	for (int i = 0; i < data.size(); i+=2)
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
}

void ParaOGLManager::InitAndDrawPolygonColumnHorizontal(VINT data, float length)
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










