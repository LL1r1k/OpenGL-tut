#include "Model.h"


Model::Model(glm::vec3 position, Material* mat, Texture* texDif, Texture* texSpec, std::vector<Mesh*>& meshes)
{
	originPos = position;
	material = mat;
	textureDif = texDif;
	textureSpec = texSpec;

	for (auto i : meshes)
		this->meshes.push_back(new Mesh(*i));
	for (auto& i : this->meshes)
		i->Move(position);
}

Model::~Model()
{
	for (auto& i : meshes)
		delete i;
	meshes.clear();
}

void Model::update()
{

}

void Model::render(Shader* shader)
{
	material->sendToShader(*shader);

	shader->use();

	textureDif->bind(0);
	textureSpec->bind(1);

	for (auto i : meshes)
	{
		i->render(shader);
	}
}

void Model::Rotate(const glm::vec3 & rotation)
{
	for (auto& i : meshes)
	{
		i->Rotate(rotation);
		i->setOrigin(originPos);
	}
		
}


