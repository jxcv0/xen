#ifndef VEC_H
#define VEC_H

typedef struct vec2_t
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float values[2];
    };
} vec2_t;

typedef struct vec3_t
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float values[3];
    };
} vec3_t;

typedef struct vec4_t
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float values[4];
    };
} vec4_t;

typedef struct mat4_t
{
    float values[4][4];
} mat4_t;

mat4_t identity_mat4()
{
    mat4_t result;
    result.values[0][0] = 1.0f;
    result.values[1][1] = 1.0f;
    result.values[2][2] = 1.0f;
    result.values[3][3] = 1.0f;
    return result;
}

vec4_t row(mat4_t m, int n)
{
    vec4_t result = {
        .values = {m.values[n][0], m.values[n][1], m.values[n][2], m.values[n][3]}
    };
    return result;
}

vec4_t col(mat4_t m, int n)
{
    vec4_t result = {
        .values = {m.values[0][n], m.values[1][n], m.values[2][n], m.values[3][n]}
    };
    return result;
}

float dot_v4(vec4_t v1, vec4_t v2)
{
    return (v1.x * v2.x)
         + (v1.y * v2.y)
         + (v1.z * v2.z)
         + (v1.w * v2.w);
}

mat4_t cross_m4(mat4_t m1, mat4_t m2)
{
    mat4_t result;

    result.values[0][0] = dot_v4(row(m1, 0), col(m2, 0));
    result.values[0][1] = dot_v4(row(m1, 0), col(m2, 1));
    result.values[0][2] = dot_v4(row(m1, 0), col(m2, 2));
    result.values[0][3] = dot_v4(row(m1, 0), col(m2, 3));

    result.values[1][0] = dot_v4(row(m1, 1), col(m2, 0));
    result.values[1][1] = dot_v4(row(m1, 1), col(m2, 1));
    result.values[1][2] = dot_v4(row(m1, 1), col(m2, 2));
    result.values[1][3] = dot_v4(row(m1, 1), col(m2, 3));

    result.values[2][0] = dot_v4(row(m1, 2), col(m2, 0));
    result.values[2][1] = dot_v4(row(m1, 2), col(m2, 1));
    result.values[2][2] = dot_v4(row(m1, 2), col(m2, 2));
    result.values[2][3] = dot_v4(row(m1, 2), col(m2, 3));

    result.values[3][0] = dot_v4(row(m1, 3), col(m2, 0));
    result.values[3][1] = dot_v4(row(m1, 3), col(m2, 1));
    result.values[3][2] = dot_v4(row(m1, 3), col(m2, 2));
    result.values[3][3] = dot_v4(row(m1, 3), col(m2, 3));

    return result;
}

#endif // VEC_H
