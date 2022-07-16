#include "maths.h"
#include "logger.h"

#include <assert.h>

void row_mat4_test()
{
    mat4_t mat4 = construct_mat4(0.1f);
    mat4.values[3][2] = 1.055f;
    mat4.values[3][0] = 0.02f;

    vec4_t r = row_mat4(mat4, 3);
    float exp1[] = {0.02f, 0.0f, 1.055f, 0.1f};

    for (int i = 0; i < 4; i++)
    {
        assert(exp1[i] == r.values[i]);
    }
    
    r = row_mat4(mat4, 0);
    float exp2[] = {0.1f, 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < 4; i++)
    {
        assert(exp2[i] == r.values[i]);
    }
}

void col_mat4_test()
{
    mat4_t mat4 = construct_mat4(0.35f);

    vec4_t c = col_mat4(mat4, 1);
    float exp[] = {0.0f, 0.35f, 0.0f, 0.0f};

    for (int i = 0; i < 4; i++)
    {
        assert(exp[i] == c.values[i]);
    }
}

void fequal_test()
{
    assert(!fequal(0.1f, 0.11f));
    assert(fequal(0.0001f, 0.0001f));
    assert(fequal(1.0f, 1.0f));
}

void scalar_vec3_test()
{
    vec3_t v = {
		.values = {1.0f, 2.0f, 3.0f}
    };

    vec3_t e = {
		.values = {0.1f, 0.2f, 0.3f}
    };

    vec3_t a = scalar_vec3(v, 0.1f);

    for (int i = 0; i < 3; i++)
    {
		assert(fabs(a.values[i] - e.values[i] < FLT_EPSILON));
    }
}

void scalar_vec4_test()
{
    vec4_t v = {
		.values = {1.0f, 2.0f, 3.0f, 4.0f}
    };

    vec4_t e = {
		.values = {0.1f, 0.2f, 0.3f, 0.4f}
    };

    vec4_t a = scalar_vec4(v, 0.1f);

    for (int i = 0; i < 4; i++)
    {
		assert(fabs(a.values[i] - e.values[i] < FLT_EPSILON));
    }
}

void compare_vec3_test()
{
    vec3_t v = construct_vec3(1.0f, 3.2f, 0.0f);
    vec3_t same = construct_vec3(1.0f, 3.2f, 0.0f);
    vec3_t diff = construct_vec3(2.32f, 4.44f, 0.0f);

    assert(compare_vec3(v, same) != false);
    assert(compare_vec3(v, diff) != true);
}

void compare_vec4_test()
{
    vec4_t v = construct_vec4(1.0f, 3.2f, 0.0f, 1.0f);
    vec4_t same = construct_vec4(1.0f, 3.2f, 0.0f, 1.0f);
    vec4_t diff = construct_vec4(2.32f, 4.44f, 0.0f, 1.01f);

    assert(compare_vec4(v, same) == true);
    assert(compare_vec4(v, diff) == false);
}

void normalize_vec3_test()
{
    vec3_t v = construct_vec3(1.0f, 1.0f, 1.0f);
    vec3_t e = construct_vec3(0.5773502691896258f,
                              0.5773502691896258f,
                              0.5773502691896258f);
    vec3_t a = normalize_vec3(v);
	assert(compare_vec3(e, a));
}

void add_vec3_test()
{
    vec3_t v1 = construct_vec3(1.0f, -3.2f, 0.0f);
    vec3_t v2 = construct_vec3(5.4f, 3.2f, -5.0f);
    vec3_t e = construct_vec3(6.4f, 0.0f, -5.0f);
    vec3_t a = add_vec3(v1, v2);

    assert(compare_vec3(e, a));
}

void add_vec4_test()
{
    vec4_t v1 = construct_vec4(1.0f, -3.2f, 0.0f, 10.0f);
    vec4_t v2 = construct_vec4(5.4f, 3.2f, -5.0f, -4.0f);
    vec4_t e = construct_vec4(6.4f, 0.0f, -5.0f, 6.0f);
    vec4_t a = add_vec4(v1, v2);

    assert(compare_vec4(e, a));
}

void subtract_vec3_test()
{
    vec3_t v1 = construct_vec3(1.0f, -3.2f, 0.0f);
    vec3_t v2 = construct_vec3(5.4f, 3.2f, -5.0f);
    vec3_t e = construct_vec3(4.4f, -6.4f, 5.0f);
    vec3_t a = subtract_vec3(v1, v2);

    assert(compare_vec3(e, a));
}

void subtract_vec4_test()
{
    vec4_t v1 = construct_vec4(1.0f, -3.2f, 0.0f, 10.0f);
    vec4_t v2 = construct_vec4(5.4f, 3.2f, -5.0f, -4.0f);
    vec4_t e = construct_vec4(4.4f, -6.4f, 5.0f, 14.0f);
    vec4_t a = subtract_vec4(v1, v2);

    assert(compare_vec4(e, a));
}

