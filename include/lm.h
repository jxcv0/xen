#ifndef VEC_H
#define VEC_H

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdbool.h>

// TODO
// rewrite all this 
// make tests

float radians(float x) { return x * 0.0174533; }

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

// convenience printf function
void print_vec3(const vec3_t v)
{
    printf("| %f, %f, %f |\n", v.values[0], v.values[1], v.values[2]);
}

// convenience printf function
void print_vec4(const vec4_t v)
{
    printf("| %f, %f, %f, %f |\n", v.values[0], v.values[1], v.values[2], v.values[3]);
}

// construct a 3 dimensional vector
vec3_t construct_vec3(float x, float y, float z)
{
    vec3_t v = {
        .values = {x, y, z}
    };
    return v;
}

// construct a 4 dimensional vector
vec4_t construct_vec4(float x, float y, float z, float w)
{
    vec4_t v = {
        .values = {x, y, z, w}
    };
    return v;
}

// construct 4x4 matrix
mat4_t construct_mat4(float x)
{
    mat4_t m = {0};
    m.values[0][0] = x;
    m.values[1][1] = x;
    m.values[2][2] = x;
    m.values[3][3] = x;
    return m;
}

// get a row of a 4x4 matrix as a vec4
vec4_t row_mat4(const mat4_t m, int n)
{
    vec4_t v = construct_vec4(m.values[n][0], m.values[n][1], m.values[n][2], m.values[n][3]);
    return v;
}

// convenience printf function
void print_mat4(const mat4_t m)
{
    for (int i = 0; i < 4; i++)
    {
        printf("| %.3f, %.3f, %.3f, %.3f |\n",
            m.values[i][0], m.values[i][1], m.values[i][2], m.values[i][3]);
    }
}

// get a column of a 4x4 matrix as a vec4
vec4_t col_mat4(const mat4_t m, int n)
{
    vec4_t v = construct_vec4(m.values[0][n], m.values[1][n], m.values[2][n], m.values[3][n]);
    return v;
}

// compare 2 floats
bool fequal(float a, float b)
{
    return fabs(a - b) < FLT_EPSILON; 
}

// compare 2 vec3s
bool compare_vec3(const vec3_t v1, const vec3_t v2)
{
    for (int i = 0; i < 3; i++)
    {
        if (!fequal(v1.values[i], v2.values[i]))
        {
            return false;
        }
    }
    return true;
}

// compare 2 mat4_ts
bool compare_mat4(const mat4_t m1, const mat4_t m2)
{
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (!fequal(m1.values[i][j], m2.values[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}

// make the magnitude of a vec3 equal to 1
vec3_t normalize_vec3(const vec3_t v)
{
    vec3_t nv = {0};

    // TODO how fast is sqrt?
    float l = sqrt((v.values[0] * v.values[0])
                 + (v.values[1] * v.values[1])
                 + (v.values[2] * v.values[2]));

    nv.values[0] = v.values[0] / l;
    nv.values[1] = v.values[1] / l;
    nv.values[2] = v.values[2] / l;

    return nv;
}

// get the sum of 2 vec3s
vec3_t add_vec3(const vec3_t v1, const vec3_t v2)
{
    return construct_vec3((v1.values[0] + v2.values[0]),
                       (v1.values[1] + v2.values[1]),
                       (v1.values[2] + v2.values[2]));
}

// get the dot product of 2 vec3s
float dot_vec3(const vec3_t v1, const vec3_t v2)
{
    return (v1.values[0] * v2.values[0])
         + (v1.values[1] * v2.values[1])
         + (v1.values[2] * v2.values[2]);
}

// get the dot product of 2 vec4s
float dot_vec4(const vec4_t v1, const vec4_t v2)
{
    return (v1.values[0] * v2.values[0])
         + (v1.values[1] * v2.values[1])
         + (v1.values[2] * v2.values[2])
         + (v1.values[3] * v2.values[3]);
}

// get the cross product of 2 vec3s
vec3_t cross_vec3(const vec3_t v1, const vec3_t v2)
{
    vec3_t v = {0};

    v.values[0] = (v1.values[1] * v2.values[2]) - (v1.values[2] * v2.values[1]);
    v.values[1] = (v1.values[2] * v2.values[0]) - (v1.values[0] * v2.values[2]);
    v.values[2] = (v1.values[0] * v2.values[1]) - (v1.values[1] * v2.values[0]);

    return v;
}

// get the cross product of 2 mat4s
mat4_t cross_mat4(const mat4_t m1, const mat4_t m2)
{
    mat4_t m = {0};

    m.values[0][0] = dot_vec4(row_mat4(m1, 0), col_mat4(m2, 0));
    m.values[0][1] = dot_vec4(row_mat4(m1, 0), col_mat4(m2, 1));
    m.values[0][2] = dot_vec4(row_mat4(m1, 0), col_mat4(m2, 2));
    m.values[0][3] = dot_vec4(row_mat4(m1, 0), col_mat4(m2, 3));

    m.values[1][0] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 0));
    m.values[1][1] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 1));
    m.values[1][2] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 2));
    m.values[1][3] = dot_vec4(row_mat4(m1, 1), col_mat4(m2, 3));

    m.values[2][0] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 0));
    m.values[2][1] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 1));
    m.values[2][2] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 2));
    m.values[2][3] = dot_vec4(row_mat4(m1, 2), col_mat4(m2, 3));

    m.values[3][0] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 0));
    m.values[3][1] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 1));
    m.values[3][2] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 2));
    m.values[3][3] = dot_vec4(row_mat4(m1, 3), col_mat4(m2, 3));

    return m;
}

/* QUARANTINE
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

*/ 

#endif // VEC_H
