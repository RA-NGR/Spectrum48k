#include once <stackf.asm>

    push namespace core

ACOS: ; Computes ACOS using ROM FP-CALC
    call __FPSTACK_PUSH

    rst 28h	; ROM CALC
    defb 23h ; ACOS
    defb 38h ; END CALC

    jp __FPSTACK_POP

    pop namespace

