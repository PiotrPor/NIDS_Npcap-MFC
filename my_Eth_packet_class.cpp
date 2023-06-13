#include "my_Eth_packet_class.h"

#include <sstream> //dla: std::stringstream
#include <bitset> //dla std::bitset
#include <tchar.h> //czas
#include <ctime>

// www.guru99.com/ip-header.html

my_Eth_packet_class::my_Eth_packet_class()
{
	byte_version_HeaderLength = (byte)78; // "4 | 14" -> "0100 | 1110" -> "01001110" -> 78
	byte_TOS = (byte)0; // en.wikipedia.org/wiki/Type_of_service
	bits_PacketLength = (unsigned short)(ile_bajtow_czyta + 14);
	bits_identification = (unsigned short)1;
	bits_flags_and_offset = (unsigned short)16384; // "010 | 0000000000000" -> "0100000000000000" -> 2^14 = 16384
	byte_TTL = (byte)255; //maksymalna wartosc na 8 bitach
	byte_protocol = (byte)17; //17 = UDP // en.wikipedia.org/wiki/List_of_IP_protocol_numbers
	bits_checksum = (unsigned short)0; //real-life: ile jedynek w zapisie binarnym naglowka
	source_IP_address = (unsigned int)167837954; // "10.1.1.2" --b-> 10*2^24 + 2^16 + 2^8 + 2*2^0 = 167,837,954
	destination_IP_address = (unsigned int)167837955; // "10.1.1.3" --b-> 10*2^24 + 2^16 + 2^8 + 3*2^0 = 167,837,955
	protocolOfHeader = Other;
	char* nn = czas_char;
	nn = (char*)"19:11:30   "; //symbol [11] to '\0'
	struct_reception_time.tm_sec = 30;
	struct_reception_time.tm_min = 11;
	struct_reception_time.tm_hour = 19;
	microseconds_from_second = 0;
}

// [konstruktor parametrowy jest nizej]

my_Eth_packet_class::~my_Eth_packet_class()
{
	//puste
}

/*
    pcap_pkthdr*
	const u_char* (dostaje caly pakiet jako "u_char[]" (czyli inaczej "unsigned char") (przy typie "unsigned char" bedzie error))
	const int  (prawie zbedne okreslenie jak dlugi jest pakiet)
*/
my_Eth_packet_class::my_Eth_packet_class(pcap_pkthdr pcap_naglowek, const u_char* tablica, const int rozmiar)
{
	char* one_byte = new char[1]; //[char] because std::stringstream.read() writes to char[]
	char* two_byte = new char[2];
	char* four_byte = new char[4];
	std::string s1;
	int a;
	for (a = 0; a < rozmiar; a++)
	{
		s1.push_back(tablica[a]);
	}
	// cplusplus.com/reference/sstream/stringstream/
	// stackoverflow.com/questions/2786816/how-to-create-c-istringstream-from-a-char-array-with-null0-characters
	std::stringstream strumien(s1);
	
	char* skip_2nd = new char[22];
	strumien.read(skip_2nd, 22);
	delete[] skip_2nd;

	//IP header fields:  en.wikipedia.org/wiki/Internet_Protocol_version_4#Header
	// stackoverflow.com/a/52492318
	strumien.read(one_byte, 1);
	std::memcpy(&byte_version_HeaderLength, one_byte, 1);
	//
	strumien.read(one_byte, 1);
	std::memcpy(&byte_TOS, one_byte, 1);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&bits_PacketLength, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&bits_identification, two_byte, 2);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&bits_flags_and_offset, two_byte, 2);
	//
	strumien.read(one_byte, 1);
	std::memcpy(&byte_TTL, one_byte, 1);
	//
	strumien.read(one_byte, 1);
	std::memcpy(&byte_protocol, one_byte, 1);
	//
	strumien.read(two_byte, 2);
	std::memcpy(&bits_checksum, two_byte, 2);
	//
	strumien.read(four_byte, 4);
	std::memcpy(&source_IP_address, four_byte, 4);
	//
	strumien.read(four_byte, 4);
	std::memcpy(&destination_IP_address, four_byte, 4);

	switch (byte_protocol)
	{
	case 1: 
	  {protocolOfHeader = ICMP; break; }
	case 6: 
	  {protocolOfHeader = TCP; break; }
	case 17: 
	  {protocolOfHeader = UDP; break; }
	default: 
	  {protocolOfHeader = Other; }
	}

	byte new_head_length;
	//these steps calculate the header length
	byte_header_length = byte_version_HeaderLength;
	//second half of byte contains header's length (binary operations are used)
	new_head_length = byte_header_length << 4;
	new_head_length = new_head_length >> 4;

	int header_length = (int)new_head_length;
	header_length *= 4; // this field has length as multiples of 32 bits (4 bytes)

	//copy the data carried by packet into array for these data
	//start copying from the end of the header
	length_of_data = (int)bits_PacketLength - header_length;
	for (a = header_length; (a < rozmiar && a < ile_bajtow_czyta); a++)
	{
		data_from_packet[a - header_length] = s1[a];
	}

	//reception time
	// npcap.com/guide/npcap-tutorial.html#npcap-tutorial-interpreting
	// learn.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-timeval
	//  "time_t" [long long]   ,   "tv_sec" [long]
	time_t sekundy_od = pcap_naglowek.ts.tv_sec; //ilosc sekund od poczatku 1.I.1970
	localtime_s(&struct_reception_time, &sekundy_od); //przepisz z jednej struktury do innej
	strftime(czas_char, ile_znakow_w_czasie, "%H:%M:%S", &struct_reception_time); //czas zapisany jako prawie-string w podanym formacie
	microseconds_from_second = (double)pcap_naglowek.ts.tv_usec;

	delete[] one_byte;
	delete[] two_byte;
	delete[] four_byte;
}

