// CDDResDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SecurityGuard.h"
#include "CDDResDlg.h"
#include "afxdialogex.h"
#include "CPe.h"


const WCHAR* RES[20] = {
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
{

}

CDDResDlg::~CDDResDlg()
{
}

void CDDResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDDResDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDResDlg 消息处理程序


BOOL CDDResDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//MessageBox(0, 0, 0);

	//// 第一层：资源的类型是什么（图标，位图，菜单....）
	//// 获取资源目录
	//CPe pe;
	//pe.InitPe(L"03FileCleaner.exe");
	//PIMAGE_RESOURCE_DIRECTORY pResDirOne = pe.GetResourceDirectory();
	//// 获取资源目录项
	//PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
	//	(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
	////资源目录个数
	//DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
	////遍历第一层资源信息
	//for (int i = dwOneCount-1; i < dwOneCount; i++)
	////for (int i = 0 ; i < 1; i++)
	//{
	//	CString buffer;
	//	// 判断资源是什么类型
	//	// 资源是字符串作为标识
	//	if (pResOneEntry[i].NameIsString)
	//	{
	//		// 如果NameIsString == 1 那么 NameOffset有效
	//		PIMAGE_RESOURCE_DIR_STRING_U pName =
	//			(PIMAGE_RESOURCE_DIR_STRING_U)(pResOneEntry[i].NameOffset + (DWORD)pResDirOne);
	//		// 输出资源类型名
	//		WCHAR wResName[100];
	//		wcsncpy_s(wResName, pName->NameString, pName->Length);
	//		//wprintf(L"%s\n", wResName);
	//		buffer.Format(L"%s", wResName);
	//		MessageBox(0, buffer, 0);
	//	}
	//	// 资源是ID作为标识
	//	else 
	//	{
	//		// 系统提供的资源类型
	//		if (pResOneEntry[i].Id < 16)
	//		{
	//			wprintf(L"%s\n", RES[pResOneEntry[i].Id]);
	//			//buffer.Format(L"%s", RES[pResOneEntry[i].Id]);
	//			//MessageBox(0, buffer, 0);
	//		}
	//		else 
	//		{
	//			wprintf(L"%08x\n", pResOneEntry[i].Id);
	//			//buffer.Format(L"%s", pResOneEntry[i].Id);
	//			//MessageBox(0, buffer, 0);
	//		}
	//	}
	//	// 第二次：资源的叫什么名字 （1.png, 2.png）
	//	// 是否有第二层目录
	//	if (pResOneEntry[i].DataIsDirectory) 
	//	{
	//		// 找到第二层目录表
	//		PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
	//			(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);
	//		// 找到第二层目录项
	//		PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
	//			(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
	//		// 获取这种资源有多少个
	//		DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
	//		for (int j = 0; j < ResTowCount; j++)
	//		//for (int j = 0; j < 1; j++)
	//		{
	//			// 资源是字符串作为标识
	//			if (pResTwoEntry[j].NameIsString)
	//			{
	//				// 如果NameIsString == 1 那么 NameOffset有效
	//				PIMAGE_RESOURCE_DIR_STRING_U pName =
	//					(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
	//				// 输出资源类型名
	//				WCHAR wResName[100];
	//				wcsncpy_s(wResName, pName->NameString, pName->Length);
	//				wprintf(L"		%s\n", wResName);
	//				//buffer.Format(L"%s", wResName);
	//				//MessageBox(0, buffer, 0);
	//			}
	//			else 
	//			{
	//				printf("		%08x\n", pResTwoEntry[j].Id);
	//				//buffer.Format(L"%08X", pResTwoEntry[j].Id);
	//				//MessageBox(0, buffer, 0);
	//			}
	//			// 第三层：资源的语言，资源的信息（大小，文件中位置）
	//			if (pResTwoEntry[j].DataIsDirectory)
	//			{
	//				// 获取第三层目录表
	//				PIMAGE_RESOURCE_DIRECTORY pResDirThree =
	//					(PIMAGE_RESOURCE_DIRECTORY)(pResTwoEntry[j].OffsetToDirectory + (DWORD)pResDirOne);
	//				// 获取第三层目录项
	//				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
	//					(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);
	//				// 获取资源真正信息
	//				PIMAGE_RESOURCE_DATA_ENTRY pResData =
	//					(PIMAGE_RESOURCE_DATA_ENTRY)(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);
	//				printf("			资源位置RVA： %08X\n", pResData->OffsetToData);
	//				printf("			资源位置Size： %08X\n", pResData->Size);
	//				//buffer.Format(L"%08X", pResData->OffsetToData);
	//				//MessageBox(0, buffer, 0);
	//				//buffer.Format(L"%08X", pResData->Size);
	//				//MessageBox(0, buffer, 0);
	//			}
	//		}
	//	}
	//}




	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
