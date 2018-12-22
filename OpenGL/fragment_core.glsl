#version 450

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

vec3 calcAmbient()
{
	return material.ambient;
}

vec3 calcDiffuse()
{
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	return material.diffuse * diffuse;
}

vec3 calcSpecular()
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos- vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30);
	return material.specular * specularConstant;
}
void main()
{
	//Attenuation

	fs_color = texture(material.diffuseTex, vs_texcoord) *
	(vec4(calcAmbient(), 1.f) + vec4(calcDiffuse(), 1.f) + vec4(calcSpecular(), 1.f));
}