#include <stdafx.h>
#include "file_ex.h"

int FileHandle::Open(void* handle, size_t size)
{
	fp.Open((LPCSTR)handle);
	if (fp.IsOpen())
		return 1;

	return 0;
}

void FileHandle::Close()
{
	fp.Close();
}

size_t FileHandle::Read(void* dest, size_t size)
{
	return fp.Read(dest, size);
}

void FileHandle::Seek(__int64 pos, DWORD mode)
{
	switch (mode)
	{
	case SeekSet: fp.Seek((DWORD)pos, FILE_BEGIN); break;
	case SeekCur: fp.Seek((DWORD)pos, FILE_CURRENT); break;
	case SeekEnd: fp.Seek((DWORD)pos, FILE_END); break;
	}
	
}

size_t FileHandle::Tell()
{
	return fp.Tell();
}

//////////////////////////////////
FileMemory::~FileMemory()
{
	Close();
}

int FileMemory::Open(void* handle, size_t _size)
{
	if (handle == nullptr || _size == 0)
		return 0;

	size = _size;
	pos = 0;

	data = new BYTE[size];
	if (data == nullptr)
		throw 0;
	memcpy(data, handle, size);

	return 1;
}

void   FileMemory::Close()
{
	if (data)
	{
		delete[] data;
		data = nullptr;
	}
	pos = 0;
	size = 0;
}

size_t FileMemory::Read(void* dest, size_t _size)
{
	if (pos >= size)
		return EOF;

	if (_size + pos > size)
	{
		_size = size - pos;
		memcpy(dest, &data[pos], _size);
		pos = size;
		return _size;
	}

	memcpy(dest, &data[pos], _size);
	pos += _size;
	return _size;
}

void FileMemory::Seek(__int64 _pos, DWORD mode)
{
	switch (mode)
	{
	case SeekSet:
		pos = (DWORD)_pos;
		break;
	case SeekCur:
		if (pos + _pos >= size)
			_pos = size - _pos;
		pos += (int)_pos;
		break;
	case SeekEnd:
		if (_pos >= size)
			_pos = size;
		pos = size - (int)_pos;
		break;
	}
}

size_t FileMemory::Tell()
{
	return pos;
}