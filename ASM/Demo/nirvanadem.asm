	org 32768
.core.__START_PROGRAM:
	di
	push ix
	push iy
	exx
	push hl
	exx
	ld hl, 0
	add hl, sp
	ld (.core.__CALL_BACK__), hl
	ei
	call .core.__MEM_INIT
	jp .core.__MAIN_PROGRAM__
.core.__CALL_BACK__:
	DEFW 0
.core.ZXBASIC_USER_DATA:
	; Defines HEAP SIZE
.core.ZXBASIC_HEAP_SIZE EQU 4768
.core.ZXBASIC_MEM_HEAP:
	DEFS 4768
	; Defines USER DATA Length in bytes
.core.ZXBASIC_USER_DATA_LEN EQU .core.ZXBASIC_USER_DATA_END - .core.ZXBASIC_USER_DATA
	.core.__LABEL__.ZXBASIC_USER_DATA_LEN EQU .core.ZXBASIC_USER_DATA_LEN
	.core.__LABEL__.ZXBASIC_USER_DATA EQU .core.ZXBASIC_USER_DATA
_tile:
	DEFB 00
_spr:
	DEFW .LABEL.__LABEL10
_spr.__DATA__.__PTR__:
	DEFW _spr.__DATA__
_spr.__DATA__:
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 00h
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 0FFh
	DEFB 01h
	DEFB 01h
	DEFB 13h
	DEFB 13h
	DEFB 25h
	DEFB 25h
	DEFB 37h
	DEFB 37h
	DEFB 41h
	DEFB 41h
	DEFB 53h
	DEFB 53h
	DEFB 65h
	DEFB 65h
	DEFB 77h
	DEFB 77h
.LABEL.__LABEL10:
	DEFW 0000h
	DEFB 01h
.core.ZXBASIC_USER_DATA_END:
.core.__MAIN_PROGRAM__:
	call .core.CLS
	ld hl, _spr.__DATA__
	ld (64284), hl
#line 11 "nirvanadem.bas"

		call 64995

#line 14 "nirvanadem.bas"
.LABEL.__LABEL0:
	xor a
	ld (_tile), a
	jp .LABEL.__LABEL2
.LABEL.__LABEL5:
#line 15 "nirvanadem.bas"

		halt

#line 18 "nirvanadem.bas"
	ld a, (_tile)
	push af
	ld a, 16
	push af
	xor a
	push af
	pop af
	call _NIRVANAdrawT
	ld a, (_tile)
	add a, 2
	push af
	pop af
	ld (_tile), a
.LABEL.__LABEL2:
	ld a, 30
	ld hl, (_tile - 1)
	cp h
	sbc a, a
	push af
	pop af
	or a
	jp z, .LABEL.__LABEL5
.LABEL.__LABEL7:
	call .core.INKEY
	push hl
	ld de, .LABEL.__LABEL9
	pop hl
	ld a, 1
	call .core.__STRNE
	push af
	pop af
	or a
	jp z, .LABEL.__LABEL8
	jp .LABEL.__LABEL7
.LABEL.__LABEL8:
	jp .LABEL.__LABEL0
	ld hl, 0
	ld b, h
	ld c, l
.core.__END_PROGRAM:
	di
	ld hl, (.core.__CALL_BACK__)
	ld sp, hl
	exx
	pop hl
	exx
	pop iy
	pop ix
	ei
	ret
_NIRVANAdrawT:
#line 59 "nirvana+.bas"

		pop hl
		pop de
		ex (sp),hl
		ld e,h
		di
		call 64262
		ei

#line 68 "nirvana+.bas"
_NIRVANAdrawT__leave:
	ret
.LABEL.__LABEL9:
	DEFW 0000h
	;; --- end of user code ---
#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/cls.asm"
	;; Clears the user screen (24 rows)

#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/sysvars.asm"
	;; -----------------------------------------------------------------------
	;; ZX Basic System Vars
	;; Some of them will be mapped over Sinclair ROM ones for compatibility
	;; -----------------------------------------------------------------------

	push namespace core

