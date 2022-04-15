.data
cmd_ls:
        .string "/bin/echo"
arg_ls:
        .string "Hello World"
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

        lea arg_ls(%rip), %rdi
        push %rdi
        lea cmd_ls(%rip), %rdi
        push %rdi

        mov %rsp, %rsi
        call execve
