#ifndef MY_ETH_PACKET_CLASS_H
#define MY_ETH_PACKET_CLASS_H

#include "pcap.h" //dla: pcap_pkthdr
#include "MojeLiczbyIStale.h"

#include <string>

class my_Eth_packet_class
{
private:
    static const int ile_znakow_w_czasie = 12;
    //en.wikipedia.org/wiki/IPv4#Packet_structure
    byte byte_version_HeaderLength;            // 4+4=8 bits: version, header length ("IHL")
    byte byte_TOS;                             // 8 bits: service type (TOS)
    unsigned short bits_PacketLength;          // 16 bits: total length of the packet (header + data)
    unsigned short bits_identification;        // 16 bits: identification
    unsigned short bits_flags_and_offset;      // 3+13=16 bits: flags, offset (for fragmentation)
    byte byte_TTL;                             // 8 bits: TTL
    byte byte_protocol;                        // 8 bits: protocol number (TCP, ICMP etc)
    unsigned short bits_checksum;              // 16 bits: header's checksum
    unsigned int source_IP_address;            // 32 bits: source IP address
    unsigned int destination_IP_address;       // 32 bits: destination IP address
    //other needed
    byte byte_header_length;                   //only header's length
    ProtNumbers protocolOfHeader;              //protocol number as enumerator
    byte data_from_packet[ile_bajtow_czyta];   //data carried by packet (behind header) ??
    tm struct_reception_time;                  // cplusplus.com/reference/ctime/tm/
    double microseconds_from_second;           //microseconds to add to whole seconds (reception time)
    char czas_char[ile_znakow_w_czasie]{' '};  //by default full of whitespaces
    int length_of_data;
public:
	my_Eth_packet_class();
	my_Eth_packet_class(pcap_pkthdr pcap_naglowek, const u_char* tablica, const int rozmiar); //argumenty wytlumaczone w *.cpp
	~my_Eth_packet_class();
    //
    int czas_char_length();
    byte get_Version_8bit();
    int get_Version_int();
    int get_header_length();
    byte get_TypeOfService_byte();
    int get_packet_total_length();
    unsigned short get_identification();
    void get_flags_arr(unsigned long* bity);
    unsigned short get_FragmentationOffset();
    int get_TTL_int();
    byte get_protocol_byte();
    int get_protocol_number();
    ProtNumbers get_protocol_enum();
    unsigned short get_checksum_bits();
    unsigned int get_sourceIP_bits();
    void get_IP_as_array(unsigned int* addtab, bool isSource);
    unsigned int get_destinationIP_bits();
    std::string get_ReceptionTime_string();
    tm get_ReceptionTime_struct();
    double get_time_microsec();
    int get_data_length();
    void copy_data_from_packet_to_array(byte* tab); //given array must be the same size or bigger
};

#endif
