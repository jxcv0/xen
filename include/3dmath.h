/*
 * Copyright © 2022 Jacob Micoud
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
 
#ifndef THREEDMATH_H
#define THREEDMATH_H

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdbool.h>

// TODO
// SIMD
// optimise

static inline float radians(float x)
{
    return x * 0.0174533;
}

// compare 2 floats TODO why does this not work sometimes
static inline bool fequal(const float a, const float b)
{
	return (fabs(a - b) < FLT_EPSILON);
}

typedef struct vec2
{
    float values[2];
} vec2_t;

typedef struct vec3
{
    float values[3];
} vec3_t;

typedef struct vec4
{
    float values[4];
} vec4_t;

typedef struct mat4
{
    float values[4][4];
} mat4_t;

// convenience printf function
static inline void print_vec3(const vec3_t v)
{
    printf("| %f, %f, %f |\n", v.values[0], v.values[1], v.values[2]);
}

// convenience printf function
static inline void print_vec4(const vec4_t v)
{
    printf("| %f, %f, %f, %f |\n", v.values[0], v.values[1], v.values[2], v.values[3]);
}

// construct a 3 dimensional vector
static inline vec2_t construct_vec2(float x, float y)
{
    vec2_t v = {
        .values = {x, y}
    };
    return v;
}

// construct a 3 dimensional vector
static inline vec3_t construct_vec3(float x, float y, float z)
{
    vec3_t v = {
        .values = {x, y, z}
    };
    return v;
}

// construct a 4 dimensional vector
static inline vec4_t construct_vec4(float x, float y, float z, float w)
{
    vec4_t v = {
        .values = {x, y, z, w}
    };
    return v;
}

// construct 4x4 matrix
static inline mat4_t construct_mat4(float x)
{
    mat4_t m = {0};
    m.values[0][0] = x;
    m.values[1][1] = x;
    m.values[2][2] = x;
    m.values[3][3] = x;
    return m;
}

// get a row of a 4x4 matrix as a vec4
static inline vec4_t row_mat4(const mat4_t m, int n)
{
    vec4_t v = construct_vec4(m.values[n][0], m.values[n][1], m.values[n][2], m.values[n][3]);
    return v;
}

// convenience printf function
static inline void print_mat4(const mat4_t m)
{
    for (int i = 0; i < 4; i++)
    {
        printf("| %.3f, %.3f, %.3f, %.3f |\n",
            m.values[i][0], m.values[i][1], m.values[i][2], m.values[i][3]);
    }
}

// scale a vec3
static inline vec3_t scale_vec3(const vec3_t v, const float s)
{
    return construct_vec3(v.values[0] * s,
                          v.values[1] * s,
                          v.values[2] * s);
}

// get a column of a 4x4 matrix as a vec4
static inline vec4_t col_mat4(const mat4_t m, int n)
{
    vec4_t v = construct_vec4(m.values[0][n], m.values[1][n], m.values[2][n], m.values[3][n]);
    return v;
}

// compare 2 vec3s
static inline bool compare_vec3(const vec3_t v1, const vec3_t v2)
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

// compare 2 vec4s
static inline bool compare_vec4(const vec4_t v1, const vec4_t v2)
{
    for (int i = 0; i < 4; i++)
    {
        if (!fequal(v1.values[i], v2.values[i]))
        {
            return false;
        }
    }
    return true;
}

// compare 2 mat4s
static inline bool compare_mat4(const mat4_t m1, const mat4_t m2)
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
static inline vec3_t normalize_vec3(const vec3_t v)
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
static inline vec3_t add_vec3(const vec3_t v1, const vec3_t v2)
{
    return construct_vec3((v1.values[0] + v2.values[0]),
                          (v1.values[1] + v2.values[1]),
                          (v1.values[2] + v2.values[2]));
}

// get the sum of 2 vec4s
static inline vec4_t add_vec4(const vec4_t v1, const vec4_t v2)
{
    return construct_vec4((v1.values[0] + v2.values[0]),
                          (v1.values[1] + v2.values[1]),
                          (v1.values[2] + v2.values[2]),
                          (v1.values[3] + v2.values[3]));
}

// get the difference of 2 vec3s
static inline vec3_t subtract_vec3(const vec3_t v1, const vec3_t v2)
{
    return construct_vec3((v1.values[0] - v2.values[0]),
                          (v1.values[1] - v2.values[1]),
                          (v1.values[2] - v2.values[2]));
}

// get the difference of 2 vec4s
static inline vec4_t subtract_vec4(const vec4_t v1, const vec4_t v2)
{
    return construct_vec4((v1.values[0] - v2.values[0]),
                          (v1.values[1] - v2.values[1]),
                          (v1.values[2] - v2.values[2]),
                          (v1.values[3] - v2.values[3]));
}

// get the dot product of 2 vec3s
static inline float dot_vec3(const vec3_t v1, const vec3_t v2)
{
    return (v1.values[0] * v2.values[0])
         + (v1.values[1] * v2.values[1])
         + (v1.values[2] * v2.values[2]);
}

// get the dot product of 2 vec4s
static inline float dot_vec4(const vec4_t v1, const vec4_t v2)
{
    return (v1.values[0] * v2.values[0])
         + (v1.values[1] * v2.values[1])
         + (v1.values[2] * v2.values[2])
         + (v1.values[3] * v2.values[3]);
}

// multiply a vec3 by a scalar
static inline vec3_t scalar_vec3(const vec3_t v, float s)
{
    vec3_t sv = {
        .values ={
            v.values[0] * s,
            v.values[1] * s,
            v.values[2] * s
        }
    };
    return sv;
}

// multiply a vec4 by a scalar
static inline vec4_t scalar_vec4(const vec4_t v, float s)
{
    vec4_t sv = {
        .values = {
            v.values[0] * s,
            v.values[1] * s,
            v.values[2] * s,
            v.values[3] * s
        }
    };
    return sv;
}

// get the cross product of 2 vec3s
static inline vec3_t cross_vec3(const vec3_t v1, const vec3_t v2)
{
    vec3_t v = {0};

    v.values[0] = (v1.values[1] * v2.values[2]) - (v1.values[2] * v2.values[1]);
    v.values[1] = (v1.values[2] * v2.values[0]) - (v1.values[0] * v2.values[2]);
    v.values[2] = (v1.values[0] * v2.values[1]) - (v1.values[1] * v2.values[0]);

    return v;
}

// get the cross product of 2 mat4s
static inline mat4_t cross_mat4(const mat4_t m1, const mat4_t m2)
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

// create orthographic perspective matrix
static inline mat4_t ortho(const float left,
						   const float right,
						   const float bottom,
						   const float top,
						   const float near,
						   const float far)
{
	mat4_t m = construct_mat4(1.0f);
	m.values[0][0] = 2.0f / (right - left);
	m.values[1][1] = 2.0f / (top - bottom);
	m.values[2][2] = -2.0f / (far - near);
	m.values[3][0] = -(right + left) / (right - left);
	m.values[3][1] = -(top + bottom) / (top - bottom);
	m.values[3][2] = -(far + near) / (far - near);
	return m;
}

// create perspective projection matrix
static inline mat4_t perspective(const float fov, const float near, const float far, const float ratio)
{
    mat4_t m = {0};
    
    float tanhalffov = tan(radians(fov) / 2.0f);

    m.values[0][0] = 1.0f / (ratio * tanhalffov);
    m.values[1][1] = 1.0f  / tanhalffov;
    m.values[2][2] = (far + near) / (near - far);
    m.values[2][3] = -1.0f;
    m.values[3][2] = -(2.0f * far * near) / (far - near);

    return m;
}

// create a transformation matrix from a vec2 (for 2d)
static inline mat4_t translate_vec2(const mat4_t m, const vec2_t v, const float f)
{
	mat4_t mat = m;
	mat.values[3][0] = v.values[0];
	mat.values[3][1] = v.values[1];
	mat.values[3][2] = f;
	return mat;
}

// create a transformation matrix from a vec3
static inline mat4_t translate_vec3(const mat4_t m, const vec3_t v)
{
	mat4_t mat = m;
	mat.values[3][0] = v.values[0];
	mat.values[3][1] = v.values[1];
	mat.values[3][2] = v.values[2];
	return mat;
}

// create a transformation matrix from a vec3
static inline mat4_t rotate(const mat4_t m, const vec3_t axis, const float a)
{
    float rad = radians(a);
    float c = cos(rad);
    float s = sin(rad);
    float t = 1.0f - c;
    vec3_t temp = scalar_vec3(axis, t);

    mat4_t q = {0};
    q.values[0][0] = c + temp.values[0] * axis.values[0];
    q.values[0][1] = 0 + temp.values[0] * axis.values[1] + s * axis.values[2];
    q.values[0][2] = 0 + temp.values[0] * axis.values[2] - s * axis.values[1];

    q.values[1][0] = 0 + temp.values[1] * axis.values[0] - s * axis.values[2];
    q.values[1][1] = c + temp.values[1] * axis.values[1];
    q.values[1][2] = 0 + temp.values[1] * axis.values[2] + s * axis.values[0];

    q.values[2][0] = 0 + temp.values[2] * axis.values[0] + s * axis.values[1];
    q.values[2][1] = 0 + temp.values[2] * axis.values[1] - s * axis.values[0];
    q.values[2][2] = c + temp.values[2] * axis.values[2];

    mat4_t rm = cross_mat4(m, q);
    rm.values[3][0] = m.values[3][0];
    rm.values[3][1] = m.values[3][1];
    rm.values[3][2] = m.values[3][2];
    rm.values[3][3] = m.values[3][3];

    return rm;
}

// create view matrix
static inline mat4_t look_at(const vec3_t eye, const vec3_t ctr, const vec3_t up)
{
    vec3_t f = normalize_vec3(subtract_vec3(ctr, eye));
    vec3_t s = normalize_vec3(cross_vec3(f, up));
    vec3_t u = cross_vec3(s, f);
    
    mat4_t m = construct_mat4(1.0f);
    m.values[0][0] = s.values[0];
    m.values[1][0] = s.values[1];
    m.values[2][0] = s.values[2];

    m.values[0][1] = u.values[0];
    m.values[1][1] = u.values[1];
    m.values[2][1] = u.values[2];

    m.values[0][2] = -f.values[0];
    m.values[1][2] = -f.values[1];
    m.values[2][2] = -f.values[2];

    m.values[3][0] = -dot_vec3(s, eye);
    m.values[3][1] = -dot_vec3(u, eye);
    m.values[3][2] = dot_vec3(f, eye);

    return m;
}

#endif // THREEDMATH_H
