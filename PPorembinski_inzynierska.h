
// PPorembinski_inzynierska.h : main header file for the PPorembinski_inzynierska application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
// moved by me from the *.cpp to *.h
#include "PPorembinski_inzynierskaDoc.h"
#include "PPorembinski_inzynierskaView.h"

// CPPorembinskiinzynierskaApp:
// See PPorembinski_inzynierska.cpp for the implementation of this class
//

class CPPorembinskiinzynierskaApp : public CWinApp
{
public:
	CPPorembinskiinzynierskaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	int testowo_wpisywana_liczba;
	CView* pointer_to_my_view;
	BOOL OnIdle(LONG ile_w_kolejce); //musze ja zdefiniowac/przeladowac, bo w "afxwin.h" jest tylko wirtualna deklaracja
public:
	void store_my_view(CView* pointView);
	CView* get_my_view();
	pcap_if* lista_urzadzen;
	pcap_if* karta;
	pcap_t* otwarty_adapter;
private:
	//potrzebna w odpowiedniku "main()", tworzy efekt parse'owania surowego pakietu
	HeaderPair stworz_pare(pcap_pkthdr pcap_naglowek, const u_char* tablica, const int rozmiar);
	const double const_X_pos_frac = 0.7; //ulamki potrzebne przy ustalaniu pozycji okna wzgledem rozmiaru ekranu
	const double const_Y_pos_frac = 0.65;
	const int const_wysokosc_okna = 300;
	const int const_szerokosc_okna = 500;
};

extern CPPorembinskiinzynierskaApp theApp;