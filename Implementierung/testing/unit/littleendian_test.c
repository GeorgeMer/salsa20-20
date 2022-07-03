#include <stdio.h>
#include <stdlib.h>
#include "../../salsa_core/salsa_core_v1.h"

int main()
{
    uint8_t input[] = {0x23, 0x02, 0x01, 0x69}; 
    uint32_t output = littleendian(input);
    printf("The integer is: 0x%x\n", output);
    
    return EXIT_SUCCESS;
}