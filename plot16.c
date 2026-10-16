#include <osbind.h>
#include <stdio.h>
#include <linea.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;

/*void blank_screen();*/

void plot_16bit_bitmap(UINT16 *base, int row, int col,
                      const UINT16 *bitmap, UINT16 height)
{
    int i;
    long offset;
    int startingBit;

    offset = ((long)col >> 4) + ((long)row * 40);
    startingBit = col % 16;

    for (i = 0; i < height; i++) {

        if (startingBit == 0){
            *(base + offset + (40L * i)) |=
            bitmap[i];
        }
        else 
        {
            /*Right shift into first byte*/
            *(base + offset + (40L * i)) |=
            bitmap[i] >> startingBit;

            *(base + offset + (40L * i) + 1) |=
            bitmap[i] << (16 - startingBit);
        }
    }
}

int main()
{
    UINT16 *base = (UINT16 *)Physbase();

    UINT16 bitmap[8] = {
        0x3FFC,  /* 0011111111111100 */
        0x4002,  /* 0100000000000010 */
        0xA005,  /* 1010000000000101 */
        0x8001,  /* 1000000000000001 */
        0xA005,  /* 1010000000000101 */
        0x9999,  /* 1001100110011001 */
        0x4002,  /* 0100000000000010 */
        0x3FFC   /* 0011111111111100 */
    };

    /* Aligned: startingBit = 0 */
    plot_16bit_bitmap(base, 396, 636, bitmap, 8);

    /* Unaligned: startingBit = 4 */
    /*plot_16bit_bitmap(base, 130, 100, bitmap, 8);*/

    Cnecin();

    return 0;
}