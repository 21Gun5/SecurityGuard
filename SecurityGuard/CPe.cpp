
#include "stdafx.h"
#include <stdio.h>
#include "CPe.h"

char * g_PeBuff = NULL;


// ��ʼ������
bool CPe::InitPe(TCHAR * path)
{
	// PE�ļ�����·��
	m_path = path;

	//1. ��PE�ļ�
	HANDLE hFile = CreateFile(
		path,		//�ļ�·��
		GENERIC_READ,	//�򿪷�ʽ������
		FILE_SHARE_READ,//����ʽ
		NULL,			//��ȫ����
		OPEN_EXISTING,  //�򿪵��ļ��������
		FILE_ATTRIBUTE_NORMAL, //�ļ�����
		NULL
	);

	// ��ȡ�ļ���С
	m_size = GetFileSize(hFile, 0);
	// ����ռ�
	m_buff = new char[m_size];
	DWORD dwReadSize;

	//2. ��PE�ļ���ȡ���ڴ�
	ReadFile(hFile, m_buff, m_size, &dwReadSize, 0);

	g_PeBuff = m_buff;

	//3. �ж��Ƿ���PE�ļ�
	return IsPeFile(m_buff);

}

// �Ƿ���PE�ļ�
bool CPe::IsPeFile(char * pbuff)
{
	// �ж��Ƿ�PE�ļ� 
	// DOS.e_magic  
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	// �ж��Ƿ�MZ 
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}
	// NT.Signature �Ƿ���� 0x00004550  ��PE00��
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}
	// ��һ��PE�ļ�
	return true;
}

//��ȡDOSͷ
PIMAGE_DOS_HEADER CPe::GetDosHeader()
{
	return (PIMAGE_DOS_HEADER)m_buff;
	//if(isLoad)
	//	return (PIMAGE_DOS_HEADER)g_PeBuff;
	//else
	//	return (PIMAGE_DOS_HEADER)m_buff;
}

// ��ȡNTͷ
PIMAGE_NT_HEADERS CPe::GetNtHeader()
{
	// ��ȡDOSͷ
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	// ��ȡNTͷ  =  DOS.e_lfanew + �ļ���ʼλ��buff
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)m_buff);
	return pNt;
}

// ��ȡ���α���һ������
PIMAGE_SECTION_HEADER CPe::GetSectionHeader()
{
	// ��ȡNTͷ
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// ��ȡ���α�
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);

	return pSection;
}

