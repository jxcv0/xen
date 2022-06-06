#include "lm.h"
#include <stdbool.h>

#define test_start() printf("TESTING | %s\n", __func__)
#define test_pass() printf("\tPASS\n")
#define test_fail() printf("\tFAIL | %d\n", __LINE__)

void row_test()
{
    test_start();

    mat4_t mat4 = construct_mat4(0.1f);
    mat4.values[3][2] = 1.055f;
    mat4.values[3][0] = 0.02f;

    vec4_t r = row(mat4, 3);
    float exp1[] = {0.02f, 0.0f, 1.055f, 0.1f};

    for (int i = 0; i < 4; i++)
    {
        if (exp1[i] != r.values[i])
        {
            test_fail();
            return;
        }
    }
    
    r = row(mat4, 0);
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

void col_test()
{
    mat4_t mat4 = construct_mat4(0.35f);
    mat4.values[3][2] = 0.881f;
    vec4_t r = row(mat4, 3);

    float exp[] = {0.0f, 0.0f, 1.055f, 0.1f};

    for (int i = 0; i < 4; i++)
    {
        if (exp[i] != r.values[i])
        {
            printf("FAIL | Expected: %f, Actual: %f\n", exp[i], r.values[i]);
            break;
        }
    }
}

int main(void)
{
    row_test();
    return 0;
}
