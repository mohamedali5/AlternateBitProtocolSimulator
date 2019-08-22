/** \brief This header file declares functions for transforming output.
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

#ifndef __TRANSFORM_OUTPUT_HPP__
#define __TRANSFORM_OUTPUT_HPP__

/** 
 *  This function is used to transform the ABP simulator generated output 
 *  to a table structured and readable format. It takes as input the 
 *  file you want to transform and the name of the output file and generates 
 *  the new output file.
 *  @param ip_file_name input file name 
 *  @param op_file_name output file name 
 */

void transform_output(const char *ip_file_name, const char *op_file_name);


#endif // __TRANSFORM_OUTPUT_HPP__