/*
 * Iteration of the decryption code with helpful variable names!
 *
 * Special thanks for ISSOtm for his writeup that taught me
 * that this implements something called a linear congruential generator.
 * Even though I *did* look at his writeup because I was completely stuck,
 * I tried to solve the rest on my own.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    uint8_t buffer[512];
    FILE *f = fopen("encrypted.bin", "rb");
    if (!f)
    {
        perror("Could not find file");
        exit(-1);
    }
    if (fread(buffer, 1, 512, f) != 512)
    {
        perror("Couldn't read map data");
        exit(-1);
    }
    fclose(f);

    uint32_t outer_count = 0x7ffffb0a;
    uint32_t outer_lcg = 0x5d0b1c11;
    uint16_t index = 0;
    while (outer_count--)
    {
        printf("%08x\n", outer_count);

        uint32_t inner_counter = 0x1b080733;
        uint16_t inner_lcg = (outer_lcg >> 8) & 0xff;
        uint8_t a = outer_lcg >> 16;
        uint8_t c = outer_lcg >> 24;

        index = 0;
        while (inner_counter--)
        {
            inner_lcg = (inner_lcg / 2) * a + c;
            buffer[index++] ^= inner_lcg & 0xff;
            index %= 0x200;
        }
        outer_lcg = outer_lcg * 0x35e79125 + 0x56596b10;
    }

    for (index = 0; index < 512; index++)
    {
        printf("%02x", buffer[index]);
    }
    putchar('\n');
}