SCREEN_ADDR:        DW 16384  ; Screen address (can be pointed to other place to use a screen buffer)
SCREEN_ATTR_ADDR:   DW 22528  ; Screen attribute address (ditto.)

	; These are mapped onto ZX Spectrum ROM VARS

	CHARS               EQU 23606  ; Pointer to ROM/RAM Charset
	TV_FLAG             EQU 23612  ; Flags for controlling output to screen
	UDG                 EQU 23675  ; Pointer to UDG Charset
	COORDS              EQU 23677  ; Last PLOT coordinates
	FLAGS2              EQU 23681  ;
	ECHO_E              EQU 23682  ;
	DFCC                EQU 23684  ; Next screen addr for PRINT
	DFCCL               EQU 23686  ; Next screen attr for PRINT
	S_POSN              EQU 23688
	ATTR_P              EQU 23693  ; Current Permanent ATTRS set with INK, PAPER, etc commands
	ATTR_T              EQU 23695  ; temporary ATTRIBUTES
	P_FLAG              EQU 23697  ;
	MEM0                EQU 23698  ; Temporary memory buffer used by ROM chars

	SCR_COLS            EQU 33     ; Screen with in columns + 1
	SCR_ROWS            EQU 24     ; Screen height in rows
	SCR_SIZE            EQU (SCR_ROWS << 8) + SCR_COLS
	pop namespace
#line 4 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/cls.asm"

	    push namespace core

CLS:
	    PROC

	    ld hl, 0
	    ld (COORDS), hl
	    ld hl, SCR_SIZE
	    ld (S_POSN), hl
	    ld hl, (SCREEN_ADDR)
	    ld (DFCC), hl
	    ld (hl), 0
	    ld d, h
	    ld e, l
	    inc de
	    ld bc, 6143
	    ldir

	    ; Now clear attributes

	    ld hl, (SCREEN_ATTR_ADDR)
	    ld (DFCCL), hl
	    ld d, h
	    ld e, l
	    inc de
	    ld a, (ATTR_P)
	    ld (hl), a
	    ld bc, 767
	    ldir
	    ret

	    ENDP

	    pop namespace
#line 75 "nirvana+.bas"
#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/inkey.asm"
	; INKEY Function
	; Returns a string allocated in dynamic memory
	; containing the string.
	; An empty string otherwise.

#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/alloc.asm"
; vim: ts=4:et:sw=4:
	; Copyleft (K) by Jose M. Rodriguez de la Rosa
	;  (a.k.a. Boriel)
;  http://www.boriel.com
	;
	; This ASM library is licensed under the MIT license
	; you can use it for any purpose (even for commercial
	; closed source programs).
	;
	; Please read the MIT license on the internet

	; ----- IMPLEMENTATION NOTES ------
	; The heap is implemented as a linked list of free blocks.

; Each free block contains this info:
	;
	; +----------------+ <-- HEAP START
	; | Size (2 bytes) |
	; |        0       | <-- Size = 0 => DUMMY HEADER BLOCK
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   | <-- If Size > 4, then this contains (size - 4) bytes
	; | (0 if Size = 4)|   |
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   |
	; | (0 if Size = 4)|   |
	; +----------------+   |
	;   <Allocated>        | <-- This zone is in use (Already allocated)
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   |
	; | (0 if Size = 4)|   |
	; +----------------+ <-+
	; | Next (2 bytes) |--> NULL => END OF LIST
	; |    0 = NULL    |
	; +----------------+
	; | <free bytes...>|
	; | (0 if Size = 4)|
	; +----------------+


	; When a block is FREED, the previous and next pointers are examined to see
	; if we can defragment the heap. If the block to be freed is just next to the
	; previous, or to the next (or both) they will be converted into a single
	; block (so defragmented).


	;   MEMORY MANAGER
	;
	; This library must be initialized calling __MEM_INIT with
	; HL = BLOCK Start & DE = Length.

	; An init directive is useful for initialization routines.
	; They will be added automatically if needed.

#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/error.asm"
	; Simple error control routines
; vim:ts=4:et:

	    push namespace core

	ERR_NR    EQU    23610    ; Error code system variable


	; Error code definitions (as in ZX spectrum manual)

; Set error code with:
	;    ld a, ERROR_CODE
	;    ld (ERR_NR), a


	ERROR_Ok                EQU    -1
	ERROR_SubscriptWrong    EQU     2
	ERROR_OutOfMemory       EQU     3
	ERROR_OutOfScreen       EQU     4
	ERROR_NumberTooBig      EQU     5
	ERROR_InvalidArg        EQU     9
	ERROR_IntOutOfRange     EQU    10
	ERROR_NonsenseInBasic   EQU    11
	ERROR_InvalidFileName   EQU    14
	ERROR_InvalidColour     EQU    19
	ERROR_BreakIntoProgram  EQU    20
	ERROR_TapeLoadingErr    EQU    26


	; Raises error using RST #8
