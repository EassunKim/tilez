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

;bitmap_visible
;input:
;d0 = row
;d1 = col 
;d2 = height 
;d3 = width
; Output 
; d4 = 1 if visible, 0 otherwise
;d4/d5 mopdified

;Private Helper
bitmap_visible:
    clr.w   d4

    tst.w   d2
    beq     bv_done

    move.w  d3,d5
    neg.w   d5

    cmp.w   d5,d1
    ble     bv_done

    cmpi.w  #SCREEN_WIDTH,d1
    bge     bv_done

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

;clip_rows
;Input:
;d0 = row
;d2 = height
;Output:
;d4 = startRow
;d5 = endRow
;d4,d5,d6,d7 modified

;Private Helper
clip_rows:
    clr.w   d4
    move.w  d2,d5

    tst.w   d0
    bpl     cr_check_bottom

    move.w  d0,d4
    neg.w   d4

cr_check_bottom:
    move.w  d0,d6
    ext.l   d6

    clr.l   d7
    move.w  d2,d7
    add.l   d7,d6

    cmpi.l  #SCREEN_HEIGHT,d6
    ble     cr_done

    move.w  #SCREEN_HEIGHT,d5
    sub.w   d0,d5

cr_done:
    rts


;plot_8bit_bitmap
;Input:
;base = screen buffer address
;row    = starting row
;col    = starting column
;bitmap = bitmap data address
;height = bitmap height

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

    ;offset calculations

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

    ;if col >=, adding horizontal offset

    tst.w   d1
    bmi     p8_setup_loop

    move.w  d1,d6
    lsr.w   #3,d6
    adda.w  d6,a0

    move.w  d1,d3
    andi.w  #7,d3

p8_setup_loop:
    ;skip vertical clips
    adda.w  d4,a1

    ;Visible rows = endrow - startRow

    move.w  d5,d2
    sub.w   d4,d2

    tst.w   d2
    beq     p8_done

p8_plot_loop:
; read a bitmap row

    clr.l   d4
    move.b  (a1)+,d4

    ;negative column

    tst.w   d1
    bpl     p8_first_byte

    move.w  d1,d6
    neg.w   d6
    lsl.b   d6,d4

p8_first_byte:
    ;first byte

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

    ;second byte

    moveq   #8,d6
    sub.w   d3,d6

    move.b  d4,d7
    lsl.b   d6,d7 

    move.l  a0,a2
    addq.l  #1,a2
    or.b    d7,(a2)

p8_next_row:
    adda.l  #BM_ROW_BYTES,a0

    subq.w  #1,d2
    bne     p8_plot_loop

p8_done:
    movem.l (sp)+,d0-d7/a0-a2
    unlk    a6
    rts 




;plot_16bit_bitmap
;Input:
;base = screen buffer address
;row    = starting row
;col    = starting column
;bitmap = bitmap data address
;height = bitmap height

_plot_16bit_bitmap:
    link    a6,#0
    movem.l d0-d7/a0-a2,-(sp)

    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d0
    move.w  BM_COL(a6),d1
    move.l  BM_BITMAP(a6),a1
    move.w  BM_HEIGHT(a6),d2

    ;Check if bitmap visible
    move.w  #16,d3
    jsr     bitmap_visible

    tst.w   d4
    beq     p16_done

    ;Get visible rows 
    
    jsr     clip_rows

    ;offset calculations

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

    ;if col >=, adding horizontal offset

    tst.w   d1
    bmi     p16_setup_loop

    move.w  d1,d6
    lsr.w   #4,d6
    lsl.w   #1,d6
    adda.w  d6,a0

    move.w  d1,d3
    andi.w  #15,d3

p16_setup_loop:
    ;skip vertical clips
    move.w  d4,d6
    lsl.w   #1,d6
    adda.w  d6,a1

    ;Visible rows = endrow - startRow

    move.w  d5,d2
    sub.w   d4,d2

    tst.w   d2
    beq     p16_done

p16_plot_loop:
; read a bitmap row

    clr.l   d4
    move.w  (a1)+,d4

    ;negative column

    tst.w   d1
    bpl     p16_first_byte

    move.w  d1,d6
    neg.w   d6
    lsl.w   d6,d4

p16_first_byte:
    ;first byte

    move.w  d4,d6 
    lsr.w   d3,d6 
    or.w    d6,(a0)

    ;check if a second byte is needed

    tst.w   d3 
    beq     p16_next_row

    move.w  d1,d6
    asr.w   #4,d6

    cmpi.w  #39,d6
    bge     p16_next_row

    ;second byte

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


;plot_32bit_bitmap
;Input:
;base = screen buffer address
;row    = starting row
;col    = starting column
;bitmap = bitmap data address
;height = bitmap height

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

    ;Get visible rows 
    
    jsr     clip_rows

    ;offset calculations

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

    ;if col >=, adding horizontal offset

    tst.w   d1
    bmi     p32_setup_loop

    move.w  d1,d6
    lsr.w   #5,d6
    lsl.w   #2,d6
    adda.w  d6,a0

    move.w  d1,d3
    andi.w  #31,d3

