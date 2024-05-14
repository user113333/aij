SRC:=main.c

build:
	gcc $(SRC) raylib/lib/libraylib.a -Iraylib/include -lgdi32 -lwinmm -o aij.exe

clean:
	rm aij.exe
