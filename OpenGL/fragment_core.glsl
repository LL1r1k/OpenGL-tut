#version 450

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord);
}