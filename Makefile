# Define el directorio donde están los archivos fuente (.cpp y .s)
SRC_DIR := src

# Define el directorio donde se generarán los archivos objeto (.o)
OBJ_DIR := obj

# Encuentra todos los archivos fuente .cpp en el directorio SRC_DIR
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Genera una lista de archivos objeto .o para cada archivo fuente .cpp
# Convierte 'src/file.cpp' a 'obj/file.o'
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Encuentra todos los archivos ensamblador .s en el directorio SRC_DIR
ASSEMBLER_FILES := $(wildcard $(SRC_DIR)/*.s)

# Genera una lista de archivos objeto .o para cada archivo ensamblador .s
# Convierte 'src/file.s' a 'obj/file.o'
OBJ_ASSEMBLER_FILES := $(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(ASSEMBLER_FILES))

# Define las banderas para el compilador de C++ para generar código compatible con sistemas operativos de 32 bits
CPPFLAGS := -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore

# Parámetros para el linker, especificando el formato ELF de 32 bits
LINKER_PARAMS := -melf_i386

# Reglas para compilar archivos .cpp en archivos objeto .o
# $@ : Representa el archivo objetivo (en este caso, el archivo .o en OBJ_DIR)
# $< : Representa el primer requisito (en este caso, el archivo fuente .cpp en SRC_DIR)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

# Reglas para ensamblar archivos .s en archivos objeto .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	as --32 -o $@ $<

# Construcción de un archivo ISO para el kernel
bin/kernel.iso: bin/kernel.bin
	# Crea los directorios necesarios para generar el ISO
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	
	# Copia el kernel binario generado al directorio ISO
	cp bin/kernel.bin iso/boot/kernel.bin
	
	# Limpia los binarios previos
	rm -rf bin/*
	
	# Genera un archivo de configuración GRUB para arrancar el kernel
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "CEST" {'                >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin'      >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	
	# Crea la imagen ISO con grub-mkrescue
	grub-mkrescue --output=bin/kernel.iso iso
	
	# Limpia los archivos temporales generados
	rm -rf iso
	rm -rf obj/*

# Construcción del kernel binario usando el linker script y los archivos objeto
bin/kernel.bin: src/linker.ld $(OBJ_FILES) $(OBJ_ASSEMBLER_FILES)
	# Enlaza todos los archivos objeto en un binario del kernel usando el script de linker
	ld $(LINKER_PARAMS) -T $< -o $@ $(OBJ_FILES) $(OBJ_ASSEMBLER_FILES)

# Configuración inicial para preparar el entorno de desarrollo
first-time:
	# Instala los paquetes necesarios (compiladores, herramientas de binarios y VirtualBox)
	sudo apt-get -y install g++ binutils libc6-dev-i386 VirtualBox grub-legacy xorriso
	
	# Construye el archivo ISO del kernel
	bin/kernel.iso
	
	# Reinicia VirtualBox si está en ejecución y arranca el sistema operativo
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm 'My Operating System' &

# Instalación del kernel binario en el sistema (requiere permisos de superusuario)
install: bin/kernel.bin
	# Copia el archivo binario del kernel al directorio /boot
	sudo cp $< /boot/kernel.bin
# limpia en caso de error los archivos .o y la carpeta iso
clean:
	rm -rf $(OBJ_DIR)/* bin/*2 iso

