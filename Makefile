# Makefile

CXX = g++
CXXFLAGS = -fPIC -std=c++17 -I./include
LDFLAGS = -lcurl -L./lib

SRCDIR = src
BUILDDIR = build
TARGET = compare_packages
LIB = libpackage_fetcher.so

# Исходные файлы
SRCFILES = $(SRCDIR)/package_fetcher.cpp $(SRCDIR)/main.cpp $(SRCDIR)/compare_versions.cpp
OBJFILES = $(addprefix $(BUILDDIR)/,$(notdir $(SRCFILES:.cpp=.o)))

all: $(LIB) $(TARGET)

# Правило для сборки динамической библиотеки package_fetcher
$(LIB): $(BUILDDIR)/package_fetcher.o
	$(CXX) -shared -o lib/$(LIB) $^ $(LDFLAGS)

# Правило для сборки основного исполняемого файла
$(TARGET): $(OBJFILES)
	$(CXX) -o $@ $^ -L./lib -lpackage_fetcher $(LDFLAGS)

# Правило для компиляции объектных файлов
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Установка файлов в систему
install: $(TARGET) $(LIB)
	install -d /usr/local/bin /usr/local/lib /usr/local/include
	install $(TARGET) /usr/local/bin/
	install lib/$(LIB) /usr/local/lib/
	install include/package_fetcher.h /usr/local/include/
	install include/compare_versions.h /usr/local/include/ # установка заголовка compare_versions
	ldconfig

# Очистка сгенерированных файлов
clean:
	rm -rf $(BUILDDIR) $(TARGET) lib/$(LIB)

.PHONY: all clean install
