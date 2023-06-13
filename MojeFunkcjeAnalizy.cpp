#include "MojeFunkcjeAnalizy.h"

#include <ctime>

void shift_vector_left_loss(std::vector<HeaderPair>* wkt)
{
	size_t a;
	size_t rozmiar = wkt->size();
	for (a = 1; a < rozmiar; a++)
	{
		wkt->at(a-1) = wkt->at(a);
	}
	wkt->pop_back(); //ostatni jest usuwany
	return;
}

double sekundy_miedzy_pakietami(my_Eth_packet_class* pierwszy, my_Eth_packet_class* drugi) 
{
	double sekundy_miedzy = 0;
	// cplusplus.com/reference/ctime/mktime/
	tm czas_1 = pierwszy->get_ReceptionTime_struct();
	tm czas_2 = drugi->get_ReceptionTime_struct();
	time_t sekundy_pierwszego = mktime(&czas_1);
	time_t sekundy_ostatniego = mktime(&czas_2);
	// cplusplus.com/reference/ctime/difftime/
	double roznica_sekund = difftime(sekundy_ostatniego, sekundy_pierwszego);
	//milisekundy
	double us1, us2;
	us1 = pierwszy->get_time_microsec();
	us2 = drugi->get_time_microsec();
	sekundy_miedzy += (us2 - us1)/1000000;
	return sekundy_miedzy;
}

bool czy_odebrano_kiedys_taki_protokol(std::vector<HeaderPair>* wkt, ProtNumbers pp)
{
	bool czy = false;
	size_t a, roz;
	roz = wkt->size();
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == pp)
		{
			czy = true;
			break;
		}
	}
	return czy;
}

bool czyDDOS(std::vector<HeaderPair>* wkt)
{
	bool wniosek = false;
	size_t ilosc = wkt->size();
	if (ilosc < 2)
	{
		return false;
	}
	double roznica_sekund = sekundy_miedzy_pakietami(wkt->at(0).e, wkt->at(ilosc - 1).e);
	if ((double)ilosc / roznica_sekund > (double)max_per_sec)
	{
		wniosek = true;
	}
	return wniosek;
}

bool czySkanPortow(std::vector<HeaderPair>* wkt)
{
	bool werdykt = false;
	if (!czy_odebrano_kiedys_taki_protokol(wkt, ProtNumbers::TCP))
	{
		return false;
	}
	size_t roz = wkt->size();
	if (roz < 2)
	{
		return false;
	}
	//analiza
	std::vector<int> listport;
	std::vector<unsigned int> sprawdzone_adresy; //dla ktorych nadawcow juz sprawdzalismy
	my_Eth_packet_class* pak_1 = new my_Eth_packet_class(); //pierwszy i ostatni pakiet nadawcy (potrzebne do liczenia czasu)
	my_Eth_packet_class* pak_2 = new my_Eth_packet_class();
	size_t a;
	unsigned int ipv4;
	double sekundy_odstepu;
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == ProtNumbers::TCP)
		{
			ipv4 = wkt->at(a).e->get_sourceIP_bits();
			if (!czy_jest_w_wektorze_uint(&sprawdzone_adresy, ipv4)) //tego nadawcy nie sprawdzalismy
			{
				sprawdzone_adresy.push_back(ipv4);
				DajListeOdwiedzonychPortowTCPPrzezTenAdres(wkt, &listport, ipv4, pak_1, pak_2);
				if (listport.size() >= max_visited_ports)
				{
					werdykt = true;
					break;
				}
				sekundy_odstepu = sekundy_miedzy_pakietami(pak_1, pak_2);
				if ((double)listport.size() / sekundy_odstepu >= max_pps_for_one)
				{
					werdykt = true;
					break;
				}
				listport.clear();
			}
		}
	}
	delete pak_1;
	delete pak_2;
	return werdykt;
}

void DajListeOdwiedzonychPortowTCP(std::vector<HeaderPair>* wkt, std::vector<int>* odwiedzone)
{
	size_t a;
	int nr_port;
	HeaderPair temppair;
	size_t ile_odebranych = wkt->size();
	for (a = 0; a < ile_odebranych; a++)
	{
		temppair = wkt->at(a);
		if (temppair.prot == ProtNumbers::TCP)
		{
			nr_port = temppair.s.t->get_destination_port();
			if (!czy_jest_w_wektorze_int(odwiedzone, nr_port))
			{
				odwiedzone->push_back(nr_port);
			}
		}
	}
	return;
}

