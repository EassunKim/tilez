#include <osbind.h>
#include <stdio.h>
#include <linea.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
/*void blank_screen();*/

void plot_32bit_bitmap(UINT32 *base, int row, int col,
     const UINT32 *bitmap, UINT16 height)
{
    int i;
    long offset;
    int startingBit;

    offset = ((long)col >> 5) + ((long)row * 20);
    startingBit = col % 32;

    for (i = 0; i < height; i++) {

        if (startingBit == 0){
            *(base + offset + (20L * i)) |=
            bitmap[i];
        }
        else 
        {
            /*Right shift into first byte*/
            *(base + offset + (20L * i)) |=
            bitmap[i] >> startingBit;

            *(base + offset + (20L * i) + 1) |=
            bitmap[i] << (32 - startingBit);
        }
    }
}

int main()
{
    UINT32 *base = (UINT32 *)Physbase();

    UINT32 bitmap[8] = {
        0x3FFFFFFCUL,
        0x40000002UL,
        0xA0000005UL,
        0x80000001UL,
        0xA0000005UL,
        0x99999999UL,
        0x40000002UL,
        0x3FFFFFFCUL
    };

    /* Aligned */
    plot_32bit_bitmap(base, 100, 96, bitmap, 8);

    /* Unaligned */
    plot_32bit_bitmap(base, 130, 100, bitmap, 8);

    Cnecin();

    return 0;
}