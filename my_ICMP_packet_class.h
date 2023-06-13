#ifndef MY_ICMP_PACKET_CLASS_H
#define MY_ICMP_PACKET_CLASS_H

#include "MojeLiczbyIStale.h"

class my_ICMP_packet_class
{
private:
    // en.wikipedia.org/wiki/Internet_Control_Message_Protocol#Datagram_structure
    byte byte_type;                // 8 bits: type
    byte byte_code;                // 8 bits: code
    short bits_checksum;           // 16 bits: checksum (can be negative)
    unsigned int rest_of_header;   // 32 bits: rest of the header
public:
    my_ICMP_packet_class();
    my_ICMP_packet_class(byte* tablica, int rozmiar);
    ~my_ICMP_packet_class();
    byte get_type();
    byte get_code();
    short get_checksum();
    unsigned int get_rest_of_header();
};

#endif