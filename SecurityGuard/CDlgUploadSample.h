#pragma once

#define SECTION_SIZE (1024)

typedef struct _SEND_FILE_INFO
{
	char sign[9] = { "FileInfo" };
	char name[MAX_PATH];		// 文件的名称
	int section_count;			// 发送文件需要多少次
	int file_size;
	char file_md5[33]{ 0 };
	char file_description[100]{ 0 }; 
} SEND_FILE_INFO, * PSEND_FILE_INFO;

typedef struct _FILE_SECTION
{
	int index;					// 当前是第几块数据
	DWORD size;					// 当前这块数据的大小
	char data[SECTION_SIZE];	// 一次传输的大小
} FILE_SECTION, * PFILE_SECTION;

class CDlgUpload : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUpload)

public:
	CDlgUpload(CWnd* pParent = nullptr);   
	virtual ~CDlgUpload();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UPLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddFile();
	afx_msg void OnBnClickedButtonAddDir();
	virtual BOOL OnInitDialog();
	CListCtrl m_cListCtrl;
	SOCKET client;
	CEdit m_cEditFileFullPath;
	CEdit m_cEditFileMD5;
	CEdit m_cEditFileDescription;
	CString m_strFileName;
<<<<<<< HEAD
=======
	BOOL m_bCloud;
>>>>>>> v1.0
	afx_msg void OnDeleteItem();
	afx_msg void OnDownload();
	afx_msg void OnNMRClickList(NMHDR* pNMHDR, LRESULT* pResult);
};
