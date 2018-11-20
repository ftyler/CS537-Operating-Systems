537ps: main.o options_processing.o get_proc_list.o parser.o
	gcc -Wall -Wextra -o 537ps main.o options_processing.o get_proc_list.o parser.o
	rm *.o

main.o: main.c
	gcc -Wall -Wextra -c main.c

options_processing.o: options_processing.c check_numeric.o
	gcc -Wall -Wextra -c options_processing.c

get_proc_list.o: get_proc_list.c check_numeric.o
	gcc -Wall -Wextra -c get_proc_list.c

parser.o: parser.c
	gcc -Wall -Wextra -c parser.c

check_numeric.o: check_numeric.c
	gcc -Wall -Wextra -c check_numeric.c

clean:
	rm *.o
