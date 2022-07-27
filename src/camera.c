#include "camera.h"
#include "window.h"
#include "maths.h"

static vec3_t camera_pos = { .values = {0.0f, 0.0f, 0.0f} };
static vec3_t camera_dir = { .values = {0.0f, 0.0f, -1.0f} };
static vec3_t camera_up = { .values = {0.0f, 1.0f, 0.0f} };
static vec3_t camera_right = { .values = {1.0f, 0.0f, 0.0f} };
static vec3_t world_up = { .values = {0.0f, 1.0f, 0.0f} };
static bool first_mouse = true;
static float camera_rot_a = 0.0f;  // rotation about x axis
static float camera_rot_b = -90.0f; // rotation about y axis
static float prev_x = 0;
static float prev_y = 0;
static float offset_rad = 5.0f;

// set the camera update function
void set_camera_update_callback(GLFWcursorposfun callback)
{
	glfwSetCursorPosCallback(window, callback);
}

// create view matrix
mat4_t gen_view_matrix(void)
{
	return look_at(camera_dir, camera_pos, camera_up);
}

// update the camera direction based on a change in mouse position
// default version rotates about world origin
void camera_update_dir(GLFWwindow* window, double x, double y)
{
	float mouse_x = (float)x;
	float mouse_y = (float)y;

	if (first_mouse) {
		prev_x = mouse_x;
		prev_y = mouse_y;
		first_mouse = false;
	}

	float delta_x = mouse_x - prev_x;
	float delta_y = prev_y - mouse_y;

	prev_x = mouse_x;
	prev_y = mouse_y;

	// TODO make sensitivity adjustable
	camera_rot_b += delta_x * 0.1f;
	camera_rot_a += delta_y * 0.1f;

	if (camera_rot_a > 89.0f) { camera_rot_a = 89.0f; }
	if (camera_rot_a < -89.0f) { camera_rot_a = -89.0f; }

	float rads_a = radians(camera_rot_a);
	float rads_b = radians(camera_rot_b);

	camera_pos = construct_vec3(-offset_rad * (cos(rads_b) * cos(rads_a)),
				-offset_rad * (sin(rads_a)),
				-offset_rad * (sin(rads_b) * cos(rads_a)));

	camera_pos.values[1] += 1.5f;

	camera_dir = construct_vec3(cos(rads_b) * cos(rads_a),
				    sin(rads_a),
				    sin(rads_b) * cos(rads_a));

	camera_dir = normalize_vec3(camera_dir);
	camera_right = normalize_vec3(cross_vec3(world_up, camera_dir));
	camera_up = normalize_vec3(cross_vec3(camera_dir, camera_right));
}

// update the camera direction based on a change in mouse position
// debug version rotates about camera_pos
void camera_update_dir_debug(GLFWwindow* window, double x, double y)
{
	float mouse_x = (float)x;
	float mouse_y = (float)y;

	if (first_mouse) {
		prev_x = mouse_x;
		prev_y = mouse_y;
		first_mouse = false;
	}

	float delta_x = mouse_x - prev_x;
	float delta_y = prev_y - mouse_y;

	prev_x = mouse_x;
	prev_y = mouse_y;

	// TODO make sensitivity adjustable
	camera_rot_b += delta_x * 0.1f;
	camera_rot_a += delta_y * 0.1f;

	if (camera_rot_a > 89.0f) { camera_rot_a = 89.0f; }
	if (camera_rot_a < -89.0f) { camera_rot_a = -89.0f; }

	float rads_a = radians(camera_rot_a);
	float rads_b = radians(camera_rot_b);

	camera_dir = construct_vec3(cos(rads_b) * cos(rads_a),
				    sin(rads_a),
				    sin(rads_b) * cos(rads_a));

	camera_dir = normalize_vec3(camera_dir);
	camera_right = normalize_vec3(cross_vec3(world_up, camera_dir));
	camera_up = normalize_vec3(cross_vec3(camera_dir, camera_right));
}
