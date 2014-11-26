        segment .data
        align   8
infd    dq      0
outfd   dq      0
in_name dq      0
out_name:
        dq      0
in_size dq      0
data    dq      0
        segment .text
        global  main
        extern  open, malloc, lseek, read, write, close, printf
;       rcx/xmm0, rdx/xmm1, r8/xmm2, r9/xmm3 then stack
;       must had 4 quadwords free on the stack top before a call
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32

;       Check the command line parameter count
        cmp     rcx, 3
        jne     .param_error

;       Save the input and output file names
        mov     r8, [rdx+8]
        mov     r9, [rdx+16]
        mov     [in_name], r8
        mov     [out_name], r9

;       Report the files being copied
        segment .data
.files_fmt:
        db      "copying %s to %s",0x0a,0
        segment .text
        lea     rcx, [.files_fmt]
        mov     rdx, [in_name]
        mov     r8, [out_name]
        xor     eax, eax
        call    printf

;       Try to open the input file
        mov     rcx, [in_name]
        xor     edx, edx
        call    open
        cmp     eax, 0
        jl      .input_open_failed
        mov     [infd], rax

;       Try to open the output file
        mov     rcx, [out_name]
        xor     edx, edx
        call    open
        cmp     eax, 0
        jge     .output_exists

;       Try to create the output file
        mov     rcx, [out_name]
        mov     edx, 0x8101      ;  0x8000 = O_BINARY
                                 ;  0x0100 = O_CREAT
                                 ;  0x0000 = O_RDONLY
                                 ;  0x0001 = O_WRONLY
                                 ;  0x0002 = O_RDWR
        mov     r8d, 700o
        call    open
        cmp     eax, 0
        jl      .output_open_failed
        mov     [outfd], rax

;       Determine the input file size
        mov     rcx, [infd]
        xor     edx, edx
        mov     r8d, 2
        call    lseek
        mov     [in_size], rax
        mov     rcx, [infd]
        xor     edx, edx
        mov     r8d, 0
        call    lseek

;       Allocate the data array
        mov     rcx, [in_size]
        call    malloc
        cmp     rax, 0
        je      .malloc_failed
        mov     [data], rax

;       Read the input file
        mov     rcx, [infd]
        mov     rdx, [data]
        mov     r8, [in_size]
        call    read

;       Write the output file
        mov     rcx, [outfd]
        mov     rdx, [data]
        mov     r8, [in_size]
        call    write

;       Close the input and output files
        mov     rcx, [infd]
        call    close
        mov     rcx, [outfd]
        call    close

        xor     eax, eax
        leave
        ret

        segment .data
.malloc_failed_fmt:
        db      "malloc failed for %ld bytes",0x0a,0
        segment .text
.malloc_failed:
        lea     rcx, [.malloc_failed_fmt]
        mov     rdx, [in_size]
        call    printf
        mov     eax, 1
        leave
        ret

        segment .data
.output_exist_fmt:
        db      "%s already exists",0x0a,0
        segment .text
.output_exists:
        lea     rcx, [.output_exist_fmt]
        mov     rdx, [out_name]
        call    printf
        mov     eax, 1
        leave
        ret

        segment .data
.output_open_fmt:
        db      "Could not open output_file %s",0x0a,0
        segment .text
.output_open_failed:
        lea     rcx, [.output_open_fmt]
        mov     rdx, [out_name]
        call    printf
        mov     eax, 1
        leave
        ret

        segment .data
.input_open_fmt:
        db      "Could not open input_file %s",0x0a,0
        segment .text
.input_open_failed:
        lea     rcx, [.input_open_fmt]
        mov     rdx, [in_name]
        call    printf
        mov     eax, 1
        leave
        ret

        segment .data
.param_fmt:
        db      "Usage: copy input_file output_file",0x0a,0
        segment .text
.param_error:
        lea     rcx, [.param_fmt]
        call    printf
        mov     eax, 1
        leave
        ret
