/*
 * Used to calculate the period of LCGS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * might be used to optimize the inner LCG maybe?
 */
int detect_cycle_16(uint8_t a, uint8_t c, uint16_t s)
{
    uint16_t tortoise = s / 2 * a + c;
    uint16_t hare = (s / 2 * a + c) / 2 * a + c;
    while (tortoise != hare)
    {
        tortoise = tortoise / 2 * a + c;
        hare = (hare / 2 * a + c) / 2 * a + c;
    }

    uint32_t period = 1;
    tortoise = s / 2 * a + c;
    while (tortoise != hare)
    {
        period++;
        tortoise = tortoise / 2 * a + c;
    }
    return period;
}

/*
 * Used to find the period of the outer LGC.
 * Not using this in the decrypt code because we already know the return value
 * and it takes a bit of time to run
 */
int detect_cycle(uint32_t a, uint32_t c, uint32_t s)
{
    uint32_t tortoise = s * a + c;
    uint32_t hare = (s * a + c) * a + c;
    while (tortoise != hare)
    {
        tortoise = tortoise * a + c;
        hare = (hare * a + c) * a + c;
    }

    printf("Found repetition! Calculating distance...\n");
    uint32_t period = 1;
    tortoise = tortoise * a + c;
    while (tortoise != hare)
    {
        period++;
        tortoise = tortoise * a + c;
    }
    printf("In decimal:\n");
    printf("%d\n", period);
    printf("In hex:\n");
    printf("%08x\n", period);
    return 0;
}
