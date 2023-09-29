TGZ_NAME := nom1_prenom1-nom2_prenom2
# sans accent, sans espace…

STEP = 2

MODULES := linker loader utils types

OBJECTS := $(MODULES:%=%.o)

TARGET := ./main

C_FLAGS := -Wall -std=gnu11 -Wextra -pedantic -ggdb
## gnu11 is C11 + extensions including and getline


.PHONY : tgz vg


%.o : %.h %.c makefile
	gcc $(C_FLAGS) -o $@ -c $*.c


$(TARGET) : $(OBJECTS) $(TARGET).c $(wildcard *.h) makefile
	gcc $(C_FLAGS) -o $@ $(OBJECTS) $(TARGET).c 


linker.o : utils.h types.h

loader.o : utils.h types.h
utils.o : types.h

MODULES := module0.modu module1.modu

vg : $(TARGET)
	valgrind --leak-check=full --track-origins=yes $(TARGET) $(STEP) $(MODULES) >/dev/null

run : $(TARGET)
	$(TARGET) $(STEP) $(MODULES)

nmv : $(TARGET)
	nemiver $(TARGET) $(STEP) $(MODULES)

test : $(TARGET)
	$(TARGET) $(STEP) $(MODULES) | diff - output_$(STEP).txt
# on peut aussi utiliser meld pour comparer 2 fichier (avec une interface graphique)


FICHIERS_A_RENDRE := linker.c loader.c

tgz : $(TGZ_NAME).tgz

$(TGZ_NAME).tgz : $(FICHIERS_A_RENDRE)
	tar czf $(TGZ_NAME).tgz $(FICHIERS_A_RENDRE)

clean :
	rm -f %.o main
