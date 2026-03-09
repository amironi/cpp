// Lab1Dlg.h : header file
//

#define DEFAULT_PORT 1975
#define LOCALHOST "127.0.0.1"
#define MAXLEN 65536
#define MAX_CLIENTS 5

#if !defined(AFX_LAB1DLG_H__BE3436C7_FA57_11D6_8C0E_0050FC38BDC3__INCLUDED_)
#define AFX_LAB1DLG_H__BE3436C7_FA57_11D6_8C0E_0050FC38BDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// PACKET_INFO

typedef struct
{
	SOCKET server;
	CEdit *num, *size, *time, *rate;
}
PACKET_INFO;


/////////////////////////////////////////////////////////////////////////////
// CLab1Dlg dialog

class CLab1Dlg : public CDialog
{
// Construction
public:
	CLab1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLab1Dlg)
	enum { IDD = IDD_LAB1_DIALOG };
	CProgressCtrl	m_Progress;
	CEdit	m_TXTime;
	CEdit	m_TXSize;
	CEdit	m_TXRate;
	CEdit	m_TXNum;
	CEdit	m_RXTime;
	CEdit	m_RXSize;
	CEdit	m_RXRate;
	CEdit	m_RXNum;
	CEdit	m_RemoteIP;
	CEdit	m_Port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLab1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLab1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnServer();
	afx_msg void OnClient();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static UINT ServerThread (LPVOID pVoid);
	void DieWithError (char *err_msg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAB1DLG_H__BE3436C7_FA57_11D6_8C0E_0050FC38BDC3__INCLUDED_)
