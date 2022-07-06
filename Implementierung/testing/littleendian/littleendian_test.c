#include <stdio.h>
#include <stdlib.h>
#include "../../salsa_core/salsa_core_v1.h"

int main()
{
    uint8_t input[] = {0x23, 0x02, 0x01, 0x69}; 
    uint32_t output = littleendian(input);
    printf("The integer is: 0x%x\n", output);

    uint8_t out[4];
    littleendian_inv(out, output);
    for (int i = 0; i < 4; i++) printf("0x%x\n", out[i]);

    return EXIT_SUCCESS;
}