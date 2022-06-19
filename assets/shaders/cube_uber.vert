#version 460 core
layout (location = 0) in vec3 in_pos;
layout (location = 2) in vec3 in_normal;

out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{

	frag_pos = vec3(model * vec4(in_pos, 1.0f));
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
	normal = in_normal;
}
