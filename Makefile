EXEC = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -g
PROJECT_NAME = ted

clear:
	rm *.o
	rm $(PROJECT_NAME)

$(PROJECT_NAME): main.o forms.o list.o paths.o qry.o SRbTree.o svg.o system.o
	$(EXEC) $(CFLAGS) main.o forms.o list.o paths.o qry.o SRbTree.o svg.o system.o -o $(PROJECT_NAME) -lm

main.o: main.c 
	$(EXEC) $(CFLAGS) -c main.c

forms.o: forms.c forms.h
	$(EXEC) $(CFLAGS) -c forms.c

list.o: list.c list.h
	$(EXEC) $(CFLAGS) -c list.c

paths.o: paths.c paths.h
	$(EXEC) $(CFLAGS) -c paths.c

qry.o: qry.c qry.h
	$(EXEC) $(CFLAGS) -c qry.c

SRbTree.o: SRbTree.c SRbTree.h
	$(EXEC) $(CFLAGS) -c SRbTree.c

svg.o: svg.c svg.h
	$(EXEC) $(CFLAGS) -c svg.c

system.o: system.c system.h
	$(EXEC) $(CFLAGS) -c system.c
