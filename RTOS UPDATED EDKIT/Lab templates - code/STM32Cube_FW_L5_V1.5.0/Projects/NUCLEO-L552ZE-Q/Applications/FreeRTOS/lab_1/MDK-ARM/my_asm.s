        AREA    MY_ASM, CODE, READONLY
        EXPORT  my_strcpy
        EXPORT  my_capitalize

;------------------------------------------------
; void my_strcpy(const char *src, char *dst);
;------------------------------------------------
my_strcpy
loop
        LDRB    r2, [r0]       ; Load byte from src
        ADDS    r0, r0, #1     ; Increment src pointer
        STRB    r2, [r1]       ; Store byte in dst
        ADDS    r1, r1, #1     ; Increment dst pointer
        CMP     r2, #0         ; Check if null terminator
        BNE     loop           ; If not, repeat
        BX      lr             ; Return

;------------------------------------------------
; void my_capitalize(char *str);
;------------------------------------------------
my_capitalize
cap_loop
        LDRB    r1, [r0]       ; Load byte from str
        CMP     r1, #'a'-1     ; Compare with 'a' - 1
        BLS     cap_skip       ; If lower, skip
        CMP     r1, #'z'       ; Compare with 'z'
        BHI     cap_skip       ; If higher, skip
        SUBS    r1, r1, #32    ; Convert to uppercase
        STRB    r1, [r0]       ; Store modified byte
cap_skip
        ADDS    r0, r0, #1     ; Increment pointer
        CMP     r1, #0         ; Check for null terminator
        BNE     cap_loop       ; If not null, continue
        BX      lr             ; Return

        END

