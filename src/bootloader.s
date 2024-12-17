.set MAGIC, 0x1badb002       # Valor mágico requerido por Multiboot.
.set FLAGS, (1<<0 | 1<<1)    # Indicadores Multiboot; bits 0 y 1 activados.
.set CHECKSUM, -(MAGIC + FLAGS)  # Suma de verificación para validar los encabezados.

.section .multiboot          # Sección para los encabezados Multiboot.
    .long MAGIC              # Encabezado Multiboot: valor mágico.
    .long FLAGS              # Encabezado Multiboot: indicadores.
    .long CHECKSUM           # Encabezado Multiboot: suma de verificación.

.section .text               # Sección de código ejecutable.
.extern kernel               # El símbolo 'kernel' se define en otro archivo.
.global loader               # Define 'loader' como punto de entrada global.

loader:                      # Punto de entrada principal del bootloader.
    mov $kernel_stack, %esp  # Configura el stack apuntando al espacio reservado.
    push %eax                # Guarda el registro 'eax' en el stack.
    push %ebx                # Guarda el registro 'ebx' en el stack.
    call kernel              # Transfiere control al kernel.


_stop:                       # Si el kernel no toma el control, detiene el sistema.
    cli                      # Deshabilita las interrupciones.
    hlt                      # Coloca la CPU en modo inactivo.
    jmp _stop                # Bucle infinito para detener la ejecución.

.section .bss                # Sección de datos no inicializados.
.space 10*1024*1024          # Reserva 10 MB para la pila.
kernel_stack:                # Etiqueta para la dirección de inicio de la pila.
