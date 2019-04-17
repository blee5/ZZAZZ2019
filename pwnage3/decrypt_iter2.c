/*
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    unsigned char buffer[512];
    FILE *f = fopen("encrypted.hex", "rb");
    if (fread(buffer, 1, 512, f) != 512)
    {
        perror("Couldn't read map data");
        exit(-1);
    }
    fclose(f);

    uint32_t outer_count = 0x7ffffb0a;
    uint32_t outer_lcg = 0x5d0b1c11;
    uint16_t index = 0;
    /*
     * The outer loop is a LCG,
     * with a = 0x35e79125, c = 0x5659b10
     * and m = 2 ^ 32.
     */
    while (outer_count--)
    {
        uint32_t inner_counter = 0x1b080733;
        /*
         * The inner loop is another LCG,
         * with the seed, a, and c being dependent on the outer LCG,
         * and m = 2 ^ 32.
         */
        uint16_t inner_lcg = (outer_lcg >> 8) & 0xff;
        uint16_t a = (outer_lcg >> 16) & 0xff;
        uint16_t c = (outer_lcg >> 24) & 0xff;
        while (inner_counter--)
        {
            inner_lcg = inner_lcg * (a / 2) + c;
            buffer[index++] ^= inner_lcg & 0xff;
            index %= 0x200;
        }
        outer_lcg = outer_lcg * 0x35e79125 + 0x56596b10;
        printf("%08x\n", outer_count);
    }

    for (index = 0; index < 512; index++)
    {
        printf("%02x ", buffer[index]);
        if (index % 0xf == 0)
            putchar('\n');
    }
    putchar('\n');
}
