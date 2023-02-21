;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; GDT flusher (initializes GDT)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL gdt_flush]           ; Allows the C code to call gdt_flush().

gdt_flush:
   mov eax, [esp+4]          ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]                ; Load the new GDT pointer

   mov ax, 0x10              ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax                ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush           ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IDT flusher (initializes IDT)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL idt_flush]           ; Allows the C code to call idt_flush().

idt_flush:
   mov eax, [esp+4]          ; Get the pointer to the IDT, passed as a parameter.
   lidt [eax]                ; Load the IDT pointer.
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ISR wrapper (called when interrupt receive)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[GLOBAL isr_wrapper]         ; Allows the C code to call isr_wrapper().
[EXTERN isr_handler]         ; Allows the ASM code to call the C code interrupt handler.

; .globl   isr_wrapper
; .align   4
 
; isr_wrapper:
;    cli                 ; Disable interrupts
;    push byte 0         ; Push a dummy error code (if ISR0 doesn't push it's own error code)
;    ; push byte 0         ; Push the interrupt number (0)
;    pushad
;    ; cld                       ; C code following the sysV ABI requires DF to be clear on function entry
;    call interrupt_handler    ; call C function to handle interrupt
;    popad
;    sti
;    iret

isr_wrapper:
   cli                 ; Disable interrupts
   push byte 0         ; Push a dummy error code (if ISR0 doesn't push it's own error code)
   ; push byte 0         ; Push the interrupt number (0)
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ; Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call isr_handler

   pop eax        ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   sti
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP