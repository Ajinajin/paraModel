#include "resourcemanager.h"

std::map<QString, CShaderProc> ResourceManager::map_Shaders;
//std::map<QString, Texture2D> ResourceManager::map_Textures;

CShaderProc ResourceManager::loadShader(const QString& name, const QString& vShaderFile, const QString& fShaderFile, const QString& gShaderfile)
{
	CShaderProc shader;
	shader.compile(vShaderFile, fShaderFile, gShaderfile == NULL ? NULL : gShaderfile);
	map_Shaders[name] = shader;
	return map_Shaders[name];
}

CShaderProc ResourceManager::getShader(const QString&  name)
{
	return map_Shaders[name];
}

//Texture2D ResourceManager::loadTexture(const QString&  name, const QString& file, GLboolean alpha)
//{
//	Texture2D texture;
//
//	if (alpha)
//	{
//		texture.internal_format = QOpenGLTexture::RGBAFormat;
//		texture.wrap_s = QOpenGLTexture::ClampToBorder; //纹理的边界部分 不处理。
//		texture.wrap_t = QOpenGLTexture::ClampToBorder;
//	}
//
//	texture.generate(file);
//	map_Textures[name] = texture;
//	return texture;
//}

//Texture2D ResourceManager::getTexture(const QString&  name)
//{
//	return map_Textures[name];
//}

void ResourceManager::clear()
{
	for (std::map<QString, CShaderProc>::iterator iter = map_Shaders.begin(); iter != map_Shaders.end(); ++iter)
		delete iter->second.shaderProgram;
	/*for (std::map<QString, Texture2D>::iterator iter = map_Textures.begin(); iter != map_Textures.end(); ++iter)
		delete iter->second.texture;*/

}
