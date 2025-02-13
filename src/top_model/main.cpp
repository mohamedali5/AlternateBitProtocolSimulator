/** \brief Main source file
 * 
 * Initializes COUPLEDs by setting the input and output ports.
 * Contains main function which is invoked to start the ABP simulation.
 * Initializes the different submodules of the ABP Simulator:
 *      1- Application Generator
 *      2- Sender
 *      3- Receiver
 *      4- Subnet
 *      5- Network
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

//updated relative paths --Syed Omar

#include "../../lib/DESTimes/include/NDTime.hpp"

#include "../../lib/vendor/iestream.hpp"


#include "../../include/data_structures/message.hpp"

#include "../../include/atomics/sender_cadmium.hpp"
#include "../../include/atomics/receiver_cadmium.hpp"
#include "../../include/atomics/subnet_cadmium.hpp"
#include "../../include/transform_output/transform_output.hpp"

//define new input and output filename and path

#define ABP_OUTPUT_FILE_PATH "data/top_model/abp_output.txt"
#define ABP_TRANSFORM_OUTPUT_FILE_PATH "data/top_model/abp_output_transform.txt"

using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;



/**
 * Setting input ports for messages
 */
struct inp_control : public cadmium::in_port<message_t> {};
struct inp_1 : public cadmium::in_port<message_t> {};
struct inp_2 : public cadmium::in_port<message_t> {};

/**
 * Setting output ports for messages 
 */
struct outp_ack : public cadmium::out_port<message_t> {};
struct outp_1 : public cadmium::out_port<message_t> {};
struct outp_2 : public cadmium::out_port<message_t> {};
struct outp_pack : public cadmium::out_port<message_t> {};


/**
 * This is application generator class takes file path
 * parameter and waits for input
 * @tParam message T
 */
template<typename T>
class ApplicationGen : public iestream_input<message_t,T>{
    public:
    ApplicationGen() = default;
    /**
     * A parameterized contructor for class application generator 
     * takes input path of the file that containes the input for
     * the application to run
     * @param file_path
     */
    ApplicationGen(const char* file_path) : 
        iestream_input<message_t,T>(file_path) {}
};


/**\brief  Main function
 * 
 * Initializes the different submodules of the ABP Simulator:
 *      1- Application Generator
 *      2- Sender
 *      3- Receiver
 *      4- Subnet
 *      5- Network

 * Reads input from file, runs ABP Simulator, then prints output to file
 * Prints information about the simulator's time metric
 * @param argc Integer argument - count of the command line arguments
 * @param argv Argument vector - the command line arguments
 */
