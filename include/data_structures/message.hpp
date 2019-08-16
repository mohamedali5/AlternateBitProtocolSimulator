
/** \brief Header file for message containing headers
 * 
 * Defines a message_t structure
 */

#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/** Define structure of message_t
 *  message_t will containg a a field value of type float
 */
struct message_t{

    message_t(){}
    message_t(float i_value)
    :value(i_value){}

  	float   value;

    void clear(){
        message_t(); 
    }
};

istream& operator>> (istream& is, message_t& msg);

ostream& operator<<(ostream& os, const message_t& msg);


#endif ///< BOOST_SIMULATION_MESSAGE_HPP - end of ifndef