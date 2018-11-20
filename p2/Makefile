# Francesca Tyler, NetID: ftyler, CS login: francesca
# Bennett Bremer, NetID: bbremer, CS login: bbremer

CC = gcc

prodcomm: main.o queue.o reader.o munch1.o munch2.o writer.o
	gcc -Wextra -Wall -pthread -g -o prodcomm main.o queue.o reader.o munch1.o munch2.o writer.o

main.o: main.c
	$(CC) -Wextra -Wall -pthread -g -c main.c

reader.o: reader.c
	$(CC) -Wextra -Wall -pthread -g -c reader.c

munch1.o: munch1.c
	$(CC) -Wextra -Wall -pthread -g -c munch1.c

munch2.o: munch2.c
	$(CC) -Wextra -Wall -pthread -g -c munch2.c

writer.o: writer.c
	$(CC) -Wextra -Wall -pthread -g -c writer.c

queue.o: queue.c
	$(CC) -Wextra -Wall -g -c queue.c

clean:
	rm -f *.o
	rm -rf scan-build-out

scan-build: clean
	scan-build -o scan-build-out make

scan-view: scan-build
	firefox -new-window scan-build-out/*/index.html &
