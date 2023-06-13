#include "my_TCP_packet_class.h"
#include <sstream>

my_TCP_packet_class::my_TCP_packet_class()
{
	usSourcePort = 1;
	usDestinationPort = 1;
	uiSequenceNumber = 555;
	uiAcknowledgementNumber = 555;
	usHeaderLengthAndFlags = 555;
	usWindow = 555;
	sChecksum = 555;
	usUrgentPointer = 0;
	byHeaderLength = (byte)20;
	MessageLength = ile_bajtow_czyta - 20;
	byTCPData[0] = 0;
}

/*
  wskaznik do poczatku tablicy, ktora jest danymi z naglowka Ethernet
  rozmiar, ile jest bajtow w danych z pakietu Ethernet
*/
my_TCP_packet_class::my_TCP_packet_class(byte* tablica, int rozmiar)
{
	// networklessons.com/cisco/ccie-routing-switching-written/tcp-header
	//check "my_Eth_packet_class" constructor for comments
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
	strumien.read(two_byte, 2);
	std::memcpy(&usSourcePort, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usDestinationPort, two_byte, 2);
	//
	strumien.read(four_byte, 4);
	std::memcpy(&uiSequenceNumber, four_byte, 4);
	//
	strumien.read(four_byte, 4);
	std::memcpy(&uiAcknowledgementNumber, four_byte, 4);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usHeaderLengthAndFlags, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usWindow, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&sChecksum, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&usUrgentPointer, two_byte, 2);

	//get header length out of the 16 bits in the field (it's the first 4 bits)
	byHeaderLength = (byte)(usHeaderLengthAndFlags >> 12);
	//Message length = Total length of the TCP packet - Header length
	MessageLength = rozmiar - (int)byHeaderLength;

	//Copy the TCP data into the data buffer
	int headlen = (int)byHeaderLength;
	for (a = headlen; a < rozmiar && a < ile_bajtow_czyta; a++)
	{
		byTCPData[a - headlen] = s1[a];
	}

	delete[] one_byte;
	delete[] two_byte;
	delete[] four_byte;
}

my_TCP_packet_class::~my_TCP_packet_class()
{
	//puste
}

int my_TCP_packet_class::get_source_port()
{
	return (int)usSourcePort;
}

int my_TCP_packet_class::get_destination_port()
{
	return usDestinationPort;
}

unsigned int my_TCP_packet_class::get_sequence_number()
{
	return uiSequenceNumber;
}

unsigned int my_TCP_packet_class::get_acknowledgement_number()
{
	//Only if ACK flag is set, we have a valid value in the acknowlegement field
	if ((usHeaderLengthAndFlags & 0x10) != 0)
	{
		return uiAcknowledgementNumber;
	}
	else
	{
		return 0;
	}
}

//put names of flags (only the set ones) to an array of 6 strings
void my_TCP_packet_class::get_flags_as_string_array(std::string* tab)
{
	int a;
	for (a = 0; a < 6; a++)
	{
		tab[a] = "";
	}
	//The last 6 bits of "data offset and flags" contain flag bits
	unsigned int flag_bits = usHeaderLengthAndFlags & 0x3F; // "..." AND "0000 0000 0011 1111" 
	//human reader will read from left to right, so first bit (and) string describe FIN
	if ((flag_bits & 0x01) != 0) // this AND will preserve the first bit
	{ 
		tab[5] = "FIN"; 
	}
	if ((flag_bits & 0x02) != 0) // this AND will preserve the second bit
	{
		tab[4] = "SYN";
	}
	if ((flag_bits & 0x04) != 0)
	{
		tab[3] = "RST";
	}
	if ((flag_bits & 0x08) != 0)
	{
		tab[2] = "PSH";
	}
	if ((flag_bits & 0x10) != 0) // 0b10000 = 16 = 0x10
	{
		tab[1] = "ACK";
	}
	if ((flag_bits & 0x20) != 0) // 0b100000 = 32 = 0x20
	{
		tab[0] = "URG";
	}
	return;
}

unsigned short my_TCP_packet_class::get_window_size()
{
	return usWindow;
}

short my_TCP_packet_class::get_checksum()
{
	return sChecksum;
}

unsigned short my_TCP_packet_class::get_UrgentPointer_bits()
{
	return usUrgentPointer;
}

int my_TCP_packet_class::get_header_length()
{
	return (int)byHeaderLength;
}

int my_TCP_packet_class::get_message_length()
{
	return MessageLength;
}
