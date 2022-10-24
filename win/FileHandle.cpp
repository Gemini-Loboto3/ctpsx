#include <stdafx.h>
#include "FileHandle.h"
#include "game.h"

int CFile::Open(LPCSTR filename)
{
	char path[MAX_PATH];
#ifdef _DEBUG
	sprintf_s(path, MAX_PATH, "%s", filename);
#else
	sprintf_s(path, MAX_PATH, "%s%s", prog.path, filename);
#endif

	handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// successful
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	// failure
	return 1;
}

int CFile::Create(LPCSTR filename)
{
	char path[MAX_PATH];
#ifdef _DEBUG
	sprintf_s(path, MAX_PATH, "%s", filename);
#else
	sprintf_s(path, MAX_PATH, "%s%s", prog.path, filename);
#endif

	handle = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// successful
	if (handle == INVALID_HANDLE_VALUE)
		return 0;
	// failure
	return 1;
}

void CFile::Close()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
}

DWORD CFile::GetSize()
{
	return GetFileSize(handle, NULL);
}

DWORD CFile::Read(LPVOID buffer, DWORD size)
{
	DWORD read;
	ReadFile(handle, buffer, size, &read, NULL);
	return read;
}

DWORD CFile::Write(LPVOID buffer, DWORD size)
{
	DWORD wrote;
	WriteFile(handle, buffer, size, &wrote, NULL);
	return wrote;
}

DWORD CFile::Seek(DWORD pos, DWORD mode)
{
	return SetFilePointer(handle, pos, nullptr, mode);
}

DWORD CFile::Tell()
{
	return SetFilePointer(handle, 0, nullptr, FILE_CURRENT);
}
