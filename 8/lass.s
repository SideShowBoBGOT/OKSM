.data
    cmd_cat:
            .string "/bin/cat"
    arg_cat:
            .string "log"
    cmd_awk1:
            .string "/bin/awk"
    arg_awk1:
            .string "BEGIN{FS=\" \"}($0 !~ /.*\"-\".*/){print $11}"
    cmd_sed:
            .string "/bin/sed"
    arg_sed:
            .string "s/\"//g"
    cmd_sort1:
            .string "/bin/sort"
    arg_sort1:
            .string "-s"
    cmd_uniq:
            .string "/bin/uniq"
    arg_uniq:
            .string "-c"
    cmd_awk2:
            .string "/bin/awk"
    arg_awk2:
            .string "BEGIN{FS=\" \";nrows=}{print $2\t$1\t$1/nrows*100\"%\"}"
    cmd_sort2:
            .string "/bin/sort"
    arg_sort2:
            .string "-k"
            .string "3n"
    cmd_tail:
            .string "/bin/tail"
    arg_tail:
            .string "-10"
    cmd_tac:
            .string "/bin/tac"
    zero_string:
            .int 0
    fds:
            .space 4, 0 # pipe 0
            .space 4, 0
            .space 4, 0 # pipe 1
            .space 4, 0
            .space 4, 0 # pipe 2
            .space 4, 0
            .space 4, 0 # pipe 3
            .space 4, 0
            .space 4, 0 # pipe 4
            .space 4, 0
            .space 4, 0 # pipe 5
            .space 4, 0
            .space 4, 0 # pipe 6
            .space 4, 0
            .space 4, 0 # pipe 7
            .space 4, 0
    commands:
            .quad cmd_cat
            .quad arg_cat
            .quad zero_string
            .quad cmd_awk1
            .quad arg_awk1
            .quad zero_string
            .quad cmd_sed
            .quad arg_sed
            .quad zero_string
            .quad cmd_sort1
            .quad arg_sort1
            .quad zero_string
            .quad cmd_uniq
            .quad arg_uniq
            .quad zero_string
            .quad cmd_awk2
            .quad arg_awk2
            .quad zero_string
            .quad cmd_sort2
            .quad arg_awk2
            .quad zero_string
            .quad cmd_sort2
            .quad arg_sort2
            .quad zero_string
            .quad cmd_tail
            .quad arg_tail
            .quad zero_string
            .quad cmd_tac
            .quad zero_string
.text
.global main
main:
        call init_pipes

        call do_fork

        cmp $0, %rax
        je child_cat

        call do_fork

        cmp $0, %rax
        je child_awk1

        call close_pipes

        mov $0, %rdi
        call wait

        mov $0, %rdi
        call wait
finish: 
        mov $60, %rax
        mov $0, %rdi
        syscall
init_pipes:
    push %rax
    push %rbx
    xor %rbx, %rbx 
    mov $1, %rbx
    lea fds(%rip), %rdi
    loop_init_pipes:
        xor %rax, %rax
        mov $22, %al
        syscall
        add $8, %rdi
        dec %rbx
        cmp $0, %rbx
        jg loop_init_pipes
    pop %rbx
    pop %rax
    ret
do_fork:
    xor %rax, %rax
    mov $57, %al
    syscall
    ret 
close_pipes:
        push %rax
        push %rbx
        push %rcx
        mov $1, %rbx
        lea fds(%rip), %rdi
        loop_close_pipes:
            push %rdi

            xor %rax, %rax
            mov $3, %al
            mov 4(%rdi), %edx
            mov %rdx, %rdi
            syscall

            pop %rdi
            push %rdi

            xor %rax, %rax
            mov $3, %al
            mov 0(%rdi), %edx
            mov %rdx, %rdi
            syscall

            pop %rdi

            add $8, %rdi
            dec %rbx
            cmp $0, %rbx
            jg loop_close_pipes
        pop %rcx
        pop %rbx
        pop %rax
        ret
main_loop:
        # %rbx is a counter
        mov $0, %rbx
        # for (int i = 0; i < NUM_PROCS; i++)
        loop_procs:
            call do_fork
            cmp $0, %rax
            je child
            child:
                cmp $0, %rbx #  if (i == 0)
                je begin_thread
                begin_thread:
                    push %rbx
                    
                    # rbx * 8
                    shl %rbx
                    shl %rbx
                    shl %rbx

                    mov %rbx, %rcx

                    xor %rax, %rax
                    mov $33, %al
                    lea fds(%rip), %rdi
                    add %rcx, %rdi
                    mov 4(%rdi), %ebx
                    mov %rbx, %rdi
                    xor %rsi, %rsi
                    inc %rsi
                    syscall

                    pop %rbx
                    jp loop_procs_finish

                cmp $1, %rbx # else if (i == NUM_PROCS - 1)
                je begin_thread
                last_thread:
                    push %rbx

                    dec %rbx

                    # rbx * 8
                    shl %rbx
                    shl %rbx
                    shl %rbx

                    mov %rbx, %rcx

                    xor %rax, %rax
                    mov $33, %al
                    lea fds(%rip), %rdi
                    add %rcx, %rdi
                    mov 0(%rdi), %ebx
                    mov %rbx, %rdi
                    xor %rsi, %rsi
                    syscall

                    pop %rbx
                    jp loop_procs_finish
                common_thread: #  else
                    push %rbx
                    
                    # rbx * 8
                    shl %rbx
                    shl %rbx
                    shl %rbx

                    mov %rbx, %rcx

                    xor %rax, %rax
                    mov $33, %al
                    lea fds(%rip), %rdi
                    add %rcx, %rdi
                    mov 4(%rdi), %ebx
                    mov %rbx, %rdi
                    xor %rsi, %rsi
                    inc %rsi
                    syscall

                    pop %rbx

                    push %rbx

                    dec %rbx

                    # rbx * 8
                    shl %rbx
                    shl %rbx
                    shl %rbx

                    mov %rbx, %rcx

                    xor %rax, %rax
                    mov $33, %al
                    lea fds(%rip), %rdi
                    add %rcx, %rdi
                    mov 0(%rdi), %ebx
                    mov %rbx, %rdi
                    xor %rsi, %rsi
                    syscall

                    pop %rbx
            loop_procs_finish:
                call close_pipes
                # execve command                
                inc %rbx
                cmp $2, %rbx
                jl loop_procs
        call close_pipes
        # wait for everyone

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
        
        call close_pipes
    
        xor %rdx, %rdx
        push %rdx

        lea arg_cat(%rip), %rdi
        push %rdi
        lea cmd_cat(%rip), %rdi
        push %rdi

        mov %rsp, %rsi
        call execve
child_awk1:
        # dup2(fds[1], STDOUT_FILEINO)
        xor %rax, %rax
        mov $33, %al
        lea fds(%rip), %rdi
        mov 0(%rdi), %ebx
        mov %rbx, %rdi
        xor %rsi, %rsi
        syscall
        
        call close_pipes
    
        xor %rdx, %rdx
        push %rdx

        lea arg_awk1(%rip), %rdi
        push %rdi
        lea cmd_awk1(%rip), %rdi
        push %rdi

        mov %rsp, %rsi
        call execve
