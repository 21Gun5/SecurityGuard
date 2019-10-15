// CDDResDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDResDlg.h"
#include "afxdialogex.h"
#include "CPe.h"


const WCHAR* RES1[20] = {
	L"光标",
	L"位图",
	L"图标",
	L"菜单",
	L"对话框",
	L"字符串列表",
	L"字体目录",
	L"字体",
	L"快捷键",
	L"非格式化资源",
	L"消息列表",
	L"鼠标指针数组",
	L"NULL",
	L"图标组",
	L"NULL",
	L"版本信息",
};

// CDDResDlg 对话框

IMPLEMENT_DYNAMIC(CDDResDlg, CDialogEx)

CDDResDlg::CDDResDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDDResDlg, pParent)
	, m_editRVA(_T(""))
	, m_editSize(_T(""))
	, m_editCurNum(_T(""))
	, m_editTotalNum(_T(""))
	, m_editType(_T(""))
{

}

CDDResDlg::~CDDResDlg()
{
}

void CDDResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT_RVA, m_editRVA);
	//  DDX_Control(pDX, IDC_EDIT_Size, m_editSize);
	DDX_Text(pDX, IDC_EDIT_Size, m_editSize);
	DDX_Text(pDX, IDC_EDIT_Size2, m_editCurNum);
	DDX_Text(pDX, IDC_EDIT_Size3, m_editTotalNum);
	DDX_Text(pDX, IDC_EDIT_Size4, m_editType);
	DDX_Control(pDX, IDC_LIST_TYPE, m_listType);
	DDX_Control(pDX, IDC_LIST_RES, m_listRes);
}


BEGIN_MESSAGE_MAP(CDDResDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TYPE, &CDDResDlg::OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RES, &CDDResDlg::OnClickListRes)
END_MESSAGE_MAP()


// CDDResDlg 消息处理程序