// ��������ڴ�ת���ļ�ƫ��
DWORD CPe::RvaToFoa(DWORD dwRva)
{
	// 1. �ж����RVA����ʲô����
	// ��ȡ���α�
	PIMAGE_SECTION_HEADER pSection = GetSectionHeader();
	// ��ȡ���θ���
	PIMAGE_FILE_HEADER pFileHeader = &GetNtHeader()->FileHeader;
	DWORD dwSectionCount = pFileHeader->NumberOfSections;
	DWORD Offset = 0;


	// ѭ���ж�RVA����ʲô����
	for (int i = 0; i < dwSectionCount; i++)
	{
		// RVA > ����������ڵ�RVA && 
		// RVA<= �������ڵ�RVA+�������ļ��еĴ�С
		if ((dwRva > pSection[i].VirtualAddress) &&
			dwRva <= (pSection[i].VirtualAddress + pSection[i].SizeOfRawData))
		{
			// ���㹫ʽ  offset = RVA - �������ڵ�RVA + �������ļ��е�ƫ��
			Offset = dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return Offset;
}

// ��ʾ��չͷ�е�����
void CPe::ShowOptionHeader()
{
	// 1.��ȡ��չͷ
	PIMAGE_NT_HEADERS pNt = GetNtHeader();

	//��ʾ ��չͷ�е�����
	printf("OEP: %08x\nImageBase %08x\n",
		pNt->OptionalHeader.AddressOfEntryPoint,
		pNt->OptionalHeader.ImageBase
	);

	// ��ʾ����Ŀ¼��
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

// ��ʾ������Ϣ
void CPe::ShowSectionInfo()
{
	// ��ȡ���α���Ϣ
	PIMAGE_SECTION_HEADER pSection = GetSectionHeader();

	// ��ȡ���θ���
	DWORD dwSize = GetNtHeader()->FileHeader.NumberOfSections;

	// �������α���Ϣ
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
	// ͨ������Ŀ¼��� 0 ��
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// ��ȡ�������RVA
	DWORD ExportRva = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	// תFOA
	DWORD dwOffset = RvaToFoa(ExportRva);
	// ������ĵ�ַ
	PIMAGE_EXPORT_DIRECTORY pExport =
		(PIMAGE_EXPORT_DIRECTORY)(dwOffset + (DWORD)m_buff);

	return pExport;
}

void CPe::ShowExportInfo()
{
	// ��ȡ��������Ϣ
	PIMAGE_EXPORT_DIRECTORY pExport = GetExportDirectory();

	// ����dll����  RVA
	char * pDllName = (char*)(RvaToFoa(pExport->Name) + m_buff);
	printf("%s\n", pDllName);

	// ������ַ�� 
	DWORD * pEAT =(DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + m_buff);
	// �������Ʊ�
	DWORD * pENT =(DWORD*)(RvaToFoa(pExport->AddressOfNames) + m_buff);
	// ������ű� Ԫ����word
	WORD * pEOT =(WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + m_buff);
	// ������ַ�����
	DWORD dwAddrCount = pExport->NumberOfFunctions;
	// �������Ʊ����
	DWORD dwNameCount = pExport->NumberOfNames;

	// ������ַ��
	for (int i = 0; i < dwAddrCount; i++)
	{
		//����������
		printf("��� %d ", i + pExport->Base);

		// ������ű�
		for (int j = 0; j < dwNameCount; j++)
		{
			// ��ű����е�ַ������ ����������������ֵģ�
			if (i == pEOT[j]) {
				// �������������� 
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
	// ͨ������Ŀ¼��� 1 ��
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// ��ȡ������RVA
	DWORD dwImportRva = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	// תFOA
	DWORD dwOffset = RvaToFoa(dwImportRva);
	// ������ĵ�ַ
	PIMAGE_IMPORT_DESCRIPTOR pImport =
		(PIMAGE_IMPORT_DESCRIPTOR)(dwOffset + (DWORD)m_buff);

	return pImport;


	return PIMAGE_IMPORT_DESCRIPTOR();
}

void CPe::ShowImportInfo()
{
	// ��ȡ�����ṹ������
	PIMAGE_IMPORT_DESCRIPTOR pImport = GetImportDirectory();
	// ������0��β
	while (pImport->Name)
	{
		// ��ʾdll����
		char * pDllname = (char*)(RvaToFoa(pImport->Name) + (DWORD)m_buff);
		printf("%s\n", pDllname);

		// ��ȡIAT
		PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)
			(RvaToFoa(pImport->OriginalFirstThunk) + (DWORD)m_buff);

		// ����IAT�к���
		while (pIAT->u1.Ordinal)
		{
			// �жϵ���ĺ��������Ƶ��뻹����ŵ���
			// �ж����λ�Ƿ�λ1 �����Ϊ1��ô��Ҫ����
			if (pIAT->u1.Ordinal & 0x80000000)
			{
				printf("��� %d\n", pIAT->u1.Function & 0x7FFFFFFF);
			}
			else {
				// ����ĺ���������
				PIMAGE_IMPORT_BY_NAME pName =
					(PIMAGE_IMPORT_BY_NAME)(RvaToFoa(pIAT->u1.AddressOfData) + (DWORD)m_buff);
				printf("��� %d  ��������%s\n", pName->Hint, pName->Name);

			}
			// ������һ������
			pIAT++;
		}
		// ������һ�������
		pImport++;
	}

}