
/** \brief Source file for message source code
 * 
 * Contains input and output stream methods from the header file
 * which return float values of message structure
 */

#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "../../include/data_structures/message.hpp"

/** Output stream
 * it is an operator applied to an output stream is known as 
 * insertion operator, and performs formatted output character sequence
 * @param[out] os reference to ostream - output stream
 * @param[in] msg const reference to struct message_t - message
 */
ostream& operator<<(ostream& os, const message_t& msg){
	os << msg.value;
    return os;
}

/** Input stream
 * it is an operator applied to an output stream is known as 
 * insertion operator, and performs formatted output character sequence
 * @param[out]  is reference to istream - iutput stream
 * @param[in]  msg const reference to struct message_t - message
 */
istream& operator>> (istream& is, message_t& msg){
    is >> msg.value;
    return is;
}
