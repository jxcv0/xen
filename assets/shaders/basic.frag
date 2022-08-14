#version 460 core

// TODO in vec3 frag_pos;
in vec3 frag_col;
// TODO in vec3 frag_norm;

out vec4 color;

void main()
{
	color = vec4(frag_col, 1.0f);
}