__ERROR:
	    ld (__ERROR_CODE), a
	    rst 8
__ERROR_CODE:
	    nop
	    ret

	; Sets the error system variable, but keeps running.
	; Usually this instruction if followed by the END intermediate instruction.
__STOP:
	    ld (ERR_NR), a
	    ret

	    pop namespace
#line 69 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/alloc.asm"
#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/heapinit.asm"
; vim: ts=4:et:sw=4:
	; Copyleft (K) by Jose M. Rodriguez de la Rosa
	;  (a.k.a. Boriel)
;  http://www.boriel.com
	;
	; This ASM library is licensed under the BSD license
	; you can use it for any purpose (even for commercial
	; closed source programs).
	;
	; Please read the BSD license on the internet

	; ----- IMPLEMENTATION NOTES ------
	; The heap is implemented as a linked list of free blocks.

; Each free block contains this info:
	;
	; +----------------+ <-- HEAP START
	; | Size (2 bytes) |
	; |        0       | <-- Size = 0 => DUMMY HEADER BLOCK
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   | <-- If Size > 4, then this contains (size - 4) bytes
	; | (0 if Size = 4)|   |
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   |
	; | (0 if Size = 4)|   |
	; +----------------+   |
	;   <Allocated>        | <-- This zone is in use (Already allocated)
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   |
	; | (0 if Size = 4)|   |
	; +----------------+ <-+
	; | Next (2 bytes) |--> NULL => END OF LIST
	; |    0 = NULL    |
	; +----------------+
	; | <free bytes...>|
	; | (0 if Size = 4)|
	; +----------------+


	; When a block is FREED, the previous and next pointers are examined to see
	; if we can defragment the heap. If the block to be breed is just next to the
	; previous, or to the next (or both) they will be converted into a single
	; block (so defragmented).


	;   MEMORY MANAGER
	;
	; This library must be initialized calling __MEM_INIT with
	; HL = BLOCK Start & DE = Length.

	; An init directive is useful for initialization routines.
	; They will be added automatically if needed.




	; ---------------------------------------------------------------------
	;  __MEM_INIT must be called to initalize this library with the
	; standard parameters
	; ---------------------------------------------------------------------
	    push namespace core

__MEM_INIT: ; Initializes the library using (RAMTOP) as start, and
	    ld hl, ZXBASIC_MEM_HEAP  ; Change this with other address of heap start
	    ld de, ZXBASIC_HEAP_SIZE ; Change this with your size

	; ---------------------------------------------------------------------
	;  __MEM_INIT2 initalizes this library
; Parameters:
;   HL : Memory address of 1st byte of the memory heap
;   DE : Length in bytes of the Memory Heap
	; ---------------------------------------------------------------------
__MEM_INIT2:
	    ; HL as TOP
	    PROC

	    dec de
	    dec de
	    dec de
	    dec de        ; DE = length - 4; HL = start
	    ; This is done, because we require 4 bytes for the empty dummy-header block

	    xor a
	    ld (hl), a
	    inc hl
    ld (hl), a ; First "free" block is a header: size=0, Pointer=&(Block) + 4
	    inc hl

	    ld b, h
	    ld c, l
	    inc bc
	    inc bc      ; BC = starts of next block

	    ld (hl), c
	    inc hl
	    ld (hl), b
	    inc hl      ; Pointer to next block

	    ld (hl), e
	    inc hl
	    ld (hl), d
	    inc hl      ; Block size (should be length - 4 at start); This block contains all the available memory

	    ld (hl), a ; NULL (0000h) ; No more blocks (a list with a single block)
	    inc hl
	    ld (hl), a

	    ld a, 201
	    ld (__MEM_INIT), a; "Pokes" with a RET so ensure this routine is not called again
	    ret

	    ENDP

	    pop namespace

#line 70 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/alloc.asm"


	; ---------------------------------------------------------------------
	; MEM_ALLOC
	;  Allocates a block of memory in the heap.
	;
	; Parameters
	;  BC = Length of requested memory block
	;
; Returns:
	;  HL = Pointer to the allocated block in memory. Returns 0 (NULL)
	;       if the block could not be allocated (out of memory)
	; ---------------------------------------------------------------------

	    push namespace core

