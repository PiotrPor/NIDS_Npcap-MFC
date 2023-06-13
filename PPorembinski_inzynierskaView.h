// PPorembinski_inzynierskaView.h : interface of the CPPorembinskiinzynierskaView class
//
#pragma once

#include <string>
#include "OknoDetali.h"

//numery ID elementow interfejsu graficznego
const int ID_przycisk_stanu = 101;
const int ID_przycisk_detale = 102;

class CPPorembinskiinzynierskaView : public CView
{
protected: // create from serialization only
	CPPorembinskiinzynierskaView() noexcept;
	DECLARE_DYNCREATE(CPPorembinskiinzynierskaView)

// Attributes
public:
	CPPorembinskiinzynierskaDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPPorembinskiinzynierskaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:

//------------------------------------------------------------------------------
// (to nie koniec deklaracji klasy!!)

// Generated message map functions
// tutaj dodaje funkcje "afx_msg void ***"
protected:
	afx_msg void PoKliknieciu_PrzySt();
	afx_msg void PoKliknieciu_PrzyDe();
	DECLARE_MESSAGE_MAP()

//zmienne dodane przeze mnie
private:
	bool jest_zagrozenie; //czy jest stan alarmowy
	//do przycisku resetu alarmu
	CButton przycisk_stanu;
	const int PrzySt_szer = 100; //szerokosc przycisku stanu
	const int PrzySt_wys = 40; //wysokosc przycisku stanu
	const int PrzySt_Y = 100; //odleglosc lewego gornego rogu od gornej krawedzi
	const int PrzySt_X = 200; //odleglosc lewego gornego rogu od lewej krawedzi
	//do naczelnego napisu
	// learn.microsoft.com/en-us/cpp/mfc/reference/cstatic-class?view=msvc-170#cstatic
	CStatic narysowany_tekst;
	std::string tresc_dobrego = "Nie wykryto problemu";
	std::string tresc_zlego = "Podejrzenie ataku sieciowego";
	std::string tresc_DDOS = "Podejrzenie ataku DDOS";
	std::string tresc_skan = "Podejrzenie skanowania portow";
	std::string tresc_ping = "Podejrzenie uzycia Ping-Of-Death";
	const int NarTekst_szer = 230;
	const int NarTekst_wys = 20;
	const int NarTekst_Y = 20;
	const int NarTekst_X = 200;
	//przycisk do wyswietlenia info o pakietach
	CButton przycisk_detale;
	const int PrzyDe_szer = 200;
	const int PrzyDe_wys = 40;
	const int PrzyDe_Y = 50;
	const int PrzyDe_X = 200;
	//do obrazka
	CStatic element_obrazek;
	CImage pomocobrazka;
	const int EmObr_szer = 100;
	const int EmObr_wys = 100;
	const int EmObr_X = 50;
	const int EmObr_Y = 10;
	//testowe pole tekstowe
	CStatic pole_licznika;
	//
	CBitmap bitmapa_dobrej;
	CBitmap bitmapa_zlej;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ZmianaStanu();
	void WpiszInformacyjnieLiczbe(int nowaliczba);
	void WszczynaAlarm(RodzajAlarmu ra);
};

#ifndef _DEBUG  // debug version in PPorembinski_inzynierskaView.cpp
inline CPPorembinskiinzynierskaDoc* CPPorembinskiinzynierskaView::GetDocument() const
   { return reinterpret_cast<CPPorembinskiinzynierskaDoc*>(m_pDocument); }
#endif