void DajListeOdwiedzonychPortowTCPPrzezTenAdres(std::vector<HeaderPair>* wkt, std::vector<int>* odwiedzone, unsigned int adres, my_Eth_packet_class* jego_pierwszy, my_Eth_packet_class* jego_ostatni)
{
	bool jest_pierwszym = true;
	size_t a, ile_odebranych, indeks_pierwszego = 0, indeks_ostatniego = 0;
	int spr_port;
	unsigned int spr_IP;
	HeaderPair temppair;
	ile_odebranych = wkt->size();
	for (a = 0; a < ile_odebranych; a++)
	{
		temppair = wkt->at(a);
		if (temppair.prot == ProtNumbers::TCP)
		{
			spr_IP = temppair.e->get_sourceIP_bits();
			if (spr_IP == adres)
			{
				if (jest_pierwszym)
				{
					indeks_pierwszego = a;
					jest_pierwszym = false;
				}
				indeks_ostatniego = a;
				spr_port = temppair.s.t->get_destination_port();
				if (!czy_jest_w_wektorze_int(odwiedzone, spr_port))
				{
					odwiedzone->push_back(spr_port);
				}
			}
		}
	}
	jego_pierwszy = wkt->at(indeks_pierwszego).e;
	jego_ostatni = wkt->at(indeks_ostatniego).e;
	return;
}

bool czyPingOfDeath(std::vector<HeaderPair>* wkt)
{
	bool wniosek = false;
	size_t a, roz;
	roz = wkt->size();
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == ProtNumbers::ICMP)
		{
			if (wkt->at(a).e->get_data_length() > ICMP_packet_size_limit)
			{
				wniosek = true;
				break;
			}
		}
	}
	return wniosek;
}

void sort_by_protocol(std::vector<HeaderPair>* wkt, std::vector<HeaderPair>* posortowana)
{
	size_t a, roz;
	roz = 0;
	roz = wkt->size();
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == ProtNumbers::TCP)
		{
			posortowana->push_back(wkt->at(a));
		}
	}
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == ProtNumbers::UDP)
		{
			posortowana->push_back(wkt->at(a));
		}
	}
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == ProtNumbers::ICMP)
		{
			posortowana->push_back(wkt->at(a));
		}
	}
	for (a = 0; a < roz; a++)
	{
		if (wkt->at(a).prot == ProtNumbers::Other)
		{
			posortowana->push_back(wkt->at(a));
		}
	}
	return;
}

void sort_by_IP(std::vector<HeaderPair>* wkt, std::vector<HeaderPair>* posortowana, bool czy_wedlug_zrodlowego)
{
	size_t a, b, roz;
	unsigned int ad1, ad2;
	HeaderPair tymcz;
	roz = 0;
	roz = wkt->size();
	for (a = 0; a < roz; a++)
	{
		posortowana->push_back(wkt->at(a));
	}
	//sortowanie
	for (a = 0; a < roz; a++)
	{
		for (b = 0; b < roz - a - 1; b++)
		{
			if (czy_wedlug_zrodlowego)
			{
				ad1 = posortowana->at(b).e->get_sourceIP_bits();
				ad2 = posortowana->at(b + 1).e->get_sourceIP_bits();
			}
			else
			{
				ad1 = posortowana->at(b).e->get_destinationIP_bits();
				ad2 = posortowana->at(b + 1).e->get_destinationIP_bits();
			}
			if (ad1 > ad2)
			{
				tymcz = posortowana->at(b);
				posortowana->at(b) = posortowana->at(b + 1);
				posortowana->at(b + 1) = tymcz;
			}
		}
	}
	return;
}

//od najdawniejszego do najswiezszego
void sort_by_arrival_time(std::vector<HeaderPair>* wkt, std::vector<HeaderPair>* posortowana)
{
	size_t a, b, roz;
	double roznica_czasu;
	my_Eth_packet_class* p1;
	my_Eth_packet_class* p2;
	HeaderPair tymcz;
	roz = 0;
	roz = wkt->size();
	for (a = 0; a < roz; a++)
	{
		posortowana->push_back(wkt->at(a));
	}
	for (a = 0; a < roz; a++)
	{
		for (b = 0; b < roz - a - 1; b++)
		{
			p1 = posortowana->at(b).e;
			p2 = posortowana->at(b+1).e;
			roznica_czasu = sekundy_miedzy_pakietami(p1, p2);
			if (roznica_czasu < 0) //jesli ujemne, to pierwszy przyszedl pozniej niz drugi
			{
				tymcz = posortowana->at(b);
				posortowana->at(b) = posortowana->at(b + 1);
				posortowana->at(b + 1) = tymcz;
			}
		}
	}
	return;
}
