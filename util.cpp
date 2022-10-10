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
