/** \brief Code Optimization source file
 *
 *  Authors      : Syed Omar Azam, Ammar Al-Mahbashi and Mohamed Ali
 *  Date Created : 19-Aug-2019
 *  Version      : 1.0
 *
 *  This header file removed redundant code from all the four
 *  .cpp files and serve as a platform to optimize code for the simulator.
 *  In future developers can potentially use this header file to write
 *  common used code by different simulator modules.
 *
 *  This function code is used by the following components
 *      1- ABP Simulator
 *      2- Sender
 *      3- Receiver
 *      4- Subnet
 */

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>


#include "../../lib/DESTimes/include/NDTime.hpp"
#include "../../lib/vendor/iestream.hpp"
#include "../../include/data_structures/message.hpp"
#include "../../include/atomics/subnet_cadmium.hpp"
#include "../../include/atomics/receiver_cadmium.hpp"
#include "../../include/atomics/sender_cadmium.hpp"
#include "../../include/transform_output/transform_output.hpp"

//define new input and output filename and path
//#define input_file_name "test/data/subnet_test_output.txt"
//#define output_file_name "test/data/subnet_test_output_transform.txt"

using namespace std;
using hclock = chrono::high_resolution_clock;
using TIME = NDTime;

// vairables to hold input and output file paths.
const char *input_file_name, *output_file_name;

/**
* This function is used to set the input and output file paths
* for all the four components of the simulator. The idea behind
* creating this function is to remove redundancies in project
* and utilize common code functionlilty. This will help in reducing
* total LOC. It takes module_name as input and set the i/p and o/p
* variables to respective file paths.
* @param module_name of type string
*/

void set_file_name(std::string module_name){

    if(module_name == "subnet"){
       
        //#define input_file_name "test/data/subnet_test_output.txt"
        //#define output_file_name "test/data/subnet_test_output_transform.txt"
        input_file_name = "test/data/subnet_test_output.txt";
        output_file_name = "test/data/subnet_test_output_transform.txt";
        printf("subnet files assigned %s \t %s\n",input_file_name,output_file_name);
        

    }else if(module_name == "receiver"){
       
        //#define input_file_name "test/data/receiver_test_output.txt"
        //#define output_file_name "test/data/receiver_test_output_transform.txt"
        input_file_name = "test/data/receiver_test_output.txt";
        output_file_name = "test/data/receiver_test_output_transform.txt";
        printf("receiver files assigned %s \t %s\n",input_file_name,output_file_name);  

    }else if(module_name == "sender"){
       
        //#define input_file_name "test/data/sender_test_output.txt"
        //#define output_file_name "test/data/sender_test_output_transform.txt"
        input_file_name = "test/data/sender_test_output.txt";
        output_file_name = "test/data/sender_test_output_transform.txt";
        printf("sender files assigned %s \t %s\n",input_file_name,output_file_name);  

    }else if(module_name == "abp_simulator"){
       
        //#define input_file_name "data/top_model/abp_output.txt"
        //#define output_file_name "data/top_model/abp_output_transform.txt"
        input_file_name = "data/top_model/abp_output.txt";
        output_file_name = "data/top_model/abp_output_transform.txt";
        printf("abp_simulator files assigned %s \t %s\n",input_file_name,output_file_name);  

    }

}


/**
 * To generate messages and operation logs which are being passed
 * during execution time of this application and storing them
 * in the receiver_test_output file as indicated out_data.
 */
static std::ofstream out_data(input_file_name);
  

/**
 * This is a common sink provider structure 
 * which calls the ostream that is the output stream
 * and it return data stored in the file
 */
struct oss_sink_provider{
    static std::ostream& sink() {          
    	return out_data;
    }
};

/**
 * This following section of code is redundant in all four
 * components and un-used. So we commented out this section.
 * Please do not remove it as it may serve as a reference in futire.
 * Its a good coding practice to keep the old code commented as 
 * it can be used later.
 */

 /*
    using info = cadmium::logger::logger<cadmium::logger::logger_info, 
                 cadmium::dynamic::logger::formatter<TIME>, 
                 oss_sink_provider>;

    using debug = cadmium::logger::logger<cadmium::logger::logger_debug, 
                  cadmium::dynamic::logger::formatter<TIME>, 
                  oss_sink_provider>;

    using state = cadmium::logger::logger<cadmium::logger::logger_state, 
                  cadmium::dynamic::logger::formatter<TIME>, 
                  oss_sink_provider>;

    using routing = cadmium::logger::logger<cadmium::logger::logger_message_routing, 
                    cadmium::dynamic::logger::formatter<TIME>, 
                    oss_sink_provider>;

    using local_time = cadmium::logger::logger<cadmium::logger::logger_local_time, 
                       cadmium::dynamic::logger::formatter<TIME>, 
                       oss_sink_provider>;

    using log_all = cadmium::logger::multilogger<info, 
                                                 debug, 
                                                 state, 
                                                 log_messages, 
                                                 routing, 
                                                 global_time, 
                                                 local_time>;
 */

/**
 * Loggers definition of cadmium to call source loggers strcture
 * to generate the log files in a formatted manner and to store
 * them in variables and to be logged to the file
 */

using log_messages = cadmium::logger::logger<cadmium::logger::logger_messages, 
                     cadmium::dynamic::logger::formatter<TIME>, 
                     oss_sink_provider>;

using global_time = cadmium::logger::logger<cadmium::logger::logger_global_time, 
                    cadmium::dynamic::logger::formatter<TIME>, 
                    oss_sink_provider>;

using logger_top = cadmium::logger::multilogger<log_messages, global_time>;