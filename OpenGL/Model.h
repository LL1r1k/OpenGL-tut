#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

class Model
{
public:
	Model(glm::vec3 position, Material* mat, Texture* texDif, Texture* texSpec, std::vector<Mesh*>& meshes);
	~Model();

	void update();
	void render(Shader* shader);

	void Rotate(const glm::vec3& rotation);

private:
	Material* material;
	Texture* textureDif;
	Texture* textureSpec;

	std::vector<Mesh*> meshes;
	glm::vec3 originPos;
};

