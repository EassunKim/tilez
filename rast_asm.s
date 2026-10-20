;------------------------------------------------------------
; File: rast_asm.s
;
; PURPOSE: Implements raster graphics routines in Motorola
;          68000 assembly for the Atari ST 640x400
;          monochrome display.
;
; Team Members: Mohamad, Ujjwal, Matthew
; Author: Mohamad
;
; The screen contains 80 bytes per row and 32000 bytes
; in total. Coordinates are specified as (row, column).
;------------------------------------------------------------


        xdef    _clear_screen

base    equ     8

;----- Function: clear_screen -----
;
; PURPOSE: Clears the entire monochrome screen.
;
; INPUT: base - address of the screen buffer.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen buffer contains 32000 bytes.
;              The base address is valid.
;------------------------------------------------------------

_clear_screen:
        link    a6,#0
       movem.l d0-d1/a0,-(sp)
        movea.l base(a6),a0
        move.w  #7999,d0
        clr.l   d1

; Clear 8000 longwords, or 32000 bytes.
fill_loop:
        move.l  d1,(a0)+
        dbra    d0,fill_loop

        movem.l (sp)+,d0-d1/a0
        unlk    a6
        rts


        xdef    _clear_region

;----- Function: clear_region -----
;
; PURPOSE: Clears a rectangular region of the screen.
;
; INPUT: base - address of the screen buffer.
;        row - starting vertical coordinate.
;        col - starting horizontal coordinate.
;        length - number of rows to clear.
;        width - number of columns to clear.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen uses 640x400 monochrome mode.
;              Coordinates are clipped to the screen.
;------------------------------------------------------------

_clear_region:
        link    a6,#0
        movem.l d0-d7/a0-a2,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

        moveq   #0,d2
        move.w  16(a6),d2
        moveq   #0,d3
        move.w  18(a6),d3

        tst.l   d2
        beq     cr_done
        tst.l   d3
        beq     cr_done

        add.l   d0,d2
        add.l   d1,d3

; Clip the starting and ending coordinates.
        tst.l   d0
        bge     cr_rpos
        moveq   #0,d0
cr_rpos:
        tst.l   d1
        bge     cr_cpos
        moveq   #0,d1
cr_cpos:
        cmpi.l  #400,d2
        ble     cr_rend
        move.l  #400,d2
cr_rend:
        cmpi.l  #640,d3
        ble     cr_cend
        move.l  #640,d3
cr_cend:
        cmp.l   d2,d0
        bge     cr_done
        cmp.l   d3,d1
        bge     cr_done

; Calculate the screen address of the first row.
        movea.l 8(a6),a1
        move.l  d0,d6
        lsl.l   #6,d6
        move.l  d0,d7
        lsl.l   #4,d7
        add.l   d7,d6
        adda.l  d6,a1

        move.l  d0,d4

cr_row:
        move.l  d1,d5

cr_col:
        move.l  d5,d6
        lsr.l   #3,d6
        movea.l a1,a2
        adda.l  d6,a2

; Clear the selected bit in the screen byte.
        move.l  d5,d7
        eori.w  #7,d7
        andi.w  #7,d7
        bclr    d7,(a2)

        addq.l  #1,d5
        cmp.l   d3,d5
        blt     cr_col

        adda.w  #80,a1
        addq.l  #1,d4
        cmp.l   d2,d4
        blt     cr_row

cr_done:
        movem.l (sp)+,d0-d7/a0-a2
        unlk    a6
        rts


        xdef    _plot_pixel

;----- Function: plot_pixel -----
;
; PURPOSE: Draws a single pixel on the screen.
;
; INPUT: base - address of the screen buffer.
;        row - vertical pixel coordinate.
;        col - horizontal pixel coordinate.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen uses 640x400 monochrome mode.
;              Pixels outside the screen are ignored.
;------------------------------------------------------------

_plot_pixel:
        link    a6,#0
        movem.l d0-d3/a0,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

; Check the screen boundaries.
        tst.l   d0
        bmi     pp_done
        cmpi.l  #400,d0
        bge     pp_done

        tst.l   d1
        bmi     pp_done
        cmpi.l  #640,d1
        bge     pp_done

        movea.l 8(a6),a0

