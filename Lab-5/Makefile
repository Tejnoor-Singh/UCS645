CC = mpicc
CFLAGS = -O2

all: q1 q2 q3 q4 q5

q1:
	$(CC) $(CFLAGS) -o q1 q1_daxpy.c

q2:
	$(CC) $(CFLAGS) -o q2 q2_broadcast.c

q3:
	$(CC) $(CFLAGS) -o q3 q3_dotproduct.c

q4:
	$(CC) $(CFLAGS) -o q4 q4_primes.c -lm

q5:
	$(CC) $(CFLAGS) -o q5 q5_perfect.c

clean:
	rm -f q1 q2 q3 q4 q5
