#pragma once
#include<windows.h>
#include "CTools.h"

extern char * g_PeBuff;
//extern wchar_t * g_PePath;
#define PE_PATH (TCHAR*)L".//res//ForTestPE//01PE文件.exe"

class CPe
{
public:
	bool InitPe(TCHAR *path);// 初始化PE文件
	bool IsPeFile(char *pbuff);// 判断是否PE文件
	DWORD RvaToFoa(DWORD dwRva);// RVA  TO FOA 转换
	
	PIMAGE_DOS_HEADER GetDosHeader();// 获取DOS头
	PIMAGE_NT_HEADERS GetNtHeader();// 获取NT头
	PIMAGE_SECTION_HEADER GetSectionHeader();// 获取区段表
	PIMAGE_EXPORT_DIRECTORY GetExportDirectory();
	PIMAGE_IMPORT_DESCRIPTOR GetImportDirectory();
	PIMAGE_RESOURCE_DIRECTORY GetResourceDirectory();// 获取资源表
	PIMAGE_BASE_RELOCATION GetRelocalDirectory();// 获取重定位表

	PIMAGE_DELAYLOAD_DESCRIPTOR GetDelayLoadDirectory();// 获取延迟加载
	PIMAGE_TLS_DIRECTORY32 GetTLSDirectory();

	void ShowOptionHeader();// 显示扩展头信息
	void ShowSectionInfo();// 显示区段表信息
	void ShowExportInfo();// 显示导出表
	void ShowImportInfo();// 显示导入表
	void ShowResourceInfo();// 显示资源表信息
	void ShowRelocalInfo();// 解析重定位

private:
	TCHAR *m_path;//PE文件路径
	DWORD m_size;// PE文件大小
	char *m_buff;// PE文件缓冲区
};

//DWORD RvaToFoa2(DWORD dwRva);