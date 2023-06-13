#pragma once
#include "MojePomocniczeFunkcje.h"

#include <WS2tcpip.h> //for: inet_ntop
#include <atlstr.h> //for: CString

// www.codeproject.com/Questions/396705/How-to-convert-string-to-LPCTSTR
LPCTSTR changeStringToLPCTSTR(std::string givenstring)
{
	CString proba(givenstring.c_str());
	LPCTSTR L3 = proba;
	return L3;
}

int typ_adresu_sockaddr(sockaddr* sad)
{
	int typ = 0;
	// sa_family==2 --> IPv4
	// sa_family==23 --> IPv6
	if (sad->sa_family == 2) { typ = 4; }
	else if (sad->sa_family == 23) { typ = 6; }
	return typ;
}

std::string convert_sockaddr_to_string(sockaddr* sad)
{
	std::string pisany;
	int a;
	int dlugosc_tabelki=0;
	unsigned short rodzina = sad->sa_family;
	char* bajty_adresu = new char[1]; // to bedzie tablica dynamiczna
	bool czy_ipv4 = false;
	//
	//gist.github.com/jkomyno/45bee6e79451453c7bbdc22d033a282e
	if(typ_adresu_sockaddr(sad) == 4)
	{
		dlugosc_tabelki = 16; //adres IPv4 jest tam zapisany na 16 bajtach
		delete[] bajty_adresu;
		bajty_adresu = new char[dlugosc_tabelki];
		// stackoverflow.com/a/18579605
		sockaddr_in* s2 = reinterpret_cast<sockaddr_in*>(sad);
		inet_ntop(rodzina, &s2->sin_addr, bajty_adresu, dlugosc_tabelki);
		czy_ipv4 = true;
	}
	else if (typ_adresu_sockaddr(sad) == 6)
	{
		dlugosc_tabelki = 46; //adres IPv6 jest tam zapisany na 46 bajtach
		delete[] bajty_adresu;
		bajty_adresu = new char[dlugosc_tabelki];
		sockaddr_in6* s2 = reinterpret_cast<sockaddr_in6*>(sad);
		inet_ntop(rodzina, &s2->sin6_addr, bajty_adresu, dlugosc_tabelki);
	}
	else
	{
		//puste
	}
	//
	for (a = 0; a < dlugosc_tabelki; a++)
	{
		pisany.push_back(bajty_adresu[a]);
	}
	if (czy_ipv4) { shorten_IPaddress_string(&pisany); }
	delete[] bajty_adresu;
	return pisany;
}

//po zadzialaniu "inet_ntop()" lub czegos podobnego
//jesli adres jest krotszy niz maksymalna dlugosc bedzie mial doklejone znaki na koncu
// aktualnie string nie jest "null-terminated"
void shorten_IPaddress_string(std::string* ss)
{
	size_t a;
	int b;
	char lit;
	bool czy_chciany_znak;
	std::string pisany = *ss;
	size_t dlugosc_tabelki = pisany.size();
	char dobre[] = { '0','1','2','3','4','5','6','7','8','9','.' };
	for (a = dlugosc_tabelki - 1; a > 0; a--)
	{
		lit = pisany[a];
		czy_chciany_znak = false;
		for (b = 0; b < 11; b++)
		{
			if (dobre[b] == lit)
			{
				czy_chciany_znak = true;
				break;
			}
		}
		if (!czy_chciany_znak)
		{
			//remove 1 character, starting at 'a' index (the 'a'-th char is the first for removal)
			//string is automaticly shortened
			pisany.erase(a, 1);
			a--; //inaczej sprawdzalby [a+1] char 2 razy
		}
	}
	*ss = pisany;
	return;
}

std::string IPv4_from_array_to_string(unsigned int* tab)
{
	std::string napisany = "";
	napisany.append(std::to_string(tab[0]));
	napisany.append(".");
	napisany.append(std::to_string(tab[1]));
	napisany.append(".");
	napisany.append(std::to_string(tab[2]));
	napisany.append(".");
	napisany.append(std::to_string(tab[3]));
	return napisany;
}

bool are_IP_arrays_equal(unsigned int* t1, unsigned int* t2)
{
	bool wniosek = true;
	int a;
	for (a = 0; a < 4; a++)
	{
		if (t1[a] != t2[a])
		{
			wniosek = false;
			break;
		}
	}
	return wniosek;
}

bool czy_jest_w_wektorze_int(std::vector<int>* w, int n)
{
	bool werdykt = false;
	size_t a, b = w->size();
	for (a = 0; a < b; a++)
	{
		if (w->at(a) == n)
		{
			werdykt = true;
			break;
		}
	}
	return werdykt;
}

bool czy_jest_w_wektorze_uint(std::vector<unsigned int>* w, unsigned int n)
{
	bool werdykt = false;
	size_t a, b = w->size();
	for (a = 0; a < b; a++)
	{
		if (w->at(a) == n)
		{
			werdykt = true;
			break;
		}
	}
	return werdykt;
}

// stackoverflow.com/a/2602885
void reverse_bit_order_8bit(unsigned char* bb)
{
	/*
	W zapisie bitowym
	 - zamienia miejscami sasiednie czworki
	 - zamienia miejscami sasiednie dwojki
	 - zamienia miejscami sasiednie bity
	------------------------------
	1) abcd, efgh
	2) efgh, abcd
	3) gh, ef, cd, ab
	4) h, g, f, e, d, c, b, a
	*/
	unsigned char nn = *bb;
	nn = (nn & 0xF0) >> 4 | (nn & 0x0F) << 4;
	nn = (nn & 0xCC) >> 2 | (nn & 0x33) << 2;
	nn = (nn & 0xAA) >> 1 | (nn & 0x55) << 1;
	*bb = nn;
	return;
}
