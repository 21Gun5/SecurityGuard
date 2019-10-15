#include "stdafx.h"
#include <stdio.h>
#include "CPe.h"

char * g_PeBuff = NULL;

bool CPe::InitPe(TCHAR * path)
{
	// PE文件保存路径
	m_path = path;

	//1. 打开PE文件
	HANDLE hFile = CreateFile(
		path,		//文件路径
		GENERIC_READ,	//打开方式（读）
		FILE_SHARE_READ,//共享方式
		NULL,			//安全属性
		OPEN_EXISTING,  //打开的文件必须存在
		FILE_ATTRIBUTE_NORMAL, //文件属性
		NULL
	);

	// 获取文件大小
	m_size = GetFileSize(hFile, 0);
	// 申请空间
	m_buff = new char[m_size];
	DWORD dwReadSize;

	//2. 把PE文件读取到内存
	ReadFile(hFile, m_buff, m_size, &dwReadSize, 0);

	g_PeBuff = m_buff;

	//3. 判断是否是PE文件
	return IsPeFile(m_buff);

}
bool CPe::IsPeFile(char * pbuff)
{
	// 判断是否PE文件 
	// DOS.e_magic  
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	// 判断是否MZ 
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}
	// NT.Signature 是否等于 0x00004550  ‘PE00’
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}
	// 是一个PE文件
	return true;
}
DWORD CPe::RvaToFoa(DWORD dwRva)
{
	// 1. 判断这个RVA落在什么区段
	// 获取区段表
	PIMAGE_SECTION_HEADER pSection = GetSectionHeader();
	// 获取区段个数
	PIMAGE_FILE_HEADER pFileHeader = &GetNtHeader()->FileHeader;
	DWORD dwSectionCount = pFileHeader->NumberOfSections;
	DWORD Offset = 0;


	// 循环判断RVA落在什么区段
	for (int i = 0; i < dwSectionCount; i++)
	{
		// RVA > 这个区段所在的RVA && 
		// RVA<= 区段所在的RVA+区段在文件中的大小
		if ((dwRva >= pSection[i].VirtualAddress) &&
			dwRva < (pSection[i].VirtualAddress + pSection[i].SizeOfRawData))
		{
			// 计算公式  offset = RVA - 区段所在的RVA + 区段在文件中的偏移
			Offset = dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return Offset;
}

//获取DOS头
PIMAGE_DOS_HEADER CPe::GetDosHeader()
{
	return (PIMAGE_DOS_HEADER)m_buff;
	//if(isLoad)
	//	return (PIMAGE_DOS_HEADER)g_PeBuff;
	//else
	//	return (PIMAGE_DOS_HEADER)m_buff;
}
PIMAGE_NT_HEADERS CPe::GetNtHeader()
{
	// 获取DOS头
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	// 获取NT头  =  DOS.e_lfanew + 文件起始位置buff
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)m_buff);
	return pNt;
}
PIMAGE_SECTION_HEADER CPe::GetSectionHeader()
{
	// 获取NT头
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// 获取区段表
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);

	return pSection;
}
PIMAGE_EXPORT_DIRECTORY CPe::GetExportDirectory()
{
	// 通过数据目录表第 0 项
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// 获取导出表的RVA
	DWORD ExportRva = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	// 转FOA
	DWORD dwOffset = RvaToFoa(ExportRva);
	// 导出表的地址
	PIMAGE_EXPORT_DIRECTORY pExport =
		(PIMAGE_EXPORT_DIRECTORY)(dwOffset + (DWORD)m_buff);

	return pExport;
}
PIMAGE_IMPORT_DESCRIPTOR CPe::GetImportDirectory()
{
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	DWORD dwImportRva = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	DWORD dwOffset = RvaToFoa(dwImportRva);
	PIMAGE_IMPORT_DESCRIPTOR pImport =(PIMAGE_IMPORT_DESCRIPTOR)(dwOffset + (DWORD)m_buff);
	return pImport;
	//return PIMAGE_IMPORT_DESCRIPTOR();
}
PIMAGE_RESOURCE_DIRECTORY CPe::GetResourceDirectory()
{
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	DWORD dwRva = pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	DWORD dwOffset = RvaToFoa(dwRva);
	PIMAGE_RESOURCE_DIRECTORY pRes =(PIMAGE_RESOURCE_DIRECTORY)(dwOffset + m_buff);
	return pRes;
}


