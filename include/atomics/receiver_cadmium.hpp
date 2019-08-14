/** \brief This header file implements the Receiver class.
*
* receiver is to receive the data and send back an 
* acknowledgement extracted from the received data after
* a time period
* 
* The receiver has two phases: passive and active.
* Receiver is in passive phase initially. Whenever it 
* receives a packet, it goes to active phase, send out
* the acknowledgement, once it is sent it go to passive 
* state again. The receiver has a constant delay
*
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
*/

#ifndef BOOST_SIMULATION_PDEVS_RECEIVER_HPP
#define BOOST_SIMULATION_PDEVS_RECEIVER_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>

#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;


/** 
* Structure Port definition for output and input messages.
*/
struct receiver_defs{
    struct out : public out_port<message_t> {};
    struct in : public in_port<message_t> {};
};

/** 
* The Receiver class receives message and sends out acknowledge.
*/  
template<typename TIME>
class Receiver{
    // putting definitions in context
    using defs = receiver_defs; 
    public:
    //Parameters to be overwriten when instantiating the atomic model
    TIME PREPARATION_TIME; /**< PREPARATION_TIME is a constant that contains the time delay from acknowledge to output. */
    /** 
    * Default constructor for Receiver class.
    * Initializes  delay constant, state acknowledgement number
    * and sending setting to false
    */
    Receiver() noexcept{
        PREPARATION_TIME = TIME("00:00:10");
        state.ack_num = 0;
        state.sending = false;
    }
            
    /**
     * Structure state definition which holds acknowledge number and receiver state.
     */
    struct state_type{
        int ack_num;    /**< Alternating bit retrieved from the message and acknowledgement number. */
        bool sending;   /**< State of the reciver passive or active */
    };  state_type state;

    // ports definition
    using input_ports = std::tuple<typename defs::in>;
    using output_ports = std::tuple<typename defs::out>;

    /**
    * Internal transition function that set sending state to
    * state
    */
    void internal_transition(){
        state.sending = false; 
    }

    /**
    * External transition function that extract the messages: 
    * if the number of messages is greater than 1
    * it asserts sending state to false and concatenate the message
    * that only one message is expected per time unit. 
    * It then sets the acknowledge to the message value and 
    * sets the sending state to true.
    * @param e of type time 
    * @param mbs of type message bags
    */
    void external_transition(TIME e, 
                             typename make_message_bags<input_ports>::type mbs){ 

        if(get_messages<typename defs::in>(mbs).size()>1){
            assert(false && "one message per time uniti");
        }
        for(const auto &x : get_messages<typename defs::in>(mbs)){
            state.ack_num = static_cast < int > (x.value);
            state.sending = true;
        }                            
    }

    /** 
    * Confluence transition function that calls internal 
    * transition fucntion followed by external transition functions.
    * @param e of type time 
    * @param mbs of type message bags
    */
    void confluence_transition(TIME e, 
                               typename make_message_bags<input_ports>::type mbs){
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }


    /**
    * Output function sends the acknowledgement number to the output port.
    * The ouput value is calculated as the reminder of after
    * dividing acknowledgement number by 10. 
    * @return message bags
    */
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        message_t out;              
        out.value = state.ack_num % 10;
        get_messages<typename defs::out>(bags).push_back(out);
        return bags;
    }

    /**
    * time_advance function that sets the next internal transition time.
    * If the current sending state is  true then the next internal 
    * time is set to PREPARATION_TIME. Otherwise it is set
    * to infinity.      
    * @return Next internal time
    */
    TIME time_advance() const {  
        TIME next_internal;
        if (state.sending){
            next_internal = PREPARATION_TIME;
        }else{
            next_internal = std::numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    /**
    * Friend is a function that outputs acknowledge number to ostring stream.
    * @param os the ostring stream
    * @param i structure state_type
    * @return os the ostring stream
    */

    friend std::ostringstream& operator<<(std::ostringstream& os, 
                                          const typename Receiver<TIME>::state_type& i){
        os << "ack_num: " << i.ack_num; 
        return os;
    }
};     
  
#endif // BOOST_SIMULATION_PDEVS_RECEIVER_HPP