int main(int argc, char ** argv){

    if (argc < 2){
        cout << "you are using this program with wrong parameters."
        "The program should be invoked as follow:";
        cout << argv[0] << " path to the input file " << endl;
        return 1; 
    }

    auto start = hclock::now(); //to measure simulation execution time
    const char *output_file = ABP_OUTPUT_FILE_PATH;
    const char *transform_output_file = ABP_TRANSFORM_OUTPUT_FILE_PATH;

    /**
     * To generate messages and operation logs which are being passed
     * during execution time of this application and storing them
     * in the abp_output file as indicated out_data.
     */
    static std::ofstream out_data(output_file);

    /**
     * This is a common sink provider structure 
     * which calls the ostream that is the output stream
     * and it return data stored in the file
     */
    struct oss_sink_provider{
        static std::ostream& sink(){          
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
     * Assign first argument value to input data control for application
     * generator file for execution and
     * runs the execution for number of input times 
     */
    string input_data_control = argv[1];
    const char * i_input_data_control = input_data_control.c_str();

    /**
     * Initialized generator which has output file path, Time and 
     * with given input generates the output
     */
    std::shared_ptr<cadmium::dynamic::modeling::model> generator_con = 
                    cadmium::dynamic::translate::make_dynamic_atomic_model
                    <ApplicationGen, 
                     TIME, 
                     const char* >("generator_con" , 
                                   std::move(i_input_data_control)
                                   );


    /**
     * Identify output data which has been sent from sender1
     */
    std::shared_ptr<cadmium::dynamic::modeling::model> sender1 = 
                    cadmium::dynamic::translate::make_dynamic_atomic_model
                    <Sender,
                     TIME>("sender1"
                           );


    /**
     * Identify output data which has been receoved from receiver1
     */
    std::shared_ptr<cadmium::dynamic::modeling::model> receiver1 = 
                    cadmium::dynamic::translate::make_dynamic_atomic_model
                    <Receiver, 
                     TIME>("receiver1"
                           );

    /**
     * Identify output data which has been receoved from subnet1 and subnet 2
     */
    std::shared_ptr<cadmium::dynamic::modeling::model> subnet1 = 
                    cadmium::dynamic::translate::make_dynamic_atomic_model
                    <Subnet, 
                     TIME>("subnet1"
                           );
    std::shared_ptr<cadmium::dynamic::modeling::model> subnet2 = 
                    cadmium::dynamic::translate::make_dynamic_atomic_model
                    <Subnet, 
                     TIME>("subnet2"
                           );

    /**
     * Store values of operations of network input and output ports
     * which have been performed for each time frame and
     * then store in output file
     */
    cadmium::dynamic::modeling::Ports iports_Network = {
        typeid(inp_1),
        typeid(inp_2)
    };
    cadmium::dynamic::modeling::Ports oports_Network = {
        typeid(outp_1),
        typeid(outp_2)
    };
    cadmium::dynamic::modeling::Models submodels_Network = {
        subnet1, 
        subnet2
    };
    cadmium::dynamic::modeling::EICs eics_Network = {
        cadmium::dynamic::translate::make_EIC<inp_1, 
                                              subnet_defs::in>("subnet1"
                                                               ),
        cadmium::dynamic::translate::make_EIC<inp_2, 
                                              subnet_defs::in>("subnet2"
                                                               )
    };
    cadmium::dynamic::modeling::EOCs eocs_Network = {
        cadmium::dynamic::translate::make_EOC<subnet_defs::out,
                                              outp_1>("subnet1"
                                                      ),
        cadmium::dynamic::translate::make_EOC<subnet_defs::out,
                                              outp_2>("subnet2"
                                                      )
    };
    cadmium::dynamic::modeling::ICs ics_Network = {};
    std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> NETWORK = 
    std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>("Network", 
                                                                submodels_Network, 
                                                                iports_Network, 
                                                                oports_Network, 
                                                                eics_Network, 
                                                                eocs_Network, 
                                                                ics_Network 
                                                                );


    /**
     * Store values of operations of input control and
     * output packet acknowledgement of ABP simulator input/output ports
     * which have been performed for each time frame and
     * then store in output file
     */
    cadmium::dynamic::modeling::Ports iports_ABPSimulator = {
        typeid(inp_control)
    };
    cadmium::dynamic::modeling::Ports oports_ABPSimulator = {
        typeid(outp_ack),
        typeid(outp_pack)
    };
    cadmium::dynamic::modeling::Models submodels_ABPSimulator = {
        sender1, 
        receiver1,
        NETWORK
    };
    cadmium::dynamic::modeling::EICs eics_ABPSimulator = {
        cadmium::dynamic::translate::make_EIC<inp_control, 
                                              sender_defs::controlIn>("sender1"
                                                                      )
    };
    cadmium::dynamic::modeling::EOCs eocs_ABPSimulator = {
        cadmium::dynamic::translate::make_EOC<sender_defs::packetSentOut,
                                              outp_pack>("sender1"
                                                         ),
        cadmium::dynamic::translate::make_EOC<sender_defs::ackReceivedOut,
                                              outp_ack>("sender1"
                                                        )
    };
    cadmium::dynamic::modeling::ICs ics_ABPSimulator = {
        cadmium::dynamic::translate::make_IC<sender_defs::dataOut, 
                                             inp_1>("sender1",
                                                    "Network"
                                                    ),
        cadmium::dynamic::translate::make_IC<outp_2, 
                                             sender_defs::ackIn>("Network",
                                                                 "sender1"),
        cadmium::dynamic::translate::make_IC<receiver_defs::out, 
                                             inp_2>("receiver1",
                                                    "Network"
                                                    ),
        cadmium::dynamic::translate::make_IC<outp_1, 
                                             receiver_defs::in>("Network",
                                                                "receiver1"
                                                                )
    };
    std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> ABPSimulator = 
    std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>("ABPSimulator", 
                                                                submodels_ABPSimulator, 
                                                                iports_ABPSimulator, 
                                                                oports_ABPSimulator, 
                                                                eics_ABPSimulator, 
                                                                eocs_ABPSimulator, 
                                                                ics_ABPSimulator 
                                                                );


    /**
     * Store values of top model operations
     * which have been performed for each time frame and
     * then store in output file
     */
    cadmium::dynamic::modeling::Ports iports_TOP = {};
    cadmium::dynamic::modeling::Ports oports_TOP = {
        typeid(outp_pack),
        typeid(outp_ack)
    };
    cadmium::dynamic::modeling::Models submodels_TOP = {
        generator_con, 
        ABPSimulator
    };
    cadmium::dynamic::modeling::EICs eics_TOP = {};
    cadmium::dynamic::modeling::EOCs eocs_TOP = {
        cadmium::dynamic::translate::make_EOC<outp_pack,
                                              outp_pack>("ABPSimulator"
                                                         ),
        cadmium::dynamic::translate::make_EOC<outp_pack,
                                              outp_ack>("ABPSimulator"
                                                       )
    };
    cadmium::dynamic::modeling::ICs ics_TOP = {
        cadmium::dynamic::translate::make_IC<iestream_input_defs<message_t>::out,
                                                                 inp_control>("generator_con",
                                                                              "ABPSimulator"
                                                                              )
    };
    std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP = 
    std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>( "TOP", 
                                                                submodels_TOP, 
                                                                iports_TOP, 
                                                                oports_TOP, 
                                                                eics_TOP, 
                                                                eocs_TOP, 
                                                                ics_TOP 
                                                                );

    /**
     * Create a model and measure elapsed time form creations in 
     * seconds during run time. Create runner,  and measure elapsed 
     * time form creations in seconds. Simulation starts and the
     * time took to complete the simulation. Simulation is ran
     *  until 04:00:00:000 time period.
     */
    auto elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, 
                    std::ratio<1>>>(hclock::now() - start).count(); ///< Run-time to create model
    cout << "Model Created. Elapsed time: " << elapsed1 << "sec" << endl;
    
    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, 
               std::ratio<1>>>(hclock::now() - start).count(); ///< Run-time to create runner

    cout << "Runner Created. Elapsed time: " << elapsed1 << "sec" << endl;

    cout << "Simulation starts" << endl;

    r.run_until(NDTime("04:00:00:000"));

    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, 
                   std::ratio<1>>>(hclock::now() - start).count();
                   ///< Run-time of simulation
    cout << "Simulation took:" << elapsed << "sec" << endl;

    /**
     * transform the output file generated by simulator to readable text
     */
    transform_output(output_file,transform_output_file);

    return 0;
}
