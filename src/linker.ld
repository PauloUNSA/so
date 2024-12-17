/* Define el punto de entrada del programa: 'loader' */
ENTRY(loader)

/* Especifica el formato de salida como ELF de 32 bits para arquitectura i386 */
OUTPUT_FORMAT(elf32-i386)

/* Declara la arquitectura objetivo como i386 (Intel 32 bits) */
OUTPUT_ARCH(i386:i386)

SECTIONS
{
  /* Define la dirección de inicio de la memoria donde se carga el kernel */
  . = 0x0100000;

  /* Sección .text: Contiene el código ejecutable y datos de solo lectura */
  .text :
  {
    /* Incluye la sección .multiboot para cumplir con el estándar Multiboot */
    *(.multiboot)
    
    /* Incluye todas las secciones que comienzan con '.text' (código ejecutable) */
    *(.text*)
    
    /* Incluye la sección .rodata (datos de solo lectura) */
    *(.rodata)
  }

  /* Sección .data: Contiene variables inicializadas y constructores globales */
  .data  :
  {
    /* Marca el inicio de los constructores */
    start_ctors = .;

    /* Asegura que la sección .init_array (constructores) no sea eliminada */
    KEEP(*( .init_array ))

    /* Ordena las secciones .init_array.* según la prioridad */
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )))

    /* Marca el final de los constructores */
    end_ctors = .;

    /* Incluye todas las secciones que comienzan con '.data' (variables inicializadas) */
    *(.data)
  }

  /* Sección .bss: Contiene variables no inicializadas */
  .bss  :
  {
    /* Incluye todas las secciones que comienzan con '.bss' */
    *(.bss)
  }

  /* Descarta secciones innecesarias para el kernel */
  /DISCARD/ : 
  { 
    /* Elimina la sección .fini_array (destructores globales) */
    *(.fini_array*) 
    
    /* Elimina la sección .comment (metadatos del compilador) */
    *(.comment) 
  }
}
