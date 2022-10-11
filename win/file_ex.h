#pragma once
#include "filehandle.h"
#include <vector>

class FileEx
{
public:
	virtual int    Open(void* handle, size_t size) { return 0; }
	virtual void   Close() {}
	virtual size_t Read(void* dest, size_t size) { return 0; }
	virtual void   Seek(__int64 pos, DWORD mode) {}
	virtual size_t Tell() { return 0; }

	enum SEEK
	{
		SeekSet,
		SeekCur,
		SeekEnd
	};
};

class FileHandle : public FileEx
{
public:
	virtual int    Open(void* handle, size_t size);
	virtual void   Close();
	virtual size_t Read(void* dest, size_t size);
	virtual void   Seek(__int64 pos, DWORD mode);
	virtual size_t Tell();

	CFile fp;
};

class FileMemory : public FileEx
{
public:
	FileMemory() :
		data(nullptr),
		pos(0),
		size(0)
	{}
	~FileMemory();

	virtual int    Open(void* handle, size_t size);
	virtual void   Close();
	virtual size_t Read(void* dest, size_t size);
	virtual void   Seek(__int64 pos, DWORD mode);
	virtual size_t Tell();

	BYTE *GetBuf() { return &data[0]; }

	BYTE *data;
	size_t pos, size;
};
