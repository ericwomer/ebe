#
#       Program to test bit field extraction and insertion
#
        .section .text
        .global  main
main:
        push    %ebp
        mov     %esp, %ebp
        frame   2, 2
        sub     $frame_size, %esp

        push    $prompt
        call    printf
        add     $4, %esp

        push    $count
        push    $to
        push    $from
        push    $read_fmt
        call    scanf
        add     $16, %esp

        push    count
        push    from
        push    $array
        call    extract
        add     $12, %esp
        
        push    count
        push    to
        push    $array
        push    %eax
        call    insert
        add     $12, %esp
        
        mov     %eax, field
        leave
        ret
        
        .section .text
extract:
        push    %ebp
        mov     %esp, %ebp
        frame   3, 1
        sub     $frame_size, %esp
        
        mov     currPar2(%ebp), %ecx   # get from bit position
        mov     %ecx, %edx
        and     $0x1f, %edx
        mov     %edx, local1(%ebp)
        shr     $5, %ecx               # dword number from the array
        mov     currPar1(%ebp), %eax   # get array address
        movq    (%eax,%ecx,4), %xmm0   # get 64 bits containing the field
        mov     $64, %ecx
        mov     currPar3(%ebp), %edx   # get count
        sub     %edx, %ecx
        sub     local1(%ebp), %ecx     # add start bit position in dword
        mov     %ecx, shift_amount
        movd    %xmm0, %eax
        psllq   shift_amount, %xmm0    # clear out high order bits
        movd    %xmm0, %eax
        mov     $64, %ecx
        sub     currPar3(%ebp), %ecx
        mov     %ecx, shift_amount
        psrlq   shift_amount, %xmm0    # flush low order bits
        movd    %xmm0, %eax
        
        leave
        ret
        
insert:
        .equ    .count, currPar4
        .equ    .to,    currPar3
        .equ    .array, currPar2
        .equ    .field, currPar1
        push    %ebp
        mov     %esp, %ebp
        frame   4, 2
        sub     $frame_size, %esp

        leave
        ret

        .section .data
        .align   16
shift_amount:
        .int    0, 0, 0, 0
from:   .int    0
to:     .int    0
count:  .int    0
array:  .int    0x12345678, 0x54321789, 0x321fedcb, 0xdeadbeef
field:  .int    0
prompt: .asciz  "Enter from, to and count: "
read_fmt:
        .asciz  "%d %d %d"
