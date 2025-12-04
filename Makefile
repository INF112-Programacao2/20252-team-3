CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = a.exe

SOURCES = \
	alimento.cpp \
	diario_alimentar.cpp \
	funcoes_leitura.cpp \
	liquido.cpp \
	macronutriente.cpp \
	micronutriente.cpp \
	nutriente.cpp \
	perfil_nutricional.cpp \
	solido_contavel.cpp \
	solido_nao_contavel.cpp \
	usuario.cpp \
	main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-del /q $(OBJECTS) $(TARGET)

.PHONY: all clean run