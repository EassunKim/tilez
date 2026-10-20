#include "raster.h"
#include "font.h"

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
/*Private helper*/
static int bitmap_visible(int row, int col, UINT16 height, int width)
{
    if (col <= -width || col >= 640 || 
        row >= 400 || (long)row + height <=0) {
        return 0;
    }
    return 1;
}
/*Private Helper*/
static void clip_rows(int row, UINT16 height,
                      int *startRow, int *endRow)
{
    *startRow = 0;
    *endRow = height;

    if (row < 0){
        /*Start row begins at first positive row of the bitmap*/
        *startRow = -row; 
    }
    /*Check if any rows below the bottom edge*/
    if ((long)row + height > 400){
        /*End row ends at the last row of the bitmap still on screen*/
        *endRow = 400 - row;
    }
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

void plot_8bit_bitmap(UINT8 *base, int row, int col,
                      const UINT8 *bitmap, UINT16 height)
{
    int i, startingBit, startRow, endRow;
    long offset;
    UINT8 bits;

    if (!bitmap_visible(row, col, height, 8)){
        return; 
    }

    clip_rows(row, height, &startRow, &endRow);
    /*Assume rows begin left of screen*/
    /*offset calculation, adding startRow forces visible portion of 
    row to begin at 0, startRow/endRow remain consistent,
    so loop only on eligible rows */
    offset = ((long)row + startRow << 6) + 
    ((long)row + startRow << 4);
    /*At the left of screen, first visible bit map bit placed
    at the first bit*/
    startingBit = 0;

    if (col >= 0) /*Not left of screen*/
    {/*Calculate offset regularly, >> 3, row may not begin at 0*/
        offset += ((long)col >> 3); 
        startingBit = col & 7; /*col % 8 - Q11*/
    }

    /*Loop through visible rows, if no clipping then 0-8 */
    for (i = startRow; i < endRow; i++) {

        bits = bitmap[i];
        /*Handles left edge */
        if (col < 0){
            /*Shift left to first visible portion of 8bit value
            111|10000 = 1000000*/
            bits = (UINT8)(bits << (-col));
        }
        *(base + offset ) |= bits >> startingBit;
        /*Checks if there is another byte available on the row*/
        if (startingBit !=0 && (col >> 3) < 79){
            *(base + offset  + 1) |= bits << (8 - startingBit);
        }
        offset += 80;
    }
}

void plot_16bit_bitmap(UINT16 *base, int row, int col,
                      const UINT16 *bitmap, UINT16 height)
{
    int i, startingBit, startRow, endRow;
    long offset;
    UINT16 bits;

    if (!bitmap_visible(row, col, height, 16)){
        return; 
    }
    clip_rows(row, height, &startRow, &endRow);

    offset = ((long)row + startRow << 5) + 
    ((long)row + startRow << 3);
    startingBit = 0;

    if (col >= 0){
        offset += ((long)col >> 4);
        startingBit = col & 15;
    }
    for (i = startRow; i < endRow; i++) {
        bits = bitmap[i];

        if (col < 0){
            bits = (UINT16)(bits << (-col));
        }
        *(base + offset ) |= bits >> startingBit;
        
        if (startingBit !=0 && (col >> 4) < 39){
            *(base + offset  + 1) |= bits << (16 - startingBit);
        }
        offset += 40;
    }
}

void plot_32bit_bitmap(UINT32 *base, int row, int col,
     const UINT32 *bitmap, UINT16 height)
{    
    int i, startingBit, startRow, endRow;
    long offset;
    UINT32 bits;

    if (!bitmap_visible(row, col, height, 32)){
        return; 
    }
    clip_rows(row, height, &startRow, &endRow);

    offset = ((long)row + startRow << 4) + 
    ((long)row + startRow << 2);
    startingBit = 0;

    if (col >= 0){
        offset += ((long)col >> 5);
        startingBit = col & 31;
    }
    for (i = startRow; i < endRow; i++) {
        bits = bitmap[i];

        if (col < 0){
            bits = (UINT32)(bits << (-col));
        }
        *(base + offset ) |= bits >> startingBit;
        
        if (startingBit !=0 && (col >> 5) < 19){
            *(base + offset  + 1) |= bits << (32 - startingBit);
        }
        offset += 20;
    }
}

void plot_character(UINT8 *base, int row, int col, char ch)
{
    const UINT8 *glyph;
    UINT8 ascii = (UINT8)ch;

    if (ascii < 32 || ascii > 126){
        return;
    }

    glyph = font + ((ascii - 32) << 3);

    plot_8bit_bitmap(base, row, col, glyph, 8);

}


void plot_string(UINT8 *base, int row, int col, char *ch)
{
    int i = 0;
    while (ch[i] != '\0')
    {
        plot_character(base, row, col, ch[i]);
        col +=8;
        i++;
    }
}
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


static int bitmap_visible(int row, int col, UINT16 height, int width)
{
    if (col <= -width || col >= 640 || 
        row >= 400 || (long)row + height <=0) {
        return 0;
    }
    return 1;
}

static void clip_rows(int row, UINT16 height,
                      int *startRow, int *endRow)
{
    *startRow = 0;
    *endRow = height;

    if (row < 0){
        /*Start row begins at first positive row of the bitmap*/
        *startRow = -row; 
    }
    /*Check if any rows below the bottom edge*/
    if ((long)row + height > 400){
        /*End row ends at the last row of the bitmap still on screen*/
        *endRow = 400 - row;
    }
}

void plot_8bit_bitmap(UINT8 *base, int row, int col,
                      const UINT8 *bitmap, UINT16 height)
{
    int i, startingBit, startRow, endRow;
    long offset;
    UINT8 bits;

    if (!bitmap_visible(row, col, height, 8)){
        return; 
    }

    clip_rows(row, height, &startRow, &endRow);
    /*Assume rows begin left of screen*/
    /*offset calculation, adding startRow forces visible portion of 
    row to begin at 0, startRow/endRow remain consistent,
    so loop only on eligible rows */
    offset = (((long)row + startRow) << 6) +
         (((long)row + startRow) << 4); 
    
    /*At the left of screen, first visible bit map bit placed
    at the first bit*/
    startingBit = 0;

    if (col >= 0) /*Not left of screen*/
    {/*Calculate offset regularly, >> 3, row may not begin at 0*/
        offset += ((long)col >> 3); 
        startingBit = col & 7; /*col % 8 - Q11*/
    }

    /*Loop through visible rows, if no clipping then 0-8 */
    for (i = startRow; i < endRow; i++) {

        bits = bitmap[i];
        /*Handles left edge */
        if (col < 0){
            /*Shift left to first visible portion of 8bit value
            111|10000 = 1000000*/
            bits = (UINT8)(bits << (-col));
        }
        *(base + offset ) |= bits >> startingBit;
        /*Checks if there is another byte available on the row*/
        if (startingBit !=0 && (col >> 3) < 79){
            *(base + offset  + 1) |= bits << (8 - startingBit);
        }
        offset += 80;
    }
}


