CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I lib/cadmium/include

all: build/main.o build/main_receiver.o build/main_sender.o build/main_subnet.o build/message.o
	$(CC) -g -o bin/ABP build/main.o build/message.o
	$(CC) -g -o bin/RECEIVER_TEST build/main_receiver.o build/message.o
	$(CC) -g -o bin/SENDER_TEST build/main_sender.o build/message.o
	$(CC) -g -o bin/SUBNET_TEST build/main_subnet.o build/message.o
build/main.o: src/top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/top_model/main.cpp -o build/main.o

build/main_receiver.o: test/src/main_receiver.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/main_receiver.cpp -o build/main_receiver.o
	
build/main_sender.o: test/src/main_sender.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/main_sender.cpp -o build/main_sender.o

build/main_subnet.o: test/src/main_subnet.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/main_subnet.cpp -o build/main_subnet.o

build/message.o: 
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/data_structures/message.cpp -o build/message.o

clean:
	rm -f bin/*.exe build/*.o *~
	-for d in src/data_structures; do (cd $$d; rm -f *.o *~ ); done 