#pragma once

class CTexture
{
public:
	CTexture()
	{}

	virtual int Create(WORD* clut, BYTE* data, int bpp, int w, int h) { return 0; }
	virtual int CreateUV(CTexture* src, int u, int v, int w, int h) { return 0; }
	virtual void Release() { w = h = u = v = 0; }

	int w = 0, h = 0,
		u = 0, v = 0,
		uw = 0, vh = 0;
	int is_sub = 0;
};
