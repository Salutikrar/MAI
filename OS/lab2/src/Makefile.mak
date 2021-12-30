all: child.out main

child:
        gcc child.c -o child.out

var13:
        gcc main.c -o main

clean:
        rm -rf main child.out