MEM_ALLOC:
__MEM_ALLOC: ; Returns the 1st free block found of the given length (in BC)
	    PROC

	    LOCAL __MEM_LOOP
	    LOCAL __MEM_DONE
	    LOCAL __MEM_SUBTRACT
	    LOCAL __MEM_START
	    LOCAL TEMP, TEMP0

	TEMP EQU TEMP0 + 1

	    ld hl, 0
	    ld (TEMP), hl

__MEM_START:
	    ld hl, ZXBASIC_MEM_HEAP  ; This label point to the heap start
	    inc bc
	    inc bc  ; BC = BC + 2 ; block size needs 2 extra bytes for hidden pointer

__MEM_LOOP:  ; Loads lengh at (HL, HL+). If Lenght >= BC, jump to __MEM_DONE
	    ld a, h ;  HL = NULL (No memory available?)
	    or l
#line 113 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/alloc.asm"
	    ret z ; NULL
#line 115 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/alloc.asm"
	    ; HL = Pointer to Free block
	    ld e, (hl)
	    inc hl
	    ld d, (hl)
	    inc hl          ; DE = Block Length

	    push hl         ; HL = *pointer to -> next block
	    ex de, hl
	    or a            ; CF = 0
	    sbc hl, bc      ; FREE >= BC (Length)  (HL = BlockLength - Length)
	    jp nc, __MEM_DONE
	    pop hl
	    ld (TEMP), hl

	    ex de, hl
	    ld e, (hl)
	    inc hl
	    ld d, (hl)
	    ex de, hl
	    jp __MEM_LOOP

__MEM_DONE:  ; A free block has been found.
	    ; Check if at least 4 bytes remains free (HL >= 4)
	    push hl
	    exx  ; exx to preserve bc
	    pop hl
	    ld bc, 4
	    or a
	    sbc hl, bc
	    exx
	    jp nc, __MEM_SUBTRACT
	    ; At this point...
	    ; less than 4 bytes remains free. So we return this block entirely
	    ; We must link the previous block with the next to this one
	    ; (DE) => Pointer to next block
	    ; (TEMP) => &(previous->next)
	    pop hl     ; Discard current block pointer
	    push de
	    ex de, hl  ; DE = Previous block pointer; (HL) = Next block pointer
	    ld a, (hl)
	    inc hl
	    ld h, (hl)
	    ld l, a    ; HL = (HL)
	    ex de, hl  ; HL = Previous block pointer; DE = Next block pointer
TEMP0:
	    ld hl, 0   ; Pre-previous block pointer

	    ld (hl), e
	    inc hl
	    ld (hl), d ; LINKED
	    pop hl ; Returning block.

	    ret

__MEM_SUBTRACT:
	    ; At this point we have to store HL value (Length - BC) into (DE - 2)
	    ex de, hl
	    dec hl
	    ld (hl), d
	    dec hl
	    ld (hl), e ; Store new block length

	    add hl, de ; New length + DE => free-block start
	    pop de     ; Remove previous HL off the stack

	    ld (hl), c ; Store length on its 1st word
	    inc hl
	    ld (hl), b
	    inc hl     ; Return hl
	    ret

	    ENDP

	    pop namespace

#line 7 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/inkey.asm"

	    push namespace core

INKEY:
	    PROC
	    LOCAL __EMPTY_INKEY
	    LOCAL KEY_SCAN
	    LOCAL KEY_TEST
	    LOCAL KEY_CODE

	    ld bc, 3	; 1 char length string
	    call __MEM_ALLOC

	    ld a, h
	    or l
	    ret z	; Return if NULL (No memory)

	    push hl ; Saves memory pointer

	    call KEY_SCAN
	    jp nz, __EMPTY_INKEY

	    call KEY_TEST
	    jp nc, __EMPTY_INKEY

	    dec d	; D is expected to be FLAGS so set bit 3 $FF
	    ; 'L' Mode so no keywords.
	    ld e, a	; main key to A
	    ; C is MODE 0 'KLC' from above still.
	    call KEY_CODE ; routine K-DECODE
	    pop hl

	    ld (hl), 1
	    inc hl
	    ld (hl), 0
	    inc hl
	    ld (hl), a
	    dec hl
	    dec hl	; HL Points to string result
	    ret