//BOOL CDDResDlg::OnInitDialog()
//{
//	CDialogEx::OnInitDialog();
//
//	// TODO:  在此添加额外的初始化
//
//	// 第一层：资源的类型是什么（图标，位图，菜单....）
//	// 获取资源目录
//	CPe pe;
//	pe.InitPe(L"03FileCleaner.exe");
//	PIMAGE_RESOURCE_DIRECTORY pResDirOne = pe.GetResourceDirectory();
//	// 获取资源目录项
//	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
//		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
//	//资源目录个数
//	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
//	//遍历第一层资源信息
//	//HTREEITEM typeNode[30] = {};
//	for (int i = 0; i < dwOneCount; i++)
//	{
//		HTREEITEM typeNode = NULL;
//		CString buffer;
//		// 判断资源是什么类型
//		// 资源是字符串作为标识
//		if (pResOneEntry[i].NameIsString)
//		{
//			// 如果NameIsString == 1 那么 NameOffset有效
//			PIMAGE_RESOURCE_DIR_STRING_U pName =
//				(PIMAGE_RESOURCE_DIR_STRING_U)(pResOneEntry[i].NameOffset + (DWORD)pResDirOne);
//			// 输出资源类型名
//			WCHAR wResName[100];
//			wcsncpy_s(wResName, pName->NameString, pName->Length);
//			//wprintf(L"%s\n", wResName);
//			buffer.Format(L"%s", wResName);
//			//MessageBox(0, buffer, 0);
//			typeNode = m_tree.InsertItem(buffer);
//		}
//		// 资源是ID作为标识
//		else
//		{
//			// 系统提供的资源类型
//			if (pResOneEntry[i].Id < 16)
//			{
//				//wprintf(L"%s\n", RES[pResOneEntry[i].Id]);
//				buffer.Format(L"%s", RES1[pResOneEntry[i].Id]);
//				//MessageBox(0, buffer, 0);
//				typeNode = m_tree.InsertItem(buffer);
//			}
//			else
//			{
//				//wprintf(L"%08x\n", pResOneEntry[i].Id);
//				buffer.Format(L"%08X", pResOneEntry[i].Id);
//				//MessageBox(0, buffer, 0);
//				typeNode = m_tree.InsertItem(buffer);
//			}
//		}
//		// 第二次：资源的叫什么名字 （1.png, 2.png）
//		// 是否有第二层目录
//		if (pResOneEntry[i].DataIsDirectory)
//		{
//			// 找到第二层目录表
//			PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
//				(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);
//			// 找到第二层目录项
//			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
//				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
//			// 获取这种资源有多少个
//			DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
//			for (int j = 0; j < ResTowCount; j++)
//			{
//				// 资源是字符串作为标识
//				if (pResTwoEntry[j].NameIsString)
//				{
//					// 如果NameIsString == 1 那么 NameOffset有效
//					PIMAGE_RESOURCE_DIR_STRING_U pName =
//						(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
//					// 输出资源类型名
//					WCHAR wResName[100];
//					wcsncpy_s(wResName, pName->NameString, pName->Length);
//					buffer.Format(L"%s", wResName);
//					m_tree.InsertItem(buffer, typeNode);
//				}
//				else
//				{
//					buffer.Format(L"%08X", pResTwoEntry[j].Id);
//					m_tree.InsertItem(buffer, typeNode);
//				}
//
//				//// 第三层：资源的语言，资源的信息（大小，文件中位置）
//				//if (pResTwoEntry[j].DataIsDirectory)
//				//{
//				//	// 获取第三层目录表
//				//	PIMAGE_RESOURCE_DIRECTORY pResDirThree =
//				//		(PIMAGE_RESOURCE_DIRECTORY)(pResTwoEntry[j].OffsetToDirectory + (DWORD)pResDirOne);
//				//	// 获取第三层目录项
//				//	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
//				//		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);
//				//	// 获取资源真正信息
//				//	PIMAGE_RESOURCE_DATA_ENTRY pResData =
//				//		(PIMAGE_RESOURCE_DATA_ENTRY)(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);
//				//	// 存入数组
//				//	//m_arr[i][j] = pResData->OffsetToData;
//				//	m_arr[i][j] = (int)pResTwoEntry;
//				//	//printf("			资源位置RVA： %08X\n", pResData->OffsetToData);
//				//	//printf("			资源位置Size： %08X\n", pResData->Size);
//				//	//buffer.Format(L"%08X", pResData->OffsetToData);
//				//	//MessageBox(0, buffer, 0);
//				//	//buffer.Format(L"%08X", pResData->Size);
//				//	//MessageBox(0, buffer, 0);
//				//}
//			}
//		}
//	}
//	UpdateData(TRUE);
//	m_editTotalNum.Format(_T("%d"), m_tree.GetCount());
//	UpdateData(FALSE);
//	return TRUE;  // return TRUE unless you set the focus to a control
//				  // 异常: OCX 属性页应返回 FALSE
//
//}

