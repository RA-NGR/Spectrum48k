' ----------------------------------------------------------------
' ZX BASIC INTERFACE LIBRARY FOR NIRVANA+ ENGINE - by Einar Saukas
'
' If you use this interface library, you must load afterwards the
' NIRVANA+ ENGINE and a bicolor tile set. For a detailed sample
' see file "nirvanadem.bas".
' ----------------------------------------------------------------

#ifndef __LIBRARY_NIRVANA_PLUS__
#define __LIBRARY_NIRVANA_PLUS__
#pragma push(case_insensitive)
#pragma case_insensitive = true

' ----------------------------------------------------------------
' Activate NIRVANA ENGINE
' ----------------------------------------------------------------
#define NIRVANAstart() \
    asm                \
        call 64995     \
    end asm

' ----------------------------------------------------------------
' Deactivate NIRVANA ENGINE
' ----------------------------------------------------------------
#define NIRVANAstop() \
    asm               \
        call 65012    \
    end asm

' ----------------------------------------------------------------
' Execute HALT (wait for next frame).
'
' If an interrupt occurs while certain routines are under execution,
' the entire screen will "glitch" (routines NIRVANAfill and
' NIRVANAdraw) or a sprite may be displayed at an incorrect location
' (routine NIRVANAsprite).
'
' Routine NIRVANAhalt can be used to avoid these problems. Immediately
' after calling it, your program will have some time (about 12.5K T) to
' execute a few other routines without any interruption.
' ----------------------------------------------------------------
#define NIRVANAhalt() \
    asm               \
        halt          \
    end asm

' ----------------------------------------------------------------
' Instantly draw tile (16x16 pixels) at specified position
'
' Parameters:
'     tile: tile index (0-255)
'     lin: pixel line (0-200, even values only)
'     col: char column (0-30)
'
' WARNING: If this routine is under execution when interrupt occurs,
'          it will make the entire screen "glitch" (see NIRVANAhalt)
' ----------------------------------------------------------------
sub FASTCALL NIRVANAdrawT(tile as UBYTE, lin as UBYTE, col as UBYTE)
    asm
        pop hl          ; RET address
        pop de          ; D=lin
        ex (sp),hl      ; H=col
        ld e,h          ; E=col
        di
        call 64262      ; execute 'NIRVANA_drawT'
        ei
    end asm
end sub

' ----------------------------------------------------------------
' Instantly change the attributes in a tile area (16x16 pixels) to
' the specified value (use the same INK and PAPER values to "erase"
' a tile)
'
' Parameters:
'     attr: attribute value (0-255), INK+8*PAPER+64*BRIGHT+128*FLASH
'     lin: pixel line (0-200, even values only)
'     col: char column (0-30)
'
' WARNING: If this routine is under execution when interrupt occurs,
'          it will make the entire screen "glitch" (see NIRVANAhalt)
' ----------------------------------------------------------------
sub FASTCALL NIRVANAfillT(attr as UBYTE, lin as UBYTE, col as UBYTE)
    asm
        pop hl          ; RET address
        pop de          ; D=lin
        ex (sp),hl      ; H=col
        ld e,h          ; E=col
        di
        call 64928      ; execute 'NIRVANA_fillT'
        ei
    end asm
end sub

' ----------------------------------------------------------------
' Instantly print a 8x8 character at specified position, afterwards
' paint it with a provided sequence of 4 attribute values.
'
' Parameters:
'     ch: character code (0-255)
'     attrs: attributes address
'     lin: pixel line (16-192, even values only)
'     col: char column (0-31)
' ----------------------------------------------------------------
sub FASTCALL NIRVANAprintC(ch as UBYTE, attrs as UINTEGER, lin as UBYTE, col as UBYTE)
    asm
        pop hl          ; RET address
        pop bc          ; BC=attrs
        pop de          ; D=lin
        ex (sp),hl      ; H=col
        ld e,h          ; E=col
        jp 56323        ; execute 'NIRVANA_printC'
    end asm
end sub

' ----------------------------------------------------------------
' Instantly paint a 8x8 character at specified position with a
' provided sequence of 4 attribute values.
'
' Parameters:
'     attrs: attributes address
'     lin: pixel line (16-192, even values only)
'     col: char column (0-31)
' ----------------------------------------------------------------
sub FASTCALL NIRVANApaintC(attrs as UINTEGER, lin as UBYTE, col as UBYTE)
    asm
        ld b,h
        ld c,l          ; BC=attrs
        pop hl          ; RET address
        pop de          ; D=lin
        ex (sp),hl      ; H=col
        ld e,h          ; E=col
        jp 56418        ; execute 'NIRVANA_paintC'
    end asm
end sub

