/*
 * Uses LCG properties to optimize loops
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// > including .c files
// > greentexting in sourcecode
#include "cycle_detection.c"

int main()
{
    uint8_t buffer[512];
    uint16_t index;
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

    /*
     * Outer LCG
     * a = 0x35e79125
     * c = 0x5659b10
     * m = 2 ^ 32.
     *
     * period: 0x40000000 (2^30).
     */
    uint32_t outer_count = 0x7ffffb0a;
    uint32_t outer_period = 0x40000000;
    uint32_t outer_lcg = 0x5d0b1c11;

    /*
     * First, we skip the first 0x40000000 iterations,
     * since the state of our LCG is back to the original state.
     *
     * The xors during next 0x3ffffb0a iterations will cancel out
     * the xors with identical bytes found in the previous period,
     * So we don't have to do anything except advance the LCG,
     * since we didn't do anything for the previous period either.
     */
    outer_count -= outer_period;
    while (outer_count--)
    {
        outer_lcg = outer_lcg * 0x35e79125 + 0x56596b10;
    }

    /*
     * Run the iterations that actually have an effect
     */
	outer_count = 0x4f6; // 0x40000000 - 0x3ffffb0a
    while (outer_count--)
    {
        printf("%08x (%d)\n", outer_count, outer_count);

        /*
         * Inner LCG:
         * a, c, and the seed are dependent on the
         * pseudorandom number from the outer LCG.
         * m = 2 ^ 16.
         * can this be optimized??
         */
        uint16_t inner_lcg = (outer_lcg >> 8) & 0xff;
        uint8_t a = outer_lcg >> 16;
        uint8_t c = outer_lcg >> 24;

        uint32_t inner_counter = 0x1b080733;
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

    f = fopen("decrypted.bin", "wb");
    fwrite(buffer, 1, 512, f);
    fclose(f);

    return 0;
}