BOOL CDDResDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

		// 设置list扩展风格
	m_listType.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRes.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// dll 列表插入列
	m_listType.InsertColumn(0, L"Index", 0, 100);
	m_listType.InsertColumn(1, L"TypeName", 0, 250);
	// func 列表插入列
	m_listRes.InsertColumn(0, L"Index", 0, 100);
	m_listRes.InsertColumn(1, L"ResourceName", 0, 250);
	m_listRes.InsertColumn(2, L"TypeIndex", 0, 150);
	m_listRes.InsertColumn(3, L"TypeName", 0, 150);



	// 第一层：资源的类型是什么（图标，位图，菜单....）
	// 获取资源目录
	CPe pe;
	pe.InitPe(L"03FileCleaner.exe");
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = pe.GetResourceDirectory();
	// 获取资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
	//资源目录个数
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
	//遍历第一层资源信息
	//HTREEITEM typeNode[30] = {};
	int index = 0;
	//int TotalNum = 0;
	for (int i = 0; i < dwOneCount; i++)
	{
		HTREEITEM typeNode = NULL;
		CString buffer;
		m_listType.InsertItem(index, _T(""));// 插入行
		// 判断资源是什么类型
		// 资源是字符串作为标识
		if (pResOneEntry[i].NameIsString)
		{
			// 如果NameIsString == 1 那么 NameOffset有效
			PIMAGE_RESOURCE_DIR_STRING_U pName =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pResOneEntry[i].NameOffset + (DWORD)pResDirOne);
			// 输出资源类型名
			WCHAR wResName[100];
			wcsncpy_s(wResName, pName->NameString, pName->Length);
			buffer.Format(L"%d", index);
			m_listType.SetItemText(index, 0, buffer);
			buffer.Format(L"%s", wResName);
			m_listType.SetItemText(index, 1, buffer);
		}
		// 资源是ID作为标识
		else
		{
			// 系统提供的资源类型
			if (pResOneEntry[i].Id < 16)
			{
				buffer.Format(L"%d", index);
				m_listType.SetItemText(index, 0, buffer);
				buffer.Format(L"%s", RES1[pResOneEntry[i].Id]);
				m_listType.SetItemText(index, 1, buffer);
			}
			else
			{
				buffer.Format(L"%d", index);
				m_listType.SetItemText(index, 0, buffer);
				buffer.Format(L"%08X", pResOneEntry[i].Id);
				m_listType.SetItemText(index, 1, buffer);
			}
		}
		// 第二次：资源的叫什么名字 （1.png, 2.png）
		// 是否有第二层目录
		if (pResOneEntry[i].DataIsDirectory)
		{
			// 找到第二层目录表
			PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
				(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);
			// 找到第二层目录项
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
			// 获取这种资源有多少个
			//DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
			//int index2 = 0;
			//for (int j = 0; j < ResTowCount; j++)
			//{
			//	m_listRes.InsertItem(index2, _T(""));// 插入行
			//	// 资源是字符串作为标识
			//	if (pResTwoEntry[j].NameIsString)
			//	{
			//		// 如果NameIsString == 1 那么 NameOffset有效
			//		PIMAGE_RESOURCE_DIR_STRING_U pName =
			//			(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
			//		// 输出资源类型名
			//		WCHAR wResName[100];
			//		wcsncpy_s(wResName, pName->NameString, pName->Length);
			//		buffer.Format(L"%d", index2);
			//		m_listRes.SetItemText(index2, 0, buffer);
			//		buffer.Format(L"%d", index);
			//		m_listRes.SetItemText(index2, 1, buffer);
			//		buffer.Format(L"%s", wResName);
			//		m_listRes.SetItemText(index2, 2, buffer);
			//	}
			//	else
			//	{
			//		buffer.Format(L"%d", index2);
			//		m_listRes.SetItemText(index2, 0, buffer);
			//		buffer.Format(L"%d", index);
			//		m_listRes.SetItemText(index2, 1, buffer);
			//		buffer.Format(L"%08X", pResTwoEntry[j].Id);
			//		m_listRes.SetItemText(index2, 2, buffer);
			//	}
			//	//// 第三层：资源的语言，资源的信息（大小，文件中位置）
			//	//if (pResTwoEntry[j].DataIsDirectory)
			//	//{
			//	//	// 获取第三层目录表
			//	//	PIMAGE_RESOURCE_DIRECTORY pResDirThree =
			//	//		(PIMAGE_RESOURCE_DIRECTORY)(pResTwoEntry[j].OffsetToDirectory + (DWORD)pResDirOne);
			//	//	// 获取第三层目录项
			//	//	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
			//	//		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);
			//	//	// 获取资源真正信息
			//	//	PIMAGE_RESOURCE_DATA_ENTRY pResData =
			//	//		(PIMAGE_RESOURCE_DATA_ENTRY)(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);
			//	//	// 存入数组
			//	//	//m_arr[i][j] = pResData->OffsetToData;
			//	//	m_arr[i][j] = (int)pResTwoEntry;
			//	//	//printf("			资源位置RVA： %08X\n", pResData->OffsetToData);
			//	//	//printf("			资源位置Size： %08X\n", pResData->Size);
			//	//	//buffer.Format(L"%08X", pResData->OffsetToData);
			//	//	//MessageBox(0, buffer, 0);
			//	//	//buffer.Format(L"%08X", pResData->Size);
			//	//	//MessageBox(0, buffer, 0);
			//	//}
			//	index2++;
			//}
			//TotalNum = index2;
		}
		index++;
	}

	UpdateData(TRUE);
	m_editTotalNum.Format(_T("%d"), index);
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE

}





