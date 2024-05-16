SRC:=src/main.c src/buffer.c

.PHONY: build
all:
	echo nothing

.PHONY: build
build:
	gcc $(SRC) raylib/lib/libraylib.a -Iraylib/include -lgdi32 -lwinmm -g -o aij.exe

.PHONY: build
clean:
	rm aij.exe
