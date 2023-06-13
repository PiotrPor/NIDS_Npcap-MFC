// OknoDetali.cpp : implementation file
//

#include "pch.h"
#include "PPorembinski_inzynierska.h"
#include "OknoDetali.h"
#include "afxdialogex.h"

// OknoDetali dialog

IMPLEMENT_DYNAMIC(OknoDetali, CDialog)

OknoDetali::OknoDetali(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DETALE, pParent)
	, chosen_SortBy(2) //domyslnie wybrana opcja: sortuj wg czasu odebrania
	, indeks_szczegolowego(-1) //wiadomo, ze narazie zadnego nie opiszemy
{
	//puste
}

OknoDetali::~OknoDetali()
{
}

// dodane przeze mnie mniej wiecej wedlug porad z linku ponizej
// stackoverflow.com/a/961880
BOOL OknoDetali::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add extra initialization below
	
	//ustawienie pozycji i rozmiaru okna
	// learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-movewindow
	// learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
	double screen_width = (double)GetSystemMetrics(SM_CXSCREEN);
	double screen_height = (double)GetSystemMetrics(SM_CYSCREEN);
	int x2 = 0, y2 = 0;
	x2 = (int)(X_pos_fr * screen_width);
	y2 = (int)(Y_pos_fr * screen_height);
	this->MoveWindow(x2, y2, width_for_pos, height_for_pos, TRUE);

	//wypelnienie listy pakietow (tej do wypelnienia listy)
	size_t a, roz;
	roz = odebrane_pakiety.size();
	wektor_pokazywanych.clear();
	ile_pokazywanych_pakietow = 0;
	for (a = 0; a < roz; a++)
	{
		wektor_pokazywanych.push_back(odebrane_pakiety.at(a));
		ile_pokazywanych_pakietow++;
	}
	//-------------------
	//
	ustalono_zakres_suwaka = false;
	SortChoiceSlider.SetRange(min_for_slider, max_for_slider, TRUE); //wartosci od 0 do 2
	SortChoiceSlider.SetPos(2); //domyslna wartosc startowa
	ustalono_zakres_suwaka = true;

	napelnij_listbox_odebranych();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void OknoDetali::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTA_PAKIETOW, pokazywane_listbox);
	DDX_Control(pDX, IDC_LIST2, szczegolowe_listbox);
	DDX_Control(pDX, IDC_SORT_SLIDER, SortChoiceSlider);
}


BEGIN_MESSAGE_MAP(OknoDetali, CDialog)
	ON_LBN_SELCHANGE(IDC_LISTA_PAKIETOW, &OknoDetali::OnLbnSelchangeListaPakietow)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// OknoDetali message handlers


void OknoDetali::odswiez_pokazywana_liste()
{
	wektor_pokazywanych.clear(); //dla funkcji od sortowania wektor na posortowane musi byc pusty
	if (chosen_SortBy == 0) //opcja 1: wg protokolu
	{
		sort_by_protocol(&odebrane_pakiety, &wektor_pokazywanych);
	}
	if (chosen_SortBy == 1) //opcja 2: wg adresu IPv4 nadawcy
	{
		sort_by_IP(&odebrane_pakiety, &wektor_pokazywanych, true);
	}
	if (chosen_SortBy == 2) //opcja 3: wg czasu odebrania (od najwczesniejszego do najswiezszego)
	{
		sort_by_arrival_time(&odebrane_pakiety, &wektor_pokazywanych);
	}
	napelnij_listbox_odebranych();
	return;
}

void OknoDetali::napelnij_listbox_odebranych()
{
	unsigned int a;
	std::string linijka;
	LPCTSTR nowa; //metoda dodawania string przyjmuje LPCTSTR
	while (pokazywane_listbox.GetCount() > 0) 
	{
		pokazywane_listbox.DeleteString(0);
	}
	for (a = 0; a < ile_pokazywanych_pakietow; a++)
	{
		linijka = std::to_string(a + 1) + ")  ";
		if (chosen_SortBy == 0)
		{
			ProtNumbers ptnb = wektor_pokazywanych.at(a).e->get_protocol_enum();
			linijka += protocol_enum_to_string(ptnb);
		}
		if (chosen_SortBy == 1)
		{
			unsigned int tabelka[4];
			wektor_pokazywanych.at(a).e->get_IP_as_array(tabelka, true);
			linijka += IPv4_from_array_to_string(tabelka);
		}
		if (chosen_SortBy == 2)
		{
			linijka += wektor_pokazywanych.at(a).e->get_ReceptionTime_string();
		}
		// learn.microsoft.com/en-us/cpp/mfc/reference/clistbox-class?view=msvc-170#addstring
		nowa = changeStringToLPCTSTR(linijka);
		pokazywane_listbox.AddString(nowa);
	}
	return;
}

