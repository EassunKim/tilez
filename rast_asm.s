
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


