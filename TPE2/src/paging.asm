GLOBAL _set_cr

        _set_cr:
        push   ebp
        mov    ebp, esp
        mov    eax, 00200h 
        mov    cr3, eax     ; setea CR3 con la direccion de el directorio de paginas
        
        mov    ebx, cr0      ;guardo el valor del CR0
        or     ebx, 80000000h ;seteo el ultimo bit en 1 que es el de PG
        mov    cr0,ebx        ;guardo CR0 modificado
        mov    esp,ebp
        pop    ebp
        ret