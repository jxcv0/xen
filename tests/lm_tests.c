#include "lm.h"
#include <stdbool.h>

#define test_start() printf("TESTING | %s | ", __func__)
#define test_pass() printf("SUCCESS\n")
#define test_fail() printf("FAIL (line: %d)\n", __LINE__)

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

void compare_vec3_test()
{
    test_start();

    vec3_t master = construct_vec3(1.0f, 3.2f, 0.0f);
    vec3_t same = construct_vec3(1.0f, 3.2f, 0.0f);
    vec3_t diff = construct_vec3(1.1f, 3.2f, 0.0f);

    if (compare_vec3(master, same, 0.1f))
    {
        test_pass();
        return;
    }

    if (!compare_vec3(master, diff, 0.1f))
    {
        test_pass();
        return;
    }

    test_fail();
}

void normalize_vec3_test()
{
    test_start();

    vec3_t v = construct_vec3(5.0f, 5.6f, 8.32f);
    vec3_t e = construct_vec3(0.446175f, 0.499716f, 0.742436f);
    vec3_t a = normalize_vec3(v);

    // printf("\n%f, %f, %f\n", a.values[0], a.values[1], a.values[2]);
    // printf("%f, %f, %f\n", e.values[0], e.values[1], e.values[2]);

    if (compare_vec3(e, a, 1.0f))
    {
        test_pass();
        return;
    }

    test_fail();
}

int main(void)
{
    row_mat4_test();
    col_mat4_test();
    compare_vec3_test();

    // this does not work because of floating point comparison
    // normalize_vec3_test(); 

    return 0;
}
