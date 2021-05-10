section .text
global f1
global ff1
global f2
global ff2
global f3
global ff3
    
f1:
    push ebp
    mov ebp, esp
    finit
    fldl2e                  ; st0 = log2(e)
    fmul qword [ebp + 8]    ; st0 = x * log2(e)
    fldz
    fld1
    fsubp                   ; st0 = -1.  st1 = x * log2(e)
    fmulp                   ; st0 = - x * log2(e)
    fld1                    
    fscale                  ; st0 = 2 ^ [- x * log2(e)] (st0 = st0 * 2 ^ st1).  st1 = - x * log2(e)
    fxch
    fld1
    fxch
    fprem                   ; st0 = {- x * log2(e)} (st0 = {st0 / st1}). st1 = 1. st2 = 2 ^ [- x * log2(e)]
    f2xm1                   ; st0 = 2 ^ {} - 1 (st0 = 2 ^ st0 - 1). st1 = 1. st2 = 2 ^ []
    faddp
    fmulp                   ; st0 = 2 ^ {} * 2 ^ [] = 2 ^ (- x * log2(e)) = e ^ (-x)
    fld1
    fld1
    faddp
    fld1
    faddp
    faddp                   ; st0 = e ^ (-x) + 3
    mov esp, ebp
    pop ebp
    ret
    
ff1:
    push ebp
    mov ebp, esp
    finit
    fldl2e
    fmul qword [ebp + 8]
    fldz
    fld1
    fsubp
    fmulp
    fld1
    fscale
    fxch
    fld1
    fxch
    fprem
    f2xm1
    faddp
    fmulp                   ; Аналогично st0 = 2 ^ {} * 2 ^ [] = 2 ^ (- x * log2(e)) = e ^ (-x)
    fldz
    fld1
    fsubp
    fmulp                   ; st0 = - e ^ (-x)
    mov esp, ebp
    pop ebp
    ret
    
f2:
    push ebp
    mov ebp, esp
    finit
    fld qword [ebp + 8]     ; st0 = x
    fld1
    fld1
    faddp                   ; st0 = 2. st1 = x
    fmul st1, st0           ; st0 = 2. st1 = 2x
    fsubp                   ; st0 = 2x - 2
    mov esp, ebp
    pop ebp
    ret
    
ff2:
    push ebp
    mov ebp, esp
    finit
    sub esp, 4
    mov dword [esp], 2
    fild dword [esp]        ; st0 = 2
    mov esp, ebp
    pop ebp
    ret
    
f3:
    push ebp
    mov ebp, esp
    finit
    fld1                    ; st0 = 1
    fdiv qword [ebp + 8]    ; st0 = 1/x
    mov esp, ebp
    pop ebp
    ret
    
ff3:
    push ebp
    mov ebp, esp
    finit
    fld1                    ; st0 = 1
    fdiv qword [ebp + 8]    ; st0 = 1/x
    fdiv qword [ebp + 8]    ; st0 = 1/(x^2)
    fldz
    fld1
    fsubp
    fmulp                   ; st0 = - 1/(x^2)
    mov esp, ebp
    pop ebp
    ret
