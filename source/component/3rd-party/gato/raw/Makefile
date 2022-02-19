sinclude scripts/env.mk

MAIN_SRC	:= examples/graph/ui.c
ifeq ("$(origin MAIN)", "command line")
MAIN_SRC	:= $(MAIN)
endif

X_CFLAGS	+= -std=gnu11 -O3 -g
X_ASFLAGS	+= -O3 -g
X_LIBS		+= m pthread
# X_DEFINES	+= PROFILE
# X_LDFLAGS	+= -lprofiler
X_LDFLAGS	+= `pkg-config sdl2 --libs --cflags`
X_INCDIRS	+= include

SRC			+= $(MAIN_SRC) render/*.c image/image.c
NAME		:= Gato