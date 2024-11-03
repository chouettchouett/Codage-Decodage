EXEC = projet_avance_automne_2024
EXEC_PART1 = crypto_sym
EXEC_PART2 = dh_gen_group
EXEC_PART3 = break_code

CC = gcc
CFLAGS = -std=c99 -Wextra -Wall -pedantic
LDFLAGS = -lm

SRC = $(wildcard src/*.c) $(SRC_PART1) $(SRC_PART2) $(SRC_PART3)
SRC_PART1 = $(wildcard src/Partie1/*.c)
SRC_PART2 = $(wildcard src/Partie2/*.c)
SRC_PART3 = $(wildcard src/Partie3/*.c)

OBJ= $(SRC:src/%.c=obj/%.o)
OBJ_PART1 = $(SRC_PART1:src/Partie1/%.c=obj/Partie1/%.o)
OBJ_PART2 = $(SRC_PART2:src/Partie2/%.c=obj/Partie2/%.o)
OBJ_PART3 = $(SRC_PART3:src/Partie3/%.c=obj/Partie3/%.o)


all: $(EXEC)
part1: $(EXEC_PART1)
part2: $(EXEC_PART2)
part3: $(EXEC_PART3)


obj/%.o: src/%.c
	@mkdir -p obj obj/Partie1 obj/Partie2 obj/Partie3
	@$(CC) -o $@ -c $< $(CFLAGS)

obj/Partie1%.o: src/Partie1/%.c
	@mkdir -p obj/Partie1
	@$(CC) -o $@ -c $< $(CFLAGS)

obj/Partie2%.o: src/Partie2/%.c
	@mkdir -p obj/Partie2
	@$(CC) -o $@ -c $< $(CFLAGS)

obj/Partie3%.o: src/Partie3/%.c
	@mkdir -p obj/Partie3
	@$(CC) -o $@ -c $< $(CFLAGS)


$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_PART1): $(OBJ_PART1)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_PART2): $(OBJ_PART2)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_PART3): $(OBJ_PART3)
	@$(CC) -o $@ $^ $(LDFLAGS)


.PHONY: clean

clean:
	@rm -rf obj/ $(EXEC) $(EXEC_PART1) $(EXEC_PART2) $(EXEC_PART3)