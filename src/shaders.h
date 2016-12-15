#pragma once
const char* vertex_shader =
R"zzz(
#version 330 core
uniform vec4 light_position;
uniform vec3 camera_position;
in vec4 vertex_position;
in vec4 normal;
in vec2 uv;
out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;
out vec4 diffuse;
void main() {
	gl_Position = vertex_position;
	vs_light_direction = light_position - gl_Position;
	vs_camera_direction = vec4(camera_position, 1.0) - gl_Position;
	vs_normal = normal;
	vs_uv = uv;
	diffuse = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
)zzz";

const char* geometry_shader =
R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 light_position;
in vec4 vs_light_direction[];
in vec4 vs_camera_direction[];
in vec4 vs_normal[];
in vec2 vs_uv[];
in vec4 diffuse[];
out vec4 face_normal;
out vec4 light_direction;
out vec4 camera_direction;
out vec4 world_position;
out vec4 vertex_normal;
out vec2 uv_coords;
out vec4 g_diffuse;
void main() {
	int n = 0;
	vec3 a = gl_in[0].gl_Position.xyz;
	vec3 b = gl_in[1].gl_Position.xyz;
	vec3 c = gl_in[2].gl_Position.xyz;
	vec3 u = normalize(b - a);
	vec3 v = normalize(c - a);
	face_normal = normalize(vec4(normalize(cross(u, v)), 0.0));
	for (n = 0; n < gl_in.length(); n++) {
		light_direction = normalize(vs_light_direction[n]);
		camera_direction = normalize(vs_camera_direction[n]);
		g_diffuse = diffuse[n];
		world_position = gl_in[n].gl_Position;
		vertex_normal = vs_normal[n];
		uv_coords = vs_uv[n];
		gl_Position = projection * view * model * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz";

const char* fragment_shader =
R"zzz(
#version 330 core
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 camera_direction;
in vec2 uv_coords;
in vec4 g_diffuse;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;
uniform float alpha;
uniform sampler2D textureSampler;
out vec4 fragment_color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main() {
	vec3 texcolor = texture(textureSampler, uv_coords).xyz;
	if (length(texcolor) == 0.0) {
		//vec3 color = vec3(0.0, 1.0, 0.0);
		//vec3 color = vec3(g_diffuse);
		vec3 color = vec3(g_diffuse);
		//vec2 randuv = vec2(rand(light_direction.xy), rand(light_direction.zw));
		//vec3 color = vec3(g_diffuse) + texture(textureSampler, randuv).xyz;
		//vec3 color = texture(textureSampler, randuv).xyz;
		//vec3 color = vec3(g_diffuse) + vec3(randuv.x, randuv.y, 1.0);
		float dot_nl = dot(normalize(-light_direction), normalize(vertex_normal));
		dot_nl = clamp(dot_nl, 0.0, 1.0);
		vec4 spec = specular * pow(max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction)), shininess);
		color = clamp(dot_nl * color + color * vec3(ambient) + vec3(spec), 0.0, 1.0);
		fragment_color = vec4(color, 1.0);
	} else {
		fragment_color = vec4(texcolor.rgb, 1.0);
	}
	//fragment_color = g_diffuse;
}
)zzz";

const char* floor_fragment_shader =
R"zzz(
#version 330 core
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 world_position;
out vec4 fragment_color;
void main() {
	vec4 pos = world_position;
	float check_width = 5.0;
	float i = floor(pos.x / check_width);
	float j  = floor(pos.z / check_width);
	vec3 color = mod(i + j, 2) * vec3(1.0, 1.0, 1.0);
	float dot_nl = dot(normalize(light_direction), normalize(face_normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
}
)zzz";