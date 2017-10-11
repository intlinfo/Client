#include "stdafx.h"
#include "CryFileHelper.h"
#include "WinCrypt.h"
#pragma comment(lib, "Crypt32.lib")

bool CryFileHelper::GetFileMD5(PBYTE pbData, DWORD dwDataLen, LPSTR lpszDest)
{
	HCRYPTPROV hProv = 0;
	BYTE Hash[16];
	LPBYTE pbHash = (LPBYTE)Hash;
	DWORD dwHashLen = 16;
	ZeroMemory(pbHash, dwHashLen);
	if (!CryptAcquireContext(&hProv, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		return false;
	}
	HCRYPTHASH hHash = 0;
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		return false;
	}
	if (!CryptHashData(hHash, pbData, dwDataLen, NULL))
	{
		return false;
	}
	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))
	{
		sprintf(lpszDest, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", Hash[0], Hash[1], Hash[2], Hash[3], Hash[4], Hash[5], Hash[6], Hash[7], Hash[8], Hash[9], Hash[10], Hash[11], Hash[12], Hash[13], Hash[14], Hash[15]);
	}
	return true;
}
HRESULT CryFileHelper::ReadFile(LPCTSTR lpszFileName, CAtlFile& file, CAtlFileMapping<>& fmap, ULONGLONG dwMaxFileSize)
{
	ASSERT(lpszFileName != nullptr);

	HRESULT hr = file.Create(lpszFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);

	if (SUCCEEDED(hr))
	{
		ULONGLONG ullLen;
		hr = file.GetSize(ullLen);

		if (SUCCEEDED(hr))
		{
			if (ullLen > 0 && ullLen <= dwMaxFileSize)
				hr = fmap.MapFile(file);
			else if (ullLen == 0)
				hr = HRESULT_FROM_WIN32(ERROR_FILE_INVALID);
			else
				hr = HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);
		}
	}
	return hr;
}
HRESULT CryFileHelper::MakeFilePackage(LPCTSTR lpszFileName, CAtlFile& file, CAtlFileMapping<>& fmap)
{
	return ReadFile(lpszFileName, file, fmap);
}

bool CryFileHelper::MakeFile(LPCTSTR lpszFileName, LPTSTR lpszFileData)
{
	CAtlFile file;
	CAtlFileMapping<> fmap;

	HRESULT hr = MakeFilePackage(lpszFileName, file, fmap);

	if (FAILED(hr))
	{
		SetLastError(HRESULT_CODE(hr));
		return false;
	}

	if (!GetFileMD5((PBYTE)fmap.GetData(), fmap.GetMappingSize(), lpszFileData))
	{
		return false;
	}
	return true;
}