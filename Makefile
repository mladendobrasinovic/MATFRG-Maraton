### Recepti kompilacije
CC = gcc

FLAGS = -Wall -Wextra -ansi
CFLAGS = $(FLAGS) -O3

LINK = -lGL -lGLU -lglut -lm

## Genericko pravilo za kompilaciju objektnih datoteka
%.o: %.c
	@echo ============= build [$^] =============
	$(CC) $(CFLAGS) $< -c -o $@ 

	@echo [OK] --- [$@]
	@echo

## Sve .o datoteka koje se koriste pri stvaranju programa
OBJ = main.o

maraton: $(OBJ)
	@echo ============= compile [$@] ===========
	gcc $(CFLAGS) $(OBJ) -o maraton $(LINK) 
	@make -s sweep

	@echo [OK] --- [$@]
	@echo

### Pomocne komande
.PHONY: clean sweep main

clean:
	@rm -f *.o *~ *#

sweep:
	@rm -f *.o

# Upotreba za ciscenje direktorijuma:
#$ make clean
