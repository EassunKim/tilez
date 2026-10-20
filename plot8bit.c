
#include <osbind.h>
#include <stdio.h>
#include <linea.h>

typedef unsigned char UINT8;
typedef unsigned short UINT16;

/*void blank_screen();*/

void plot_8bit_bitmap(UINT8 *base, int row, int col,
                      const UINT8 *bitmap, UINT16 height)
{
    int i;
    long offset;
    int startingBit;

    offset = ((long)col >> 3) + ((long)row * 80);
    startingBit = col % 8;

    for (i = 0; i < height; i++) {

        if (startingBit == 0){
            *(base + offset + (80L * i)) |=
            bitmap[i];
        }
        else 
        {
            /*Right shift into first byte*/
            *(base + offset + (80L * i)) |=
            bitmap[i] >> startingBit;

            *(base + offset + (80L * i) + 1) |=
            bitmap[i] << (8 - startingBit);
        }
    }
}

int main()
{
    UINT8 *base = (UINT8 *)Physbase();

    UINT8 bitmap[8] = {
        0x3C,  /* 00111100 */
    0x42,  /* 01000010 */
    0xA5,  /* 10100101 */
    0x81,  /* 10000001 */

    0xA5,  /* 10100101 */
    0x99,  /* 10011001 */
    0x42,  /* 01000010 */
    0x3C   /* 00111100 */
    };

    /*blank_screen();*/

    plot_8bit_bitmap(base, 396, 636, bitmap, 8);

    Cnecin();

    return 0;
}