' ----------------------------------------------------------------
' Instantly change attributes in 8x8 character area to the
' specified value (use the same INK and PAPER values to "erase" a
' character).
'
' Parameters:
'     attr: attribute value (0-255), INK+8*PAPER+64*BRIGHT+128*FLASH
'     lin: pixel line (16-192, even values only)
'     col: char column (0-31)
' ----------------------------------------------------------------
sub FASTCALL NIRVANAfillC(attr as UBYTE, lin as UBYTE, col as UBYTE)
    asm
        ld c,a          ; C=attr
        pop hl          ; RET address
        pop de          ; D=lin
        ex (sp),hl      ; H=col
        ld e,h          ; E=col
        jp 65313        ; execute 'NIRVANA_fillC'
    end asm
end sub

' ----------------------------------------------------------------
' Update sprite information, so the specified tile will automatically
' appear at the specified location when the next interrupt occurs
' (and automatically redrawn at every interrupt afterwards, until
' this sprite information is updated again).
'
' Sprites are displayed in increasing priority order, i.e.
' sprite 7 will appear in front of everything else.

' Notice there's no way to disable sprites. If you don't want to
' display a certain sprite, simply move it to line zero, so it
' will be hidden outside the screen.
'
' Parameters:
'     sprite: sprite number (0-7)
'     tile: tile index (0-255)
'     lin: pixel line (0-200, even values only)
'     col: char column (0-30)
'
' WARNING: If this routine is under execution when interrupt occurs,
'          a sprite (containing partially updated information) may
'          be displayed at an incorrect location on screen (see
'          NIRVANAhalt)
' ----------------------------------------------------------------
sub FASTCALL NIRVANAspriteT(sprite as UBYTE, tile as UBYTE, lin as UBYTE, col as UBYTE)
    asm
        add a,a
        add a,a
        add a,a         ; A=sprite*8
        ld hl,56472
        add a,l
        ld l,a          ; HL=56472+sprite*8
        pop de          ; RET address
        pop af          ; A=tile
        ld (hl),a
        dec l
        dec l
        pop af          ; A=lin
        ld (hl),a
        dec l
        pop af          ; A=col
        ld (hl),a
        push de
    end asm
end sub

' ----------------------------------------------------------------
' Instantly draw wide tile (24x16 pixels) at specified position
'
' Parameters:
'     tile: wide tile index (0-255)
'     lin: pixel line (0-200, even values only)
'     col: char column (0-29)
'
' WARNING: If this routine is under execution when interrupt occurs,
'          it will make the entire screen "glitch" (see NIRVANAhalt)
'
' WARNING: Only use this routine if NIRVANA_drawW was enabled!!!
' ----------------------------------------------------------------
sub FASTCALL NIRVANAdrawW(tile as UBYTE, lin as UBYTE, col as UBYTE)
    asm
        pop hl          ; RET address
        pop de          ; D=lin
        ex (sp),hl      ; H=col
        ld e,h          ; E=col
        push ix
        di
        call 56085      ; execute 'NIRVANA_drawW'
        ei
        pop ix
    end asm
end sub

' ----------------------------------------------------------------
' Reconfigure NIRVANA ENGINE to read bicolor tiles (16x16 pixels)
' from another address (default value is 48000).
'
' Parameters:
'     addr: New tile images address
' ----------------------------------------------------------------
#define NIRVANAtiles(addr)   POKE UINTEGER 64284, (addr)

' ----------------------------------------------------------------
' Reconfigure NIRVANA ENGINE to read wide bicolor tiles (24x16
' pixels) from another address (default value is 54000).
'
' Parameters:
'     addr: New wide tile images address
'
' WARNING: Only use this routine if NIRVANA_drawW was enabled!!!
' ----------------------------------------------------------------
#define NIRVANAwides(addr)   POKE UINTEGER 56111, (addr)

' ----------------------------------------------------------------
' Reconfigure NIRVANA ENGINE to read character table from another address
'
' Parameters:
'     addr: New character table address
' ----------------------------------------------------------------
#define NIRVANAchars(addr)   POKE UINTEGER 56341, (addr)

' ----------------------------------------------------------------
' Advanced conversions
' ----------------------------------------------------------------
#define ROW2LIN(row)            (((row)+1)<<3)
#define LIN2ROW_UP(lin)         (((lin)>>3)-1)
#define LIN2ROW_DOWN(lin)       (((lin)-1)>>3)

#define SPRITECOL(sprite)       (56469+((sprite)<<3))
#define SPRITELIN(sprite)       (56470+((sprite)<<3))
#define SPRITEVAL(sprite)       (56472+((sprite)<<3))

' ----------------------------------------------------------------
' THE END
' ----------------------------------------------------------------
#pragma pop(case_insensitive)
#endif
