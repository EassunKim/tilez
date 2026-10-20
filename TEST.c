/*
 * File: TEST.c
 * Course: COMP 2659
 * Project: Checkpoint 2
 *
 * Team Members: [Mohamed, Ujjwal, Matthew]
 * Author: [Ujjwal]
 *
 * PURPOSE:
 * Tests the raster graphics library to comfirm the implemented
 * plotting functions work correctly. Tests include
 * normal drawing, screen boundary clipping, and
 * invalid coordinates. The tester reports pass/fail status.
 */
#include <stdio.h>
#include <string.h>
#include "raster.h"

#define N 32000
static UINT8 screen[N + 16];
static int passes = 0;
static int failures = 0;

static int is_set(int r, int c)
{
    return (screen[8 + r * 80 + c / 8] & (0x80 >> (c % 8))) != 0;
}

static void reset_screen(void)
{
    memset(screen, 0, sizeof(screen));
    memset(screen, 0x5A, 8);
    memset(screen + 8 + N, 0x5A, 8);
}

static void verify(const char *name, int condition)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (screen[i] != 0x5A || screen[8 + N + i] != 0x5A)
            condition = 0;
    }
    if (condition) { passes++; printf("PASS %s\n", name); }
    else { failures++; printf("FAIL %s\n", name); }
}

