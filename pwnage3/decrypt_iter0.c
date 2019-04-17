/*
 * Iteration of the decryption code faithful to the original assembly
 * (With *some* optimizations like repeated additions written multiplications...)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    uint32_t A0FF = 0x7ffffb0a; // outer loop counter
    uint32_t A103 = 0x1b080733; // inner loop counter value
    uint32_t A107; // inner loop counter
    uint32_t A0F3 = 0x5d0b1c11;
    unsigned char buffer[512];
    uint16_t index = 0;

    FILE *f = fopen("encrypted.hex", "rb");
    if (fread(buffer, 1, 512, f) != 512)
    {
        perror("Couldn't read map data");
        exit(-1);
    }
    fclose(f);

    while (A0FF--)
    {
        A107 = A103;
        uint16_t A10B = (A0F3 >> 8) & 0xff;
        uint16_t A10D = (A0F3 >> 16) & 0xff;
        uint16_t A10E = (A0F3 >> 24) & 0xff;
        while (A107--)
        {
            A10B = (A10B / 2) * A10D + A10E;
            buffer[index++] ^= A10B & 0xff;
            index %= 0x200;
        }
        A0F3 = A0F3 * 0x35e79125 + 0x56596b10;
        printf("%08x\n", A0FF);
    }
    for (index = 0; index < 512; index++)
    {
        printf("%02x ", buffer[index]);
        if (index % 0xf == 0)
            putchar('\n');
    }
    putchar('\n');
}
