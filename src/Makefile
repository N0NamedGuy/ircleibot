# ircleibot
SRCS = agenda.c cli.c greeter.c strutils.c botcmd.c globals.c events.c shell.c votings.c logger.c main.c			# ficheiros .c
HDRS = agenda.h cli.h greeter.h strutils.h botcmd.h globals.h events.h shell.h votings.h logger.h				# ficheiros .h
PROG = ircleibot			# executavel
CC = gcc
#
# Daqui para a frente nao deve necessitar de alterar nada

ALLOBJS = $(SRCS:.c=.o) $(OBJS)

# Opcoes de compilacao
# (as que sao aqui sugeridas ajudam quem nao tem pratica no C)
CFLAGS += -g -W -Wall -Wshadow
LIBS += -lircclient -lpthread -lreadline

# se queremos tudo: make all

all:	$(PROG)

# primeiro faz o PROG

$(PROG): $(ALLOBJS)
	cc -o $(PROG) $(ALLOBJS) $(LIBS)

# Outros objectivos
clean:
	rm -f $(PROG) $(ALLOBJS) dep.mak

# regra implicita ("todos os makes a sabem ...")
# %.o: %.c
#     cc -c $(CFLAGS) $<

# gerar automaticamente as dependencias entre ficheiros para dep.mak
#  e inclui-las neste Makefile
dep.mak: $(SRCS) $(HDRS) $(SRCS2) $(HDRS2)
	cc -MM $(SRCS) $(SRCS2) > dep.mak

include dep.mak
