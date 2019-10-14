#####################################################################
# ECEn 425 Lab 4d Makefile

lab4d.bin:	lab4dfinal.s
		nasm lab4dfinal.s -o lab4d.bin -l lab4d.lst

lab4dfinal.s:	clib.s myisr.s myinth.s yaks.s lab4d_app.s yakc.s
		cat clib.s myisr.s myinth.s yakc.s yaks.s lab4d_app.s > lab4dfinal.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 myinth.i myinth.s

lab4d_app.s:	lab4d_app.c yakk.h
		cpp lab4d_app.c lab4d_app.i
		c86 lab4d_app.i lab4d_app.s

yakc.s:		yakc.c yakk.h
		cpp yakc.c yakc.i
		c86 yakc.i yakc.s

clean:
		rm lab4d.bin lab4d.lst lab4dfinal.s myinth.s myinth.i \
		yakc.s yakc.i lab4d_app.s lab4d_app.i