std::string OknoDetali::protocol_enum_to_string(ProtNumbers pnb)
{
	std::string pisemnie = "Other";
	if (pnb == ProtNumbers::ICMP) { pisemnie = "ICMP"; }
	if (pnb == ProtNumbers::TCP) { pisemnie = "TCP"; }
	if (pnb == ProtNumbers::UDP) { pisemnie = "UDP"; }
	return pisemnie;
}

void OknoDetali::packet_description_as_string_array(std::string* tab, int ile_linii, ProtNumbers prot)
{
	if (indeks_szczegolowego < 0) //indeks opisywanego musi byc nieujemny
	{
		return;
	}
	//przygotowanie linijek z opisami
	tab[0] = "Czas odebrania:  ";
	tab[1] = "Protokol:  ";
	tab[2] = "IP nadawcy:  ";
	tab[3] = "IP docelowy  ";
	//doklejanie konkretnych informacji
	HeaderPair opisywany;
	opisywany = wektor_pokazywanych.at(indeks_szczegolowego);
	tab[0] += opisywany.e->get_ReceptionTime_string();
	tab[1] += protocol_enum_to_string(opisywany.e->get_protocol_enum());
	unsigned int adres[4];
	opisywany.e->get_IP_as_array(adres, true);
	tab[2] += IPv4_from_array_to_string(adres);
	opisywany.e->get_IP_as_array(adres, false);
	tab[3] += IPv4_from_array_to_string(adres);
	//--------
	if (prot != ProtNumbers::Other && ile_linii > 4)
	{
		tab[4] = "----------------";
		if (prot == ProtNumbers::ICMP)
		{
			tab[5] = "Typ:  " + std::to_string((unsigned int)(opisywany.s.i->get_type()));
			tab[6] = "Kod:  " + std::to_string((unsigned int)(opisywany.s.i->get_code()));
		}
		if (prot == ProtNumbers::TCP)
		{
			tab[5] = "Port nadawcy:  " + std::to_string((unsigned int)(opisywany.s.t->get_source_port()));
			tab[6] = "Port docelowy:  " + std::to_string((unsigned int)(opisywany.s.t->get_destination_port()));
		}
		if (prot == ProtNumbers::UDP)
		{
			tab[5] = "Port nadawcy:  " + std::to_string((unsigned int)(opisywany.s.u->get_source_port_int()));
			tab[6] = "Port docelowy:  " + std::to_string((unsigned int)(opisywany.s.u->get_destination_port_int()));
		}
	}
	return;
}

void OknoDetali::OnLbnSelchangeListaPakietow()
{
	// TODO: Add your control notification handler code here
	
	// learn.microsoft.com/en-us/cpp/mfc/reference/clistbox-class?view=msvc-170#getcursel
	indeks_szczegolowego = pokazywane_listbox.GetCurSel();
	//wyczysci listbox na szczegolowy opis
	while (szczegolowe_listbox.GetCount() > 0)
	{
		szczegolowe_listbox.DeleteString(0);
	}
	int wszystkich = 0;
	ProtNumbers jegoprot = wektor_pokazywanych[indeks_szczegolowego].prot;
	if(jegoprot == ProtNumbers::ICMP)
	{
		wszystkich = ilosc_linijek_opisu[1];
	}
	else if (jegoprot == ProtNumbers::TCP)
	{
		wszystkich = ilosc_linijek_opisu[2];
	}
	else if (jegoprot == ProtNumbers::UDP)
	{
		wszystkich = ilosc_linijek_opisu[3];
	}
	else
	{
		wszystkich = ilosc_linijek_opisu[0];
	}
	std::string* linijki_opisu = new std::string[wszystkich];
	packet_description_as_string_array(linijki_opisu, wszystkich, jegoprot);
	LPCTSTR kolejna;
	int a;
	for (a = 0; a < wszystkich; a++)
	{
		kolejna = changeStringToLPCTSTR(linijki_opisu[a]);
		szczegolowe_listbox.AddString(kolejna); //metoda x.AddString() przyjmuje LPCTSTR jako argument
	}
	delete[] linijki_opisu;
	int nieuzywana_liczba = szczegolowe_listbox.GetCount();
	return;
}

// www.tutorialspoint.com/mfc/mfc_slider_controls.htm
//funkcja dodana przy uzyciu podmenu z "Messages" w widoku klas w eksploratorze 
// ta obsluguje pionowy suwak, OnHScroll() obsluguje poziomy
void OknoDetali::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)&SortChoiceSlider) //sprawdza czy wezwania przychodzi od wlasciwego slider'a
	{
		chosen_SortBy = SortChoiceSlider.GetPos();
		odswiez_pokazywana_liste();
		UpdateData(FALSE); //dzieki temu wartosci zmiennych sa przekazane miedzy kontrolerami i ich zmiennymi
	}
	else
	{
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	}
	return;
}



