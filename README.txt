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
build [This folde contains all .o files generated for the project]
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
		receiverCadmium.hpp
		senderCadmium.hpp
		subnetCadmium.hpp
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
			./bin/test/SUBNET
		7.1. To check the output of the test, open  "test/data/subnet_test_output.txt"
	8 - To run receiver and sender tests, the steps are analogous to 7
	9 - User also have option to compile simulator or testing part alone by using the following commands.
		make clean_simulation; make simulation
		make clean_test; make test
	10 -To run simulation, refer to steps 4 to 6.
	11 -To run tests, refer to steps 7 to 8.

