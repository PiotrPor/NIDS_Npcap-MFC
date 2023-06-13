#ifndef MY_UDP_PACKET_CLASS_H
#define MY_UDP_PACKET_CLASS_H

#include "MojeLiczbyIStale.h"

class my_UDP_packet_class
{
private:
    unsigned short usSourcePort;            //16 bits for the source port number        
    unsigned short usDestinationPort;       //16 bits for the destination port number
    unsigned short usLength;                //16 bits: length of whole packet (in bytes)
    short sChecksum;                        //16 bits for the checksum (can be negative)            
    //
    byte byUDPData[ile_bajtow_czyta];       //data carried by the UDP packet
    int length_of_header = 8;
    int MessageLength;
public:
    my_UDP_packet_class();
    my_UDP_packet_class(byte* tablica, int rozmiar);
    ~my_UDP_packet_class();
    int get_source_port_int();
    int get_destination_port_int();
    int get_whole_packet_length();
    int get_length_of_header();
    int get_length_of_message();
};

#endif