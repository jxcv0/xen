#version 460 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out vec3 frag_pos;
out vec2 tex_coord;
out vec3 normal;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vec3 T = normalize(vec3(model * vec4(in_tangent, 0.0f)));
	vec3 B = normalize(vec3(model * vec4(in_bitangent, 0.0f)));
	vec3 N = normalize(vec3(model * vec4(in_normal, 0.0f)));
	TBN = mat3(T, B, N);

	frag_pos = vec3(model * vec4(in_pos, 1.0f));
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
	tex_coord = in_tex_coord;
	normal = in_normal;
}