void dot_vec3_test()
{
    vec3_t v1 = construct_vec3(1.0f, -3.2f, 0.0f);
    vec3_t v2 = construct_vec3(5.4f, 3.2f, -5.0f);

    assert(dot_vec3(v1, v2) - (-4.840001f) < 0.00001); // why does this work but not FLT_EPSILON??
}

void dot_vec4_test()
{
    vec4_t v1 = construct_vec4(1.0f, -3.2f, 0.0f, 1.0f);
    vec4_t v2 = construct_vec4(5.4f, 3.2f, -5.0f, -0.5f);

	assert(dot_vec4(v1, v2) - (-5.34f) < 0.00001);
}

void cross_vec3_test()
{
    vec3_t v1 = construct_vec3(1.0f, 2.0f, 3.0f);
    vec3_t v2 = construct_vec3(1.0f, 5.0f, 7.0f);
    vec3_t e = construct_vec3(-1.0f, -4.0f, 3.0f);
    vec3_t a = cross_vec3(v1, v2);

    assert(compare_vec3(e, a) == true);
}

void cross_mat4_test()
{
    mat4_t m1;
    mat4_t m2;

    float x = 0.0f;
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m1.values[i][j] = x;
            m2.values[i][j] = x;
            x += 1.0f;
        }
    }

    mat4_t e = {
        .values = {
            {56.0f,  62.0f,  68.0f,  74.0f},
            {152.0f, 174.0f, 196.0f, 218.0f},
            {248.0f, 286.0f, 324.0f, 362.0f},
            {344.0f, 398.0f, 452.0f, 506.0f}
        }
    };
    mat4_t a = cross_mat4(m1, m2);

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            assert(fabs(a.values[i][j] - e.values[i][j]) < FLT_EPSILON);
        }
    }
}

void perspective_test()
{
    mat4_t a = perspective(45.0f, 0.1f, 100.0f, (800.0f/600.0f));

    mat4_t e = {
        .values = {
            {1.81066f, 0.0f, 0.0f, 0.0f},
            {0.0f, 2.41421f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.002f, -1.0f},
            {0.0f, 0.0f, -0.200f, 0.0f}
        }
    };

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // FIXME more rounding errors
            assert(fabs(a.values[i][j] - e.values[i][j]) < 0.001f);
        }
    }
}

void translate_test()
{
    mat4_t m = construct_mat4(1.0f);
    vec3_t v = construct_vec3(1.1f, 1.2f, 1.3f);
    mat4_t e = {
        .values = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {1.1f, 1.2f, 1.3f, 1.0f},
	    }
    };
    mat4_t a = translate(m, v);

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            assert(fabs(a.values[i][j] - e.values[i][j]) < FLT_EPSILON);
        }
    }
}

void rotate_test()
{
    mat4_t m = construct_mat4(1.0f);
    assert(m.values[3][3] == 1.0f);
    vec3_t axis = { .values = {0.0f, 1.0f, 0.0f} };
    mat4_t e = {
        .values = {
            {0.866025f, 0.0f, -0.5f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.5f, 0.0f, 0.866025f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
	    }
    };
    mat4_t a = rotate(m, axis, 30.0f);

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            assert(fabs(a.values[i][j] - e.values[i][j]) < 0.002);
        }
    }
}

void look_at_test()
{
    vec3_t eye = construct_vec3(3.0f, 3.0f, 3.0f);
    vec3_t ctr = construct_vec3(1.0f, 0.0f, 1.0f);
    vec3_t up = construct_vec3(0.0f, 1.0f, 0.0f);

    mat4_t e = {
        .values = {
            {-0.707107f, 0.0f, -0.707107f, 0.0f},
            {0.0f, 1.0f, -0.0f, 0.0f},
            {0.707107f, -0.0f, -0.707107f, 0.0f},
            {-0.0f, -3.0f, 4.24264f, 1.0f}
        }
    };
    mat4_t a = look_at(eye, add_vec3(eye, ctr), up);

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            assert(fabs(a.values[i][j] - e.values[i][j]) < 0.002);
        }
    }
}

int main(void)
{
    row_mat4_test();
    col_mat4_test();
    fequal_test();
    scalar_vec3_test();
    scalar_vec4_test();
    compare_vec3_test();
    compare_vec4_test();
    normalize_vec3_test();
    add_vec3_test();
    add_vec4_test();
    dot_vec3_test();
    dot_vec4_test();
    cross_vec3_test();
    cross_mat4_test();
    perspective_test();
    translate_test();
    rotate_test();
    look_at_test();

    xen_log("maths_tests passed");
    return 0;
}
