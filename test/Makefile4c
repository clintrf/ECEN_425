#####################################################################
# ECEn 425 Lab 4c Makefile

lab4c.bin:	lab4cfinal.s
		nasm lab4cfinal.s -o lab4c.bin -l lab4c.lst

lab4cfinal.s:	clib.s myisr.s myinth.s yaks.s lab4c_app.s yakc.s
		cat clib.s myisr.s myinth.s yakc.s yaks.s lab4c_app.s > lab4cfinal.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 myinth.i myinth.s

lab4c_app.s:	lab4c_app.c yakk.h
		cpp lab4c_app.c lab4c_app.i
		c86 lab4c_app.i lab4c_app.s

yakc.s:		yakc.c yakk.h
		cpp yakc.c yakc.i
		c86 yakc.i yakc.s

clean:
		rm lab4c.bin lab4c.lst lab4cfinal.s myinth.s myinth.i \
		yakc.s yakc.i lab4c_app.s lab4c_app.i