; Calculate byte offset using row * 80 + col / 8.
        move.l  d0,d2
        lsl.l   #6,d2
        move.l  d0,d3
        lsl.l   #4,d3
        add.l   d3,d2

        move.l  d1,d3
        lsr.l   #3,d3
        add.l   d3,d2
        adda.l  d2,a0

; Set the required pixel bit.
        eori.w  #7,d1
        andi.w  #7,d1
        bset    d1,(a0)

pp_done:
        movem.l (sp)+,d0-d3/a0
        unlk    a6
        rts


        xdef    _plot_horizontal_line

;----- Function: plot_horizontal_line -----
;
; PURPOSE: Draws a horizontal line on the screen.
;
; INPUT: base - address of the screen buffer.
;        row - vertical position of the line.
;        col - starting horizontal position.
;        length - number of pixels in the line.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen uses 640x400 monochrome mode.
;              The line is clipped horizontally.
;------------------------------------------------------------

_plot_horizontal_line:
        link    a6,#0
        movem.l d0-d6/a0,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

        moveq   #0,d2
        move.w  16(a6),d2

        tst.l   d0
        bmi     ph_done
        cmpi.l  #400,d0
        bge     ph_done

        tst.l   d2
        beq     ph_done

        move.l  d1,d3
        add.l   d2,d3

; Clip the horizontal endpoints.
        tst.l   d1
        bge     ph_start_ok
        moveq   #0,d1

ph_start_ok:
        cmpi.l  #640,d3
        ble     ph_end_ok
        move.l  #640,d3

ph_end_ok:
        cmp.l   d3,d1
        bge     ph_done

        movea.l 8(a6),a0

        move.l  d0,d4
        lsl.l   #6,d4
        move.l  d0,d5
        lsl.l   #4,d5
        add.l   d5,d4
        adda.l  d4,a0

; Draw each pixel along the line.
ph_loop:
        move.l  d1,d4
        lsr.l   #3,d4

        move.l  d1,d5
        eori.w  #7,d5
        andi.w  #7,d5

        movea.l a0,a1
        adda.l  d4,a1
        bset    d5,(a1)

        addq.l  #1,d1
        cmp.l   d3,d1
        blt     ph_loop

ph_done:
        movem.l (sp)+,d0-d6/a0
        unlk    a6
        rts


        xdef    _plot_vertical_line

;----- Function: plot_vertical_line -----
;
; PURPOSE: Draws a vertical line on the screen.
;
; INPUT: base - address of the screen buffer.
;        row - starting vertical position.
;        col - horizontal position of the line.
;        length - number of pixels in the line.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen uses 640x400 monochrome mode.
;              The line is clipped vertically.
;------------------------------------------------------------

_plot_vertical_line:
        link    a6,#0
        movem.l d0-d6/a0-a1,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

        moveq   #0,d2
        move.w  16(a6),d2

        tst.l   d1
        bmi     pv_done
        cmpi.l  #640,d1
        bge     pv_done

        tst.l   d2
        beq     pv_done

        move.l  d0,d3
        add.l   d2,d3

; Clip the vertical endpoints.
        tst.l   d0
        bge     pv_start_ok
        moveq   #0,d0

pv_start_ok:
        cmpi.l  #400,d3
        ble     pv_end_ok
        move.l  #400,d3

pv_end_ok:
        cmp.l   d3,d0
        bge     pv_done

        movea.l 8(a6),a0

        move.l  d0,d4
        lsl.l   #6,d4
        move.l  d0,d5
        lsl.l   #4,d5
        add.l   d5,d4

        move.l  d1,d5
        lsr.l   #3,d5
        add.l   d5,d4
        adda.l  d4,a0

        eori.w  #7,d1
        andi.w  #7,d1

; Move 80 bytes to reach the next screen row.
pv_loop:
        bset    d1,(a0)
        adda.w  #80,a0

        addq.l  #1,d0
        cmp.l   d3,d0
        blt     pv_loop

pv_done:
        movem.l (sp)+,d0-d6/a0-a1
        unlk    a6
        rts


        xdef    _plot_line

