#--------------------------------------Makefile-------------------------------------
CFILES = $(wildcard ./src/*.c)
OFILES = $(CFILES:./src/%.c=./build/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib
LDFLAGS = -nostdlib 

all: clean kernel8.img run

#Create UART0 or UART1 preprocessing define
#Syntax: add -DVAR to compiler's flag will create #define VAR
GCCFLAGS += -DUART$(UARTTYPE)

./build/boot.o: ./src/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/uart.o: 
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/uart.c -o $@

./build/%.o: ./src/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o ./build/uart.o $(OFILES)
	aarch64-none-elf-ld $(LDFLAGS) ./build/boot.o $(OFILES)  -T ./src/link.ld -o ./build/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel8.elf kernel8.img

clean:
	rm -rf *.img .\build\*.elf .\build\*.o

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio




