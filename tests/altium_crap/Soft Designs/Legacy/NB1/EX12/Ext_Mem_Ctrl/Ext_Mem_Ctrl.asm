;...............................................................................
EAL .equ  0AFH
;P0  .equ  80H
;P1  .equ  90H
;P2  .equ  0A0H
;P3  .equ  0B0H
;DPL .equ  82H
;DPH .equ  83H
;PSW .equ  0D0H
;ACC .equ  0E0H
;...............................................................................

;...............................................................................
MainCountLow  .equ     38H
RightShift    .equ     42H
LeftShift     .equ     43H
CountInner    .equ     40H
CountOuter    .equ     41H

ExtMemCnt               .equ            44H
;...............................................................................
    .Section Text, Code
;    .org 0000H

MainProc:
;...............................................................................
    LJMP  INIT
;...............................................................................

INIT:
    MOV   DPL,#00H
    MOV   DPH,#22H
    MOV   PSW,#00H
    MOV P0, #00H
    MOV MainCountLow, #00H
    MOV RightShift, #01H
    MOV LeftShift, #80H
    CLR EAL


;...............................................................................
MainLoop :
;...............................................................................

    LCALL    VeryLongDelay
    LCALL    IncrementCounter
    LCALL    ShiftForDiode
    LCALL    DisplayCounter

    SJMP    MainLoop
;...............................................................................

;...............................................................................
IncrementCounter
;...............................................................................
    INC  MainCountLow
    RET
;...............................................................................

;...............................................................................
ShiftForDiode:
;...............................................................................
    MOV   A, RightShift
    RR A
    MOV   RightShift, A
    MOV   A, LeftShift
    RL A
    MOV   LeftShift, A
    RET
;...............................................................................


;...............................................................................
DisplayCounter:
;...............................................................................
        PUSH DPL
        PUSH DPH
        PUSH ACC
        MOV DPH, #01H
        MOV DPL, #0FFH
        MOV A, MainCountLow
        MOVX @DPTR, A
        MOV A, #00H
        MOVX A, @DPTR
        MOV ExtMemCnt, A
        POP ACC
        POP DPH
        POP DPL
        MOV P0, ExtMemCnt
        RET
;...............................................................................


;...............................................................................
LongDelay:
;...............................................................................
    MOV   CountOuter, #0AAH
LoopOuter:
       MOV   CountInner, #0AAH
LoopInner:
             DEC   CountInner
          MOV A, CountInner
       JNZ   LoopInner
        DEC  CountOuter
       MOV A, CountOuter
    JNZ   LoopOuter
    RET
;...............................................................................


;...............................................................................
VeryLongDelay:
;...............................................................................
    LCALL    LongDelay
    LCALL    LongDelay
    LCALL    LongDelay
    LCALL    LongDelay
    LCALL    LongDelay
    RET
;...............................................................................

    .END
