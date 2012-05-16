GLOBAL _set_cr
EXTERN getCR3

_set_cr:
        push   ebp
        mov    ebp, esp

        call   getCR3
        mov    cr3, eax         ; setea CR3 con la direccion de el directorio de paginas
        
        mov    eax, cr0      ;guardo el valor del CR0
        or     eax, 80000000h ;seteo el ultimo bit en 1 que es el de PG

        jmp	   _debug ; SACAAARR !!!!!!!!!!!!!!!!!!!!!

        mov    cr0,eax        ;guardo CR0 modificado
        mov    esp,ebp
        pop    ebp
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
