GLOBAL _set_cr, _lcr0, _scr0, _scr3, _lcr3, _midebug, _fill_page1
EXTERN getCR3

_set_cr:
        push   ebp
        mov    ebp, esp

        call   getCR3
        mov    cr3, eax         ; setea CR3 con la direccion de el directorio de paginas
        
        mov    esp,ebp
        pop    ebp
        retn


 ; _lcr0
 ; Loads CR0 with a value sent as a parameter.

 _lcr0:

     push    ebp
     mov     ebp, esp
     push    ebx
     mov     ebx, [ss: ebp + 8]
     mov     cr0, ebx
     pop     ebx
     pop     ebp

     retn


 ; _lcr3
 ; Loads CR3 with a value sent as a parameter.

 _lcr3:

     push    ebp
     mov     ebp, esp
     push    ebx
     mov     ebx, [ss: ebp + 8]
     mov     cr3, ebx
     pop     ebx
     pop     ebp

     retn


 ; _scr0
 ; Stores the CR0 value in the sent parameter.

 _scr0:

     push    ebp
     mov     ebp, esp
     push    ebx
     push    eax

     mov     ebx, [ss: ebp + 8]
     mov     eax, cr0
     mov     [ds: ebx], eax

	 pop	 eax
     pop     ebx
     pop     ebp

     retn


 ; _scr3
 ; Stores the CR3 value in the sent parameter.

 _scr3:

	push	ebp
	mov		ebp, esp
	push	ebx
	push	eax

	mov		ebx, [ss: ebp + 8]
	mov		eax, cr3
	mov		[ds: ebx], eax

	pop		eax
	pop		ebx
	pop		ebp

	retn

; Mapeo 1:1 de la primer pagina
_fill_page1:

	mov		eax, 0
	mov		ebx, 0

.fill_table:
	mov		ecx, ebx ; ecx = eax = ebx = 0
	cmp		eax, 530
	jg		.notpresent
	or		ecx, 1
.notpresent:
	mov		[201000h+eax*4], ecx
	add		ebx, 4096
	inc		eax
	cmp		eax, 1024
	je		.end
	jmp		.fill_table
.end:
	ret

_midebug:

        push    bp
        mov     bp, sp
		mov		eax, cr0
		pop     bp
        retn
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
