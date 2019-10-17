#pragma once
#include<windows.h>
#include "CTools.h"

extern char * g_PeBuff;
//extern wchar_t * g_PePath;
#define PE_PATH (TCHAR*)L".//res//ForTestPE//01PE�ļ�.exe"

class CPe
{
public:
	bool InitPe(TCHAR *path);// ��ʼ��PE�ļ�
	bool IsPeFile(char *pbuff);// �ж��Ƿ�PE�ļ�
	DWORD RvaToFoa(DWORD dwRva);// RVA  TO FOA ת��
	
	PIMAGE_DOS_HEADER GetDosHeader();// ��ȡDOSͷ
	PIMAGE_NT_HEADERS GetNtHeader();// ��ȡNTͷ
	PIMAGE_SECTION_HEADER GetSectionHeader();// ��ȡ���α�
	PIMAGE_EXPORT_DIRECTORY GetExportDirectory();
	PIMAGE_IMPORT_DESCRIPTOR GetImportDirectory();
	PIMAGE_RESOURCE_DIRECTORY GetResourceDirectory();// ��ȡ��Դ��
	PIMAGE_BASE_RELOCATION GetRelocalDirectory();// ��ȡ�ض�λ��

	PIMAGE_DELAYLOAD_DESCRIPTOR GetDelayLoadDirectory();// ��ȡ�ӳټ���
	PIMAGE_TLS_DIRECTORY32 GetTLSDirectory();

	void ShowOptionHeader();// ��ʾ��չͷ��Ϣ
	void ShowSectionInfo();// ��ʾ���α���Ϣ
	void ShowExportInfo();// ��ʾ������
	void ShowImportInfo();// ��ʾ�����
	void ShowResourceInfo();// ��ʾ��Դ����Ϣ
	void ShowRelocalInfo();// �����ض�λ

private:
	TCHAR *m_path;//PE�ļ�·��
	DWORD m_size;// PE�ļ���С
	char *m_buff;// PE�ļ�������
};

//DWORD RvaToFoa2(DWORD dwRva);