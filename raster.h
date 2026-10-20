/*------------------------------------------------------------
 * File: raster.h
 *
 * PURPOSE: Declares the raster graphics functions used
 *          to draw shapes, bitmaps, and text on the
 *          Atari ST monochrome screen.
 *
 * Team Members: Mohamad, Ujjwal, Matthew
 * Author: Mohamad, Ujjwal
 *------------------------------------------------------------*/

#ifndef RASTER_H
#define RASTER_H

#include "types.h"

/*----- Function: clear_screen -----
 *
 * PURPOSE: Clears the entire screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen buffer is valid.
 */
void clear_screen(UINT32 *base);


/*----- Function: clear_region -----
 *
 * PURPOSE: Clears a rectangular region of the screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row, col - top-left pixel of the region.
 *        length - number of rows to clear.
 *        width - number of columns to clear.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen buffer is valid.
 */
void clear_region(UINT32 *base, int row, int col, UINT16 length, UINT16 width);


/*----- Function: plot_pixel -----
 *
 * PURPOSE: Draws a single pixel on the screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the pixel.
 *        col - horizontal position of the pixel.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_pixel(UINT8 *base, int row, int col);


/*----- Function: plot_horizontal_line -----
 *
 * PURPOSE: Draws a horizontal line starting at the
 *          specified screen position.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the line.
 *        col - starting horizontal position.
 *        length - number of pixels in the line.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_horizontal_line(UINT32 *base, int row, int col, UINT16 length);


/*----- Function: plot_vertical_line -----
 *
 * PURPOSE: Draws a vertical line starting at the
 *          specified screen position.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - starting vertical position.
 *        col - horizontal position of the line.
 *        length - number of pixels in the line.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_vertical_line(UINT32 *base, int row, int col, UINT16 length);


/*----- Function: plot_line -----
 *
 * PURPOSE: Draws a line between two specified points
 *          on the screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *        start_row - vertical position of the first point.
 *        start_col - horizontal position of the first point.
 *        end_row - vertical position of the second point.
 *        end_col - horizontal position of the second point.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_line(UINT32 *base, int start_row, int start_col, int end_row, int end_col);


/*----- Function: plot_rectangle -----
 *
 * PURPOSE: Draws the outline of a rectangle on the screen
 *          using the specified position and dimensions.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the top-left corner.
 *        col - horizontal position of the top-left corner.
 *        length - height of the rectangle in pixels.
 *        width - width of the rectangle in pixels.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_rectangle(UINT32 *base, int row, int col, UINT16 length, UINT16 width);


/*----- Function: plot_square -----
 *
 * PURPOSE: Draws the outline of a square on the screen
 *          using the specified position and side length.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the top-left corner.
 *        col - horizontal position of the top-left corner.
 *        side - length of each side in pixels.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_square(UINT32 *base, int row, int col, UINT16 side);


/*----- Function: plot_triangle -----
 *
 * PURPOSE: Draws the outline of a right triangle using
 *          the specified position, dimensions, and direction.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the right-angle corner.
 *        col - horizontal position of the right-angle corner.
 *        triangle_base - width of the triangle in pixels.
 *        height - height of the triangle in pixels.
 *        direction - location of the right-angle corner:
 *                    0 = top-left
 *                    1 = top-right
 *                    2 = bottom-left
 *                    3 = bottom-right
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 */
void plot_triangle(UINT32 *base, int row, int col, UINT16 triangle_base, UINT16 height, UINT8 direction);


/*----- Function: plot_8bit_bitmap -----
 *
 * PURPOSE: Draws an 8-pixel-wide bitmap onto the
 *          Atari ST monochrome screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the bitmap.
 *        col - horizontal position of the bitmap.
 *        bitmap - pointer to the bitmap pixel data.
 *        height - number of rows in the bitmap.
 *
 * OUTPUT: Modifies the screen buffer to draw the bitmap, 
 *         but thers no return value.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              Each bitmap row contains 8 pixels.
 *              Contains atleast height rows.
 */

void plot_8bit_bitmap(UINT8 *base, int row, int col, const UINT8 *bitmap, UINT16 height);


/*----- Function: plot_16bit_bitmap -----
 *
 * PURPOSE: Draws an 16-pixel-wide bitmap onto the
 *          Atari ST monochrome screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the bitmap.
 *        col - horizontal position of the bitmap.
 *        bitmap - pointer to the bitmap pixel data.
 *        height - number of rows in the bitmap.
 *
 * OUTPUT: Modifies the screen buffer to draw the bitmap, 
 *         but thers no return value.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              Each bitmap row contains 16 pixels.
 *              Contains atleast height rows.
 */
void plot_16bit_bitmap(UINT16 *base, int row, int col, const UINT16 *bitmap, UINT16 height);


/*----- Function: plot_32bit_bitmap -----
 *
 * PURPOSE: Draws a 32-pixel-wide bitmap onto the
 *          Atari ST monochrome screen.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the bitmap.
 *        col - horizontal position of the bitmap.
 *        bitmap - pointer to the bitmap pixel data.
 *        height - number of rows in the bitmap.
 *
 * OUTPUT: None.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              Each bitmap row contains 32 pixels.
 */
void plot_32bit_bitmap(UINT32 *base, int row, int col, const UINT32 *bitmap, UINT16 height);


/*----- Function: plot_character -----
 *
 * PURPOSE: Draws a single character on the screen
 *          using a bitmap from the font table.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the character.
 *        col - horizontal position of the character.
 *        ch - character to draw.
 *
 * OUTPUT: Modifies the screen buffer to draw the bitmap, 
 *         but thers no return value.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              The required character exists in the font table.
 *              Font table contains ASCII 32 to 126, represented
 *              by an 8x8 bitmap. Chars outside this range are ignored.
 */
void plot_character(UINT8 *base, int row, int col, char ch);


/*----- Function: plot_string -----
 *
 * PURPOSE: Draws a string of characters on the screen
 *          using bitmaps from the font table.
 *
 * INPUT: base - pointer to the screen buffer.
 *        row - vertical position of the string.
 *        col - horizontal starting position.
 *        ch - pointer to the string to draw.
 *
 * OUTPUT: Modifies the screen buffer to draw the string,
 *         but there is no return.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              The string is null-terminated.
 *              Characters are represented by 8x8 bitmaps.
 *              Required characters are printable ASCII (32-126).
 */
void plot_string(UINT8 *base, int row, int col, char *ch);

#endif
