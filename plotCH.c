#include <linea.h>
#include <osbind.h>
#include <stdio.h>
typedef unsigned char UINT8;

void blank_screen();

void plot_character(UINT8 *base, int row, int col, char ch)
{
	UINT8 *font;
	long offset;
	int i;

	
	linea0();                    /* initialize line-a variables */
	font = (UINT8 *)V_FNT_AD;     /* get start address of font table */

    offset = ((long)col >> 3) + ((long)row * 80);
	
	for (i = 0; i < 16; i++){
		*(base + offset  + i * 80) = *(font + ch + i * 256);
	}

}
void plot_string(UINT8 *base, int row, int col, char *ch)
{
    int i = 0;

    while (ch[i] != '\0')
    {
        plot_character(base, row, col + i * 8, ch[i]);
        i++;
    }
}
void blank_screen()
{
	printf("\033E\033f\n");     /* VT52: clear screen, hide cursor */
}

int main()
{
    UINT8 *base = (UINT8 *)Physbase();

    blank_screen();

    plot_string(base, 100, 616, "HELLO");

    Cnecin();
    return 0;
}