//---------------------------------------------------------------------------------------

int my_Eth_packet_class::czas_char_length()
{ return ile_znakow_w_czasie; }

byte my_Eth_packet_class::get_Version_8bit()
{
	return (byte)(byte_version_HeaderLength >> 4); //przesuniecie bitowe
}

int my_Eth_packet_class::get_Version_int()
{
	return (int)(byte_version_HeaderLength >> 4);
}

int my_Eth_packet_class::get_header_length()
{
	return (int)byte_header_length;
}

byte my_Eth_packet_class::get_TypeOfService_byte()
{
	return byte_TOS;
}

int my_Eth_packet_class::get_packet_total_length()
{
	return (int)bits_PacketLength;
}

unsigned short my_Eth_packet_class::get_identification()
{
	return bits_identification;
}

void my_Eth_packet_class::get_flags_arr(unsigned long* bity)
{
	bity[0] = 0; //zawsze
	// en.cppreference.com/w/cpp/utility/bitset
	// www.geeksforgeeks.org/c-bitset-and-its-application/
	std::bitset<16> wartosc(bits_flags_and_offset);
	std::bitset<16> and_1(std::string("0100000000000000"));
	std::bitset<16> and_2(std::string("0010000000000000"));
	bity[1] = (wartosc &= and_1).to_ulong();
	bity[2] = (wartosc &= and_2).to_ulong();
	return;
}

int my_Eth_packet_class::get_TTL_int()
{
	return (int)byte_TTL;
}

byte my_Eth_packet_class::get_protocol_byte()
{
	return byte_protocol;
}

int my_Eth_packet_class::get_protocol_number()
{
	return (int)byte_protocol;
}

ProtNumbers my_Eth_packet_class::get_protocol_enum()
{
	return protocolOfHeader;
}

unsigned int my_Eth_packet_class::get_sourceIP_bits()
{
	return source_IP_address;
}

void my_Eth_packet_class::get_IP_as_array(unsigned int* addtab, bool isSource)
{
	unsigned int pot = 256;
	unsigned int fulladres;
	unsigned int czesci[4];
	if (isSource)
	{
		fulladres = source_IP_address;
	}
	else
	{
		fulladres = destination_IP_address;
	}
	czesci[0] = fulladres / (pot * pot * pot);
	fulladres -= czesci[0] * (pot * pot * pot);
	czesci[1] = fulladres / (pot * pot);
	fulladres -= czesci[1] * (pot * pot);
	czesci[2] = fulladres / pot;
	fulladres -= czesci[2] * pot;
	czesci[3] = fulladres;
	//
	addtab[0] = czesci[0];
	addtab[1] = czesci[1];
	addtab[2] = czesci[2];
	addtab[3] = czesci[3];
	return;
}

unsigned int my_Eth_packet_class::get_destinationIP_bits()
{
	return destination_IP_address;
}

unsigned short my_Eth_packet_class::get_FragmentationOffset()
{
	unsigned short odstep;
	odstep = bits_flags_and_offset << 3;
	odstep >>= 3;
	return 0;
}

unsigned short my_Eth_packet_class::get_checksum_bits()
{
	return bits_checksum;
}

std::string my_Eth_packet_class::get_ReceptionTime_string()
{
	std::string czas;
	int a;
	for (a = 0; a < ile_znakow_w_czasie; a++)
	{
		czas.push_back(czas_char[a]);
	}
	int tysieczne = (int)microseconds_from_second;
	czas.push_back('.');
	czas.append(std::to_string(tysieczne));
	return czas;
}

tm my_Eth_packet_class::get_ReceptionTime_struct()
{
	return struct_reception_time;
}

double my_Eth_packet_class::get_time_microsec()
{
	return microseconds_from_second;
}

int my_Eth_packet_class::get_data_length()
{
	return length_of_data;
}

void my_Eth_packet_class::copy_data_from_packet_to_array(byte* tab)
{
	int a;
	for (a = 0; a < length_of_data && a < ile_bajtow_czyta; a++)
	{
		tab[a] = data_from_packet[a];
	}
	return;
}
