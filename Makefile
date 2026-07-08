# Compilar: $ make
# Limpar: $ make clean
# Comando executado: gcc -o jogo  jogo.c util.c -lallegro -lallegro_main -lallegro_color -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -lm

CXX = gcc
CXXFLAGS = -Wall 
LDFLAGS = -lallegro -lallegro_main \
    -lallegro_color -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -lm

all: jogo

jogo: jogo.c util.c
	$(CXX) $(CXXFLAGS) -o $@  $^ $(LDFLAGS)

clean:
	rm -f jogo *.o

run:
	./jogo
