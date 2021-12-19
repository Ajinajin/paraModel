#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <QString>

#include "ShaderProc.h"


class ResourceManager
{
public:
	static std::map<QString, CShaderProc> map_Shaders;
	//static std::map<QString, Texture2D> map_Textures;
	static CShaderProc loadShader(const QString& name, const QString& vShaderFile, const QString& fShaderFile, const QString& gShaderfile = NULL);
	static CShaderProc getShader(const QString&  name);
	//static Texture2D loadTexture(const QString&  name, const QString& file, GLboolean alpha = false);
	//static Texture2D getTexture(const QString&  name);
	static void clear();
private:
	//虚函数，不要对象，只是用静态方法
	ResourceManager() {} 

};

#endif // RESOURCEMANAGER_H
