/** \brief This header file implements the Sender class.
*
* The sender switch from initial phase passive to active 
* when an external signal is received. Once activated it start 
* sending the packet with the alternating bit. Whenever a packet 
* is sent, it waits for an acknowledgement during an specific 
* waiting time. if the acknowledgment does not arrive within
* the time window, the sender will re-send the previous packet
* with the alternating bit. If the expected acknowledgement 
* is received within the time window, the sender will send 
* the next packet. When there are no more packets to send, 
* the sender will go again to the passive phase.
*
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
*/

#ifndef BOOST_SIMULATION_PDEVS_SENDER_HPP
#define BOOST_SIMULATION_PDEVS_SENDER_HPP

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
* Structure Port definition for packetSentOut and 
* acknowldgement received, data ouput, control input and
* incoming acknowldgement
*/
struct sender_defs{
    struct packetSentOut : public out_port<message_t> {};
    struct ackReceivedOut : public out_port<message_t> {};
    struct dataOut : public out_port<message_t> {};
    struct controlIn : public in_port<message_t> {};
    struct ackIn : public in_port<message_t> {};
};

/** 
* The Sender class receives message and sends out acknowledge.
*/  
template<typename TIME>
class Sender{
    // putting definitions in context
    using defs = sender_defs; 
    public:
    //Parameters to be overwriten when instantiating the atomic model
    TIME   PREPARATION_TIME; /**< PREPARATION_TIME is a constant that contains the time delay from acknowledge to output. */
    TIME   timeout;          /**< timeout is a onstant that contains time delay from output to acknowledge */
   
    /** 
    * Default constructor for Receiver class.
    * Initializes  delay constant, timeout constant 
    * set alternetbit state to 0, next internal state  to 0
    * and set model active state to false
    */
    Sender() noexcept{
        PREPARATION_TIME = TIME("00:00:10");
        timeout = TIME("00:00:20");
        state.alt_bit = 0;
        state.next_internal = std::numeric_limits<TIME>::infinity();
        state.model_active = false;
    }
            
    /**
     * Structure state definition which has state variables.
     */
    struct state_type{
        bool ack;
        int packet_num;
        int total_packet_num;
        int alt_bit;
        bool sending;
        bool model_active;
        TIME next_internal;
    }; state_type state;

    // ports definition
    using input_ports = std::tuple<typename defs::controlIn, 
                                   typename defs::ackIn>;

    using output_ports = std::tuple<typename defs::packetSentOut, 
                                    typename defs::ackReceivedOut, 
                                    typename defs::dataOut>;

    // internal transition
    /**
    * Internal transition function that check acknowledgement state
    * and sending state so it sets the next state based on the current state.
    */
    void internal_transition(){
        if (state.ack){
            if (state.packet_num < state.total_packet_num){
                state.packet_num ++;
                state.ack = false;
                state.alt_bit = (state.alt_bit + 1) % 2;
                state.sending = true;
                state.model_active = true; 
                state.next_internal = PREPARATION_TIME;   
            } else{
                state.model_active = false;
                state.next_internal = std::numeric_limits<TIME>::infinity();
            }
        } else{
            if(state.sending){
                state.sending = false;
                state.model_active = true;
                state.next_internal = timeout;
            } else{
                state.sending = true;
                state.model_active = true;
                state.next_internal = PREPARATION_TIME;    
            } 
        }   
    }

    /**
    * External transition function that extract the messages: 
    * if the number of messages is greater than 1
    * it asserts sending state to false and concatenate the message
    * that only one message is expected per time unit. 
    * It then set the next state based on the current state
    * It also chceck the next internal transtion is not equal to infinity
    * it sets the next internal state -e
    * @param e of type time 
    * @param mbs of type message bags
    */
    void external_transition(TIME e, 
                             typename make_message_bags<input_ports>::type mbs){ 

        if((get_messages<typename defs::controlIn>(mbs).size()+
            get_messages<typename defs::ackIn>(mbs).size())>1){
            assert(false && "one message per time uniti");
        }
        for(const auto &x : get_messages<typename defs::controlIn>(mbs)){
            if(state.model_active == false){
                state.total_packet_num = static_cast < int > (x.value);
                if (state.total_packet_num > 0){
                    state.packet_num = 1;
                    state.ack = false;
                    state.sending = true;
                    state.alt_bit = state.packet_num % 2;  //set initial alt_bit
                    state.model_active = true;
                    state.next_internal = PREPARATION_TIME;
                } else{
                    if(state.next_internal != std::numeric_limits<TIME>::infinity()){
                        state.next_internal = state.next_internal - e;
                    }
               }
            }
        }
        for(const auto &x : get_messages<typename defs::ackIn>(mbs)){
            if(state.model_active == true){ 
                if (state.alt_bit == static_cast < int > (x.value)){
                    state.ack = true;
                    state.sending = false;
                    state.next_internal = TIME("00:00:00");
                } else{
                    if(state.next_internal != std::numeric_limits<TIME>::infinity()){
                        state.next_internal = state.next_internal - e;
                    }
                }
            }
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
    * Output function sends the packet number to the output port.
    * The ouput value is calculated as packet number *10  + alternating bit
    * the ouput is pushed to message bags. If Acknolwegement state is true
    * set output value to alternatbit state and push it to message bag
    * @return Message bags
    */
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        message_t out;
        if (state.sending){
            out.value = state.packet_num * 10 + state.alt_bit;
            get_messages<typename defs::dataOut>(bags).push_back(out);
            out.value = state.packet_num;
            get_messages<typename defs::packetSentOut>(bags).push_back(out);
        } else{
            if(state.ack){
                out.value = state.alt_bit;
                get_messages<typename defs::ackReceivedOut>(bags).push_back(out);
            }
        }   
        return bags;
    }

    /**
    * time_advance function return the next internal transition time.
    * @return Next internal time
    */
    TIME time_advance() const {  
        return state.next_internal;
    }

    /**
    * Friend is a function that outputs packet number and total
    * packet number to ostring stream.
    * @param os the ostring stream
    * @param i structure state_type
    * @return os the ostring stream
    */
    friend std::ostringstream& operator<<(std::ostringstream& os, 
                                          const typename Sender<TIME>::state_type& i){
        os << "packet_num: " << i.packet_num << " & total_packet_num: " <<
        i.total_packet_num; 
        return os;
    }
};     

#endif // BOOST_SIMULATION_PDEVS_SENDER_HPP