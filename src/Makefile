CC      = gcc
PROF    =
NOCRYPT =
C_FLAGS = -O -Wall $(PROF) $(NOCRYPT)
L_FLAGS = -O -lcrypt $(PROF)

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o const.o \
          db.o fight.o handler.o interp.o magic.o save.o special.o update.o

merc: $(O_FILES)
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
