CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I lib/cadmium/include

#CREATE BIN AND BUILD FOLDERS IF ALREADY NOT PRESENT
bin_folder := $(shell mkdir -p bin/top_model bin/test)
build_folder := $(shell mkdir -p build/top_model build/test)

# TO RUN ALL FOUR COMPONENTS (SIMULATOR, SENDER, RECEIVER, SUBNET)
all: simulation test

# TO RUN SIMULATOR
simulation: build/top_model/main.o build/message.o build/transform_output.o
	$(CC) -g -o bin/top_model/ABP build/top_model/main.o build/message.o build/transform_output.o

# TO RUN SENDER, RECEIVER, SUBNET 
test: build/test/main_sender.o build/test/main_receiver.o build/test/main_subnet.o build/message.o build/transform_output.o
	$(CC) -g -o bin/test/RECEIVER_TEST build/test/main_receiver.o build/message.o build/transform_output.o
	$(CC) -g -o bin/test/SENDER_TEST build/test/main_sender.o build/message.o build/transform_output.o
	$(CC) -g -o bin/test/SUBNET_TEST build/test/main_subnet.o build/message.o build/transform_output.o

build/top_model/main.o: src/top_model/main.cpp 
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/top_model/main.cpp -o build/top_model/main.o	

build/test/main_sender.o: test/src/main_sender.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/main_sender.cpp -o build/test/main_sender.o

build/test/main_receiver.o: test/src/main_receiver.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/main_receiver.cpp -o build/test/main_receiver.o

build/test/main_subnet.o: test/src/main_subnet.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/main_subnet.cpp -o build/test/main_subnet.o

build/message.o: src/data_structures/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/data_structures/message.cpp -o build/message.o

build/transform_output.o: src/transform_output/transform_output.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/transform_output/transform_output.cpp -o build/transform_output.o

# TO CLEAN ALL
clean: clean_simulation clean_test

# TO CLEAN SIMULATION ONLY
clean_simulation:
	rm -f bin/top_model/*.exe  build/top_model/*.o build/message.o *~ 

# TO CLEAN TEST ONLY
clean_test:
	rm -f bin/test/*.exe build/test/*.o build/message.o *~ 