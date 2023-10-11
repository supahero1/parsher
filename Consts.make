VERSION = 1
PATCH = 3

CLI_VERSION = 1
CLI_PATCH = 0

PROJECT_NAME = parsher
PROJECT_NAME_UP = $(shell echo "$(PROJECT_NAME)" | tr "[:lower:]" "[:upper:]")

SHELL = bash

FMT_OD :=@printf "$(shell tput bold)
FMT_DO :=$(shell tput sgr0)\n"

# DIR
DIR_TOP     := $(shell pwd)
DIR_OUT     ?= bin
DIR_INCLUDE ?= /usr/local/include
DIR_LIB     ?= /usr/local/lib
DIR_BIN     ?= /usr/local/bin

# BUILD_TYPE
BUILD_O   = 1
BUILD_LIB = 2
BUILD_EXE = 3

ifneq ($(VERBOSE),1)
Q = @
COVFLAGS = -q
endif

CFLAGS  = -Wall -Wextra -flto -O3

ifeq ($(DEBUG),1)
CFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -g3
endif

ifeq ($(GPROF),1)
CFLAGS += -pg
endif

ifeq ($(COVERAGE),1)
CFLAGS += --coverage
endif

ifeq ($(OPT),1)
CFLAGS += -march=native
endif

CLIBS = -lcmph
