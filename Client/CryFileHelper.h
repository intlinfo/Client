#pragma once
#include <atlfile.h>
class CryFileHelper
{
public:
	bool MakeFile(LPCTSTR lpszFileName, LPTSTR lpszFileData);
private:
	bool GetFileMD5(PBYTE pbData, DWORD dwDataLen, LPSTR lpszDest);
	HRESULT ReadFile(LPCTSTR lpszFileName, CAtlFile& file, CAtlFileMapping<>& fmap, ULONGLONG dwMaxFileSize = 0xFFFFFFFFFFFFFFFF);
	HRESULT MakeFilePackage(LPCTSTR lpszFileName, CAtlFile& file, CAtlFileMapping<>& fmap);
};