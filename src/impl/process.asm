[GLOBAL read_eip]
read_eip:
    pop eax                     ; Get the return address
    jmp eax                     ; Return. Can't use RET because return
                                ; address popped off the stack. 

[GLOBAL copy_page_physical]
copy_page_physical:
    push ebx              ; According to __cdecl, we must preserve the contents of EBX.
    pushf                 ; push EFLAGS, so we can pop it and reenable interrupts
                          ; later, if they were enabled anyway.
    cli                   ; Disable interrupts, so we aren't interrupted.
                          ; Load these in BEFORE we disable paging!
    mov ebx, [esp+12]     ; Source address
    mov ecx, [esp+16]     ; Destination address
  
    mov edx, cr0          ; Get the control register...
    and edx, 0x7fffffff   ; and...
    mov cr0, edx          ; Disable paging.
  
    mov edx, 1024         ; 1024*4bytes = 4096 bytes
  
.loop:
    mov eax, [ebx]        ; Get the word at the source address
    mov [ecx], eax        ; Store it at the dest address
    add ebx, 4            ; Source address += sizeof(word)
    add ecx, 4            ; Dest address += sizeof(word)
    dec edx               ; One less word to do
    jnz .loop             
  
    mov edx, cr0          ; Get the control register again
    or  edx, 0x80000000   ; and...
    mov cr0, edx          ; Enable paging.
  
    popf                  ; Pop EFLAGS back.
    pop ebx               ; Get the original value of EBX back.
    ret 

[GLOBAL perform_task_switch]
perform_task_switch:
    cli;
    mov ecx, [esp+4]      ; EIP
    mov eax, [esp+8]      ; physical address of current directory
    mov ebp, [esp+12]     ; EBP
    mov esp, [esp+16]     ; ESP
    mov cr3, eax          ; set the page directory
    mov eax, 0x31415926   ; yeeeeee pi number
    sti;
    jmp ecx

global switch_to_user
switch_to_user:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push esp
    pushfd
    ; code segment selector, rpl = 3
    push 0x1b
    lea eax, [user_start]
    push eax
    iretd
user_start:
    add esp, 4
    ret