CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../cadmium/include

all: main.o main_receiver.o main_sender.o main_subnet.o data_structures/message.o
	$(CC) -g -o ABP main.o data_structures/message.o
	$(CC) -g -o RECEIVER_TEST main_receiver.o data_structures/message.o
	$(CC) -g -o SENDER_TEST main_sender.o data_structures/message.o
	$(CC) -g -o SUBNET_TEST main_subnet.o data_structures/message.o
main.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) top_model/main.cpp -o main.o

main_receiver.o: test/receiver/main_receiver.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/receiver/main_receiver.cpp -o main_receiver.o
	
main_sender.o: test/sender/main_sender.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/sender/main_sender.cpp -o main_sender.o

main_subnet.o: test/subnet/main_subnet.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/subnet/main_subnet.cpp -o main_subnet.o

data_structures/message.o: 
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) data_structures/message.cpp -o data_structures/message.o

clean:
	rm -f ABP *.o *~
	-for d in data_structures; do (cd $$d; rm -f *.o *~ ); done
