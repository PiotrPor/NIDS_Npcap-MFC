
// PPorembinski_inzynierska.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "PPorembinski_inzynierska.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "PPorembinski_inzynierskaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPPorembinskiinzynierskaApp

BEGIN_MESSAGE_MAP(CPPorembinskiinzynierskaApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CPPorembinskiinzynierskaApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CPPorembinskiinzynierskaApp construction

CPPorembinskiinzynierskaApp::CPPorembinskiinzynierskaApp() noexcept
{

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	//SetAppID(_T("PPorembinskiinzynierska.AppID.NoVersion"));
	SetAppID(_T("PiotrPorembinski.PracaInzynierska.v2"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CPPorembinskiinzynierskaApp object

CPPorembinskiinzynierskaApp theApp;


// CPPorembinskiinzynierskaApp initialization

BOOL CPPorembinskiinzynierskaApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Praca inzynierska - Piotr Porembinski"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_PPorembinskiinzynierskaTYPE,
		RUNTIME_CLASS(CPPorembinskiinzynierskaDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CPPorembinskiinzynierskaView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//Zmiana poczatkowej pozycji i rozmiarow glownego okna
	// learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
	// learn.microsoft.com/en-us/cpp/mfc/reference/cwnd-class?view=msvc-170#setwindowpos
	// learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
	double screen_width = (double)GetSystemMetrics(SM_CXSCREEN);
	double screen_height = (double)GetSystemMetrics(SM_CYSCREEN);
	int x2 = 0, y2 = 0;
	x2 = (int)(const_X_pos_frac * screen_width);
	y2 = (int)(const_Y_pos_frac * screen_height);
	pMainFrame->SetWindowPos(&CWnd::wndTop, x2, y2, const_szerokosc_okna, const_wysokosc_okna, SWP_SHOWWINDOW);

	//=====================================================================================
	//          dalsza czesc to tak jakby wstep do main() (reszta w OnIdle())

	
	unsigned int interface_number, a;
	int proponowany_indeks;
	char komunikat_bledu[PCAP_ERRBUF_SIZE];
	
	
	a = 0;
	ile_pakietow_odebrano = 0; // w "moje_inne_potrzebne.h"
	proponowany_indeks = 0;

	// The user didn't provide a packet source: Retrieve the local device list
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &lista_urzadzen, komunikat_bledu) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", komunikat_bledu);
		return -1;
	}

	// 4 = WiFi ; 8 = RJ45
	interface_number = 4; //za'hardcode'owane przeze mnie

	// Jump to the selected adapter
	for (karta = lista_urzadzen, a = 0; a < interface_number - 1; karta = karta->next, a++);

	std::string nazwa_karty = karta->name;
	otwarty_adapter = pcap_open(nazwa_karty.c_str(), ile_bajtow_czyta, PCAP_OPENFLAG_PROMISCUOUS, ile_ms_trwa_pakiet, NULL, komunikat_bledu);

	if (otwarty_adapter == NULL)
	{
		fprintf(stderr, "\nError opening adapter\n");
		return -1;
	}
	
	//---------------------------------------------------
	testowo_wpisywana_liczba = 0;
	// =====================================================================================

	return TRUE;
}

int CPPorembinskiinzynierskaApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	//moje
	odebrane_pakiety.clear();

	return CWinApp::ExitInstance();
}

// CPPorembinskiinzynierskaApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CPPorembinskiinzynierskaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// zditect.com/code/mfc-onidle-idle-processing.html
BOOL CPPorembinskiinzynierskaApp::OnIdle(LONG ile_w_kolejce) //'BOOL' = int
{
	struct pcap_pkthdr* aktualny_naglowek;
	const u_char* dane_pakietu; //przy "pcap_next_ex()" jest to caly pakiet
	int return_value;
	HeaderPair pomocnicza;
	//
	BOOL ile_jeszcze;
	ile_jeszcze = CWinApp::OnIdle(ile_w_kolejce);
	if (pointer_to_my_view == NULL)
	{
		return ile_jeszcze;
	}
	if (ile_w_kolejce == 0) //chyba nadal mozna cos robic, ale krotko
	{
		//tu cos robimy i nie trzeba wpisywac tego w petle
		CPPorembinskiinzynierskaApp* myprog = (CPPorembinskiinzynierskaApp*)AfxGetApp();
		CPPorembinskiinzynierskaView* mojwidok = (CPPorembinskiinzynierskaView*)myprog->get_my_view();

		// Read the packets
		// not "while(){}" loop but "if(){}" instead because "OnIdle()" makes it already behave like a loop
		if ((return_value = pcap_next_ex(otwarty_adapter, &aktualny_naglowek, &dane_pakietu)) > 0)
		{

			ile_pakietow_odebrano++;
			pomocnicza = stworz_pare(*aktualny_naglowek, dane_pakietu, ile_bajtow_czyta);
			if (ile_pakietow_odebrano > limit_ilosci_pakietow)
			{
				shift_vector_left_loss(&odebrane_pakiety);
			}
			odebrane_pakiety.push_back(pomocnicza);
			mojwidok->WpiszInformacyjnieLiczbe(ile_pakietow_odebrano);

			//analiza
			if (czyDDOS(&odebrane_pakiety))
			{
				mojwidok->WszczynaAlarm(RodzajAlarmu::DDOS);
			}
			else if (czySkanPortow(&odebrane_pakiety))
			{
				mojwidok->WszczynaAlarm(RodzajAlarmu::PortScanning);
			}
			else if (czyPingOfDeath(&odebrane_pakiety))
			{
				mojwidok->WszczynaAlarm(RodzajAlarmu::PingOfDeath);
			}
		}
		//
		ile_jeszcze = TRUE;
	}
	return ile_jeszcze;
}

void CPPorembinskiinzynierskaApp::store_my_view(CView* pointView)
{
	pointer_to_my_view = pointView;
	return;
}

CView* CPPorembinskiinzynierskaApp::get_my_view()
{
	return pointer_to_my_view;
}

HeaderPair CPPorembinskiinzynierskaApp::stworz_pare(pcap_pkthdr pcap_naglowek, const u_char* tablica, const int rozmiar)
{
	int a;
	unsigned char odwr;
	unsigned char* nowatablica = new unsigned char[rozmiar];
	for (a = 0; a < rozmiar; a++)
	{
		odwr = tablica[a];
		reverse_bit_order_8bit(&odwr);
		nowatablica[a] = odwr;
	}
	HeaderPair nowa;
	nowa.e = new my_Eth_packet_class(pcap_naglowek, nowatablica, rozmiar);
	nowa.prot = nowa.e->get_protocol_enum();
	byte dane_pierwszego[ile_bajtow_czyta];
	int dlugosc_danych_pierwszego;
	if (nowa.prot == ProtNumbers::TCP || nowa.prot == ProtNumbers::UDP || nowa.prot == ProtNumbers::ICMP)
	{
		nowa.e->copy_data_from_packet_to_array(dane_pierwszego);
		dlugosc_danych_pierwszego = nowa.e->get_data_length();
		if (nowa.prot == ProtNumbers::TCP)
		{
			nowa.s.t = new my_TCP_packet_class(dane_pierwszego, dlugosc_danych_pierwszego);
		}
		if (nowa.prot == ProtNumbers::UDP)
		{
			nowa.s.u = new my_UDP_packet_class(dane_pierwszego, dlugosc_danych_pierwszego);
		}
		if (nowa.prot == ProtNumbers::ICMP)
		{
			nowa.s.i = new my_ICMP_packet_class(dane_pierwszego, dlugosc_danych_pierwszego);
		}
	}
	delete[] nowatablica;
	return nowa;
}


// CPPorembinskiinzynierskaApp message handlers
