/** \brief This header file implements the Subnet class.
*
* The subnets passe the packets after some time delay with packet 
* with a probability of 95% of the packets will be passed in each
* of the subnet, and remining of the data will be
* lost through the subnet.
* 
* The subnet has two phases: passive and active.
* subent is in passive phase initially. Whenever it 
* receives a packet, it goes to active phase, send out
* the acknowledgement with 95% probability, once it is sent it go to passive 
* state again.
*
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
*/

#ifndef BOOST_SIMULATION_PDEVS_SUBNET_HPP
#define BOOST_SIMULATION_PDEVS_SUBNET_HPP

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

//updated relative path --Syed Omar
#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

/** 
* Structure Port definition for output and input messages.
*/
struct subnet_defs{
    struct out : public out_port<message_t> {};
    struct in : public in_port<message_t> {};
};
//This is a meta-model, it should be overloaded for declaring the "id" parameter
template<typename TIME>
class Subnet{
    /// putting definitions in context
    using defs=subnet_defs; 
    public:
    //Parameters to be overwriten when instantiating the atomic model
           
    /** 
    * Default constructor for subnet class.
    * Initializes state structure of transmiting  to false
    * and index to zero
    */
    Subnet() noexcept{
        state.transmiting = false;
        state.index = 0;
    }
            
    /**
     * Structure state definition which contains state
     * type variables.
     */
    struct state_type{
        bool transmiting;
        int packet;
        int index;
    }; state_type state;
    // Intalizing input and output ports
    using input_ports=std::tuple<typename defs::in>;
    using output_ports=std::tuple<typename defs::out>;

    /**
    * Internal transition function that set transmitting state to
    * state
    */
    void internal_transition(){
        state.transmiting = false;  
    }

    /**
    * External transition function that extract the messages: 
    * if the number of messages is greater than 1
    * it asserts transmitting state to flase and concatenate the message
    * that only one message is expected per time unit. 
    * It then sets the packet to the message value and 
    * sets the transmitting state to true.
    * @param e of type time 
    * @param mbs of type message bags
    */
    void external_transition(TIME e, 
                             typename make_message_bags<input_ports>::type mbs){ 
        state.index ++;
        if(get_messages<typename defs::in>(mbs).size()>1){
            assert(false && "One message at a time");  
        }              
        for (const auto &x : get_messages<typename defs::in>(mbs)){
            state.packet = static_cast < int > (x.value);
            state.transmiting = true; 
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
    * Output function sends the packet number to the output port,
    * if the division of a random number with RAND_MAX is less thab 0.95.
    * Then push the out value to message bags.
    * @return message bags
    */
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
         message_t out;
        if ((double)rand() / (double) RAND_MAX  < 0.95){
            out.value = state.packet;
            get_messages<typename defs::out>(bags).push_back(out);
        }
        return bags;
    }

     /**
    * time_advance function that sets the next internal transition time.
    * If the current sending state is  true then the next internal 
    * time is set to time. Otherwise it is set
    * to infinity.      
    * @return Next internal time
    */
    TIME time_advance() const {
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(3.0, 1.0); 
        TIME next_internal;
        if (state.transmiting){
            std::initializer_list<int> time = {0, 0, 
                                               static_cast < int > (round(distribution(generator)))};
            // time is hour min and second
            next_internal = TIME(time);
        } else{
            next_internal = std::numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    /**
    * Friend is a function that outputs index number and transmiting state
    * @param os the ostring stream
    * @param i structure state_type
    * @return os the ostring stream
    */
    friend std::ostringstream& operator<<(std::ostringstream& os, 
                                          const typename Subnet<TIME>::state_type& i){
        os << "index: " << i.index << " & transmiting: " << i.transmiting; 
        return os;
    }
};    
#endif // BOOST_SIMULATION_PDEVS_SUBNET_HPP