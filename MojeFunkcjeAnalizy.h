#pragma once

#include <vector>
#include "moje_inne_potrzebne.h"
#include "MojePomocniczeFunkcje.h"

//w funkcjach, ktore wkladaja informacje od drugiego wektora, ten drugi musi byc pusty i/lub swiezo stworzony

void shift_vector_left_loss(std::vector<HeaderPair>* wkt);

double sekundy_miedzy_pakietami(my_Eth_packet_class* pierwszy, my_Eth_packet_class* drugi);

bool czy_odebrano_kiedys_taki_protokol(std::vector<HeaderPair>* wkt, ProtNumbers pp);

bool czyDDOS(std::vector<HeaderPair>* wkt);

bool czySkanPortow(std::vector<HeaderPair>* wkt);

void DajListeOdwiedzonychPortowTCP(std::vector<HeaderPair>* wkt, std::vector<int>* odwiedzone);

void DajListeOdwiedzonychPortowTCPPrzezTenAdres(std::vector<HeaderPair>* wkt, std::vector<int>* odwiedzone, unsigned int adres, my_Eth_packet_class* jego_pierwszy, my_Eth_packet_class* jego_ostatni);

bool czyPingOfDeath(std::vector<HeaderPair>* wkt);

void sort_by_protocol(std::vector<HeaderPair>* wkt, std::vector<HeaderPair>* posortowana);

void sort_by_IP(std::vector<HeaderPair>* wkt, std::vector<HeaderPair>* posortowana, bool czy_wedlug_zrodlowego); //sortuje rosnaco

void sort_by_arrival_time(std::vector<HeaderPair>* wkt, std::vector<HeaderPair>* posortowana);