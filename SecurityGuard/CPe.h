#pragma once
#include<windows.h>
#include "CTools.h"

extern char * g_PeBuff;

class CPe
{
public:
	bool InitPe(TCHAR *path);// 初始化PE文件
	bool IsPeFile(char *pbuff);// 判断是否PE文件
	PIMAGE_DOS_HEADER GetDosHeader();// 获取DOS头
	PIMAGE_NT_HEADERS GetNtHeader();// 获取NT头
	PIMAGE_SECTION_HEADER GetSectionHeader();// 获取区段表
	DWORD RvaToFoa(DWORD dwRva);// RVA  TO FOA 转换
	void ShowOptionHeader();// 显示扩展头信息
	void ShowSectionInfo();// 显示区段表信息
	PIMAGE_EXPORT_DIRECTORY GetExportDirectory();
	void ShowExportInfo();// 显示导出表
	PIMAGE_IMPORT_DESCRIPTOR GetImportDirectory();
	void ShowImportInfo();// 显示导入表


private:
	TCHAR *m_path;//PE文件路径
	DWORD m_size;// PE文件大小
	char *m_buff;// PE文件缓冲区
};

