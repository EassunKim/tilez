SCREEN_WIDTH     equ     640
SCREEN_HEIGHT     equ     400

BM_BASE         equ     8
BM_ROW          equ     12
BM_COL          equ     14
BM_BITMAP       equ     16
BM_HEIGHT       equ     20
BM_ROW_BYTES    equ     80

    xdef    _plot_8bit_bitmap
    xdef    _plot_16bit_bitmap
    xdef    _plot_32bit_bitmap
    xdef    _plot_character
    xdef    _plot_string
    xref    _font

/*----- Function: bitmap_visible - PRIVATE HELPER-----
 *
 * PURPOSE: Checks whether any portion of a bitmap
 *          is visible within the screen boundaries.
 *
 * INPUT: row - starting row of the bitmap.
 *        col - starting column of the bitmap.
 *        height - number of rows in the bitmap.
 *        width - number of pixels per bitmap row.
 *
 * OUTPUT: Returns 1 if any portion of the bitmap is
 *         visible, or 0 if completely off-screen.
 *
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              A bitmap with zero height is not visible.
 */


bitmap_visible:
    clr.w   d4

    ;check for 0 bitmap height
    tst.w   d2
    beq     bv_done

    ;check if bitmap is outside horizontal boundary
    move.w  d3,d5
    neg.w   d5

    cmp.w   d5,d1
    ble     bv_done

    ;Check if bitmap is below the screen
    cmpi.w  #SCREEN_WIDTH,d1
    bge     bv_done

    ;Check if bitmap ends above the scree
    cmpi.w  #SCREEN_HEIGHT,d0
    bge     bv_done

    move.w  d0,d4
    ext.l   d4

    clr.l   d5
    move.w  d2,d5
    add.l   d5,d4

    tst.l   d4
    ble     bv_not_visible

    moveq   #1,d4
    rts 

bv_not_visible:
    clr.l   d4

bv_done:
    rts

/*----- Function: clip_rows - PRIVATE HELPER -----
 *
 * PURPOSE: Determines which rows of a bitmap are visible
 *          within the screen's vertical boundaries.
 *
 * INPUT: row - starting row of the bitmap.
 *        height - total number of bitmap rows.
 *
 * OUTPUT: startRow - pointer to the first visible bitmap row.
 *         endRow - pointer to the ending row (exclusive).
 *         No return value.
 *
 * ASSUMPTIONS: The screen height is 400 pixels.
 *              The caller has already checked bitmap visibility.
 */

clip_rows:
    ;initialize visible range to entire bitmap
    clr.w   d4
    move.w  d2,d5

    ;adjust startRow if bitmap is above the screen
    tst.w   d0
    bpl     cr_check_bottom

    move.w  d0,d4
    neg.w   d4

cr_check_bottom:
    ;Calculate row + height to check bottom boundary
    move.w  d0,d6
    ext.l   d6

    clr.l   d7
    move.w  d2,d7
    add.l   d7,d6

    cmpi.l  #SCREEN_HEIGHT,d6
    ble     cr_end

    ;adjust endRow to exclude rows below the screen
    move.w  #SCREEN_HEIGHT,d5
    sub.w   d0,d5

cr_end:
    rts


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

_plot_8bit_bitmap:
    link    a6,#0
    movem.l d0-d7/a0-a2,-(sp)

    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d0
    move.w  BM_COL(a6),d1
    move.l  BM_BITMAP(a6),a1
    move.w  BM_HEIGHT(a6),d2

    ;Check if bitmap visible
    move.w  #8,d3
    jsr     bitmap_visible

    tst.w   d4
    beq     p8_done

    ;Get visible rows 
    
    jsr     clip_rows

    ;offset calculations :
    ;(row + startRow) * 80 bytes, 80 = 64 + 16 using shifts. 

    ext.l   d0
    move.w  d4,d6
    ext.l   d6
    add.l   d6,d0
    
    move.l  d0,d6 
    lsl.l   #6,d0 
    lsl.l   #4,d6
    add.l   d6,d0 

    adda.l  d0,a0 

    ;startingBit = 0

    clr.w   d3

    ;For positive columns, calculate byte offset and starting bit
    tst.w   d1
    bmi     p8_setup_loop

    move.w  d1,d6
    lsr.w   #3,d6
    adda.w  d6,a0

    move.w  d1,d3
    andi.w  #7,d3

