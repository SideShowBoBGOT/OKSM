.data
cmd_cat:
        .string "/bin/cat"
arg_cat:
        .string "log.txt"
cmd_wc:
        .string "/bin/wc"
arg_wc:
        .string "-l"
fds:
        .space 4, 0
        .space 4, 0
.text
.global main
main:
        # pipe(fds)
        xor %rax, %rax
        mov $22, %al
        lea fds(%rip), %rdi
        syscall

        xor %rax, %rax # fork
        mov $57, %al
        syscall 

        cmp $0, %rax
        je child_cat

        xor %rax, %rax # fork
        mov $57, %al
        syscall 

        cmp $0, %rax
        je child_wc

        # close(fds[0])
        xor %rax, %rax
        mov $3, %al
        lea fds(%rip), %rdi
        mov (%rdi), %ebx
        mov %rbx, %rdi
        syscall

        # close(fds[1])
        xor %rax, %rax
        mov $3, %al
        lea fds(%rip), %rdi
        mov 4(%rdi), %ebx
        mov %rbx, %rdi
        syscall

        mov $0, %rdi
        call wait

        mov $0, %rdi
        call wait
finish: 
        mov $60, %rax
        mov $0, %rdi
        syscall
child_cat:
        # dup2(fds[1], STDOUT_FILEINO)
        xor %rax, %rax
        mov $33, %al
        lea fds(%rip), %rdi
        mov 4(%rdi), %ebx
        mov %rbx, %rdi
        xor %rsi, %rsi
        inc %rsi
        syscall
        

        # close(fds[0])
        xor %rax, %rax
        mov $3, %al
        lea fds(%rip), %rdi
        mov (%rdi), %ebx
        mov %rbx, %rdi
        syscall

        # close(fds[1])
        xor %rax, %rax
        mov $3, %al
        lea fds(%rip), %rdi
        mov 4(%rdi), %ebx
        mov %rbx, %rdi
        syscall
    
        xor %rdx, %rdx
        push %rdx

        lea arg_cat(%rip), %rdi
        push %rdi
        lea cmd_cat(%rip), %rdi
        push %rdi

        mov %rsp, %rsi
        call execve
child_wc:
        # dup2(fds[1], STDOUT_FILEINO)
        xor %rax, %rax
        mov $33, %al
        lea fds(%rip), %rdi
        mov 0(%rdi), %ebx
        mov %rbx, %rdi
        xor %rsi, %rsi
        syscall
        

        # close(fds[0])
        xor %rax, %rax
        mov $3, %al
        lea fds(%rip), %rdi
        mov (%rdi), %ebx
        mov %rbx, %rdi
        syscall

        # close(fds[1])
        xor %rax, %rax
        mov $3, %al
        lea fds(%rip), %rdi
        mov 4(%rdi), %ebx
        mov %rbx, %rdi
        syscall
    
        xor %rdx, %rdx
        push %rdx

        lea arg_wc(%rip), %rdi
        push %rdi
        lea cmd_wc(%rip), %rdi
        push %rdi

        mov %rsp, %rsi
        call execve