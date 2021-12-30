done: prog1 prog2

lib1.so: lib1.c
	gcc -shared lib1.c -o lib1.so -Wall

lib2.so: lib2.c
	gcc -shared lib2.c -o lib2.so -Wall

prog2: lib1.so lib2.so prog2.c
	gcc prog2.c -ldl -o prog2 -Wall

prog1: lib1.so prog1.c
	gcc prog1.c -L"/home/salutik/un/oc/lab5" -Wl,-R. -l1 -l1 -o prog1 -Wall
