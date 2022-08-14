#version 460 core
layout (location = 0) in vec3 in_pos;
// TODO layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_col;

// TODO out vec3 frag_pos;
out vec3 frag_col;
// TODO out vec3 frag_norm;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	// TODO frag_pos = vec3(model * vec4(in_pos, 1.0f));
	frag_col = in_col;
	// TODO frag_norm = in_norm;
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
}