__EMPTY_INKEY:
	    pop hl
	    xor a
	    ld (hl), a
	    inc hl
	    ld (hl), a
	    dec hl
	    ret

	KEY_SCAN	EQU 028Eh
	KEY_TEST	EQU 031Eh
	KEY_CODE	EQU 0333h

	    ENDP

	    pop namespace

#line 76 "nirvana+.bas"
#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/string.asm"
	; String library

#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/free.asm"
; vim: ts=4:et:sw=4:
	; Copyleft (K) by Jose M. Rodriguez de la Rosa
	;  (a.k.a. Boriel)
;  http://www.boriel.com
	;
	; This ASM library is licensed under the BSD license
	; you can use it for any purpose (even for commercial
	; closed source programs).
	;
	; Please read the BSD license on the internet

	; ----- IMPLEMENTATION NOTES ------
	; The heap is implemented as a linked list of free blocks.

; Each free block contains this info:
	;
	; +----------------+ <-- HEAP START
	; | Size (2 bytes) |
	; |        0       | <-- Size = 0 => DUMMY HEADER BLOCK
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   | <-- If Size > 4, then this contains (size - 4) bytes
	; | (0 if Size = 4)|   |
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   |
	; | (0 if Size = 4)|   |
	; +----------------+   |
	;   <Allocated>        | <-- This zone is in use (Already allocated)
	; +----------------+ <-+
	; | Size (2 bytes) |
	; +----------------+
	; | Next (2 bytes) |---+
	; +----------------+   |
	; | <free bytes...>|   |
	; | (0 if Size = 4)|   |
	; +----------------+ <-+
	; | Next (2 bytes) |--> NULL => END OF LIST
	; |    0 = NULL    |
	; +----------------+
	; | <free bytes...>|
	; | (0 if Size = 4)|
	; +----------------+


	; When a block is FREED, the previous and next pointers are examined to see
	; if we can defragment the heap. If the block to be breed is just next to the
	; previous, or to the next (or both) they will be converted into a single
	; block (so defragmented).


	;   MEMORY MANAGER
	;
	; This library must be initialized calling __MEM_INIT with
	; HL = BLOCK Start & DE = Length.

	; An init directive is useful for initialization routines.
	; They will be added automatically if needed.



	; ---------------------------------------------------------------------
	; MEM_FREE
	;  Frees a block of memory
	;
; Parameters:
	;  HL = Pointer to the block to be freed. If HL is NULL (0) nothing
	;  is done
	; ---------------------------------------------------------------------

	    push namespace core

MEM_FREE:
__MEM_FREE: ; Frees the block pointed by HL
	    ; HL DE BC & AF modified
	    PROC

	    LOCAL __MEM_LOOP2
	    LOCAL __MEM_LINK_PREV
	    LOCAL __MEM_JOIN_TEST
	    LOCAL __MEM_BLOCK_JOIN

	    ld a, h
	    or l
	    ret z       ; Return if NULL pointer

	    dec hl
	    dec hl
	    ld b, h
	    ld c, l    ; BC = Block pointer

	    ld hl, ZXBASIC_MEM_HEAP  ; This label point to the heap start

__MEM_LOOP2:
	    inc hl
	    inc hl     ; Next block ptr

	    ld e, (hl)
	    inc hl
	    ld d, (hl) ; Block next ptr
	    ex de, hl  ; DE = &(block->next); HL = block->next

	    ld a, h    ; HL == NULL?
	    or l
	    jp z, __MEM_LINK_PREV; if so, link with previous

	    or a       ; Clear carry flag
	    sbc hl, bc ; Carry if BC > HL => This block if before
	    add hl, bc ; Restores HL, preserving Carry flag
	    jp c, __MEM_LOOP2 ; This block is before. Keep searching PASS the block

	;------ At this point current HL is PAST BC, so we must link (DE) with BC, and HL in BC->next

__MEM_LINK_PREV:    ; Link (DE) with BC, and BC->next with HL
	    ex de, hl
	    push hl
	    dec hl

	    ld (hl), c
	    inc hl
	    ld (hl), b ; (DE) <- BC

	    ld h, b    ; HL <- BC (Free block ptr)
	    ld l, c
	    inc hl     ; Skip block length (2 bytes)
	    inc hl
	    ld (hl), e ; Block->next = DE
	    inc hl
	    ld (hl), d
	    ; --- LINKED ; HL = &(BC->next) + 2

	    call __MEM_JOIN_TEST
	    pop hl

