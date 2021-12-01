#ifndef SHADERPROC_H
#define SHADERPROC_H

#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QString>

class CShaderProc
{
	friend class ResourceManager;
public:
	CShaderProc() {}
	void compile(const QString& vertexSource, const QString& fragmentSource, const QString& geometrySource = NULL);
	//  Shader(const QString& vertexSourcePath, const QString& fragmentSourcePath);

	~CShaderProc();

	GLuint getShaderID()
	{
		return shaderProgram->programId();
	}
	CShaderProc& use()
	{
		shaderProgram->bind();
		return *this;
	}

	//设置着色器中的uniform变量 下同
	void setFloat(const QString& name, const GLfloat& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}

	void setFloatArray(const QString& name, const GLfloat* &value ,const int size)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValueArray(loc,value,size,1);
	}

	void setInteger(const QString& name, const GLint& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}

	void setVector2f(const QString& name, const GLfloat& x, const GLfloat& y)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, QVector2D(x, y));
	}

	void setVector2f(const QString& name, const QVector2D& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}

	void setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, QVector3D(x, y, z));
	}

	void setVector3f(const QString& name, const QVector3D& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}

	void setVector4f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, QVector4D(x, y, z, w));
	}

	void setVector4f(const QString& name, const QVector4D& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}

	void setMatrix4f(const QString& name, const QMatrix4x4& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}

	void setBool(const QString& name, const GLboolean& value)
	{
		GLuint loc = shaderProgram->uniformLocation(name);
		shaderProgram->setUniformValue(loc, value);
	}
private:
	QOpenGLShaderProgram *shaderProgram;

};

#endif // SHADERPROC_H
