#include "raster.h"

/* Atari ST high-resolution monochrome: 640 x 400, 80 bytes per row. */
#define SCREEN_ROWS 400
#define SCREEN_COLS 640
#define SCREEN_BYTES 32000

/* Private helper: does not write outside the screen. */
static void set_pixel(UINT8 *base, long row, long col)
{
    long offset;
    UINT8 mask;

    if (row < 0 || row >= SCREEN_ROWS || col < 0 || col >= SCREEN_COLS)
        return;

    /* row * 80 == row * (64 + 16), with no multiplication. */
    offset = (row << 6) + (row << 4) + (col >> 3);
    mask = (UINT8)(0x80U >> (col & 7));
    base[offset] |= mask;
}

static void erase_pixel(UINT8 *base, long row, long col)
{
    long offset;
    UINT8 mask;

    if (row < 0 || row >= SCREEN_ROWS || col < 0 || col >= SCREEN_COLS)
        return;

    offset = (row << 6) + (row << 4) + (col >> 3);
    mask = (UINT8)(0x80U >> (col & 7));
    base[offset] &= (UINT8)~mask;
}

/* Bresenham line: safe for coordinates outside the display. */
static void draw_line(UINT8 *base, long r0, long c0, long r1, long c1)
{
    long dx, dy, sx, sy, err, twice_err;

    dx = c1 - c0;
    if (dx < 0) dx = -dx;
    dy = r1 - r0;
    if (dy < 0) dy = -dy;

    sx = (c0 < c1) ? 1 : -1;
    sy = (r0 < r1) ? 1 : -1;
    err = dx - dy;

    for (;;) {
        set_pixel(base, r0, c0);
        if (r0 == r1 && c0 == c1)
            break;

        twice_err = err + err;
        if (twice_err > -dy) {
            err -= dy;
            c0 += sx;
        }
        if (twice_err < dx) {
            err += dx;
            r0 += sy;
        }
    }
}



void plot_line(UINT32 *base, int start_row, int start_col,
               int end_row, int end_col)
{
    draw_line((UINT8 *)base, start_row, start_col, end_row, end_col);
}

void plot_rectangle(UINT32 *base, int row, int col,
                    UINT16 length, UINT16 width)
{
    long top, bottom, left, right;

    if (length == 0 || width == 0)
        return;

    top = row;
    left = col;
    bottom = top + (long)length - 1;
    right = left + (long)width - 1;

    draw_line((UINT8 *)base, top, left, top, right);
    draw_line((UINT8 *)base, bottom, left, bottom, right);
    draw_line((UINT8 *)base, top, left, bottom, left);
    draw_line((UINT8 *)base, top, right, bottom, right);
}

void plot_square(UINT32 *base, int row, int col, UINT16 side)
{
    plot_rectangle(base, row, col, side, side);
}

void plot_triangle(UINT32 *base, int row, int col,
                   UINT16 triangle_base, UINT16 height, UINT8 direction)
{
    long r, c, horizontal_end, vertical_end;
    UINT8 *bytes = (UINT8 *)base;

    if (triangle_base == 0 || height == 0 || direction > 3)
        return;

    r = row;
    c = col;

    /* The supplied coordinate is the RIGHT-ANGLE vertex. */
    horizontal_end = (direction == 0 || direction == 2)
                   ? c + (long)triangle_base - 1
                   : c - (long)triangle_base + 1;
    vertical_end = (direction == 0 || direction == 1)
                 ? r + (long)height - 1
                 : r - (long)height + 1;

    draw_line(bytes, r, c, r, horizontal_end);
    draw_line(bytes, r, c, vertical_end, c);
    draw_line(bytes, r, horizontal_end, vertical_end, c);
}
