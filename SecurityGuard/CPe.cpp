#include "stdafx.h"
#include <stdio.h>
#include "CPe.h"

char * g_PeBuff = NULL;

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
		if ((dwRva >= pSection[i].VirtualAddress) &&
			dwRva < (pSection[i].VirtualAddress + pSection[i].SizeOfRawData))
		{
			// ���㹫ʽ  offset = RVA - �������ڵ�RVA + �������ļ��е�ƫ��
			Offset = dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return Offset;
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
PIMAGE_NT_HEADERS CPe::GetNtHeader()
{
	// ��ȡDOSͷ
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	// ��ȡNTͷ  =  DOS.e_lfanew + �ļ���ʼλ��buff
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)m_buff);
	return pNt;
}
PIMAGE_SECTION_HEADER CPe::GetSectionHeader()
{
	// ��ȡNTͷ
	PIMAGE_NT_HEADERS pNt = GetNtHeader();
	// ��ȡ���α�
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);

	return pSection;
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
	// ��ȡ����Ŀ¼��� 5��
	DWORD dwRva = GetNtHeader()->OptionalHeader.DataDirectory[5].VirtualAddress;

	// ��ȡ�ض�λ��ṹ��
	PIMAGE_BASE_RELOCATION pRelocal =
		(PIMAGE_BASE_RELOCATION)(RvaToFoa(dwRva) + m_buff);

	return pRelocal;
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
void CPe::ShowExportInfo()
{
	// ��ȡ��������Ϣ
	PIMAGE_EXPORT_DIRECTORY pExport = GetExportDirectory();

	// ����dll����  RVA
	char * pDllName = (char*)(RvaToFoa(pExport->Name) + m_buff);
	printf("%s\n", pDllName);

	// ������ַ�� 
	DWORD * pEAT = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + m_buff);
	// �������Ʊ�
	DWORD * pENT = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + m_buff);
	// ������ű� Ԫ����word
	WORD * pEOT = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + m_buff);
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
void CPe::ShowResourceInfo()
{
	// ��һ�㣺��Դ��������ʲô��ͼ�꣬λͼ���˵�....��
	// ��ȡ��ԴĿ¼
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = GetResourceDirectory();

	// ��ȡ��ԴĿ¼��
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);

	//��ԴĿ¼����
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;


	//������һ����Դ��Ϣ
	for (int i = 0; i < dwOneCount; i++)
	{
		// �ж���Դ��ʲô����
		// ��Դ���ַ�����Ϊ��ʶ
		if (pResOneEntry[i].NameIsString)
		{
			// ���NameIsString == 1 ��ô NameOffset��Ч
			PIMAGE_RESOURCE_DIR_STRING_U pName =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pResOneEntry[i].NameOffset + (DWORD)pResDirOne);
			// �����Դ������
			WCHAR wResName[100];
			wcsncpy_s(wResName, pName->NameString, pName->Length);
			wprintf(L"%s\n", wResName);
		}
		// ��Դ��ID��Ϊ��ʶ
		else {
			// ϵͳ�ṩ����Դ����
			if (pResOneEntry[i].Id < 16)
			{
				//wprintf(L"%s\n", RES[pResOneEntry[i].Id]);//here
			}
			else {
				wprintf(L"%08x\n", pResOneEntry[i].Id);
			}

		}


		// �ڶ��Σ���Դ�Ľ�ʲô���� ��1.png, 2.png��
		// �Ƿ��еڶ���Ŀ¼
		if (pResOneEntry[i].DataIsDirectory) {

			// �ҵ��ڶ���Ŀ¼��
			PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
				(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);

			// �ҵ��ڶ���Ŀ¼��
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);

			// ��ȡ������Դ�ж��ٸ�
			DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
			for (int j = 0; j < ResTowCount; j++)
			{
				// ��Դ���ַ�����Ϊ��ʶ
				if (pResTwoEntry[j].NameIsString)
				{
					// ���NameIsString == 1 ��ô NameOffset��Ч
					PIMAGE_RESOURCE_DIR_STRING_U pName =
						(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
					// �����Դ������
					WCHAR wResName[100];
					wcsncpy_s(wResName, pName->NameString, pName->Length);
					wprintf(L"		%s\n", wResName);
				}
				else {
					printf("		%08x\n", pResTwoEntry[j].Id);
				}

				// �����㣺��Դ�����ԣ���Դ����Ϣ����С���ļ���λ�ã�
				if (pResTwoEntry[j].DataIsDirectory)
				{
					// ��ȡ������Ŀ¼��
					PIMAGE_RESOURCE_DIRECTORY pResDirThree =
						(PIMAGE_RESOURCE_DIRECTORY)(pResTwoEntry[j].OffsetToDirectory + (DWORD)pResDirOne);
					// ��ȡ������Ŀ¼��
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);

					// ��ȡ��Դ������Ϣ
					PIMAGE_RESOURCE_DATA_ENTRY pResData =
						(PIMAGE_RESOURCE_DATA_ENTRY)(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);

					printf("			��Դλ��RVA�� %08X\n", pResData->OffsetToData);
					printf("			��Դλ��Size�� %08X\n", pResData->Size);
				}
			}
		}
	}
}
void CPe::ShowRelocalInfo()
{
	typedef	struct {
		WORD Offset : 12;  // (1) ��СΪ12Bit���ض�λƫ��
		WORD Type : 4;   // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
	}*PTYPEOFFSET;	    // ����ṹ����A1Pass�ܽ��


	// ��ȡ�ض�λ��
	PIMAGE_BASE_RELOCATION pRelocal = GetRelocalDirectory();

	// �������е��ض�λ����
	while (pRelocal->SizeOfBlock)
	{
		// ��ȡ�ض�λ�����
		DWORD dwCount = (pRelocal->SizeOfBlock - 8) / 2;

		//��0x1000�ֽ��е���Ҫ�ض�λ���ݵ�ƫ�ƣ�
		printf("virtualaddress: %08x\n", pRelocal->VirtualAddress);
		printf("Items: %d\n", dwCount);
		// ��ȡ�ض�λ��
		PTYPEOFFSET pRelocalBlock = (PTYPEOFFSET)(pRelocal + 1);
		// �����ض�λ������
		for (int i = 0; i < dwCount; i++)
		{
			// ��x32λPE�ļ�������Ϊ0x03	��ʾ��Ҫ�ض�λ������  
			// ��x64λPE�ļ�������Ϊ0x10	��ʾ��Ҫ�ض�λ������  
			if (pRelocalBlock[i].Type == 3)
			{
				// Ҫ�޸����ݵ�RVA
				DWORD Rva = pRelocalBlock[i].Offset + pRelocal->VirtualAddress;
				printf("	RVA:%08X\n", Rva);
				printf("	FOA:%08X\n", RvaToFoa(Rva));
				printf("\n");
				// �޸��ض�λΪ0x00800000
				*(DWORD*)(RvaToFoa(Rva) + m_buff) =
					*(DWORD*)(RvaToFoa(Rva) + m_buff) - 0x00400000 + 0x00800000;
			}
		}

		// ������һ�ض�λ��
		pRelocal =
			(PIMAGE_BASE_RELOCATION)((DWORD)pRelocal + pRelocal->SizeOfBlock);
	}

	// �����ļ� - �޸��ض�λ
	//HANDLE hFile = CreateFile(
	//	L"1.exe",		//�ļ�·��
	//	GENERIC_WRITE,	//�򿪷�ʽ������
	//	FILE_SHARE_READ,//����ʽ
	//	NULL,			//��ȫ����
	//	CREATE_NEW,  //�򿪵��ļ��������
	//	FILE_ATTRIBUTE_NORMAL, //�ļ�����
	//	NULL
	//);
	//DWORD dwWriteSize;
	//WriteFile(hFile, m_buff, m_size, &dwWriteSize, 0);
	//CloseHandle(hFile);

}