int main(void)
{
    UINT32 *base = (UINT32 *)(screen + 8);
    UINT8 *bytes = screen + 8;
    reset_screen();
    plot_pixel(bytes, 0, 0);
    plot_pixel(bytes, 399, 639);
    plot_pixel(bytes, -1, 5);
    plot_pixel(bytes, 400, 10);
    plot_pixel(bytes, 5, 640);
    verify("pixel corners + invalid coordinates", is_set(0, 0) && is_set(399, 639));

    reset_screen();
    plot_horizontal_line(base, 5, -5, 10);
    verify("horizontal clips left", is_set(5, 0) && is_set(5, 4) && !is_set(5, 5));
    reset_screen();
    plot_horizontal_line(base, 10, 637, 10);
    verify("horizontal clips right", is_set(10, 637) && is_set(10, 639) && !is_set(11, 0));
    reset_screen();
    plot_vertical_line(base, -3, 8, 7);
    verify("vertical clips top", is_set(0, 8) && is_set(3, 8) && !is_set(4, 8));
    reset_screen();
    plot_vertical_line(base, 398, 8, 10);
    verify("vertical clips bottom", is_set(398, 8) && is_set(399, 8));

    reset_screen();
    plot_line(base, -10, -10, 10, 10);
    verify("diagonal clips upper-left", is_set(0, 0) && is_set(10, 10));
    reset_screen();
    plot_line(base, 395, 635, 405, 645);
    verify("diagonal clips bottom-right", is_set(395, 635) && is_set(399, 639));
    reset_screen();
    plot_line(base, 8, 30, 8, 30);
    verify("single-point line", is_set(8, 30));

    reset_screen();
    plot_rectangle(base, 398, 638, 6, 6);
    verify("rectangle partially outside screen", is_set(398, 638) && is_set(398, 639) && is_set(399, 638));
    reset_screen();
    plot_rectangle(base, 20, 20, 0, 5);
    verify("zero-height rectangle does nothing", !is_set(19, 20) && !is_set(20, 20));
    reset_screen();
    plot_square(base, 0, 0, 3);
    verify("square 3x3", is_set(0, 0) && is_set(0, 2) && is_set(2, 0) && is_set(2, 2) && !is_set(1, 1));

    reset_screen();
    memset(screen + 8, 0xff, N);
    clear_region(base, -2, -2, 4, 4);
    verify("clear_region clips top-left", !is_set(0, 0) && !is_set(1, 1) && is_set(2, 2));
    reset_screen();
    memset(screen + 8, 0xff, N);
    clear_screen(base);
    verify("clear_screen clears all", !is_set(0, 0) && !is_set(399, 639));

    reset_screen();
    plot_triangle(base, 10, 10, 5, 5, 0);
    verify("triangle top-left", is_set(10, 14) && is_set(14, 10) && is_set(10, 10));
    reset_screen();
    plot_triangle(base, 10, 10, 5, 5, 1);
    verify("triangle top-right", is_set(10, 6) && is_set(14, 10) && is_set(10, 10) && !is_set(10, 14));
    reset_screen();
    plot_triangle(base, 10, 10, 5, 5, 2);
    verify("triangle bottom-left", is_set(10, 14) && is_set(6, 10) && is_set(10, 10) && !is_set(14, 10));
    reset_screen();
    plot_triangle(base, 10, 10, 5, 5, 3);
    verify("triangle bottom-right", is_set(10, 6) && is_set(6, 10) && is_set(10, 10));
    reset_screen();
    plot_triangle(base, 399, 639, 5, 5, 0);
    verify("triangle clips at bottom-right", is_set(399, 639));

    reset_screen();
    plot_horizontal_line(base, 4, 4, 0);
    plot_vertical_line(base, 4, 4, 0);
    plot_triangle(base, 4, 4, 0, 2, 0);
    verify("zero dimensions do nothing", !is_set(4, 4));


/* 8 bit bitmap tests */
{
    const UINT8 bitmap8[] = {
        0x81, 0x42, 0x24, 0x18,
        0x18, 0x24, 0x42, 0x81
    };

    reset_screen();
    plot_8bit_bitmap(bytes, 10, 16, bitmap8, 8);
    verify("8-bit bitmap normal", is_set(10, 16) && is_set(10, 23) && is_set(11, 17) && is_set(13, 19) 
    && is_set(17, 23) && !is_set(10, 17));

    reset_screen();
    plot_8bit_bitmap(bytes, -2, 16, bitmap8, 8);
    verify("8-bit bitmap clips top", is_set(0, 18) && is_set(0, 21) && is_set(5, 16) && is_set(5, 23));

    reset_screen();
    plot_8bit_bitmap(bytes, 10, 637, bitmap8, 8);
    verify("8-bit bitmap clips right", is_set(10, 637) && is_set(11, 638) && is_set(12, 639) && 
    !is_set(10, 638));

    reset_screen();
    plot_8bit_bitmap(bytes, 398, 16, bitmap8, 8);
    verify("8-bit clips bottom", is_set(398, 16) && is_set(398, 23) && is_set(399, 17) && 
    is_set(399, 22) && !is_set(397, 16));
}
{
    const UINT8 bitmap8extra[] = {
        0xFF, 0x81, 0x81, 0xFF
    };

    reset_screen();
    plot_8bit_bitmap(bytes, 10, -3, bitmap8extra, 4);
    verify("8-bit clips left", is_set(10, 0) && is_set(10, 4) && is_set(11, 4) &&!is_set(11, 0));

    reset_screen();
    plot_8bit_bitmap(bytes, 20, 20, bitmap8extra, 4);
    verify("8-bit height 4", is_set(20, 20) && is_set(23, 27) && !is_set(24, 20));
}


/* 16-bit bitmap tests */
{
    const UINT16 bitmap16[] = {
        0x8001, 0x4002, 0x2004, 0x1008,
        0x1008, 0x2004, 0x4002, 0x8001
    };

    reset_screen();
    plot_16bit_bitmap((UINT16 *)base, 20, 32, bitmap16, 8);
    verify("16-bit bitmap normal", is_set(20, 32) && is_set(20, 47) && is_set(21, 33) && is_set(21, 46)
    && is_set(27, 47) && !is_set(20, 33));

    reset_screen();
    plot_16bit_bitmap((UINT16 *)base, 30, -1, bitmap16, 8);
    verify("16-bit bitmap clips left", is_set(30, 14) && is_set(31, 0) && is_set(31, 13));
    

    reset_screen();
    plot_16bit_bitmap((UINT16 *)base, 399, 0, bitmap16, 8);
    verify("16-bit bitmap clips bottom", is_set(399, 0) && is_set(399, 15));
}
{
    const UINT16 bitmap16extra[] = {
    0x8001, 0x4002, 0x2004 
    };

    reset_screen();
    plot_16bit_bitmap((UINT16 *)base, -1, 32, bitmap16extra, 3);
    verify("16-bit clips top", is_set(0, 33) && is_set(0, 46) && is_set(1, 34) && is_set(1, 45));

    reset_screen();
    plot_16bit_bitmap((UINT16 *)base, 30, 637, bitmap16extra, 3);
    verify("16-bit clips right", is_set(30, 637) && is_set(31, 638) && is_set(32, 639)
    && !is_set(30, 638));
}
    


/* 32-bit bitmap tests  */
{
    const UINT32 bitmap32[] = {
        0x80000001UL, 0x40000002UL,
        0x20000004UL, 0x10000008UL,
        0x10000008UL, 0x20000004UL,
        0x40000002UL, 0x80000001UL
    };

    reset_screen();
    plot_32bit_bitmap(base, 40, 64, bitmap32, 8);
    verify("32-bit bitmap normal", is_set(40, 64) && is_set(40, 95) && is_set(41, 65) && is_set(41, 94) 
    && is_set(47, 95) && !is_set(40, 65));

    reset_screen();
    plot_32bit_bitmap(base, 50, 630, bitmap32, 8);
    verify("32-bit bitmap clips right", is_set(50, 630) && is_set(51, 631) &&  is_set(52, 632) 
    && !is_set(50, 631));

    reset_screen();
    plot_32bit_bitmap(base, -1, 0, bitmap32, 8);
    verify("32-bit bitmap clips top", is_set(0, 1) && is_set(0, 30));
}
{
    const UINT32 bitmap32extra[] = {
        0xFFFFFFFFUL, 0x80000001UL
    };


    reset_screen();
    plot_32bit_bitmap(base, 40, -7, bitmap32extra, 2);
    verify("32-bit clips left at -7", is_set(40, 0) && is_set(40, 24) && is_set(41, 24) && !is_set(41, 0));

    reset_screen();
    plot_32bit_bitmap(base, 398, 32, bitmap32extra, 2);
    verify("32-bit clips bottom", is_set(398, 32) && is_set(398, 63) && is_set(399, 32) 
    && is_set(399, 63) && !is_set(397, 32));

    reset_screen();
    plot_32bit_bitmap(base, 401, 32, bitmap32extra, 2);
    verify("32-bit completely off screen", !is_set(399, 32) && !is_set(399, 63) && !is_set(0, 32));


}

/*Character tests */
    reset_screen();
    plot_character(bytes, 60, 80, 'A');

    verify("character A", is_set(61, 81) && is_set(61, 84) && is_set(64, 80) &&
    is_set(64, 85) && !is_set(64, 87));

    reset_screen();
    plot_character(bytes, 60, 80, ' ');
    verify("space draws nothing", !is_set(60, 80) && !is_set(63, 84));

    reset_screen();
    plot_character(bytes, 60, 80, '\n');
    verify("non-printable character ignored", !is_set(60, 80) && !is_set(63, 84));

    reset_screen();
    plot_character(bytes, 60, -2, 'A');
    verify("character clips left", is_set(61, 0) && is_set(61, 2) && !is_set(60, 0));

    reset_screen();
    plot_character(bytes, 395, 80, 'A');
    verify("character clips bottom", is_set(399, 80) && is_set(399, 85) &&
    !is_set(394, 80));

/* String test*/
    reset_screen();
    plot_string(bytes, 80, 80, "AA");
    verify("string two characters", is_set(81, 83) && is_set(81, 91) 
    && is_set(84, 80) && is_set(84, 88));

    reset_screen();
    plot_string(bytes, 80, 80, "");
    verify("empty string draws nothing", !is_set(80, 80) && !is_set(81, 83));

    reset_screen();
    plot_string(bytes, 80, 636, "AA");
    verify("string clips at right edge", is_set(81, 639));

    reset_screen();
    plot_string(bytes, -3, 80, "AA");
    verify("string clips top", is_set(1, 80) && is_set(1, 85) 
    && is_set(1, 88));


    printf("RESULT %d passed, %d failed\n", passes, failures);
    return failures ? 1 : 0;
}
    
