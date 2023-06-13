#pragma once

// OknoDetali dialog

//#include "PPorembinski_inzynierska.h" //moved from *.h to *.cpp
#include "MojeFunkcjeAnalizy.h"

/*
ilosc linijek opisu jest zalezna od tego, jaki to protokol
  5 - tylko Ethernet i nic dalej
  7+1 - Ethernet i ICMP
  7+1 - Ethernet i TCP
  7+1 - Ethernet i UDP
*/
const int ilosc_linijek_opisu[] = {5, 8, 8, 8};


class OknoDetali : public CDialog
{
	DECLARE_DYNAMIC(OknoDetali)

public:
	OknoDetali(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OknoDetali();
	virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETALE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	const int min_for_slider = 0;
	const int max_for_slider = 2;
	//ulamki do wzglednej pozycji okna wzgledem rozmiaru ekranu
	const double X_pos_fr = 0.4; 
	const double Y_pos_fr = 0.5;
	//rozmiary
	const int width_for_pos = 500;
	const int height_for_pos = 400;
	//
	bool ustalono_zakres_suwaka;
	unsigned int ile_pokazywanych_pakietow;
	int chosen_SortBy; // ustalone przez suwak
	int indeks_szczegolowego;
	std::vector<HeaderPair> wektor_pokazywanych;
	CListBox pokazywane_listbox;
	CListBox szczegolowe_listbox;
	void odswiez_pokazywana_liste();
	void napelnij_listbox_odebranych();
	std::string protocol_enum_to_string(ProtNumbers pnb);
	void packet_description_as_string_array(std::string* tab, int ile_linii, ProtNumbers prot);
public:
	afx_msg void OnLbnSelchangeListaPakietow();
	CSliderCtrl SortChoiceSlider;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
