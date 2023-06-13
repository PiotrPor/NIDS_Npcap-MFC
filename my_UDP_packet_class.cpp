#include "my_UDP_packet_class.h"
#include <string>
#include <sstream>

my_UDP_packet_class::my_UDP_packet_class()
{
	usSourcePort = 1;
	usDestinationPort = 1;
	usLength = 10;
	sChecksum = 0;
	length_of_header = 8;
	MessageLength = usLength - length_of_header;
	byUDPData[0] = 0;
	byUDPData[1] = 0;
}

my_UDP_packet_class::my_UDP_packet_class(byte* tablica, int rozmiar)
{
	// www.geeksforgeeks.org/examples-on-udp-header/
	//check "my_Eth_packet_class" constructor for comments
	char* two_byte = new char[2];
	std::string s1;
	int a;
	for (a = 0; a < rozmiar; a++)
	{
		s1.push_back(tablica[a]);
	}
	std::stringstream strumien(s1);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usSourcePort, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usDestinationPort, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usLength, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&sChecksum, two_byte, 2);

	length_of_header = 8; // 4*16 b = 8 B
	MessageLength = rozmiar - length_of_header;

	//copy data carried by packet
	for (a = length_of_header; a < rozmiar && a < ile_bajtow_czyta; a++)
	{
		byUDPData[a - length_of_header] = s1[a];
	}

	delete[] two_byte;
}

my_UDP_packet_class::~my_UDP_packet_class()
{
	//puste
}

int my_UDP_packet_class::get_source_port_int()
{
	return (int) usSourcePort;
}

int my_UDP_packet_class::get_destination_port_int()
{
	return (int)usDestinationPort;
}

int my_UDP_packet_class::get_whole_packet_length()
{
	return (int)usLength;
}

int my_UDP_packet_class::get_length_of_header()
{
	return length_of_header;
}

int my_UDP_packet_class::get_length_of_message()
{
	return MessageLength;
}