p8_setup_loop:
    ;skip rows clipped above the screen
    adda.w  d4,a1

    ;Visible rows = endrow - startRow

    move.w  d5,d2
    sub.w   d4,d2

    tst.w   d2
    beq     p8_done

p8_plot_loop:
    ;read the next bitmap row
    clr.l   d4
    move.b  (a1)+,d4

    ;Shift bitmap bits left to compensate for a negative column
    tst.w   d1
    bpl     p8_first_byte

    move.w  d1,d6
    neg.w   d6
    lsl.b   d6,d4

p8_first_byte:
    ;Shift and OR bitmap bits into the first byte
    move.b  d4,d6 
    lsr.b   d3,d6 
    or.b    d6,(a0)

    ;check if a second byte is needed
    tst.w   d3 
    beq     p8_next_row

    move.w  d1,d6
    asr.w   #3,d6

    cmpi.w  #79,d6
    bge     p8_next_row

    ;Shift remaining bits into the next byte
    moveq   #8,d6
    sub.w   d3,d6

    move.b  d4,d7
    lsl.b   d6,d7 

    move.l  a0,a2
    addq.l  #1,a2
    or.b    d7,(a2)

p8_next_row:
    ;Advance to the next row
    adda.l  #BM_ROW_BYTES,a0

    subq.w  #1,d2
    bne     p8_plot_loop

p8_done:
    movem.l (sp)+,d0-d7/a0-a2
    unlk    a6
    rts 




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

_plot_16bit_bitmap:
    link    a6,#0
    movem.l d0-d7/a0-a2,-(sp)

    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d0
    move.w  BM_COL(a6),d1
    move.l  BM_BITMAP(a6),a1
    move.w  BM_HEIGHT(a6),d2

    ;Check if bitmap is visible
    move.w  #16,d3
    jsr     bitmap_visible

    tst.w   d4
    beq     p16_done

    ;Get visible rows 
    
    jsr     clip_rows

    ;offset calculations = (row + startRow) * 80

    ext.l   d0
    move.w  d4,d6
    ext.l   d6
    add.l   d6,d0
    
    move.l  d0,d6 
    lsl.l   #6,d0 
    lsl.l   #4,d6
    add.l   d6,d0 

    adda.l  d0,a0 

    ;startingBit = 0

    clr.w   d3

    ;Offset and starting bit for negative columns
    tst.w   d1
    bmi     p16_setup_loop

    move.w  d1,d6
    lsr.w   #4,d6
    lsl.w   #1,d6
    adda.w  d6,a0

    move.w  d1,d3
    andi.w  #15,d3

p16_setup_loop:
    ;skip clipped rows (2 bytes/row)
    move.w  d4,d6
    lsl.w   #1,d6
    adda.w  d6,a1

    move.w  d5,d2
    sub.w   d4,d2

    tst.w   d2
    beq     p16_done

p16_plot_loop:
    clr.l   d4
    move.w  (a1)+,d4

    tst.w   d1
    bpl     p16_first_byte

    move.w  d1,d6
    neg.w   d6
    lsl.w   d6,d4

p16_first_byte:
    move.w  d4,d6 
    lsr.w   d3,d6 
    or.w    d6,(a0)

    tst.w   d3 
    beq     p16_next_row

    move.w  d1,d6
    asr.w   #4,d6

    cmpi.w  #39,d6
    bge     p16_next_row

    ;Write remaining bits into the second 16-bit word
    moveq   #16,d6
    sub.w   d3,d6

    move.w  d4,d7
    lsl.w   d6,d7 

    move.l  a0,a2
    addq.l  #2,a2
    or.w    d7,(a2)

p16_next_row:
    adda.l  #BM_ROW_BYTES,a0

    subq.w  #1,d2
    bne     p16_plot_loop

p16_done:
    movem.l (sp)+,d0-d7/a0-a2
    unlk    a6
    rts 


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
 * ASSUMPTIONS: The screen uses 640x400 monochrome mode.
 *              Each bitmap row contains 8 pixels.
 *              Contains atleast height rows.
 */

