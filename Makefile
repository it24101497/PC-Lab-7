CC = mpicc
RUN = mpirun --oversubscribe -np 4

all: ex1 ex2 ex3 ex4 ex5 ex6

ex1: Exercise1/exercise1.c
	$(CC) Exercise1/exercise1.c -o Exercise1/exercise1

ex2: Exercise2/exercise2.c
	$(CC) Exercise2/exercise2.c -o Exercise2/exercise2

ex3: Exercise3/exercise3.c
	$(CC) Exercise3/exercise3.c -o Exercise3/exercise3

ex4: Exercise4/exercise4.c
	$(CC) Exercise4/exercise4.c -o Exercise4/exercise4

ex5: Exercise5/exercise5.c
	$(CC) Exercise5/exercise5.c -o Exercise5/exercise5

ex6: Exercise6/exercise6.c
	$(CC) Exercise6/exercise6.c -o Exercise6/exercise6

run: all
	@echo "\n=== Running Exercise 1 ==="
	$(RUN) ./Exercise1/exercise1
	@echo "\n=== Running Exercise 2 ==="
	$(RUN) ./Exercise2/exercise2
	@echo "\n=== Running Exercise 3 ==="
	$(RUN) ./Exercise3/exercise3
	@echo "\n=== Running Exercise 4 ==="
	$(RUN) ./Exercise4/exercise4
	@echo "\n=== Running Exercise 5 ==="
	$(RUN) ./Exercise5/exercise5
	@echo "\n=== Running Exercise 6 ==="
	$(RUN) ./Exercise6/exercise6

clean:
	rm -f Exercise1/exercise1 Exercise2/exercise2 Exercise3/exercise3 Exercise4/exercise4 Exercise5/exercise5 Exercise6/exercise6
