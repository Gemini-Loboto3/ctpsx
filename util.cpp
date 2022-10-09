#include <Windows.h>
#include <string>

// create a wide string from utf8
wchar_t* MultiToWide(const char* multi)
{
	// gather size of the new string and create a buffer
	size_t size = MultiByteToWideChar(CP_UTF8, 0, multi, -1, NULL, 0);
	wchar_t* wide = new wchar_t[size];
	// fill allocated string with converted data
	MultiByteToWideChar(CP_UTF8, 0, multi, -1, wide, size);

	return wide;
}

// create a wide string from utf8
char* WideToMulti(const wchar_t* multi)
{
	// gather size of the new string and create a buffer
	size_t size = WideCharToMultiByte(CP_UTF8, 0, multi, -1, NULL, 0, NULL, NULL);
	char* utf8 = new char[size];
	// fill allocated string with converted data
	WideCharToMultiByte(CP_UTF8, 0, multi, -1, utf8, size, 0, NULL);

	return utf8;
}

// create an std::wstring from utf8
std::wstring MultiToWide_s(const char* multi)
{
	if (!multi)
		return std::wstring(L"");

	wchar_t* wide = MultiToWide(multi);
	std::wstring wstr = wide;
	delete[] wide;

	return wstr;
}

// crate an std::wstring from utf8 str::string
std::wstring MultiToWide_s(std::string& multi)
{
	return MultiToWide_s(multi.c_str());
}

// create an std::wstring from utf8
std::string WideToMulti_s(const wchar_t* multi)
{
	if (!multi)
		return std::string("");

	char* wide = WideToMulti(multi);
	std::string wstr = wide;
	delete[] wide;

	return wstr;
}

// crate an std::wstring from utf8 str::string
std::string WideToMulti_s(std::wstring& multi)
{
	return WideToMulti_s(multi.c_str());
}

ULONG_PTR EnableVisualStyles()
{
	TCHAR dir[MAX_PATH];
	ULONG_PTR ulpActivationCookie = FALSE;
	ACTCTX actCtx =
	{
		sizeof(actCtx),
		ACTCTX_FLAG_RESOURCE_NAME_VALID
		| ACTCTX_FLAG_SET_PROCESS_DEFAULT
		| ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID,
		TEXT("shell32.dll"), 0, 0, dir, (LPCTSTR)124
	};
	UINT cch = GetSystemDirectory(dir, sizeof(dir) / sizeof(*dir));
	if (cch >= sizeof(dir) / sizeof(*dir))
		return FALSE;
	dir[cch] = TEXT('\0');
	ActivateActCtx(CreateActCtx(&actCtx), &ulpActivationCookie);
	return ulpActivationCookie;
}

void MakePageWritable(unsigned long ulAddress, unsigned long ulSize)
{
	MEMORY_BASIC_INFORMATION* mbi = new MEMORY_BASIC_INFORMATION;
	VirtualQuery((void*)ulAddress, mbi, ulSize);
	if (mbi->Protect != PAGE_EXECUTE_READWRITE)
	{
		unsigned long* ulProtect = new unsigned long;
		VirtualProtect((void*)ulAddress, ulSize, PAGE_EXECUTE_READWRITE, ulProtect);
		delete ulProtect;
	}
	delete mbi;
}