__MEM_JOIN_TEST:   ; Checks for fragmented contiguous blocks and joins them
	    ; hl = Ptr to current block + 2
	    ld d, (hl)
	    dec hl
	    ld e, (hl)
	    dec hl
	    ld b, (hl) ; Loads block length into BC
	    dec hl
	    ld c, (hl) ;

	    push hl    ; Saves it for later
	    add hl, bc ; Adds its length. If HL == DE now, it must be joined
	    or a
	    sbc hl, de ; If Z, then HL == DE => We must join
	    pop hl
	    ret nz

__MEM_BLOCK_JOIN:  ; Joins current block (pointed by HL) with next one (pointed by DE). HL->length already in BC
	    push hl    ; Saves it for later
	    ex de, hl

	    ld e, (hl) ; DE -> block->next->length
	    inc hl
	    ld d, (hl)
	    inc hl

	    ex de, hl  ; DE = &(block->next)
	    add hl, bc ; HL = Total Length

	    ld b, h
	    ld c, l    ; BC = Total Length

	    ex de, hl
	    ld e, (hl)
	    inc hl
	    ld d, (hl) ; DE = block->next

	    pop hl     ; Recovers Pointer to block
	    ld (hl), c
	    inc hl
	    ld (hl), b ; Length Saved
	    inc hl
	    ld (hl), e
	    inc hl
	    ld (hl), d ; Next saved
	    ret

	    ENDP

	    pop namespace

#line 4 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/string.asm"
#line 1 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/strlen.asm"
	; Returns len if a string
	; If a string is NULL, its len is also 0
	; Result returned in HL

	    push namespace core

__STRLEN:	; Direct FASTCALL entry
	    ld a, h
	    or l
	    ret z

	    ld a, (hl)
	    inc hl
	    ld h, (hl)  ; LEN(str) in HL
	    ld l, a
	    ret

	    pop namespace


#line 5 "D:/Owner/Documents/Arduino/Spectrum48k/ASM/Demo/src/lib/arch/zx48k/runtime/string.asm"

	    push namespace core

__STR_ISNULL:	; Returns A = FF if HL is 0, 0 otherwise
	    ld a, h
	    or l
	    sub 1		; Only CARRY if HL is NULL
	    sbc a, a	; Only FF if HL is NULL (0 otherwise)
	    ret


