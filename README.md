# ALTERNATE BIT PROTOCOL SIMULATOR IMPLEMENTED IN CADMIUM
---
## ORGANIZATION : CARLETON UNIVERSITY
---
## AUTHORS :
### ORIGINALLY DEVELOPED BY : Cristina Ruiz Martin
### MODIFIED BY : Syed Omar Azam
###               Mohamed Ali
###               Ammar Al-Mahbashi                  
---
## INTRODUCTION:
---
**ABP** (Alternating Bit Protocol) is a communication protocol to ensure reliable transmission through unreliable network. The sender sends a packet and waits for an acknowledgement. If the acknowledgement doesn't arrive within a predefined time, the sender re-sends this packet until it receives an expected acknowledgement and then sends the next packet. In order to distinguish two consecutive packets, the sender adds an additional bit on each packet (called alternating bit because the sender uses 0 and 1 alternatively). This software is a simulator to replicate the behavior of the Alternating Bit Protocol. 

The ABP Simulator consists of 3 components: sender, network and receiver. The network is decomposed further to two subnets corresponding to the sending and receiving channel respectively.


The behavior of **receiver** is to receive the data and send back an acknowledgement extracted from the received data after a time period. 

The **subnets** just pass the packets after a time delay. However, in order to simulate the unreliability of the network, only 95% of the packets will be passed in each of the subnet, i.e. 5% of the data will be lost through the subnet.

 The **receiver** and **subnets** have two phases: passive and active. They are in passive phase initially. Whenever they receive a packet, they will be in active phase, and send out the acknowledgment (receiver) or the packet with a probability of 95% (subnet) after a time duration. Once the acknowledgement or packet are sent, they go back to the passive phase.

The **sender** behavior is complex. The sender changes from initial phase passive to active when an external signal is received. Once activated it start sending the packet with the alternating bit. Every time a packet is sent, it waits for an acknowledgement during a specific waiting time. It the acknowledgment does not arrive within the time window; the sender will re-send the previous packet with the alternating bit. If the expected acknowledgement is received within the time window, the sender will send the next packet. When there are no more packets to send, the sender will go again to the passive phase.

---

This folder contains a simulator for an ALTERNATE BIT PROTOCOL implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/

README.txt	

alternatebitprotocol.doc

bin [This folder contains all .exe files generated for the project]
	test [This folder contains all .exe files generated for testing only]
		RECEIVER_TEST.exe
		SENDER_TEST.exe
		SUBNET_TEST.exe
	top_model [This folder contains all .exe files generated for simulator only]
		ABP.exe
build [This folder contains all .o files generated for the project]
	top_model [This folder contains all .o files generated for simulator only]
		main.o
	test [This folder contains all .o files generated for testing only]
		main_receiver.o
		main_sender.o
		main_subnet.o
	message.o
	
data [This folder contains some data for testing the project and some simulator outputs]
	top_model [This folder contains some data for testing and some simulator outputs]
		input_abp_0.txt
		input_abp_1.txt
		abp_output_0.txt
		abp_output_1.txt

doc [This folder contains documentaion files for the project]
	alternatebitprot.pdf
	Cadmium_Documentation_Ubuntu.pdf
	Cadmium_Windows.pdf

include [This folder contains the header files used in the project]
	atomics [This folder contains the header files]
		receiver_cadmium.hpp
		sender_cadmium.hpp
		subnet_cadmium.hpp
	data_structures [This folder contains the header file for data structures used in the project]
		message.hpp

lib [This folder contains the 3rd party files header files and functionalties used in the project]
	cadmium [This folder contains cadmium functionalities]
		see folder contents for more information
	DESTimes [This folder contains DESTimes functionalities]
		see folder contents for more information
	vendor [This folder contains 3rd party header files needed in the project]
		iestream.hpp
		NDTime.hpp

src [This folder contains source code of the project]
	data_structures [This folder contains the data structures used in the project]
		message.cpp
	top_model [This folder contains source code for the Alternate Bit Protocol simulator]	
		main.cpp

test [This folder the unit test for the different include files]
	src [This folder contains the source code of the receiver, sender and subnet]
		main_receiver.cpp
		main_sender.cpp
		main_subnet.cpp
	data [This folder contains some data for performin unit test of the receiver, sender and subnet]
		receiver_input_test.txt
		receiver_test_output.txt
		sender_input_test_ack_In.txt
		sender_input_test_control_In.txt
		sender_test_output.txt
		subnet_input_test.txt
		subnet_test_output.txt
	include [This folder contains the header files used for unit testing]
		will be updated in future

	makefile
	
/*************/
/****STEPS****/
/*************/

0 - alternatebitprotocol.doc contains the explanation of this simulator

1 - Update include paths in all the makefiles in this folder and subfolders. You need to update the following lines:
		INCLUDECADMIUM=-I ../../cadmium/include
    Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
		Example: INCLUDECADMIUM=-I ../../cadmium/include
			
2 - Run the simulator and unit tests
	1 - Open the terminal. Press in your keyboard Ctrl+Alt+t
	2 - Set the command prompt in the AlternateBitProtocolSimulator folder. To do so, type in the terminal the path to this folder.
		Example: cd ../../AlternateBitProtocolSimulator
	3 - To compile the whole project, type in the terminal:
		make clean; make all
	4 - To run the simulation, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this test you need to type:
		./bin/top_model/ABP data/top_model/input_abp_1.txt
	5 - To check the output of the simulation, open  "abp_output.txt" present in data/top_model folder.
	6 - To execute the simulator with different inputs
		6.1. Create new .txt files with the same structure as input_abp_0.txt or input_abp_1.txt
		6.2. Run the simulator using the instructions in step 4
		6.3. If you want to keep the output, rename abp_output.txt. To do so, type in the terminal: "mv abp_output.txt NEW_NAME"
			Example: mv data/top_model/abp_output.txt data/top_model/abp_output_0.txt
	7 - To run the subnet test, type in the terminal "./NAME_OF_THE_COMPILED_FILE". For this specific test you need to type:
		/bin/test/SUBNET_TEST
		7.1. To check the output of the test, open  "test/data/subnet_test_output.txt"
	8 - To run receiver and sender tests, the steps are analogous to 7.
	9 - User also have option to compile simulator or testing part alone by using the following commands.
		make clean_simulation; make simulation
		make clean_test; make test
	10 -To run simulation, refer to steps 4 to 6.
	11 -To run tests, refer to steps 7 to 8.

