#include "Material.h"

Material::Material(vec3 ambient, vec3 diffuse, vec3 specular, GLint diffuseTex, GLint specularTex)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	if(diffuseTex)
		this->diffuseTex = diffuseTex;
	if (specularTex)
		this->specularTex = specularTex;
}

Material::~Material()
{

}

void Material::sendToShader(Shader& programID)
{
	programID.setVec3f(ambient, "material.ambient");
	programID.setVec3f(diffuse, "material.diffuse");
	programID.setVec3f(specular, "material.specular");
	if (diffuseTex)
		programID.set1i(diffuseTex, "material.diffuseTex");
	if (specularTex)
	programID.set1i(specularTex, "material.specularTex");
}
