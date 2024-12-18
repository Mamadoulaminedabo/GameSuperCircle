CC = gcc

# Utiliser pkg-config pour obtenir les flags d'inclusion et de liaison
CFLAGS = -Wall $(shell pkg-config --cflags sdl2 SDL2_mixer SDL2_ttf SDL2_image) -I./libs
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_mixer SDL2_ttf SDL2_image)

# Liste des fichiers sources et objets
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

# Nom de l'exécutable
EXEC = circles

# Règle par défaut
all: $(EXEC)

# Lier les objets pour créer l'exécutable
$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

# Compiler les fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour nettoyer les fichiers objets et l'exécutable
.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
CC = gcc
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS = -Wall -Wextra `sdl2-config --cflags`
LDFLAGS = -lSDL2_image `sdl2-config --libs`
EXEC = circles

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
