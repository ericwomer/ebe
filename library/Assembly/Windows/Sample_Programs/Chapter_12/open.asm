        segment .data
fd:     dd      0
name:   db      "sample",0
        segment .text
        global  main
        extern  open
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        lea     rcx, [name] ; pathname
        mov     edx, 0x102   ; read-write | create
        mov     r8d, 600o   ; read-write for me
        call    open
        cmp     eax, 0
        jl      error       ; failed to open
        mov     [fd], eax 
        xor     eax, eax    ; success
        leave
        ret
error:  mov     eax, 1      ; not 0 means error
        leave
        ret
