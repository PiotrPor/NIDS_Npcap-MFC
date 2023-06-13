#pragma once

#include <string>
#include <vector>
#include <winsock2.h> //dla: sockaddr

// zwykly "std::string" zmieni na "LPCTSTR"
LPCTSTR changeStringToLPCTSTR(std::string givenstring);

int typ_adresu_sockaddr(sockaddr* sad);

std::string convert_sockaddr_to_string(sockaddr* sad);

void shorten_IPaddress_string(std::string* ss);

std::string IPv4_from_array_to_string(unsigned int* tab);

bool are_IP_arrays_equal(unsigned int* t1, unsigned int* t2);

bool czy_jest_w_wektorze_int(std::vector<int>* w, int n);

bool czy_jest_w_wektorze_uint(std::vector<unsigned int>* w, unsigned int n);

void reverse_bit_order_8bit(unsigned char* bb);