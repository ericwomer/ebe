        segment .data
x       dd      0.0, 1.0, 2.0
y       dd      1.0, 3.0, 5.0

        segment .text
        global  main, dot_product
main:
        lea     rcx, [x]
        lea     rdx, [y]
        call    dot_product
        xor     eax, eax
        ret

dot_product:
        movss   xmm0, [rcx]
        mulss   xmm0, [rdx]
        movss   xmm1, [rcx+4]
        mulss   xmm1, [rdx+4]
        addss   xmm0, xmm1
        movss   xmm2, [rcx+8]
        mulss   xmm2, [rdx+8]
        addss   xmm0, xmm2
        ret
