# makefile for the A start project
# Joseph Merheb
# EPITA 2020

OBJ = list.c status.c city.c main.c
astar : $(OBJ)
	cc -o astar $(OBJ) -lm
list.o : list.c list.h
	cc -c list.c
status.o : status.c status.h
	cc -c status.c
city.o : city.c city.h
	cc -c city.c
main.o : main.c
	cc -c main.c
