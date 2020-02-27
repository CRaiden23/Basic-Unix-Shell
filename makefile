# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif
# the above code was found on stackoverflow to make this cool, please don't strike us for plagiarism

newshell :	shell.o g5shell_history.o g5shell_pipe.o g5shell_exit.o g5shell_cd.o g5shell_signal.o g5shell_repiping.o redirection.o
	gcc shell.o g5shell_history.o g5shell_pipe.o g5shell_exit.o g5shell_cd.o g5shell_signal.o g5shell_repiping.o redirection.o -o newshell -Wall -ggdb

shell.o : shell.c shell.h
	gcc -c shell.c

g5shell_history.o : g5shell_history.c shell.h
	gcc -c g5shell_history.c

g5shell_pipe.o : g5shell_pipe.c shell.h
	gcc -c g5shell_pipe.c

g5shell_exit.o : g5shell_exit.c shell.h
	gcc -c g5shell_exit.c

g5shell_cd.o : g5shell_cd.c shell.h
	gcc -c g5shell_cd.c

g5shell_signal.o : g5shell_signal.c shell.h
	gcc -c g5shell_signal.c
	
g5shell_repiping.o : g5shell_repiping.c shell.h
	gcc -c g5shell_repiping.c

redirection.o : redirection.c shell.h
	gcc -c redirection.c

clean :
	rm *.o newshell

run :
	./newshell $(RUN_ARGS)
