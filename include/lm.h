#ifndef VEC_H
#define VEC_H

#include <math.h>
#include <stdio.h>

// TODO
// rewrite all this 
// make tests

float radians(float x) { return x * 0.0174533; }

// wrapper types for arrays of values
typedef struct vec2_t
{
    float values[2];
} vec2_t;

typedef struct vec3_t
{
    float values[3];
} vec3_t;

typedef struct vec4_t
{
    float values[4];
} vec4_t;

typedef struct mat4_t
{
    float values[4][4];
} mat4_t;

// construct 4x4 identity matrix
mat4_t construct_identity_mat4()
{
    mat4_t mat4 = {0};
    mat4.values[0][0] = 1.0f;
    mat4.values[1][1] = 1.0f;
    mat4.values[2][2] = 1.0f;
    mat4.values[3][3] = 1.0f;
    return mat4;
}

// get a row of a 4x4 matrix as a vec4
vec4_t row(const float m[4][4], int n)
{
    vec4_t result = {
        .values = {m[n][0], m[n][1], m[n][2], m[n][3]}
    };
    return result;
}

// get a column of a 4x4 matrix as a vec4
vec4_t col(const float m[4][4], int n)
{
    vec4_t result = {
        .values = {m[0][n], m[1][n], m[2][n], m[3][n]}
    };
    return result;
}

float scalar(const float v[3], const float scalar)
{
    return (v[0] * scalar) + (v[1] * scalar) + (v[2] * scalar);
}

vec3_t add_vec3(float v1[3], float v2[3])
{
    vec3_t result = {
        .values = {(v1[0] + v2[0]), (v1[1] + v2[1]), (v1[2] + v2[2])}
    };
    return result;
}

float dot_v3(const float v1[3], const float v2[3])
{
    return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}

float dot_v4(const float v1[4], const float v2[4])
{
    return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]) + (v1[3] * v2[3]);
}

vec3_t cross_v3(const float v1[3], const float v2[0])
{
    vec3_t result = {0};

    result.x = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    result.x = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    result.x = (v1[0] * v2[1]) - (v1[1] * v2[2]);

    return result;
}

mat4_t cross_m4(const float m1[4][4], const float m2[4][4])
{
    mat4_t result = {0};

    result.values[0][0] = dot_v4(row(m1, 0).values, col(m2, 0).values);
    result.values[0][1] = dot_v4(row(m1, 0).values, col(m2, 1).values);
    result.values[0][2] = dot_v4(row(m1, 0).values, col(m2, 2).values);
    result.values[0][3] = dot_v4(row(m1, 0).values, col(m2, 3).values);

    result.values[1][0] = dot_v4(row(m1, 1).values, col(m2, 0).values);
    result.values[1][1] = dot_v4(row(m1, 1).values, col(m2, 1).values);
    result.values[1][2] = dot_v4(row(m1, 1).values, col(m2, 2).values);
    result.values[1][3] = dot_v4(row(m1, 1).values, col(m2, 3).values);

    result.values[2][0] = dot_v4(row(m1, 2).values, col(m2, 0).values);
    result.values[2][1] = dot_v4(row(m1, 2).values, col(m2, 1).values);
    result.values[2][2] = dot_v4(row(m1, 2).values, col(m2, 2).values);
    result.values[2][3] = dot_v4(row(m1, 2).values, col(m2, 3).values);

    result.values[3][0] = dot_v4(row(m1, 3).values, col(m2, 0).values);
    result.values[3][1] = dot_v4(row(m1, 3).values, col(m2, 1).values);
    result.values[3][2] = dot_v4(row(m1, 3).values, col(m2, 2).values);
    result.values[3][3] = dot_v4(row(m1, 3).values, col(m2, 3).values);

    return result;
}

// create perspective projection matrix
mat4_t create_perspective_matrix(const float fov, const float near, const float far, const float w, const float h)
{
    mat4_t result = {0};
    
    // TODO radians?

    result.values[0][0] = cos(0.5 * fov) / sin(0.5 * fov);
    result.values[1][1] = result.values[0][0] * h / w;
    result.values[2][2] = (near - far) / (far - near);
    result.values[2][3] = 1.0f;
    result.values[3][2] = (2.0f * far * near) / (far - near);

    return result;
}

// make the magnitude of a vec3 equal to 1
vec3_t normalize_v3(const float v[3])
{
    vec3_t result = {0};

    // TODO fast?
    float l = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    result.x = v[0] / l;
    result.x = v[1] / l;
    result.x = v[2] / l;

    return result;
}

mat4_t create_identity_matrix()
{
    mat4_t result;

    result.values[0][0] = 1.0f;
    result.values[1][1] = 1.0f;
    result.values[2][2] = 1.0f;
    result.values[3][3] = 1.0f;

    return result;
}

mat4_t create_translation_matrix(const float v[3])
{
    mat4_t result = create_identity_matrix();
    result.values[3][0] = v[0];
    result.values[3][1] = v[1];
    result.values[3][2] = v[2];
    return result;
}

mat4_t rotate_m4(const float m[4][4], const float axis[3], const float a)
{
    float rad = radians(a);
    float c = cos(rad);
    float s = sin(rad);
    float t = scalar(axis, (1 - c));

    vec3_t temp = { .values = {t, t, t} };

    mat4_t q = {0};
    q.values[0][0] = c + temp.x * axis[0];
    q.values[0][1] = 0 + temp.x * axis[1] + s * axis[2];
    q.values[0][2] = 0 + temp.x * axis[2] - s * axis[0];

    q.values[1][0] = 0 + temp.y * axis[0] - s * axis[2];
    q.values[1][1] = c + temp.y * axis[1];
    q.values[1][2] = 0 + temp.y * axis[2] + s * axis[0];

    q.values[2][0] = 0 + temp.z * axis[0] + s * axis[1];
    q.values[2][1] = 0 + temp.z * axis[1] - s * axis[0];
    q.values[2][2] = c + temp.z * axis[2];

    return cross_m4(m, q.values);
}

// create view matrix
mat4_t create_view_matrix(const float pos[3], const float eye[3], const float up[3])
{
    vec3_t diff = {
        .values = {pos[0] - eye[0], pos[1] - eye[1], pos[2] - eye[2]}
    };

    vec3_t f = normalize_v3(diff.values);
    vec3_t s = normalize_v3(cross_v3(up, eye).values);
    vec3_t u = cross_v3(f.values, s.values);
    
    mat4_t result;
    result.values[0][0] = s.x;
    result.values[1][0] = s.y;
    result.values[2][0] = s.z;
    result.values[3][0] = 1.0f;

    result.values[0][1] = u.x;
    result.values[1][1] = u.y;
    result.values[2][1] = u.z;
    result.values[3][1] = 1.0f;

    result.values[0][2] = f.x;
    result.values[1][2] = f.y;
    result.values[2][2] = f.z;
    result.values[3][2] = 1.0f;

    result.values[3][0] = -1 * dot_v3(s.values, eye);
    result.values[3][1] = -1 * dot_v3(u.values, eye);
    result.values[3][2] = -1 * dot_v3(s.values, eye);
    result.values[3][3] = 1.0f;

    return result;
}

#endif // VEC_H
