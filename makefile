## Common side variable declaration
C_INC = $(wildcard common/*.h)
C_SRC = $(wildcard common/*.c)

## Raspberry side variable declaration
CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -Iraspberry/include -std=c99
LDLIBS =
RASPBERRY_SRC = $(wildcard raspberry/src/*.c)
R_INC = $(C_INC:common/%.h=raspberry/include/%.h)
R_SRC = $(C_SRC:common/%.c=raspberry/src/%.c)
ALL_RASPBERRY_SRC = $(filter-out $(R_SRC),$(RASPBERRY_SRC)) $(R_SRC)
RASPBERRY_OBJ = $(ALL_RASPBERRY_SRC:raspberry/src/%.c=raspberry/build/%.o)
BIN = Raspberry_side

.PHONY: all sync_raspberry sync_arduino flash clean 

all: $(BIN)

$(BIN): $(RASPBERRY_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

sync_raspberry: $(R_INC) $(R_SRC) 

raspberry/include/%.h : common/%.h
	@mkdir -p raspberry/include/
	cp $^ $@

raspberry/src/%.c : common/%.c
	@mkdir -p raspberry/src/
	cp $^ $@

raspberry/build/%.o: raspberry/src/%.c | sync_raspberry
	@mkdir -p raspberry/build
	$(CC) $(CFLAGS) -c $^ -o $@
	@# $^ pour appeler tous les .c présents dans src/

## arduino side variable declaration
COMPILATEUR = avr-gcc
CIBLE = atmega328p                      
OPTIONS = -Wall -Wextra -g -Os -Iarduino/include -std=c99 -mmcu=$(CIBLE) -DF_CPU=16000000UL
LIBRAIRIES =
A_INC = $(C_INC:common/%.h=arduino/include/%.h)
A_SRC = $(C_SRC:common/%.c=arduino/src/%.c)
ARDUINO_SRC = $(wildcard arduino/src/*.c)
ALL_ARDUINO_SRC = $(filter-out $(A_SRC),$(ARDUINO_SRC)) $(A_SRC)
ARDUINO_OBJ = $(ALL_ARDUINO_SRC:arduino/src/%.c=arduino/build/%.o)
NOM = Arduino_side
ELF = $(NOM).elf
HEX = $(NOM).hex

flash: $(HEX)
	avrdude -c arduino -p $(CIBLE) -P /dev/ttyACM0 -b 115200 -U flash:w:$^

$(HEX): $(ELF)
	avr-objcopy -O ihex $^ $@

$(ELF): $(ARDUINO_OBJ)
	$(COMPILATEUR) $(OPTIONS) $^ -o $@ $(LIBRAIRIES)
	@# $^ pour appeler ce qui est appelé avant le lancement de la commande, ici $(OBj) soit tous les .o présents dans /build
	@# $@ pour appeler celui qui execute la commande, ici elf

sync_arduino: $(A_SRC) $(A_INC)

arduino/include/%.h: common/%.h 
	@mkdir -p arduino/include/
	cp $^ $@

arduino/src/%.c: common/%.c 
	@mkdir -p arduino/src/
	cp $^ $@

arduino/build/%.o: arduino/src/%.c | sync_arduino
	@mkdir -p arduino/build/ #@ avant une ligne pour cacher la commande dans le terminal
	$(COMPILATEUR) $(OPTIONS) -c $^ -o $@

clean:
	@$(RM) -r $(HEX) $(ELF) $(BIN) arduino/build arduino/include raspberry/build raspberry/include
	clear
