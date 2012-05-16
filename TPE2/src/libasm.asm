GLOBAL  _read_msw,_lidt
GLOBAL  _int_20_hand,_int_21_hand, _int_80_hand, _int_74_hand, _SysCall, _debug, _sleep, _yield
GLOBAL	EOI
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL	_excp_00_hand, _excp_01_hand, _excp_02_hand, _excp_03_hand, _excp_04_hand
GLOBAL	_excp_05_hand, _excp_06_hand, _excp_07_hand, _excp_08_hand, _excp_09_hand
GLOBAL	_excp_10_hand, _excp_11_hand, _excp_12_hand, _excp_13_hand, _excp_14_hand
GLOBAL	_excp_15_hand, _excp_16_hand, _excp_17_hand, _excp_18_hand, _excp_19_hand
GLOBAL	_excp_20_hand, _excp_21_hand, _excp_22_hand, _excp_23_hand, _excp_24_hand
GLOBAL	_excp_25_hand, _excp_26_hand, _excp_27_hand, _excp_28_hand, _excp_29_hand
GLOBAL	_excp_30_hand, _invop

GLOBAL  _debug, _excp_0_hand
EXTERN  int_20, int_21, int_74, int_80, int_80, debugger, exception_handler, getSSPointer, getSPPointer

SECTION .text

_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR
		rol	ebx,16
		lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_excp_00_hand:

    push ebx
    mov  ebx, 00h
    jmp isr_common_stub
    pop ebx

_invop:
	mov ax, 25h
	push ax
	ret

_excp_01_hand:
    push ebx
    mov  ebx, 01h
    jmp isr_common_stub
    pop ebx

_excp_02_hand:
    push ebx
    mov  ebx, 02h
    jmp isr_common_stub
    pop ebx

_excp_03_hand:
    push ebx
    mov  ebx, 03h
    jmp isr_common_stub
    pop ebx

_excp_04_hand:
    push ebx
    mov  ebx, 04h
    jmp isr_common_stub
    pop ebx

_excp_05_hand:
    push ebx
    mov  ebx, 05h
    jmp isr_common_stub
    pop ebx

_excp_06_hand:
    push ebx
    mov  ebx, 06h
    jmp isr_common_stub
    pop ebx

_excp_07_hand:
    push ebx
    mov  ebx, 07h
    jmp isr_common_stub
    pop ebx

_excp_08_hand:
    push ebx
    mov  ebx, 08h
    jmp isr_common_stub
    pop ebx

_excp_09_hand:
    push ebx
    mov  ebx, 09h
    jmp isr_common_stub
    pop ebx

_excp_10_hand:
    push ebx
    mov  ebx, 0ah
    jmp isr_common_stub
    pop ebx

_excp_11_hand:
    push ebx
    mov  ebx, 0bh
    jmp isr_common_stub
    pop ebx

_excp_12_hand:
    push ebx
    mov  ebx, 0ch
    jmp isr_common_stub
    pop ebx

_excp_13_hand:
    push ebx
    mov  ebx, 0dh
    jmp isr_common_stub
    pop ebx

_excp_14_hand:
    push ebx
    mov  ebx, 0eh
    jmp isr_common_stub
    pop ebx

_excp_15_hand:
    push ebx
    mov  ebx, 0fh
    jmp isr_common_stub
    pop ebx

_excp_16_hand:
    push ebx
    mov  ebx, 10h
    jmp isr_common_stub
    pop ebx

_excp_17_hand:
    push ebx
    mov  ebx, 11h
    jmp isr_common_stub
    pop ebx

_excp_18_hand:
    push ebx
    mov  ebx, 12h
    jmp isr_common_stub
    pop ebx

_excp_19_hand:
    push ebx
    mov  ebx, 13h
    jmp isr_common_stub
    pop ebx

_excp_20_hand:
    push ebx
    mov  ebx, 14h
    jmp isr_common_stub
    pop ebx

