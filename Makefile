SRC:=src/main.c src/buffer.c

.PHONY: all build clean
all:
	echo nothing

build:
	gcc $(SRC) raylib/lib/libraylib.a -Iraylib/include -lgdi32 -lwinmm -W -g -o aij.exe

clean:
	rm aij.exe
