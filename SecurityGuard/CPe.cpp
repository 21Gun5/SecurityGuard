
#include "stdafx.h"
#include <stdio.h>
#include "CPe.h"

char * g_PeBuff = NULL;


// 初始化数据
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

// 是否是PE文件
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

//获取DOS头
PIMAGE_DOS_HEADER CPe::GetDosHeader()
{
	return (PIMAGE_DOS_HEADER)m_buff;
	//if(isLoad)
	//	return (PIMAGE_DOS_HEADER)g_PeBuff;
	//else
	//	return (PIMAGE_DOS_HEADER)m_buff;
}

// 获取NT头
PIMAGE_NT_HEADERS CPe::GetNtHeader()
{
	// 获取DOS头
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	// 获取NT头  =  DOS.e_lfanew + 文件起始位置buff
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)m_buff);
	return pNt;
}

// 获取区段表，是一个数组
PIMAGE_SECTION_HEADER CPe::GetSectionHeader()
{
	// 获取NT头
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// 获取区段表
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);

	return pSection;
}

// 相对虚拟内存转换文件偏移
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
		if ((dwRva > pSection[i].VirtualAddress) &&
			dwRva <= (pSection[i].VirtualAddress + pSection[i].SizeOfRawData))
		{
			// 计算公式  offset = RVA - 区段所在的RVA + 区段在文件中的偏移
			Offset = dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return Offset;
}

// 显示扩展头中的数据
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

// 显示区段信息
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

void CPe::ShowExportInfo()
{
	// 获取导出表信息
	PIMAGE_EXPORT_DIRECTORY pExport = GetExportDirectory();

	// 解析dll名字  RVA
	char * pDllName = (char*)(RvaToFoa(pExport->Name) + m_buff);
	printf("%s\n", pDllName);

	// 导出地址表 
	DWORD * pEAT =(DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + m_buff);
	// 导出名称表
	DWORD * pENT =(DWORD*)(RvaToFoa(pExport->AddressOfNames) + m_buff);
	// 导出序号表 元素是word
	WORD * pEOT =(WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + m_buff);
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

PIMAGE_IMPORT_DESCRIPTOR CPe::GetImportDirectory()
{
	// 通过数据目录表第 1 项
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// 获取导入表的RVA
	DWORD dwImportRva = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	// 转FOA
	DWORD dwOffset = RvaToFoa(dwImportRva);
	// 导出表的地址
	PIMAGE_IMPORT_DESCRIPTOR pImport =
		(PIMAGE_IMPORT_DESCRIPTOR)(dwOffset + (DWORD)m_buff);

	return pImport;


	return PIMAGE_IMPORT_DESCRIPTOR();
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