__STRCMP:	; Compares strings at HL (a$), DE (b$)
	            ; Returns 0 if EQual, -1 if HL < DE, +1 if HL > DE
	    ; A register is preserved and returned in A'
	    PROC ; __FASTCALL__

	    LOCAL __STRCMPZERO
	    LOCAL __STRCMPEXIT
	    LOCAL __STRCMPLOOP
	    LOCAL __EQULEN1
	    LOCAL __HLZERO

	    ex af, af'	; Saves current A register in A' (it's used by STRXX comparison functions)

	    push hl
	    call __STRLEN
	    ld a, h
	    or l
	    pop hl
	    jr z, __HLZERO  ; if HL == "", go to __HLZERO

	    push de
	    ex de, hl
	    call __STRLEN
	    ld a, h
	    or l
	    ld a, 1
	    ex de, hl   ; Recovers HL
	    pop de
	    ret z		; Returns +1 if HL != "" AND DE == ""

	    ld c, (hl)
	    inc hl
	    ld b, (hl)
	    inc hl		; BC = LEN(a$)
	    push hl		; HL = &a$, saves it

	    ex de, hl
	    ld e, (hl)
	    inc hl
	    ld d, (hl)
	    inc hl
	    ex de, hl	; HL = LEN(b$), de = &b$

	    ; At this point Carry is cleared, and A reg. = 1
	    sbc hl, bc	     ; Carry if len(a$)[BC] > len(b$)[HL]

	    ld a, 0
    jr z, __EQULEN1	 ; Jumps if len(a$)[BC] = len(b$)[HL] : A = 0

	    dec a
    jr nc, __EQULEN1 ; Jumps if len(a$)[BC] < len(b$)[HL] : A = 1

	    adc hl, bc  ; Restores HL
    ld a, 1     ; Signals len(a$)[BC] > len(b$)[HL] : A = 1
	    ld b, h
	    ld c, l

__EQULEN1:
	    pop hl		; Recovers A$ pointer
	    push af		; Saves A for later (Value to return if strings reach the end)
	    ld a, b
	    or c
	    jr z, __STRCMPZERO ; empty string being compared

    ; At this point: BC = lesser length, DE and HL points to b$ and a$ chars respectively
__STRCMPLOOP:
	    ld a, (de)
	    cpi
	    jr nz, __STRCMPEXIT ; (HL) != (DE). Examine carry
	    jp po, __STRCMPZERO ; END of string (both are equal)
	    inc de
	    jp __STRCMPLOOP

__STRCMPZERO:
	    pop af		; This is -1 if len(a$) < len(b$), +1 if len(b$) > len(a$), 0 otherwise
	    ret

__STRCMPEXIT:		; Sets A with the following value
	    dec hl		; Get back to the last char
	    cp (hl)
	    sbc a, a	; A = -1 if carry => (DE) < (HL); 0 otherwise (DE) > (HL)
	    cpl			; A = -1 if (HL) < (DE), 0 otherwise
	    add a, a    ; A = A * 2 (thus -2 or 0)
	    inc a		; A = A + 1 (thus -1 or 1)

	    pop bc		; Discard top of the stack
	    ret

__HLZERO:
	    ex de, hl
	    call __STRLEN
	    ld a, h
	    or l
	    ret z		; Returns 0 (EQ) if HL == DE == ""
	    ld a, -1
	    ret			; Returns -1 if HL == "" and DE != ""

	    ENDP

	    ; The following routines perform string comparison operations (<, >, ==, etc...)
	    ; On return, A will contain 0 for False, other value for True
	    ; Register A' will determine whether the incoming strings (HL, DE) will be freed
    ; from dynamic memory on exit:
	    ;		Bit 0 => 1 means HL will be freed.
	    ;		Bit 1 => 1 means DE will be freed.

__STREQ:	; Compares a$ == b$ (HL = ptr a$, DE = ptr b$). Returns FF (True) or 0 (False)
	    push hl
	    push de
	    call __STRCMP
	    pop de
	    pop hl
	    sub 1
	    sbc a, a
	    jp __FREE_STR


__STRNE:	; Compares a$ != b$ (HL = ptr a$, DE = ptr b$). Returns FF (True) or 0 (False)
	    push hl
	    push de
	    call __STRCMP
	    pop de
	    pop hl
	    jp __FREE_STR


__STRLT:	; Compares a$ < b$ (HL = ptr a$, DE = ptr b$). Returns FF (True) or 0 (False)
	    push hl
	    push de
	    call __STRCMP
	    pop de
	    pop hl
	    or a
	    jp z, __FREE_STR ; Returns 0 if A == B

	    dec a		; Returns 0 if A == 1 => a$ > b$
	    jp __FREE_STR


__STRLE:	; Compares a$ <= b$ (HL = ptr a$, DE = ptr b$). Returns FF (True) or 0 (False)
	    push hl
	    push de
	    call __STRCMP
	    pop de
	    pop hl

	    dec a		; Returns 0 if A == 1 => a$ < b$
	    jp __FREE_STR


__STRGT:	; Compares a$ > b$ (HL = ptr a$, DE = ptr b$). Returns FF (True) or 0 (False)
	    push hl
	    push de
	    call __STRCMP
	    pop de
	    pop hl
	    or a
	    jp z, __FREE_STR		; Returns 0 if A == B

	    inc a		; Returns 0 if A == -1 => a$ < b$
	    jp __FREE_STR


__STRGE:	; Compares a$ >= b$ (HL = ptr a$, DE = ptr b$). Returns FF (True) or 0 (False)
	    push hl
	    push de
	    call __STRCMP
	    pop de
	    pop hl

	    inc a		; Returns 0 if A == -1 => a$ < b$

__FREE_STR: ; This exit point will test A' for bits 0 and 1
	    ; If bit 0 is 1 => Free memory from HL pointer
	    ; If bit 1 is 1 => Free memory from DE pointer
	    ; Finally recovers A, to return the result
	    PROC

	    LOCAL __FREE_STR2
	    LOCAL __FREE_END

	    ex af, af'
	    bit 0, a
	    jr z, __FREE_STR2

	    push af
	    push de
	    call __MEM_FREE
	    pop de
	    pop af

__FREE_STR2:
	    bit 1, a
	    jr z, __FREE_END

	    ex de, hl
	    call __MEM_FREE

__FREE_END:
	    ex af, af'
	    ret

	    ENDP

	    pop namespace
#line 77 "nirvana+.bas"

	END
