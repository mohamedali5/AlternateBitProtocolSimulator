/** \brief Output Transformation source file
 *
 *  Authors      : Syed Omar Azam, Ammar Al-Mahbashi, Mohammed Ali
 *  Date Created : 15-Aug-2019
 *  Version      : 1.0
 *
 *  This function is used to transform the ABP simulator generated output 
 *  to a table structured and readable format. It takes as input the 
 *  file you want to transform and the name of the output file and generates 
 *  the new output file.
 *  
 *  This function can generate formatted output for the following components
 *      1- ABP Simulator
 *      2- Sender
 *      3- Receiver
 *      4- Subnet
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "../../include/transform_output/transform_output.hpp"

using namespace std;

/**
 * This function is used to trim spaces present on the right side of a string.
 * It takes two parameters as below. For each line read from ip_file_name
 * it removes the spaces on the right side so we generate readable output.
 * @param str of type string
 * @param chars of type string
 */
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

void transform_output(const char *ip_file_name, const char *op_file_name) {
	
	/**
	 * Creating a file pointer which will hold input file name
	 */
	FILE *fp;

	/**
	 * temp array is used to store the one line read from input file.
	 */
	char line[512];
	const char *pattern="00:";
	
	/**
	 * string variables to hold imtermediate sub-strings extracted from input file
	 */
	std::string in, time, port, port_value, component;
	
	/**
	 * ofstream creates a new output file if not present or clean the 
	 * existing one before writing data to it.
	 */
	ofstream outputfile(op_file_name);

	/**
	 * setting the column names for our output file.
	 * setw() function is used to give indentation in the output file.
	 */
	outputfile <<setw(7)<<"Time"<<setw(20)<<"Value"<<setw(14) \
			   <<"Port"<<setw(22)<<"Component"<<endl;
	outputfile <<"-------------------------------------------"
				 "---------------------------"<<endl;
	
	//gcc users
	if((fp = fopen(ip_file_name, "r")) == NULL) {
		printf("can't open %s \n",ip_file_name);
	}

	/**
	 * fgets loop iterate through the input file line by line until EOF and 
	 * store the line read in temp[] char array.
	 */
	while(fgets(line, 512, fp) != NULL) {

		/**
		 * The strstr() function searches the given string in the specified main string and 
		 * returns the pointer to the first occurrence of the given string.
		 * temp holds line read from file. str contains a predefined pattern to search through input file.
		 * if loop specifically evaluates that we are searching for only time in input file. If the pattern 'str'
		 * matches a time value in input file then if loop will be executed and we store the  time value in 'time'
		 * string variable. We perform rtrim() on temp to remove any spaces present after string.
		 * else loop will be executed if the line read from input file doesn't contain a time value.
		 * for each time value read by 'if loop', we retain it and read lines until we encounter another time value.
		 * This is done beacause each 'time' value will have port, port value and component  associated with it.
		 */

		if((strstr(line, pattern)) != NULL) {
			time = line;
			time = rtrim(time);
		}else{

			/**
			 * if the line read start with a [], it means we doesn't have any activity performed in that time interval 
			 * so we ignore this line from input file.
			 * otherwise if code block will be executed and it will extract all the ports, port values and component  
			 * associated with time value read previously.
			 */
			if(line[0]=='[' & line[1]!=']'){

				/**
				 * We use strtok() to extract all tokens in the line by using delimiter ']'
				 * The idea behind this is to separate out component and port from each other.
				 * token_1 holds the first part of line and token_2 holds second part of line read from imput file.
				 * e.g : token_1 = [iestream_input_defs<message_t>::out: {5}
				 *       token_2 = generated by model generator_con
				 */
				char* token = strtok(line, "]"); 
  				char* token_1 = token,* token_2 =strtok(NULL, "]");
  				
  				/**
  				 * divide token_2 by delimiter space (" ") to extract component part from it.
  				 * token_6 holds component part at the end of while loop.
  				 */
  				char* token_5 = strtok(token_2, " "), * token_6;
  				
  				/**
			     * Keep printing tokens while one of the 
			     * delimiters present in token_2. 
			     */
			    while (token_5 != NULL) { 
			        component = token_5;
			        token_5 = strtok(NULL, " ");
			    } 
			    
			    /**
			     * divide token_1 by delimiter comma (",") to extract all port and its corresponding values
			     * while loop runs until all ports and its values are obtained
			     * for each token retrieved in while loop, we check it with port constant to check if a port is
			     * present in the token. If yes then we assign the port to string variable "port"
			     */
			    char* token_3 = strtok(token_1, ",");
			    char* token_4;

			    /**
			     * Keep printing tokens while one of the 
			     * delimiters present in token_3.
			     */ 
			    while (token_3 != NULL) { 

			    	if(strstr(token_3,"out")){

						port= "out";
						
					}else if(strstr(token_3,"dataOut")){

						port="dataOut";

					}else if(strstr(token_3,"packetSentOut")){

					 	port="packetSentOut";

					} else if(strstr(token_3,"ackReceivedOut")){

					 	port="ackReceivedOut";
					}
					
					/**
					 * for each token_3, we extract the port value by using strstr() with "{" delimiter.
					 * port value is present in a "{}" brackets.
					 */
					
					token_4 = strstr(token_3,"{");
					
					/**
				 	 * if ports value is empty then we ignore such ports and continue. This is verifed if the length of
					 * token_4 is greater than 2.
					 * if a value is obtained other than empty then we assign it to port_value variable.
					 */
		            if (strlen(token_4)>2){
						
						in = token_4;
						port_value = in.substr( 1 , ( in.find_first_of('}', 0) - 1) );
	        			
	        			/**
	        			 *Getting our data and writing it to output file.
	        			 * if time is not zero then we write the line to output file.
	        			 */ 
	        			if (outputfile.is_open()){

	        				outputfile<<time<<setw(15)<<port_value<<setw(20) \
	        						  <<port<<setw(20)<<component;
				
						}else {
							cout << "Unable to open file";
						}
					}
					
			        token_3 = strtok(NULL, ","); 
			    } 
			    
			}
			
		}
	}
	
	/*Close the file if still open*/
	if(fp) {
		fclose(fp);
	}

	if(outputfile) {
		outputfile.close();
	}
}