;----- Function: plot_line -----
;
; PURPOSE: Draws a line between two points using
;          Bresenham's line algorithm.
;
; INPUT: base - address of the screen buffer.
;        start_row - starting vertical coordinate.
;        start_col - starting horizontal coordinate.
;        end_row - ending vertical coordinate.
;        end_col - ending horizontal coordinate.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen uses 640x400 monochrome mode.
;              Individual pixels are clipped.
;------------------------------------------------------------

_plot_line:
        link    a6,#0
        movem.l d0-d7/a0-a3,-(sp)

        movea.l 8(a6),a0
        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1
        move.w  16(a6),d2
        ext.l   d2
        move.w  18(a6),d3
        ext.l   d3

; Calculate absolute horizontal distance.
        move.l  d3,d4
        sub.l   d1,d4
        bpl     pl_dx_ok
        neg.l   d4
pl_dx_ok:

; Calculate absolute vertical distance.
        move.l  d2,d5
        sub.l   d0,d5
        bpl     pl_dy_ok
        neg.l   d5
pl_dy_ok:

; Determine horizontal movement direction.
        moveq   #1,d7
        cmp.l   d3,d1
        blt     pl_sx_ok
        moveq   #-1,d7
pl_sx_ok:
        movea.l d7,a1

; Determine vertical movement direction.
        moveq   #1,d7
        cmp.l   d2,d0
        blt     pl_sy_ok
        moveq   #-1,d7
pl_sy_ok:
        movea.l d7,a2

        move.l  d4,d6
        sub.l   d5,d6

pl_loop:
; Only draw pixels inside the screen.
        tst.l   d0
        bmi     pl_check_end
        cmpi.l  #400,d0
        bge     pl_check_end
        tst.l   d1
        bmi     pl_check_end
        cmpi.l  #640,d1
        bge     pl_check_end

; Calculate row * 80 and the byte offset.
        move.l  d0,d7
        lsl.l   #2,d7
        add.l   d0,d7
        lsl.l   #4,d7

        move.l  d1,-(sp)
        lsr.l   #3,d1
        add.l   d1,d7
        move.l  (sp)+,d1

        movea.l a0,a3
        adda.l  d7,a3

        move.l  d1,d7
        eori.w  #7,d7
        andi.w  #7,d7
        bset    d7,(a3)

; Stop after reaching the ending point.
pl_check_end:
        cmp.l   d2,d0
        bne     pl_step
        cmp.l   d3,d1
        beq     pl_done

; Update the Bresenham error value.
pl_step:
        move.l  d6,d7
        add.l   d7,d7

        move.l  d5,-(sp)
        neg.l   (sp)
        cmp.l   (sp)+,d7
        ble     pl_skip_col

        sub.l   d5,d6
        move.l  a1,-(sp)
        add.l   (sp)+,d1

pl_skip_col:
        cmp.l   d4,d7
        bge     pl_skip_row

        add.l   d4,d6
        move.l  a2,-(sp)
        add.l   (sp)+,d0

pl_skip_row:
        bra     pl_loop

pl_done:
        movem.l (sp)+,d0-d7/a0-a3
        unlk    a6
        rts


        xdef    _plot_rectangle

;----- Function: plot_rectangle -----
;
; PURPOSE: Draws the outline of a rectangle using
;          four lines.
;
; INPUT: base - address of the screen buffer.
;        row - top-left vertical coordinate.
;        col - top-left horizontal coordinate.
;        length - height of the rectangle.
;        width - width of the rectangle.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The screen uses 640x400 monochrome mode.
;              Drawing uses the plot_line routine.
;------------------------------------------------------------

_plot_rectangle:
        link    a6,#0
        movem.l d0-d3/a0,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

        moveq   #0,d2
        move.w  16(a6),d2
        moveq   #0,d3
        move.w  18(a6),d3

        tst.l   d2
        beq     pr_done
        tst.l   d3
        beq     pr_done

; Determine the opposite corner.
        add.l   d0,d2
        subq.l  #1,d2
        add.l   d1,d3
        subq.l  #1,d3

        movea.l 8(a6),a0

; Limit the rectangle endpoints.
        cmpi.l  #-1,d0
        bge     pr_top_high
        moveq   #-1,d0
pr_top_high:
        cmpi.l  #400,d0
        ble     pr_bottom_low
        move.l  #400,d0

