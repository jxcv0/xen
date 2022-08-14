#version 460 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_texcoord;

out vec3 frag_pos;
out vec3 frag_norm;
out vec3 tex_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	frag_pos = vec3(model * vec4(in_pos, 1.0f));
	tex_pos = in_texcoord;
	frag_norm = in_norm;
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
}