_excp_21_hand:
    push ebx
    mov  ebx, 15h
    jmp isr_common_stub
    pop ebx

_excp_22_hand:
    push ebx
    mov  ebx, 16h
    jmp isr_common_stub
    pop ebx

_excp_23_hand:
    push ebx
    mov  ebx, 17h
    jmp isr_common_stub
    pop ebx

_excp_24_hand:
    push ebx
    mov  ebx, 18h
    jmp isr_common_stub
    pop ebx

_excp_25_hand:
    push ebx
    mov  ebx, 19h
    jmp isr_common_stub
    pop ebx

_excp_26_hand:
    push ebx
    mov  ebx, 1ah
    jmp isr_common_stub
    pop ebx

_excp_27_hand:
    push ebx
    mov  ebx, 1bh
    jmp isr_common_stub
    pop ebx

_excp_28_hand:
    push ebx
    mov  ebx, 1ch
    jmp isr_common_stub
    pop ebx

_excp_29_hand:
    push ebx
    mov  ebx, 1dh
    jmp isr_common_stub
    pop ebx

_excp_30_hand:
    push ebx
    mov  ebx, 1eh
    jmp isr_common_stub
    pop ebx


isr_common_stub:

	cli
	pushad


	mov 	eax, ebx
	push 	eax
	call	exception_handler
	pop	eax

	jmp	EOI

_int_20_hand:				; Handler de INT 20 ( Timer tick)
        cli
        pushad
        call	getSPPointer
       	cmp		eax, 0
       	jz		sched
        mov		[eax],esp
sched:
        call    int_20
        call	getSPPointer
        mov		esp, [eax]
        jmp	EOI			; Envio de EOI generico al PIC

_int_21_hand:                                ; Handler de INT 21 ( Teclado )
	cli

	pushad

	in 	al, 60h
	push 	eax
	call	int_21
	pop	eax

	jmp	EOI

_int_74_hand:                                ; Handler de INT 74 ( Mouse )


	pushad
	in al, 60h
	push eax
	call int_74
	mov al,20h
	out 0A0h, al ;pic slave
	out 20h, al
	pop eax
	popa
	iret


EOI:
	mov	al,20h
	out	20h,al
	popad
	sti
	iret

_int_80_hand:                                ; Handler de INT 80 ( Syscall )
	push ebp
	mov ebp, esp				;StackFrame

	;sti
	push edi
	push esi
	push edx
	push ecx
	push ebx

	push esp		; Puntero al array de argumentos

	push eax		; Numero de Systemcall

	call int_80

	; En eax debe dejar la
	; respuesta

	; Retornal al viejo stack
	pop eax
	pop esp
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	mov esp, ebp
	pop ebp

	iret


_SysCall:
	push ebp
	mov ebp, esp

	push ebx
	push ecx
	push edx
	push esi
	push edi

	mov eax, [ebp + 8] ; Syscall number
	mov ebx, [ebp + 12]; Arg1
	mov ecx, [ebp + 16]; Arg2
	mov edx, [ebp + 20]; Arg3
	mov esi, [ebp + 24]; Arg4
	mov edi, [ebp + 28]; Arg5

	int 80h

	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx

	mov esp, ebp
	pop ebp
	ret

switch:

; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0

_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
_debugBuenaOnda:
	push eax
		mov eax, 0fdh
		push eax
		call _mascaraPIC1
		pop eax
		mov ax,1
		mov [0x123450], ax
	otravez:
		mov ax, [0x123450]
		cmp ax ,0
		jne otravez
		mov eax, 0fch
		push eax
		call _mascaraPIC1
		pop eax
	pop eax

	retn

_sleep:
	push ebp
	mov ebp, esp
	cli

	mov	ecx, 999999
loop:
	nop
	dec ecx
	jnz loop

	sti
	mov esp, ebp
	pop ebp
	ret
