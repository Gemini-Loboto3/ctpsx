#include <stdafx.h>
#include "game.h"

//////////////////////////////
// pure TIM handler
size_t CTim::Open(BYTE* pTim)
{
	Release();

	if (*(DWORD*)pTim != 0x10)
		return 0;

	TIM_CHUNK* pix;
	static int bpp_tbl[] = { 4, 8, 16, 24 };

	// has clut
	if (pTim[4] & 8)
	{
		TIM_CHUNK* pal = (TIM_CHUNK*)&pTim[8];
		WORD* _clut = (WORD*)&pal[1];
		pix = (TIM_CHUNK*)&_clut[pal->w * pal->h];
		// copy over new palette
		clut = new WORD[pal->w * pal->h];
		memcpy(clut, _clut, pal->w * pal->h * 2);
		// set clut data
		pal_w = pal->w;
		pal_h = pal->h;
	}
	// no clut
	else
	{
		clut = nullptr;
		pix = (TIM_CHUNK*)&pTim[8];
	}
	BYTE* data = (BYTE*)&pix[1];

	bpp = bpp_tbl[pTim[4] & 7];
	pixel = new BYTE[pix->w * pix->h * 2];
	memcpy(pixel, data, pix->w * pix->h * 2);
	pix_w = pix->w;
	pix_h = pix->h;

	switch (bpp)
	{
	case 4: real_w = pix_w * 4; break;
	case 8: real_w = pix_w * 2; break;
	case 16: real_w = pix_w; break;
	case 24: real_w = pix_w * 2 / 3; break;
	}

	is_ref = 0;

#ifdef _WINDOWS
	SetTexture();
#endif

	return (size_t)&data[pix->w * pix->h * 2] - (size_t)pTim;
}

CTim::~CTim()
{
	Release();
}

size_t CTim::Open(const char* filename)
{
	CFile fp;
	if (fp.Open(filename))
	{
		size_t size = fp.GetSize();

		std::vector<BYTE> buf = std::vector<BYTE>(size);
		fp.Read(buf.data(), size);
		fp.Close();

		return Open(buf.data());
	}

	return 0;
}

void CTim::Release()
{
	if (pixel != nullptr)
	{
		delete[] pixel;
		pixel = nullptr;
	}

	if (clut != nullptr)
	{
		delete[] clut;
		clut = nullptr;
	}

#ifdef _WINDOWS
	if (tex)
	{
		tex->Release();
		tex = nullptr;
	}
#endif

	pal_w = pal_h = 0;
	pix_w = pix_h = 0;
	real_w = 0;
}

void CTim::Copy(CTim* src, int u, int v, int w, int h)
{
	Create(w, h, src->bpp, src->pal_h);

	BYTE* d = pixel;
	BYTE* s = src->pixel;

	switch (src->bpp)
	{
	case 4:
		s += pix_w * v + u / 2;
		memcpy(clut, src->clut, pal_w * 2);
		for (int y = 0; y < h; y++)
		{
			memcpy(d, s, pix_w * 2);
			d += pix_w * 2;
			s += src->pix_w * 2;
		}
		break;
	case 8:
		s += pix_w * v + u;
		memcpy(clut, src->clut, pal_w * 2);
		for (int y = 0; y < h; y++)
		{
			memcpy(d, s, pix_w * 2);
			d += pix_w * 2;
			s += src->pix_w * 2;
		}
		break;
	case 16:
		break;
	case 24:
		break;
	}

	SetTexture();
	is_ref = 1;
}

void CTim::Create(int w, int h, int bpp, int pals)
{
	switch (bpp)
	{
	case 4:
		pix_w = w / 4;
		pal_w = 16;
		pal_h = pals;
		clut = new WORD[pals * 16];
		pixel = new BYTE[pix_w * h *2];
		break;
	case 8:
		pix_w = w / 2;
		pal_w = 256;
		pal_h = pals;
		clut = new WORD[pals * 256];
		pixel = new BYTE[pix_w * h * 2];
		break;
	case 15:
		clut = NULL;
		pixel = new BYTE[w * h];
		pal_w = 0;
		pal_h = 0;
		pix_w = w;
		break;
	case 24:
		clut = NULL;
		pixel = new BYTE[w * h * 3 / 2];
		pal_w = 0;
		pal_h = 0;
		pix_w = w * 3 / 2;
		break;
	}

	pix_h = h;
	real_w = w;
	this->bpp = bpp;
}

#ifdef _WINDOWS
void CTim::SetTexture()
{
	if (d3d9dev == nullptr)
		return;

	d3d9dev->CreateTexture(real_w, pix_h, 1, 0, d3dfmttex, D3DPOOL_MANAGED, &tex, nullptr);

	D3DLOCKED_RECT r;
	tex->LockRect(0, &r, nullptr, 0);

	DWORD* dst = (DWORD*)r.pBits;
	auto pitch = r.Pitch / 4;

	BYTE* src8 = pixel;
	WORD* src16 = (WORD*)pixel;

	switch (bpp)
	{
	case 4:
		for (int y = 0; y < pix_h; y++)
		{
			DWORD* d = dst;
			for (int x = 0; x < real_w; x += 2, src8++)
			{
				*d++ = torgb888(clut[*src8 & 0xf]);
				*d++ = torgb888(clut[*src8 >> 4]);
			}
			dst += pitch;
		}
		break;
	case 8:
		for (int y = 0; y < pix_h; y++)
		{
			DWORD *d = dst;
			for (int x = 0; x < real_w; x++)
				*d++ = torgb888(clut[*src8++]);
			dst += pitch;
		}
		break;
	}

	tex->UnlockRect(0);
}
#endif
