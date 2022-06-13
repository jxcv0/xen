#version 460 core
out vec4 frag_col;

in vec3 frag_pos;
in vec2 tex_coord;
in vec3 normal;
in mat3 TBN;

// each renderable object only has one material
uniform sampler2D tex_diff;
uniform sampler2D tex_spec;
uniform sampler2D tex_norm;
uniform float shininess;

uniform vec3 view_pos;

struct light_t
{
	vec3 position;
	vec3 color;

	// attenuation constants
	float constant;
	float linear;
	float quadratic;
};
uniform light_t light;

// calculate on omnidirectional light
vec3 calc_light(light_t light, vec3 normal, vec3 view_dir)
{
	
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic + (distance * distance));

	// ambient
	float ambientStrength = 0.1;	// get this from where?
	vec3 ambient = ambientStrength * light.color;

	// extra normal normals
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light.position - frag_pos);

	// diffuse
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light.color * texture(tex_diff, tex_coord).rgb;

	// specular
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
	vec3 specular = spec * light.color * texture(tex_spec, tex_coord).rgb;

	return (ambient + diffuse + specular) * attenuation;
}

void main()
{
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 normal = texture(tex_norm, tex_coord).rgb;
	normal = (normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 result = calc_light(light, normal, view_dir);
    frag_col = vec4(result, 1.0);
	frag_col = vec4(1.0, 1.0, 1.0, 1.0);
}