void CDDResDlg::OnClickListType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_listRes.DeleteAllItems();
	int index4Find = (int)m_listType.GetFirstSelectedItemPosition() - 1;
	CString name4Type = m_listType.GetItemText(index4Find, 1);
	DWORD index4Type = _ttoi(m_listType.GetItemText(index4Find, 0));
	CPe pe;
	pe.InitPe(L"03FileCleaner.exe");
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = pe.GetResourceDirectory();
	// 获取资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
	//资源目录个数
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
	//遍历第一层资源信息
	int index = 0;
	for (int i = 0; i < dwOneCount; i++)
	{
		if (index4Find != i)
		{
			continue;
		}
		CString buffer;
		// 是否有第二层目录
		if (pResOneEntry[i].DataIsDirectory)
		{
			// 找到第二层目录表
			PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
				(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);
			// 找到第二层目录项
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
			// 获取这种资源有多少个
			DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
			int index2 = 0;
			for (int j = 0; j < ResTowCount; j++)
			{
				m_listRes.InsertItem(index2, _T(""));// 插入行
				// 资源是字符串作为标识
				if (pResTwoEntry[j].NameIsString)
				{
					// 如果NameIsString == 1 那么 NameOffset有效
					PIMAGE_RESOURCE_DIR_STRING_U pName =
						(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
					// 输出资源类型名
					WCHAR wResName[100];
					wcsncpy_s(wResName, pName->NameString, pName->Length);
					buffer.Format(L"%d", index2);
					m_listRes.SetItemText(index2, 0, buffer);
					buffer.Format(L"%s", wResName);
					m_listRes.SetItemText(index2, 1, buffer);
					buffer.Format(L"%d", index4Type);
					m_listRes.SetItemText(index2, 2, buffer);
					m_listRes.SetItemText(index2, 3, name4Type);
				}
				else
				{
					buffer.Format(L"%d", index2);
					m_listRes.SetItemText(index2, 0, buffer);
					buffer.Format(L"%08X", pResTwoEntry[j].Id);
					m_listRes.SetItemText(index2, 1, buffer);
					buffer.Format(L"%d", index4Type);
					m_listRes.SetItemText(index2, 2, buffer);
					m_listRes.SetItemText(index2, 3, name4Type);	
				}
				index2++;
			}
			
			UpdateData(TRUE);
			m_editCurNum.Format(_T("%d"), index2);
			UpdateData(FALSE);
		}
		index++;
	}
}


void CDDResDlg::OnClickListRes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	int index4Find = (int)m_listRes.GetFirstSelectedItemPosition() - 1;
	
	DWORD index4Res = _ttoi(m_listRes.GetItemText(index4Find, 0));
	DWORD index4Type = _ttoi(m_listRes.GetItemText(index4Find, 2));
	CString name4Type = m_listRes.GetItemText(index4Find, 3);

	UpdateData(TRUE);
	m_editType.Format(_T("%s"), name4Type);
	UpdateData(FALSE);

	CPe pe;
	pe.InitPe(L"03FileCleaner.exe");
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = pe.GetResourceDirectory();
	// 获取资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
	//资源目录个数
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
	//遍历第一层资源信息
	for (int i = 0; i < dwOneCount; i++)
	{
		if (index4Type != i)
		{
			continue;
		}
		CString buffer;
		// 是否有第二层目录
		if (pResOneEntry[i].DataIsDirectory)
		{
			// 找到第二层目录表
			PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
				(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);
			// 找到第二层目录项
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
			// 获取这种资源有多少个
			DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
			
			for (int j = 0; j < ResTowCount; j++)
			{
				if (index4Res != j)
				{
					continue;
				}
				// 第三层：资源的语言，资源的信息（大小，文件中位置）
				if (pResTwoEntry[j].DataIsDirectory)
				{
					// 获取第三层目录表
					PIMAGE_RESOURCE_DIRECTORY pResDirThree =
						(PIMAGE_RESOURCE_DIRECTORY)(pResTwoEntry[j].OffsetToDirectory + (DWORD)pResDirOne);
					// 获取第三层目录项
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);
					// 获取资源真正信息
					PIMAGE_RESOURCE_DATA_ENTRY pResData =
						(PIMAGE_RESOURCE_DATA_ENTRY)(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);

					UpdateData(TRUE);
					m_editRVA.Format(_T("%08X"), pResData->OffsetToData);
					m_editSize.Format(_T("%08X"), pResData->Size);
					UpdateData(FALSE);
				}
			}
		}
	}

}
