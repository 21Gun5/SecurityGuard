#pragma once
#include<windows.h>
#include "CTools.h"

extern char * g_PeBuff;

class CPe
{
public:
	bool InitPe(TCHAR *path);// ��ʼ��PE�ļ�
	bool IsPeFile(char *pbuff);// �ж��Ƿ�PE�ļ�
	PIMAGE_DOS_HEADER GetDosHeader();// ��ȡDOSͷ
	PIMAGE_NT_HEADERS GetNtHeader();// ��ȡNTͷ
	PIMAGE_SECTION_HEADER GetSectionHeader();// ��ȡ���α�
	DWORD RvaToFoa(DWORD dwRva);// RVA  TO FOA ת��
	void ShowOptionHeader();// ��ʾ��չͷ��Ϣ
	void ShowSectionInfo();// ��ʾ���α���Ϣ
	PIMAGE_EXPORT_DIRECTORY GetExportDirectory();
	void ShowExportInfo();// ��ʾ������
	PIMAGE_IMPORT_DESCRIPTOR GetImportDirectory();
	void ShowImportInfo();// ��ʾ�����


private:
	TCHAR *m_path;//PE�ļ�·��
	DWORD m_size;// PE�ļ���С
	char *m_buff;// PE�ļ�������
};

