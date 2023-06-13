#ifndef MY_TCP_PACKET_CLASS_H
#define MY_TCP_PACKET_CLASS_H

#include "MojeLiczbyIStale.h"
#include <string>

class my_TCP_packet_class
{
private:
    //"TCP header fields"
    unsigned short usSourcePort;                  //16 bits for the source port number
    unsigned short usDestinationPort;             //16 bits for the destination port number
    unsigned int uiSequenceNumber;                //32 bits for the sequence number
    unsigned int uiAcknowledgementNumber;         //32 bits for the acknowledgement number
    unsigned short usHeaderLengthAndFlags;        //6+6+4 bits for flags, reserved space and data offset
    unsigned short usWindow;                      //16 bits for the window size
    short sChecksum;                              //16 bits for the checksum (can be negative)
    unsigned short usUrgentPointer;               //16 bits for the urgent pointer
    //inne
    byte byHeaderLength;                          //Header length
    int MessageLength;                            // length of the data being carried
    byte byTCPData[ile_bajtow_czyta];             //Data carried by the TCP packet
public: 
    my_TCP_packet_class();
    my_TCP_packet_class(byte* tablica, int rozmiar); //argumenty opisano w *.cpp
    ~my_TCP_packet_class();
    //
    int get_source_port();
    int get_destination_port();
    unsigned int get_sequence_number();
    unsigned int get_acknowledgement_number();
    void get_flags_as_string_array(std::string* tab);
    unsigned short get_window_size();
    short get_checksum();
    unsigned short get_UrgentPointer_bits();
    int get_header_length();
    int get_message_length();
};

#endif