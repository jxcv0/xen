#include "lm.h"
#include <stdbool.h>

void row_test()
{
    mat4_t mat4 = construct_mat4(0.1f);
    mat4.values[3][2] = 1.055f;
    vec4_t row2 = row(mat4, 3);

    float exp[] = {0.0f, 0.0f, 1.055f, 0.1f};

    int i = 0;
    for (; i < 4; i++)
    {
        if (exp[i] != row2.values[i])
        {
            printf("FAIL | Expected: %f at index %d, Actual: %f\n", exp[i], i, row2.values[i]);
            break;
        }
    }
}

int main(void)
{
    row_test();
    return 0;
}
