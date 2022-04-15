.global _start
.data
message: .string "Hola, mundo\n"
a:  .space 20
.text
_start:
    mov $0, %rax
    mov $0, %rdi
    mov $a, %rsi
    mov $20, %rdx
    syscall

    mov $1, %rax
    mov $1, %rdi
    mov $a, %rsi
    mov $30, %rdx
    syscall

    mov $60, %rax
    mov $0, %rdi
    syscall
