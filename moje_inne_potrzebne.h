#pragma once

#include <vector>
//to jest tu odlozone, zeby nie bylo petli include'owania
#include "my_Eth_packet_class.h"
#include "my_TCP_packet_class.h"
#include "my_UDP_packet_class.h"
#include "my_ICMP_packet_class.h"

union SecondPacket
{
    my_TCP_packet_class* t; //musza byc wskazniki bo inaczej jest error
    my_UDP_packet_class* u;
    my_ICMP_packet_class* i;
};

struct HeaderPair
{
    my_Eth_packet_class* e;
    SecondPacket s;
    ProtNumbers prot;
};

//dziala jak zmienna globalna
// stackoverflow.com/questions/10422034/when-to-use-extern-in-c
// w pliku "*.cpp" jest deklaracja, bo trzeba ja w takiej sytuacji dopisac
extern std::vector<HeaderPair> odebrane_pakiety;
extern int ile_pakietow_odebrano;