pr_bottom_low:
        cmpi.l  #-1,d2
        bge     pr_bottom_high
        moveq   #-1,d2
pr_bottom_high:
        cmpi.l  #400,d2
        ble     pr_left_low
        move.l  #400,d2

pr_left_low:
        cmpi.l  #-1,d1
        bge     pr_left_high
        moveq   #-1,d1
pr_left_high:
        cmpi.l  #640,d1
        ble     pr_right_low
        move.l  #640,d1

pr_right_low:
        cmpi.l  #-1,d3
        bge     pr_right_high
        moveq   #-1,d3
pr_right_high:
        cmpi.l  #640,d3
        ble     pr_top_line
        move.l  #640,d3

; Draw the top edge.
pr_top_line:
        move.w  d3,-(sp)
        move.w  d0,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

; Draw the bottom edge.
        move.w  d3,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.w  d2,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

; Draw the left edge.
        move.w  d1,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

; Draw the right edge.
        move.w  d3,-(sp)
        move.w  d2,-(sp)
        move.w  d3,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

pr_done:
        movem.l (sp)+,d0-d3/a0
        unlk    a6
        rts


        xdef    _plot_square

;----- Function: plot_square -----
;
; PURPOSE: Draws the outline of a square by calling
;          the rectangle drawing routine.
;
; INPUT: base - address of the screen buffer.
;        row - top-left vertical coordinate.
;        col - top-left horizontal coordinate.
;        side - length of each side in pixels.
;
; OUTPUT: None.
;
; ASSUMPTIONS: The rectangle routine is available.
;              The screen uses 640x400 monochrome mode.
;------------------------------------------------------------

_plot_square:
        link    a6,#0
        movem.l d0/a0,-(sp)

        movea.l 8(a6),a0
        move.w  12(a6),d0
        move.w  14(a6),d1
        move.w  16(a6),d2

; Pass the same dimension as width and height.
        move.w  d2,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)

        jsr     _plot_rectangle
        lea     12(sp),sp

        movem.l (sp)+,d0/a0
        unlk    a6
        rts


        xdef    _plot_triangle

;----- Function: plot_triangle -----
;
; PURPOSE: Draws the outline of a right triangle.
;
; INPUT: base - address of the screen buffer.
;        row - vertical coordinate of the right angle.
;        col - horizontal coordinate of the right angle.
;        triangle_base - horizontal triangle dimension.
;        height - vertical triangle dimension.
;        direction - right-angle orientation:
;                    0 = top-left
;                    1 = top-right
;                    2 = bottom-left
;                    3 = bottom-right
;
; OUTPUT: None.
;
; ASSUMPTIONS: The direction is between 0 and 3.
;              Drawing uses the plot_line routine.
;------------------------------------------------------------

_plot_triangle:
        link    a6,#0
        movem.l d0-d7/a0,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

        moveq   #0,d4
        move.w  16(a6),d4
        moveq   #0,d5
        move.w  18(a6),d5
        moveq   #0,d6
        move.b  21(a6),d6

; Reject empty triangles and invalid directions.
        tst.l   d4
        beq     pt_done
        tst.l   d5
        beq     pt_done
        cmpi.l  #3,d6
        bhi     pt_done

        move.l  d1,d2
        move.l  d0,d3

        subq.l  #1,d4
        subq.l  #1,d5

; Determine the horizontal endpoint.
        btst    #0,d6
        bne     pt_left
        add.l   d4,d2
        bra     pt_vertical

pt_left:
        sub.l   d4,d2

; Determine the vertical endpoint.
pt_vertical:
        btst    #1,d6
        bne     pt_up
        add.l   d5,d3
        bra     pt_edges

pt_up:
        sub.l   d5,d3

pt_edges:
        movea.l 8(a6),a0

; Draw the horizontal edge.
        move.w  d2,-(sp)
        move.w  d0,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

; Draw the vertical edge.
        move.w  d1,-(sp)
        move.w  d3,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

; Draw the diagonal edge.
        move.w  d1,-(sp)
        move.w  d3,-(sp)
        move.w  d2,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

pt_done:
        movem.l (sp)+,d0-d7/a0
        unlk    a6
        rts