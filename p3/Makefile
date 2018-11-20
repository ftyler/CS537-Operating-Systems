# Francesca Tyler
# Bennett Bremer

537make: main.o parse_txt.o build_graph.o create_execute.o detect_cycle.o
	gcc -Wextra -Wall -pthread -g -o 537make main.o parse_txt.o build_graph.o create_execute.o detect_cycle.o

main.o: main.c parse_txt.o build_graph.o create_execute.o detect_cycle.o
	gcc -Wextra -Wall -pthread -g -c main.c

create_execute.o: create_execute.c parse_txt.o build_graph.o
	gcc -Wextra -Wall -pthread -g -c create_execute.c

detect_cycle.o: detect_cycle.c parse_txt.o build_graph.o
	gcc -Wextra -Wall -pthread -g -c detect_cycle.c

build_graph.o: build_graph.c parse_txt.o
	gcc -Wextra -Wall -pthread -g -c build_graph.c

parse_txt.o: parse_txt.c
	gcc -Wextra -Wall -pthread -g -c parse_txt.c

clean:
	rm -f *.o
	rm -rf scan-build-out
