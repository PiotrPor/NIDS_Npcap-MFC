#pragma once

#include <vector>

typedef unsigned char byte;

const int ile_ms_trwa_pakiet = 20;
const int limit_ilosci_pakietow = 20000; //limit ilosci odebranych pakietow
const int ile_bajtow_czyta = 65600; //ile bajtow czyta dla jednego  zapakowanego pakietu (musi byc nie wieksze od 20) (duzo internetowych jest min. 280)
const int max_per_sec = 10000; //max number of packets per second
const int max_visited_ports = 1000; //max number of different ports one can visit
const int max_pps_for_one = 1; //max ports per seconds visited by one sender
const int ICMP_packet_size_limit = 6000; //max size of ICMP packet allowed

// en.wikipedia.org/wiki/List_of_IP_protocol_numbers
enum ProtNumbers
{
    ICMP = 1,
    TCP = 6,
    UDP = 17,
    Other = -1
};

enum RodzajAlarmu
{
    Another,
    DDOS,
    PortScanning,
    PingOfDeath
};