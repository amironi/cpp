// Lab1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lab1.h"
#include "Lab1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLab1Dlg dialog

CLab1Dlg::CLab1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLab1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLab1Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLab1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLab1Dlg)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_TXTIME, m_TXTime);
	DDX_Control(pDX, IDC_TXSIZE, m_TXSize);
	DDX_Control(pDX, IDC_TXRATE, m_TXRate);
	DDX_Control(pDX, IDC_TXNUM, m_TXNum);
	DDX_Control(pDX, IDC_RXTIME, m_RXTime);
	DDX_Control(pDX, IDC_RXSIZE, m_RXSize);
	DDX_Control(pDX, IDC_RXRATE, m_RXRate);
	DDX_Control(pDX, IDC_RXNUM, m_RXNum);
	DDX_Control(pDX, IDC_REMOTEIP, m_RemoteIP);
	DDX_Control(pDX, IDC_PORT, m_Port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLab1Dlg, CDialog)
	//{{AFX_MSG_MAP(CLab1Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SERVER, OnServer)
	ON_BN_CLICKED(IDC_CLIENT, OnClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLab1Dlg message handlers

BOOL CLab1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	char temp[16];
	m_Port.SetWindowText (itoa (DEFAULT_PORT, temp, 10));
	m_RemoteIP.SetWindowText (LOCALHOST);
	m_RXNum.SetWindowText (itoa (0, temp, 10));
	m_RXSize.SetWindowText (itoa (0, temp, 10));
	m_RXTime.SetWindowText (itoa (0, temp, 10));
	m_RXRate.SetWindowText (itoa (0, temp, 10));
	m_TXNum.SetWindowText (itoa (8, temp, 10));
	m_TXSize.SetWindowText (itoa (256, temp, 10));
	m_TXTime.SetWindowText (itoa (0, temp, 10));
	m_TXRate.SetWindowText (itoa (0, temp, 10));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLab1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLab1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLab1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLab1Dlg::OnServer() 
{
	SOCKET sServer;
	PACKET_INFO *pt = (PACKET_INFO *) malloc (sizeof (PACKET_INFO));
	char temp[16];
	sServer = socket (AF_INET, SOCK_STREAM, 0);		//	AF_INET and SOCK_STREAM defines TCP connection type.
	if (sServer < 0)
	{
		DieWithError ("Socket creation failed!");
		return;
	}
	struct sockaddr_in servAddr;
	memset (&servAddr, 0, sizeof (servAddr));		//	Resets buffer to servAddr.
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl (INADDR_ANY);	//	Let the TCP find my IP address.
	
	m_Port.GetWindowText (temp, 16);
	servAddr.sin_port = atoi (temp);
	int res = bind (sServer, (struct sockaddr *) &servAddr, sizeof (servAddr));	//	Associate a local address with a socket.
	if (res < 0)
	{
		DieWithError ("Bind failed!");
		return;
	}
	if (listen (sServer, MAX_CLIENTS) < 0)	//	Place a socket a state where it is listening for an incoming connection.
	{
		DieWithError ("Listen failed!");
		return;
	}
	pt->server = sServer;
	pt->num  = &m_RXNum;
	pt->size = &m_RXSize;
	pt->time = &m_RXTime;
	pt->rate = &m_RXRate;
	AfxBeginThread (ServerThread, pt);
}

void CLab1Dlg::DieWithError(char *err_msg)
{
	CString msg;
	msg.Format ("error:%s, error number = %d", err_msg, WSAGetLastError ());
	AfxMessageBox (msg);
}

UINT CLab1Dlg::ServerThread(LPVOID pVoid)
{
	PACKET_INFO *pt = (PACKET_INFO *) pVoid;
	char in_msg[MAXLEN] = {0}, temp[16];
	UINT timer;
	//struct sockaddr *client = (struct sockaddr *) malloc (sizeof (struct sockaddr));
	//	client holds the client's address who wants to connect to the server.
	int RXSize, PrevRXSize, RXNum = 0;
	SOCKET sClient = accept (pt->server, NULL, NULL);	//	Accepts an incoming connection attempt on a socket.
	//	sClient is the socket which connects the server's socket and client's address.
	if (sClient == INVALID_SOCKET)
		return 0;
	AfxBeginThread (ServerThread, pt);
	timer = GetTickCount ();
	while (1)
	{
		if ((RXSize = recv (sClient, in_msg, MAXLEN, 0)) == -1 || RXSize == 0)
		{
			RXSize = PrevRXSize;
			closesocket (sClient);
			break;
		}
		PrevRXSize = RXSize;
		if (send (sClient, in_msg, strlen (in_msg), 0) == -1)
		{
			closesocket (sClient);
			break;
		}
		RXNum++;
	}
	timer = GetTickCount () - timer;
	ltoa (timer, temp, 10);
	pt->time->SetWindowText (temp);
	if (timer)
		itoa (RXSize * RXNum * 8 / timer, temp, 10);
	else
		itoa (RXSize * RXNum * 8, temp, 10);
	pt->rate->SetWindowText (temp);
	itoa (RXNum, temp, 10);
	pt->num->SetWindowText (temp);
	itoa (RXSize, temp, 10);
	pt->size->SetWindowText (temp);
	return 0;
}

void CLab1Dlg::OnClient() 
{
	SOCKET sClient;
	char *out_msg, temp[16];
	UINT timer;
	int TXSize, TXNum;
	m_TXSize.GetWindowText (temp, 16);
	if (!(out_msg = (char *) malloc (atoi (temp) * sizeof (char))))
	{
		DieWithError ("Memory allocation failed!");
		return;
	}
	for (int i = 0; i < atoi (temp); i++)
		out_msg[i] = 'y';
	out_msg[atoi (temp)] = '\0';
	sClient = socket (AF_INET, SOCK_STREAM, 0);
	if (sClient < 0)
	{
		DieWithError ("Client socket failed!");
		return;
	}
	struct sockaddr_in remoteAddress;
	memset (&remoteAddress, 0, sizeof (remoteAddress));
	remoteAddress.sin_family = AF_INET;
	m_Port.GetWindowText (temp, 16);
	remoteAddress.sin_port = atoi (temp);;
	m_RemoteIP.GetWindowText (temp, 16);
	remoteAddress.sin_addr.S_un.S_addr = inet_addr (temp);
	if (connect (sClient, (struct sockaddr *) &remoteAddress, sizeof (remoteAddress)) < 0)
	{
		DieWithError ("Connect failed!");
		return;
	}
	m_TXNum.GetWindowText (temp, 16);
	timer = GetTickCount ();
	m_Progress.ShowWindow (SW_SHOW);
	m_Progress.SetRange (0, atoi (temp));
	for (i = 0; i < atoi (temp); i++)
	{
		send (sClient, out_msg, strlen (out_msg), 0);
		recv (sClient, out_msg, strlen (out_msg), 0);
		m_Progress.SetPos (i);
	}
	timer = GetTickCount () - timer;
	ltoa (timer, temp, 10);
	m_TXTime.SetWindowText (temp);
	m_TXSize.GetWindowText (temp, 16);
	TXSize = atoi (temp);
	m_TXNum.GetWindowText (temp, 16);
	TXNum = atoi (temp);
	if (timer)
		itoa (TXSize * TXNum * 8 / timer, temp, 10);
	else
		itoa (TXSize * TXNum * 8, temp, 10);
	m_TXRate.SetWindowText (temp);
	m_Progress.ShowWindow (SW_HIDE);
	closesocket (sClient);
}
