# Nome do compilador
CC = gcc

# Nome do executável final
EXECUTABLE = main.exe

# Pastas da biblioteca SDL
SDL_INCLUDE_DIR = "C:/SDL/x86_64-w64-mingw32/include"
SDL_LIB_DIR = "C:/SDL/x86_64-w64-mingw32/lib"

# Flags de compilação (inclui o caminho para os headers)
CFLAGS = -I $(SDL_INCLUDE_DIR)

# Flags de linkagem (inclui o caminho para as bibliotecas e a biblioteca em si)
LDFLAGS = -L $(SDL_LIB_DIR) -lSDL3

# Lista de todos os arquivos fonte (.cpp)
SOURCES = main.cpp pixels.cpp matrix.cpp

# Gera automaticamente a lista de arquivos objeto (.o) a partir dos fontes
OBJECTS = $(SOURCES:.cpp=.o)

# Regra principal: compila tudo
all: $(EXECUTABLE)

# Regra para criar o executável final a partir dos arquivos objeto
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

# Regra para compilar cada arquivo .cpp em um arquivo .o
%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# Regra para limpar os arquivos gerados (executável e objetos)
clean:
	del $(OBJECTS) $(EXECUTABLE)