PIMAGE_BASE_RELOCATION CPe::GetRelocalDirectory()
{
	// 获取数据目录表第 5项
	DWORD dwRva = GetNtHeader()->OptionalHeader.DataDirectory[5].VirtualAddress;

	// 获取重定位表结构体
	PIMAGE_BASE_RELOCATION pRelocal =
		(PIMAGE_BASE_RELOCATION)(RvaToFoa(dwRva) + m_buff);

	return pRelocal;
}

void CPe::ShowImportInfo()
{
	// 获取导入表结构体数组
	PIMAGE_IMPORT_DESCRIPTOR pImport = GetImportDirectory();
	// 数组以0结尾
	while (pImport->Name)
	{
		// 显示dll名字
		char * pDllname = (char*)(RvaToFoa(pImport->Name) + (DWORD)m_buff);
		printf("%s\n", pDllname);

		// 获取IAT
		PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)
			(RvaToFoa(pImport->OriginalFirstThunk) + (DWORD)m_buff);

		// 遍历IAT中函数
		while (pIAT->u1.Ordinal)
		{
			// 判断导入的函数是名称导入还是序号导入
			// 判断最高位是否位1 ，如果为1那么需要导入
			if (pIAT->u1.Ordinal & 0x80000000)
			{
				printf("序号 %d\n", pIAT->u1.Function & 0x7FFFFFFF);
			}
			else {
				// 导入的函数是名称
				PIMAGE_IMPORT_BY_NAME pName =
					(PIMAGE_IMPORT_BY_NAME)(RvaToFoa(pIAT->u1.AddressOfData) + (DWORD)m_buff);
				printf("序号 %d  函数名：%s\n", pName->Hint, pName->Name);

			}
			// 解析下一个函数
			pIAT++;
		}
		// 解析下一个导入表
		pImport++;
	}

}
void CPe::ShowOptionHeader()
{
	// 1.获取扩展头
	PIMAGE_NT_HEADERS pNt = GetNtHeader();

	//显示 扩展头中的数据
	printf("OEP: %08x\nImageBase %08x\n",
		pNt->OptionalHeader.AddressOfEntryPoint,
		pNt->OptionalHeader.ImageBase
	);

	// 显示数据目录表
	for (int i = 0; i < 16; i++)
	{
		printf("Directory[%d].VirtualAddress = %08x  ",
			i,
			pNt->OptionalHeader.DataDirectory[i].VirtualAddress);

		printf("Directory[%d].Size = %08x \n",
			i,
			pNt->OptionalHeader.DataDirectory[i].Size);
	}

}
void CPe::ShowSectionInfo()
{
	// 获取区段表信息
	PIMAGE_SECTION_HEADER pSection = GetSectionHeader();

	// 获取区段个数
	DWORD dwSize = GetNtHeader()->FileHeader.NumberOfSections;

	// 遍历区段表信息
	for (int i = 0; i < dwSize; i++)
	{
		printf("Name :%s", pSection[i].Name);
		printf(" VSize:%08X", pSection[i].Misc.VirtualSize);
		printf(" VOffset:%08X", pSection[i].VirtualAddress);
		printf(" RSize:%08X", pSection[i].SizeOfRawData);
		printf(" ROffset:%08X", pSection[i].PointerToRawData);
		printf("\n");
	}

}
void CPe::ShowExportInfo()
{
	// 获取导出表信息
	PIMAGE_EXPORT_DIRECTORY pExport = GetExportDirectory();

	// 解析dll名字  RVA
	char * pDllName = (char*)(RvaToFoa(pExport->Name) + m_buff);
	printf("%s\n", pDllName);

	// 导出地址表 
	DWORD * pEAT = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + m_buff);
	// 导出名称表
	DWORD * pENT = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + m_buff);
	// 导出序号表 元素是word
	WORD * pEOT = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + m_buff);
	// 导出地址表个数
	DWORD dwAddrCount = pExport->NumberOfFunctions;
	// 导出名称表个数
	DWORD dwNameCount = pExport->NumberOfNames;

	// 遍历地址表
	for (int i = 0; i < dwAddrCount; i++)
	{
		//输出函数序号
		printf("序号 %d ", i + pExport->Base);

		// 遍历序号表
		for (int j = 0; j < dwNameCount; j++)
		{
			// 序号表中有地址表的序号 （这个函数是有名字的）
			if (i == pEOT[j]) {
				// 输出这个函数名字 
				char * pFunName =
					(char *)(RvaToFoa(pENT[j]) + m_buff);
				printf("%s", pFunName);
				break;
			}
		}

		printf("\n");
	}

}
void CPe::ShowResourceInfo()
{
	// 第一层：资源的类型是什么（图标，位图，菜单....）
	// 获取资源目录
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = GetResourceDirectory();

	// 获取资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);

	//资源目录个数
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;


	//遍历第一层资源信息
	for (int i = 0; i < dwOneCount; i++)
	{
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
			wprintf(L"%s\n", wResName);
		}
		// 资源是ID作为标识
		else {
			// 系统提供的资源类型
			if (pResOneEntry[i].Id < 16)
			{
				//wprintf(L"%s\n", RES[pResOneEntry[i].Id]);//here
			}
			else {
				wprintf(L"%08x\n", pResOneEntry[i].Id);
			}

		}


		// 第二次：资源的叫什么名字 （1.png, 2.png）
		// 是否有第二层目录
		if (pResOneEntry[i].DataIsDirectory) {

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
				// 资源是字符串作为标识
				if (pResTwoEntry[j].NameIsString)
				{
					// 如果NameIsString == 1 那么 NameOffset有效
					PIMAGE_RESOURCE_DIR_STRING_U pName =
						(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
					// 输出资源类型名
					WCHAR wResName[100];
					wcsncpy_s(wResName, pName->NameString, pName->Length);
					wprintf(L"		%s\n", wResName);
				}
				else {
					printf("		%08x\n", pResTwoEntry[j].Id);
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

					printf("			资源位置RVA： %08X\n", pResData->OffsetToData);
					printf("			资源位置Size： %08X\n", pResData->Size);
				}
			}
		}
	}
}
void CPe::ShowRelocalInfo()
{
	typedef	struct {
		WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;   // (2) 大小为4Bit的重定位信息类型值
	}*PTYPEOFFSET;	    // 这个结构体是A1Pass总结的


	// 获取重定位表
	PIMAGE_BASE_RELOCATION pRelocal = GetRelocalDirectory();

	// 遍历所有的重定位数据
	while (pRelocal->SizeOfBlock)
	{
		// 获取重定位项个数
		DWORD dwCount = (pRelocal->SizeOfBlock - 8) / 2;

		//（0x1000字节中的需要重定位数据的偏移）
		printf("virtualaddress: %08x\n", pRelocal->VirtualAddress);
		printf("Items: %d\n", dwCount);
		// 获取重定位块
		PTYPEOFFSET pRelocalBlock = (PTYPEOFFSET)(pRelocal + 1);
		// 遍历重定位项数据
		for (int i = 0; i < dwCount; i++)
		{
			// （x32位PE文件）类型为0x03	表示需要重定位的数据  
			// （x64位PE文件）类型为0x10	表示需要重定位的数据  
			if (pRelocalBlock[i].Type == 3)
			{
				// 要修复数据的RVA
				DWORD Rva = pRelocalBlock[i].Offset + pRelocal->VirtualAddress;
				printf("	RVA:%08X\n", Rva);
				printf("	FOA:%08X\n", RvaToFoa(Rva));
				printf("\n");
				// 修复重定位为0x00800000
				*(DWORD*)(RvaToFoa(Rva) + m_buff) =
					*(DWORD*)(RvaToFoa(Rva) + m_buff) - 0x00400000 + 0x00800000;
			}
		}

		// 遍历下一重定位表
		pRelocal =
			(PIMAGE_BASE_RELOCATION)((DWORD)pRelocal + pRelocal->SizeOfBlock);
	}

	// 保存文件 - 修复重定位
	//HANDLE hFile = CreateFile(
	//	L"1.exe",		//文件路径
	//	GENERIC_WRITE,	//打开方式（读）
	//	FILE_SHARE_READ,//共享方式
	//	NULL,			//安全属性
	//	CREATE_NEW,  //打开的文件必须存在
	//	FILE_ATTRIBUTE_NORMAL, //文件属性
	//	NULL
	//);
	//DWORD dwWriteSize;
	//WriteFile(hFile, m_buff, m_size, &dwWriteSize, 0);
	//CloseHandle(hFile);

}



