ifeq ($(OS),Windows_NT)
	# WINDOWS
    GENERATOR := "Unix Makefiles"
	EXE       := .exe
	OPEN      := start
else
	# NON-WINDOWS
	GENERATOR := "Unix Makefiles"
	EXE       :=
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
		# MACOS
		OPEN := open
    else
		# NON-WINDOWS, NON-MACOS
        OPEN := xdg-open
	endif
endif

EXE_TARGET_NAME  := hello_world
           
HEADER_DIR := include
SOURCE_DIR := src
DOCS_DIR   := docs
BUILD_DIR  := build

CMAKE_DEFINES := -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

SOURCE_FILES := $(shell find $(SOURCE_DIR) -type f \( -iname '*.cpp' -o -iname '*.c' \))
HEADER_FILES := $(shell find $(HEADER_DIR) -type f \( -iname '*.hpp' -o -iname '*.h' \))

EXE_TARGET        := $(BUILD_DIR)/$(EXE_TARGET_NAME)$(EXE)
C_COMMANDS_TARGET := $(BUILD_DIR)/compile_commands.json
DOCS_TARGET       := $(DOCS_DIR)/html/index.html
TODO_TARGET       := $(DOCS_DIR)/html/todo.html

.PHONY: all
.PHONY: generate
.PHONY: build
.PHONY: install
.PHONY: uninstall
.PHONY: docs
.PHONY: docs-read
.PHONY: docs-todo
.PHONY: clang-tidy
.PHONY: clang-format
.PHONY: checks
.PHONY: run
.PHONY: clean
.PHONY: help

all: generate build

generate:
	cmake -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_DEFINES)

build:
	cmake --build $(BUILD_DIR) -- --no-print-directory

install:
	cmake --install $(BUILD_DIR)

uninstall:
	tr -d '\r' < $(BUILD_DIR)/install_manifest.txt | tr '\n' '\0' | xargs -0 rm -vf

docs:
	doxygen

docs-read: $(DOCS_TARGET)
	$(OPEN) $(DOCS_TARGET)

docs-todo: $(TODO_TARGET)
	$(OPEN) $(TODO_TARGET)

clang-tidy: $(C_COMMANDS_TARGET)
	clang-tidy -p $(BUILD_DIR)/ --quiet --warnings-as-errors='*' $(SOURCE_FILES) 

clang-format:
	clang-format --dry-run --Werror $(SOURCE_FILES) $(HEADER_FILES)

checks: clang-tidy clang-format docs

run: $(EXE_TARGET)
	./$(EXE_TARGET)

clean:
	rm -rf $(DOCS_DIR) $(BUILD_DIR)

help:
	@echo "    all"
	@echo "    generate"
	@echo "    build"
	@echo "    docs"
	@echo "    docs-read"
	@echo "    docs-todo"
	@echo "    clang-tidy"
	@echo "    clang-format"
	@echo "    checks"
	@echo "    run"
	@echo "    clean"

$(EXE_TARGET): $(C_COMMANDS_TARGET) $(SOURCE_FILES) $(HEADER_FILES)
	@$(MAKE) build --no-print-directory

$(C_COMMANDS_TARGET):
	@$(MAKE) generate --no-print-directory

$(DOCS_TARGET): $(SOURCE_FILES) $(HEADER_FILES)
	@$(MAKE) docs  --no-print-directory

$(TODO_TARGET): $(SOURCE_FILES) $(HEADER_FILES)
	@$(MAKE) docs  --no-print-directory
