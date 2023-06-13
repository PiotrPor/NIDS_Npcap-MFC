// PPorembinski_inzynierskaView.cpp : implementation of the CPPorembinskiinzynierskaView class
//

#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PPorembinski_inzynierska.h"
#endif

#include "PPorembinski_inzynierskaDoc.h"
#include "PPorembinski_inzynierskaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPPorembinskiinzynierskaView

IMPLEMENT_DYNCREATE(CPPorembinskiinzynierskaView, CView)

// TU PRZYPISUJ FUNKCJE DO KLAWISZY/ELEMENTOW
BEGIN_MESSAGE_MAP(CPPorembinskiinzynierskaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_przycisk_stanu, PoKliknieciu_PrzySt)
	ON_BN_CLICKED(ID_przycisk_detale, PoKliknieciu_PrzyDe)
END_MESSAGE_MAP()

// CPPorembinskiinzynierskaView construction/destruction

CPPorembinskiinzynierskaView::CPPorembinskiinzynierskaView() noexcept
{
	// TODO: add construction code here

	jest_zagrozenie = false;
	// www.codeproject.com/articles/907/cview-access-from-anywhere
	CPPorembinskiinzynierskaApp* my_app = (CPPorembinskiinzynierskaApp*)AfxGetApp();
	my_app->store_my_view(this);
}

CPPorembinskiinzynierskaView::~CPPorembinskiinzynierskaView()
{
}

BOOL CPPorembinskiinzynierskaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPPorembinskiinzynierskaView drawing

void CPPorembinskiinzynierskaView::OnDraw(CDC* /*pDC*/)
{
	CPPorembinskiinzynierskaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPPorembinskiinzynierskaView printing

BOOL CPPorembinskiinzynierskaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPPorembinskiinzynierskaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPPorembinskiinzynierskaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPPorembinskiinzynierskaView diagnostics

#ifdef _DEBUG
void CPPorembinskiinzynierskaView::AssertValid() const
{
	CView::AssertValid();
}

void CPPorembinskiinzynierskaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPPorembinskiinzynierskaDoc* CPPorembinskiinzynierskaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPPorembinskiinzynierskaDoc)));
	return (CPPorembinskiinzynierskaDoc*)m_pDocument;
}
#endif //_DEBUG


// CPPorembinskiinzynierskaView message handlers


int CPPorembinskiinzynierskaView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// [ Add your specialized creation code before ]

	// learn.microsoft.com/en-us/cpp/mfc/reference/cbutton-class?view=msvc-170#create
	// learn.microsoft.com/en-us/cpp/atl-mfc-shared/reference/crect-class?view=msvc-170#crect
	przycisk_stanu.Create(
		_T("Reset alarmu"),
		BS_PUSHBUTTON,
		CRect(PrzySt_X, PrzySt_Y, PrzySt_X + PrzySt_szer, PrzySt_Y + PrzySt_wys), //wspolrzedne sa 'const int' w klasie
		this, // "parent window" to okno w ktorym jestesmy
		ID_przycisk_stanu
	);
	przycisk_stanu.ShowWindow(SW_SHOW);

	//wielki tekst/komunikat
	// social.msdn.microsoft.com/Forums/vstudio/en-US/7c746a9a-2763-4ac3-9780-0b4258d6bb6c/how-to-set-cstatic-text-value-on-mfc?forum=vcgeneral
	narysowany_tekst.Create(
		_T("wielki napis tu"),
		WS_VISIBLE,
		CRect(NarTekst_X, NarTekst_Y, NarTekst_X + NarTekst_szer, NarTekst_Y + NarTekst_wys),
		this
	);
	narysowany_tekst.SetWindowTextW(_T("Nie wykryto problemu"));

	//klawisz do wyswietlenia detali
	przycisk_detale.Create(
		_T("Detale o pakietach"),
		BS_PUSHBUTTON,
		CRect(PrzyDe_X, PrzyDe_Y, PrzyDe_X + PrzyDe_szer, PrzyDe_Y + PrzyDe_wys),
		this,
		ID_przycisk_detale
	);
	przycisk_detale.ShowWindow(SW_SHOW);

	//obrazek
	element_obrazek.Create(
		_T("Brak obrazka"),
		WS_VISIBLE,
		CRect(EmObr_X, EmObr_Y, EmObr_X + EmObr_szer, EmObr_Y + EmObr_wys),
		this
	);

	// learn.microsoft.com/en-us/cpp/mfc/reference/cstatic-class?view=msvc-170#setbitmap
	// stackoverflow.com/questions/59031765/set-bitmap-into-cstatic-object
	// 9to5answer.com/load-a-png-resource-into-a-cbitmap
	CImage img_1, img_2;
	img_1.Load(changeStringToLPCTSTR("sit_good_100x100.bmp"));
	img_2.Load(changeStringToLPCTSTR("sit_bad_100x100.bmp"));
	bitmapa_dobrej.Attach(img_1.Detach()); //odczepianie, bo inaczej destruktor CImage usunie obrazek
	bitmapa_zlej.Attach(img_2.Detach());
	element_obrazek.ModifyStyle(SS_ENHMETAFILE, SS_BITMAP);
	//na poczatek ustawienie konkretnego poczatkowego obrazka
	element_obrazek.SetBitmap(bitmapa_dobrej);

	// testowe pole do debug
	pole_licznika.Create(
		_T("test"),
		WS_VISIBLE,
		CRect(50, 150, 200, 180),
		this
	);
	//narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_dobrego)); //daje blad
	pole_licznika.SetWindowTextW(_T("odebrane: 0\nanalizowane: 0"));

	return 0;
}

//--------------
// PODODAWANE PRZEZE MNIE FUNKCJE

void CPPorembinskiinzynierskaView::PoKliknieciu_PrzySt()
{
	ZmianaStanu();
	return;
}

void CPPorembinskiinzynierskaView::PoKliknieciu_PrzyDe()
{
	OknoDetali szczegolowe;
	szczegolowe.DoModal();
	return;
}

void CPPorembinskiinzynierskaView::ZmianaStanu()
{
	if (jest_zagrozenie)
	{
		jest_zagrozenie = false;
		narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_dobrego));
		element_obrazek.SetBitmap(bitmapa_dobrej);
	}
	else
	{
		jest_zagrozenie = true;
		narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_zlego));
		element_obrazek.SetBitmap(bitmapa_zlej);
	}
	return;
}

void CPPorembinskiinzynierskaView::WpiszInformacyjnieLiczbe(int nowaliczba)
{
	std::string copisac = "odebrane: " + std::to_string(nowaliczba);
	copisac += "\nanalizowane: " + std::to_string(odebrane_pakiety.size());
	pole_licznika.SetWindowTextW(changeStringToLPCTSTR(copisac));
	return;
}

void CPPorembinskiinzynierskaView::WszczynaAlarm(RodzajAlarmu ra)
{
	jest_zagrozenie = true;
	element_obrazek.SetBitmap(bitmapa_zlej);
	//beda ustawiane rozne komunikaty w zaleznosci od rodzaju ataku
	if (ra == RodzajAlarmu::DDOS)
	{
		narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_DDOS));
	}
	if (ra == RodzajAlarmu::PortScanning)
	{
		narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_skan));
	}
	if (ra == RodzajAlarmu::PingOfDeath)
	{
		narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_ping));
	}
	if (ra == RodzajAlarmu::Another)
	{
		narysowany_tekst.SetWindowTextW(changeStringToLPCTSTR(tresc_zlego));
	}
	return;
}
