### Recepti kompilacije
## Dokumentacija za gcc i make se moze dobiti sa info komandom

## Projekat je struktuiran tako da su .h datoteke projekta u ./headers
## direktorijumu, .c datoteke u ./sources
CC = gcc

FLAGS = -Wall -Wextra -ansi -std=c99
CFLAGS = $(FLAGS) -O3
CDBGFLAGS = $(FLAGS) -g -Og
HDIR = ./headers

LINK = -lGL -lGLU -lglut -lm

## Opste pravilo za kompilaciju objektnih datoteka
%.o: sources/%.c
	@echo ============= build [$^] =============
	$(CC) $(CFLAGS) $< -c -o $@ -I $(HDIR)

	@echo [OK] --- [$@]
	@echo

# Pravilo za kompilaciju objektnih fajlova za debagovanje
%_g.o: sources/%.c
	@echo ============= build [$^] =============
	$(CC) $(CDBGFLAGS) $< -c -o $@ -I $(HDIR)

	@echo [OK] --- [$@]
	@echo


## Sve .o datoteka koje se koriste pri stvaranju programa
# Izvuci listu svih izvrsnih kodova i odredi odgovarajuce listu objektnih
# datoteka
SRC = $(wildcard sources/*.c)
OBJ = $(SRC:sources/%.c=%.o)
GOBJ = $(SRC:sources/%.c=%_g.o)

## Konacno linkovanje .o datoteka u izvrsni kod
maraton: $(OBJ)
	@echo ============= compile [$@] =============
	gcc $(CFLAGS) $^ -o maraton $(LINK)

	@echo [OK] --- [$@]
	@echo

maraton-dbg: $(GOBJ)
	@echo ============= compile [$@] =============
	gcc $(CDBGFLAGS) $^ -o maraton-dbg $(LINK)

	@echo [OK] --- [$@]
	@echo

### Pomocne komande
.PHONY: clean sweep run

clean:
	@rm -f *.o *~ *#

sweep:
	@rm -f *.o

run:
	@echo
	@echo ------------------ BUILDING ------------------
	@echo
	@make maraton

# Nece se pokrenuti ako kompilacija nije uspesna
	@echo
	@echo ------------------ RUNNING ------------------
	@echo
	./maraton

### Upotreba komadni
## Komande se pokrecu iz osnovnog direktorijuma projekta:
#$ make clean
## Ocisti direktorijum:
#$ make run
## Kompiliraj, ako je uspesno pokreni
