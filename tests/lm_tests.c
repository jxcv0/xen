#include "lm.h"
#include <stdbool.h>
#include <float.h>

#ifdef _WIN32
#define test_start() printf("TESTING %s ", __func__)

#define test_pass() printf("SUCCESS\n")

#define test_fail() printf("FAILURE (line: %d)\n", __LINE__)

#else
#define test_start() printf("\033[0;34m"); \
                     printf("TESTING"); \
                     printf("\033[0m"); \
                     printf(" %s ", __func__)

#define test_pass() printf("\033[0;32m"); \
                    printf("SUCCESS\n"); \
                    printf("\033[0m")

#define test_fail() printf("\033[0;31m"); \
                    printf("FAILURE (line: %d)\n", __LINE__); \
                    printf("\033[0m")
#endif

void row_mat4_test()
{
    test_start();

    mat4_t mat4 = construct_mat4(0.1f);
    mat4.values[3][2] = 1.055f;
    mat4.values[3][0] = 0.02f;

    vec4_t r = row_mat4(mat4, 3);
    float exp1[] = {0.02f, 0.0f, 1.055f, 0.1f};

    for (int i = 0; i < 4; i++)
    {
        if (exp1[i] != r.values[i])
        {
            test_fail();
            return;
        }
    }
    
    r = row_mat4(mat4, 0);
    float exp2[] = {0.1f, 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < 4; i++)
    {
        if (exp2[i] != r.values[i])
        {
            test_fail();
            return;
        }
    }

    test_pass();
}

void col_mat4_test()
{
    test_start();

    mat4_t mat4 = construct_mat4(0.35f);

    vec4_t c = col_mat4(mat4, 1);
    float exp[] = {0.0f, 0.35f, 0.0f, 0.0f};

    for (int i = 0; i < 4; i++)
    {
        if (exp[i] != c.values[i])
        {
            printf("%d, %f, %f\n", i, exp[i], c.values[i]);
            test_fail();
            return;
        }
    }

    test_pass();
}

void fequal_test()
{
    test_start();

    if (fequal(0.1f, 0.11f))
    {
        test_fail();
        return;
    }

    if (!fequal(0.0001f, 0.0001f))
    {
        test_fail();
        return;
    }

    if (!fequal(1.0f, 1.0f))
    {
        test_fail();
        return;
    }

    test_pass();
}

void compare_vec3_test()
{
    test_start();

    vec3_t vec3 = construct_vec3(1.0f, 3.2f, 0.0f);
    vec3_t same = construct_vec3(1.0f, 3.2f, 0.0f);
    vec3_t diff = construct_vec3(2.32f, 4.44f, 0.0f);

    if (compare_vec3(vec3, same) == false)
    {
        test_fail();
        return;
    }

    if(compare_vec3(vec3, diff) == true)
    {
        test_fail();
        return;
    }

    test_pass();
}

void normalize_vec3_test()
{
    test_start();

    vec3_t v = construct_vec3(1.0f, 1.0f, 1.0f);
    vec3_t e = construct_vec3(0.5773502691896258f,
                              0.5773502691896258f,
                              0.5773502691896258f);
    vec3_t a = normalize_vec3(v);

    if (!compare_vec3(e, a))
    {
        test_fail();
        return;
    }

    test_pass();
}

void add_vec3_test()
{
    test_start();

    vec3_t v1 = construct_vec3(1.0f, -3.2f, 0.0f);
    vec3_t v2 = construct_vec3(5.4f, 3.2f, -5.0f);
    vec3_t e = construct_vec3(6.4f, 0.0f, -5.0f);
    vec3_t a = add_vec3(v1, v2);

    if (compare_vec3(e, a))
    {
        test_pass();
        return;
    }

    test_fail();
}

void dot_vec3_test()
{
    test_start();

    vec3_t v1 = construct_vec3(1.0f, -3.2f, 0.0f);
    vec3_t v2 = construct_vec3(5.4f, 3.2f, -5.0f);

    if (dot_vec3(v1, v2) - (-4.840001f) > 0.00001) // why does this work but not FLT_EPSILON??
    {
        test_fail();
    }
    test_pass();
}

void dot_vec4_test()
{
    test_start();

    vec4_t v1 = construct_vec4(1.0f, -3.2f, 0.0f, 1.0f);
    vec4_t v2 = construct_vec4(5.4f, 3.2f, -5.0f, -0.5f);

    if (dot_vec4(v1, v2) - (-5.34f) > 0.00001)
    {
        test_fail();
    }
    test_pass();
}

void cross_vec3_test()
{
    test_start();

    vec3_t v1 = construct_vec3(1.0f, 2.0f, 3.0f);
    vec3_t v2 = construct_vec3(1.0f, 5.0f, 7.0f);
    vec3_t e = construct_vec3(-1.0f, -4.0f, 3.0f);
    vec3_t a = cross_vec3(v1, v2);

    if (compare_vec3(e, a) == false)
    {
        test_fail();
        return;
    }
    test_pass();
}

void cross_mat4_test()
{
    test_start();

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
            if (fabs(a.values[i][j] - e.values[i][j]) > FLT_EPSILON)
            {
                test_fail();
                return;
            }
        }
    }
    
    // if (compare_mat4(a, e))
    // {
        // test_fail();
        // return;
    // }

    test_pass();
}

void perspective_test()
{
    test_start();

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
            if (fabs(a.values[i][j] - e.values[i][j]) > 0.001f)
            {
		printf("\nExpected:\n");
		print_mat4(e);
		printf("Actual:\n");
		print_mat4(a);
                test_fail();
                return;
            }
        }
    }

    test_pass();
}

void translate_test()
{
    test_start();

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
            if (fabs(a.values[i][j] - e.values[i][j]) > FLT_EPSILON)
            {
		printf("\nExpected:\n");
		print_mat4(e);
		printf("Actual:\n");
		print_mat4(a);
                test_fail();
                return;
            }
        }
    }

    test_pass();
}

int main(void)
{
    row_mat4_test();
    col_mat4_test();
    fequal_test();
    compare_vec3_test();
    normalize_vec3_test();
    add_vec3_test();
    dot_vec3_test();
    dot_vec4_test();
    cross_vec3_test();
    cross_mat4_test();
    perspective_test();
    translate_test();

    return 0;
}