p32_setup_loop:
    ;skip vertical clips
    move.w  d4,d6
    lsl.w   #2,d6
    adda.w  d6,a1

    ;Visible rows = endrow - startRow

    move.w  d5,d2
    sub.w   d4,d2

    tst.w   d2
    beq     p32_done

p32_plot_loop:
; read a bitmap row

    clr.l   d4
    move.l  (a1)+,d4

    ;negative column

    tst.w   d1
    bpl     p32_first_byte

    move.w  d1,d6
    neg.w   d6
    lsl.l   d6,d4

p32_first_byte:
    ;first byte

    move.l  d4,d6 
    lsr.l   d3,d6 
    or.l    d6,(a0)

    ;check if a second byte is needed

    tst.w   d3 
    beq     p32_next_row

    move.w  d1,d6
    asr.w   #5,d6

    cmpi.w  #19,d6
    bge     p32_next_row

    ;second byte

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

;plot_character
;Input:
;base = screen buffer address
;row = starting row 
;col = starting column 
;ch = character being drawn

_plot_character:
    link    a6,#0
    movem.l d0-d7/a0-a2,-(sp)

    move.l  BM_BASE(a6),a0
    move.w  BM_ROW(a6),d1
    move.w  BM_COL(a6),d2
    clr.w   d0 
    move.b  17(a6),d0 

    ;ASCII range
    cmpi.w  #32,d0 
    blo     pc_done 

    cmpi.w  #126,d0 
    bhi     pc_done

    ;glyph offset
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

;plot_string
;Input:
;base = screen buffer address
;row  = starting row
;col  = starting column
;ch   = string address

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

    ;plot_character call, base,row,col,ch 
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


    


    
        xdef    _clear_screen

base    equ     8

_clear_screen:
        link    a6,#0
        movem.l d0-1/a0,-(sp)
        movea.l base(a6),a0
        move.w  #7999,d0
        clr.l   d1

fill_loop:
        move.l  d1,(a0)+
        dbra    d0,fill_loop

        movem.l (sp)+,d0-1/a0
        unlk    a6
        rts

        
        xdef    _clear_region

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

_plot_pixel:
        link    a6,#0
        movem.l d0-d3/a0,-(sp)

        move.w  12(a6),d0
        ext.l   d0
        move.w  14(a6),d1
        ext.l   d1

        tst.l   d0
        bmi     pp_done
        cmpi.l  #400,d0
        bge     pp_done

        tst.l   d1
        bmi     pp_done
        cmpi.l  #640,d1
        bge     pp_done

        movea.l 8(a6),a0

        move.l  d0,d2
        lsl.l   #6,d2
        move.l  d0,d3
        lsl.l   #4,d3
        add.l   d3,d2

        move.l  d1,d3
        lsr.l   #3,d3
        add.l   d3,d2
        adda.l  d2,a0

        eori.w  #7,d1
        andi.w  #7,d1
        bset    d1,(a0)

pp_done:
        movem.l (sp)+,d0-d3/a0
        unlk    a6
        rts

        
        xdef    _plot_horizontal_line

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

        move.l  d3,d4
        sub.l   d1,d4
        bpl     pl_dx_ok
        neg.l   d4
pl_dx_ok:
        move.l  d2,d5
        sub.l   d0,d5
        bpl     pl_dy_ok
        neg.l   d5
pl_dy_ok:
        moveq   #1,d7
        cmp.l   d3,d1
        blt     pl_sx_ok
        moveq   #-1,d7
pl_sx_ok:
        movea.l d7,a1

        moveq   #1,d7
        cmp.l   d2,d0
        blt     pl_sy_ok
        moveq   #-1,d7
pl_sy_ok:
        movea.l d7,a2

        move.l  d4,d6
        sub.l   d5,d6

pl_loop:
        tst.l   d0
        bmi     pl_check_end
        cmpi.l  #400,d0
        bge     pl_check_end
        tst.l   d1
        bmi     pl_check_end
        cmpi.l  #640,d1
        bge     pl_check_end

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

pl_check_end:
        cmp.l   d2,d0
        bne     pl_step
        cmp.l   d3,d1
        beq     pl_done

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

        add.l   d0,d2
        subq.l  #1,d2
        add.l   d1,d3
        subq.l  #1,d3

        movea.l 8(a6),a0

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

pr_top_line:
        move.w  d3,-(sp)
        move.w  d0,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

        move.w  d3,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.w  d2,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

        move.w  d1,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

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

_plot_square:
        link    a6,#0
        movem.l d0/a0,-(sp)

        movea.l 8(a6),a0
        move.w  12(a6),d0
        move.w  14(a6),d1
        move.w  16(a6),d2

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

        btst    #0,d6
        bne     pt_left
        add.l   d4,d2
        bra     pt_vertical

pt_left:
        sub.l   d4,d2

pt_vertical:
        btst    #1,d6
        bne     pt_up
        add.l   d5,d3
        bra     pt_edges

pt_up:
        sub.l   d5,d3

pt_edges:
        movea.l 8(a6),a0

        move.w  d2,-(sp)
        move.w  d0,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

        move.w  d1,-(sp)
        move.w  d3,-(sp)
        move.w  d1,-(sp)
        move.w  d0,-(sp)
        move.l  a0,-(sp)
        jsr     _plot_line
        lea     12(sp),sp

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






