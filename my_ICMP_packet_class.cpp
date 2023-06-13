#include "my_ICMP_packet_class.h"

#include <string>
#include <sstream>

my_ICMP_packet_class::my_ICMP_packet_class()
{
	/*
	  en.wikipedia.org/wiki/Internet_Control_Message_Protocol#Control_messages
	    type 3 = "Destination Unreachable"
		code 5 = "Source route failed"
	*/
	byte_code = (byte)5;
	byte_type = (byte)3;
	bits_checksum = 0;
	rest_of_header = 0;
}

my_ICMP_packet_class::my_ICMP_packet_class(byte* tablica, int rozmiar)
{
	char* one_byte = new char[1];
	char* two_byte = new char[2];
	char* four_byte = new char[4];
	std::string s1;
	int a;
	for (a = 0; a < rozmiar; a++)
	{
		s1.push_back(tablica[a]);
	}
	std::stringstream strumien(s1);
	//
	strumien.read(one_byte, 1);
	std::memcpy(&byte_type, one_byte, 1);
	//
	strumien.read(one_byte, 1);
	std::memcpy(&byte_code, one_byte, 1);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&bits_checksum, two_byte, 2);
	//
	strumien.read(four_byte, 4);
	std::memcpy(&rest_of_header, four_byte, 4);

	delete[] one_byte;
	delete[] two_byte;
	delete[] four_byte;
}

my_ICMP_packet_class::~my_ICMP_packet_class()
{
	//puste
}

byte my_ICMP_packet_class::get_type()
{
	return byte_type;
}

byte my_ICMP_packet_class::get_code()
{
	return byte_code;
}

short my_ICMP_packet_class::get_checksum()
{
	return bits_checksum;
}

unsigned int my_ICMP_packet_class::get_rest_of_header()
{
	return rest_of_header;
}