_plot_32bit_bitmap:
    link    a6,#0
    movem.l d0-d7/a0-a2,-(sp)

    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d0
    move.w  BM_COL(a6),d1
    move.l  BM_BITMAP(a6),a1
    move.w  BM_HEIGHT(a6),d2

    ;Check if bitmap visible
    move.w  #32,d3
    jsr     bitmap_visible

    tst.w   d4
    beq     p32_done

    jsr     clip_rows

    ;Calculate screen offset
    ext.l   d0
    move.w  d4,d6
    ext.l   d6
    add.l   d6,d0
    
    move.l  d0,d6 
    lsl.l   #6,d0 
    lsl.l   #4,d6
    add.l   d6,d0 

    adda.l  d0,a0 

    clr.w   d3

    ;Calculate byte offset and starting bit within a 32-bit long

    tst.w   d1
    bmi     p32_setup_loop

    move.w  d1,d6
    lsr.w   #5,d6
    lsl.w   #2,d6
    adda.w  d6,a0

    move.w  d1,d3
    andi.w  #31,d3

p32_setup_loop:
    ;skip clipped rows (4 bytes per row)
    move.w  d4,d6
    lsl.w   #2,d6
    adda.w  d6,a1

    move.w  d5,d2
    sub.w   d4,d2

    tst.w   d2
    beq     p32_done

p32_plot_loop:

    clr.l   d4
    move.l  (a1)+,d4

    tst.w   d1
    bpl     p32_first_byte

    move.w  d1,d6
    neg.w   d6
    lsl.l   d6,d4

p32_first_byte:

    move.l  d4,d6 
    lsr.l   d3,d6 
    or.l    d6,(a0)


    tst.w   d3 
    beq     p32_next_row

    move.w  d1,d6
    asr.w   #5,d6

    cmpi.w  #19,d6
    bge     p32_next_row


    moveq   #32,d6
    sub.w   d3,d6

    move.l  d4,d7
    lsl.l   d6,d7 

    move.l  a0,a2
    addq.l  #4,a2
    or.l    d7,(a2)

p32_next_row:
    adda.l  #BM_ROW_BYTES,a0

    subq.w  #1,d2
    bne     p32_plot_loop

p32_done:
    movem.l (sp)+,d0-d7/a0-a2
    unlk    a6
    rts 

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

_plot_character:
    link    a6,#0, 
    movem.l d0-d7/a0-a2,-(sp)

    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d1
    move.w  BM_COL(a6),d2
    clr.w   d0 
    move.b  17(a6),d0 

    ;ASCII range check (32-126)
    cmpi.w  #32,d0 
    blo     pc_done 

    cmpi.w  #126,d0 
    bhi     pc_done

    ;glyph offset (ASCII - 32 ) * 8
    subi.w  #32,d0
    lsl.w   #3,d0 

    lea     _font,a1
    adda.w  d0,a1 

    ;call plot_8bit_bitmap {base,row,col,glyph,8} reverse
    move.w  #8,-(sp)
    move.l  a1,-(sp)
    move.w  d2,-(sp)
    move.w  d1,-(sp)
    move.l  a0,-(sp)

    jsr _plot_8bit_bitmap
    adda.l  #14,sp 

pc_done:
    movem.l (sp)+,d0-d7/a0-a2
    unlk    a6 
    rts 

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

_plot_string:
    link a6,#0
    movem.l d0-d7/a0-a2,-(sp)

    ;Paramters 
    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d0 
    move.w  BM_COL(a6),d1
    move.l  BM_BITMAP(a6),a1

ps_loop:
    ;read current character
    clr.w   d2
    move.b  (a1)+,d2

    ;null test
    tst.b   d2
    beq     ps_done

    ;plot_character call, with arguments in reverse order
    move.w  d2,-(sp)
    move.w  d1,-(sp)
    move.w  d0,-(sp)
    move.l  a0,-(sp)

    jsr _plot_character
    adda.l  #10,sp 

    ;move to next ch position
    addq.w  #8,d1 

    bra     ps_loop

ps_done:
    movem.l (sp)+,d0-d7/a0-a2
    unlk    a6 
    rts 


    


    