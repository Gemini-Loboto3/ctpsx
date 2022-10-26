#pragma once

class CTexture
{
public:
	CTexture() : w(0), h(0),
		u(0), v(0)
	{}

	virtual int Create(WORD clut, BYTE* data, int w, int h) { return 0; }
	virtual int Create(WORD clut, BYTE* data, int w, int h, int u, int v) { return 0; }
	virtual void Release() { w = h = u = v = 0; }

	int w, h,
		u, v;
};
