# Makefile

CXX = g++
CXXFLAGS = -fPIC -std=c++11 -I./include
LDFLAGS = -lcurl -L./lib

SRCDIR = src
BUILDDIR = build
TARGET = compare_packages
LIB = libpackage_fetcher.so

SRCFILES = $(SRCDIR)/package_fetcher.cpp $(SRCDIR)/main.cpp
OBJFILES = $(addprefix $(BUILDDIR)/,$(notdir $(SRCFILES:.cpp=.o)))

all: $(LIB) $(TARGET)

$(LIB): $(BUILDDIR)/package_fetcher.o
	$(CXX) -shared -o lib/$(LIB) $^ $(LDFLAGS)

$(TARGET): $(OBJFILES)
	$(CXX) -o $(TARGET) $^ -L./lib -lpackage_fetcher $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET) $(LIB)
	install -d /usr/local/bin /usr/local/lib /usr/local/include
	install $(TARGET) /usr/local/bin/
	install lib/$(LIB) /usr/local/lib/
	install include/package_fetcher.h /usr/local/include/
	ldconfig

clean:
	rm -rf $(BUILDDIR) $(TARGET) lib/$(LIB)

.PHONY: all clean install