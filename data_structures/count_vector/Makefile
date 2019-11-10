#project name
PROJECT=count_vector_test

#compiler options
CP=gcc
FLAGS=-c

#source files
C_SRC=$(wildcard *.c)
H_SRC=$(wildcard *.h)
OBJ=$(C_SRC:.c=.o)

#Make targets
all: $(OBJ)
	@$(CP) -o $(PROJECT) $^
run:
	@$(addprefix ./,$(PROJECT))
%.o: %.c %.h
	@$(CP) -o $@ $< $(FLAGS)
$(addsuffix .o,$(PROJECT)): $(addsuffix .c,$(PROJECT)) $(H_SRC)
	@$(CP) -o $@ $< $(FLAGS)
clear:
	@rm -rf *.o $(PROJECT) *~