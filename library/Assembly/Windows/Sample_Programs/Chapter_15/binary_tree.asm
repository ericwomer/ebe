        segment .data
        struc   node
n_value resq    1
n_left  resq    1
n_right resq    1
        align   8
        endstruc
        struc   tree
t_count resq    1
t_root  resq    1
        align   8
        endstruc

        segment .text
        global  main, new_tree, insert, print, rec_print, find
        extern  malloc, scanf, printf

new_tree:
        push    rbp
        mov     rbp, rsp
        frame   0, 0, 1
        sub     rsp, frame_size
        mov     rcx, tree_size
        call    malloc
        xor     edx, edx
        mov     [rax+t_root], rdx
        mov     [rax+t_count], rdx
        leave
        ret

;       insert ( t, n );
insert:
.n      equ     local1
.t      equ     local2
        push    rbp
        mov     rbp, rsp
        frame   2, 2, 2
        sub     rsp, frame_size
        mov     [rbp+.t], rcx
        mov     [rbp+.n], rdx
        call    find
        cmp     rax, 0
        jne     .done 
        mov     rcx, node_size
        call    malloc
        mov     rdx, [rbp+.n]
        mov     [rax+n_value], rdx
        xor     r8d, r8d
        mov     [rax+n_left], r8
        mov     [rax+n_right], r8
        mov     r9, [rbp+.t]
        mov     rcx, [r9+t_count]
        cmp     rcx, 0
        jne     .findparent
        inc     qword [r9+t_count]
        mov     [r9+t_root], rax
        jmp     .done
.findparent:
        inc     qword [r9+t_count]
        mov     r9, [r9+t_root]
.repeatfind:
        cmp     rdx, [r9+n_value]
        jl      .goleft
        mov     r8, r9
        mov     r9, [r8+n_right]
        cmp     r9, 0
        jne     .repeatfind
        mov     [r8+n_right], rax
        jmp     .done
.goleft:
        mov     r8, r9
        mov     r9, [r8+n_left]
        cmp     r9, 0
        jne     .repeatfind
        mov     [r8+n_left], rax
.done   leave
        ret

;       p = find ( t, n );
;       p = 0 if not found
find:
        push    rbp
        mov     rbp, rsp
        frame   2, 0, 0
        sub     rsp, frame_size
        mov     rcx, [rcx+t_root]
        xor     eax, eax
.more   cmp     rcx, 0
        je      .done
        cmp     rdx, [rcx+n_value]
        jl      .goleft
        jg      .goright
        mov     rax, rcx
        jmp     .done
.goleft:
        mov     rcx, [rcx+n_left]
        jmp     .more
.goright:
        mov     rcx, [rcx+n_right]
        jmp     .more
.done   leave
        ret

rec_print:
.t      equ     local1
        push    rbp
        mov     rbp, rsp
        frame   1, 1, 2
        sub     rsp, frame_size
        cmp     rcx, 0
        je      .done
        mov     [rbp+.t], rcx
        mov     rcx, [rcx+n_left]
        call    rec_print
        mov     rcx, [rbp+.t]
        mov     rdx, [rcx+n_value]
        segment .data
.print  db      "%ld ",0
        segment .text
        lea     rcx, [.print]
        call    printf
        mov     rcx, [rbp+.t]
        mov     rcx, [rcx+n_right]
        call    rec_print
.done   leave
        ret

;       print(t);
print:
        push    rbp
        mov     rbp, rsp
        frame   1, 0, 1
        mov     rcx, [rcx+t_root]
        call    rec_print
        segment .data
.print  db      0x0a, 0
        segment .text
        lea     rcx, [.print]
        call    printf
        leave
        ret

main:
.k      equ     local1
.t      equ     local2
        segment .data
.scanf_fmt:
        db      "%ld",0
        segment .text
        push    rbp
        mov     rbp, rsp
        frame   2, 2, 2
        sub     rsp, frame_size
        call    new_tree
        mov     [rbp+.t], rax
.more   lea     rcx, [.scanf_fmt]
        lea     rdx, [rbp+.k]
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rcx, [rbp+.t]
        mov     rdx, [rbp+.k]
        call    insert
        mov     rcx, [rbp+.t]
        call    print
        jmp     .more
.done   leave
        ret
