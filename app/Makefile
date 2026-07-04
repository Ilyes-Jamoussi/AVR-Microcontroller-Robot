########       AVR Project Makefile       ########
#####                                        #####
#####      Makefile produit et écrit par     #####
#####   Simon Barrette & Jerome Collin pour  #####
#####           INF1900 - 2016               #####
#####                                        #####
#####         Inspiré de Pat Deegan -        #####
#####  Psychogenic Inc (www.psychogenic.com) #####
##################################################

# Ce Makefile vous permet de compiler des projets
# pour les microcontrôleurs Atmel AVR sur 
# Linux ou Unix, en utilisant l'outil AVR-GCC. 
# Ce Makefile supporte C & C++

#####      Détails spécifiques à la cible      #####
#####  Vous devez les adapter à votre projet   #####

# Nom du microcontrôleur cible
# (exemple: 'at90s8515')
MCU = atmega324pa

# Nom de votre projet
# (utilisez un seul mot, exemple: 'monprojet')
PROJECTNAME = robot

# Fichiers sources
# Utilisez le suffixe .cpp pour les fichiers C++
# Listez tous les fichiers à compiler, séparés par
# un espace. Exemple : 'tp1.c tp2.cpp':
PRJSRC = $(wildcard *.cpp)

# Inclusions additionnels (ex: -I/path/to/mydir)
INC=$(shell find ../lib -type d -exec echo -I{} \;)

# Bibliothèques à lier (ex: -lmylib)
LIBS = -L../lib -lrobot

# Niveau d'optimisation
# Utilisez s (size opt), 1, 2, 3 ou 0 (off)
OPTLEVEL = s

# Programmer ID - Ne pas changer 
# Liste complète des IDs disponibles avec avrdude
AVRDUDE_PROGRAMMERID = usbasp

####################################################
#####         Configuration terminée           #####
#####                                          #####
#####  Le reste de cette section contient les  #####
##### détails d'implémentation vous permettant #####
##### de mieux comprendre le fonctionnement de ##### 
#####   ce Makefile en vue de sa modification  #####
####################################################

####### Variables #######

# Compilateur utilisé
CXX = avr-g++
# Utilisé pour copier le contenu d'un fichier objet vers un autre
OBJCOPY = avr-objcopy
# Utilisé pour le transfert vers le microcontrôleur
AVRDUDE = avrdude
# Utilisé pour supprimer les fichiers lorsqu'on appelle make clean
REMOVE = rm -f
# Format HEX - format des fichiers produits .hex
HEXFORMAT = ihex

# Définir la fréquence du CPU comme une variable
F_CPU = 8000000UL

# Flags pour le compilateur en C
CFLAGS=-I. -I/usr/include/simavr  -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -DF_CPU=$(F_CPU)

CXXFLAGS = -fno-exceptions
LDFLAGS = -Wl,-Map,$(TRG).map -mmcu=$(MCU)

TRG = $(PROJECTNAME).elf
HEXROMTRG = $(PROJECTNAME).hex
HEXTRG = $(HEXROMTRG) $(PROJECTNAME).ee.hex

CFILES = $(filter %.c, $(PRJSRC))
CPPFILES = $(filter %.cpp, $(PRJSRC))

OBJDEPS = $(CFILES:.c=.o) \
	$(CPPFILES:.cpp=.o)

.PHONY: all install clean 

all: $(TRG) $(HEXROMTRG)

debug: CXXFLAGS += -DDEBUG -g
debug: CFLAGS += -DDEBUG -g
debug: clean all

$(TRG): $(OBJDEPS)
	$(CXX) $(LDFLAGS) -o $(TRG) $(OBJDEPS) $(LIBS) -L$(INC)

%.o: %.c
	$(CXX) $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $<

-include *.d

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data \
		-O $(HEXFORMAT) $< $@

install: $(HEXROMTRG)
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID) -p $(MCU) -P usb -B 125 -e -U flash:w:$(HEXROMTRG)

clean:
	$(REMOVE) $(TRG) $(TRG).map $(OBJDEPS) $(HEXTRG) *.d
