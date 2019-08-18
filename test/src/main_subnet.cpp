/**  \brief This main file implements the operation of subnet file
 *
 * This file prepares application generator which takes the
 * input as filepath, stores output and generates
 * all the log data using the cadmium and library. 
 * It runs with respect to input provided by input file and 
 * runs simulation until time 04:00:00:000 is reached.
 *
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
#include "../../include/transform_output/transform_output.hpp"

//define new input and output filename and path
#define input_file_name "test/data/subnet_test_output.txt"
#define output_file_name "test/data/subnet_test_output_transform.txt"

using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;


/**
 * Setting input ports for messages
 */
struct inp_in : public cadmium::in_port<message_t> {};

/**
 * Setting output ports or messages
 */
struct outp_out: public cadmium::out_port<message_t> {};


/**
 * This is application generator class takes file path
 * parameter and waits for input
 * @Param message T
 */
template<typename T>
class ApplicationGen : public iestream_input<message_t,T> {
    public:
        ApplicationGen() = default;

        /**
         * A parameterized constructor for class application generator 
         * takes input path of the file that contains the input for
         * the application to run
         * @param file_path
         */
        ApplicationGen(const char* file_path) : 
            iestream_input<message_t,T>(file_path) {}
};


int main() {

    //to measure simulation execution time
    auto start = hclock::now(); 

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
     * Loggers definition of cadmium to call source loggers strcture
     * to generate the log files in a formatted mannar and to store
     * them in variables and to be logged to the file
     */
    using info = cadmium::logger::logger<cadmium::logger::logger_info, 
                 cadmium::dynamic::logger::formatter<TIME>, 
                 oss_sink_provider>;
    using debug = cadmium::logger::logger<cadmium::logger::logger_debug, 
                  cadmium::dynamic::logger::formatter<TIME>, 
                  oss_sink_provider>;
    using state = cadmium::logger::logger<cadmium::logger::logger_state, 
                  cadmium::dynamic::logger::formatter<TIME>, 
                  oss_sink_provider>;
    using log_messages = cadmium::logger::logger<cadmium::logger::logger_messages, 
                         cadmium::dynamic::logger::formatter<TIME>, 
                         oss_sink_provider>;
    using routing = cadmium::logger::logger<cadmium::logger::logger_message_routing, 
                    cadmium::dynamic::logger::formatter<TIME>, 
                    oss_sink_provider>;
    using global_time = cadmium::logger::logger<cadmium::logger::logger_global_time, 
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

    using logger_top = cadmium::logger::multilogger<log_messages, global_time>;


    /**
     * Get input receiver file for execution and
     * runs the execution for number of input times 
     */
    string input_data = "test/data/subnet_input_test.txt";
    const char * i_input_data = input_data.c_str();

    /**
     * Initialized generator which has output file path, Time and 
     * with given input generates the output
     */
    std::shared_ptr<cadmium::dynamic::modeling::model> generator = 
                    cadmium::dynamic::translate::make_dynamic_atomic_model
                    <ApplicationGen, 
                     TIME, 
                     const char* >("generator" ,
                                   std::move(i_input_data)
                                   );

    /**
     * Identify output data which has been received from subnet1
     */
    std::shared_ptr<cadmium::dynamic::modeling::model> subnet1 = 
    cadmium::dynamic::translate::make_dynamic_atomic_model
    <Subnet, 
     TIME>("subnet1"
          );


    /**
     * Store values of top model operations
     * which have been performed for each time frame and
     * then store in output file
     */
    cadmium::dynamic::modeling::Ports iports_TOP = {};
    cadmium::dynamic::modeling::Ports oports_TOP = {
        typeid(outp_out)
    };
    cadmium::dynamic::modeling::Models submodels_TOP = {
        generator,
        subnet1
    };
    cadmium::dynamic::modeling::EICs eics_TOP = {};
    cadmium::dynamic::modeling::EOCs eocs_TOP = {
        cadmium::dynamic::translate::make_EOC
        <subnet_defs::out,
         outp_out>("subnet1"
                   )
    };
    cadmium::dynamic::modeling::ICs ics_TOP = {
        cadmium::dynamic::translate::make_IC
        <iestream_input_defs<message_t>::out,
         subnet_defs::in>("generator",
                          "subnet1"
                          )
    };
    std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP = 
    std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>("TOP", 
                                                                submodels_TOP, 
                                                                iports_TOP, 
                                                                oports_TOP, 
                                                                eics_TOP, 
                                                                eocs_TOP, 
                                                                ics_TOP 
                                                                );
  
    /**
     * Create a model and measure elapsed time form creations in 
     * seconds. Create runner, and measure elapsed time form creations in
     * seconds. Simulation starts and the time took to complete 
     * the simulation. Simulation is ran until 04:00:00:000 time period.
     */

    auto elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, 
    std::ratio<1>>>(hclock::now() - start).count();
    cout << "Model Created. Elapsed time: " << elapsed1 << "sec" << endl;
    
    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, 
    std::ratio<1>>>(hclock::now() - start).count();
    cout << "Runner Created. Elapsed time: " << elapsed1 << "sec" << endl;

    cout << "Simulation starts" << endl;

    r.run_until(NDTime("04:00:00:000"));
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, 
    std::ratio<1>>>(hclock::now() - start).count();
    cout << "Simulation took:" << elapsed << "sec" << endl;

    /**
     * transform the output file generated by simulator to readable text
     */
    transform_output(input_file_name,output_file_name);

    
    return 0;
}