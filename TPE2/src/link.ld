ENTRY (_loader)

SECTIONS{
    . = 0x00100000;

	 .__mbHeader : {
      *(.__mbHeader)
   }
    .text :{
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata*)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    .bss : {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}