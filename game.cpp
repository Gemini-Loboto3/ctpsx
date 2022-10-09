#include <stdafx.h>
#include <stdio.h>
#include "inject.h"
#include "game.h"

#define D3D_DEBUG_INFO
#include <d3d9.h>

#define USE_SOFTWARE	1

#define RENDER_W		1280
#define RENDER_H		960

LPDIRECT3D9 d3d9 = nullptr;
LPDIRECT3DDEVICE9 d3d9dev = nullptr;
LPDIRECT3DTEXTURE9 d3d9tex = nullptr;
LPDIRECT3DTEXTURE9 d3d9text = nullptr;
D3DFORMAT d3dfmttex;

const UINT FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

typedef struct fvf
{
	float x, y, z, w;
	D3DCOLOR diffuse;
	float tu, tv;
} fvf;

int get_color_distance(byte r0, byte g0, byte b0,
	byte r1, byte g1, byte b1)
{
	int rdiff = r0 - r1;
	int gdiff = g0 - g1;
	int bdiff = b0 - b1;

	return rdiff * rdiff + gdiff * gdiff + bdiff * bdiff;
}

byte find_closest_color(byte r, byte g, byte b)
{
	int shortest = INT_MAX;
	int index = -1;

	for (int i = 0; i < 256; i++)
	{
		int dist = get_color_distance(r, g, b,
			pal_obj.pal.palPalEntry[i].peRed, pal_obj.pal.palPalEntry[i].peGreen, pal_obj.pal.palPalEntry[i].peBlue);
		if (dist < shortest)
		{
			index = i;
			shortest = dist;
		}
	}

	return byte(index);
}


#define align(x,y)		((x) + ((y) - 1)) & ~(y - 1)

class D3D9Bitmap
{
public:
	void Create(size_t _size, UINT _w, UINT _h)
	{
		size = _size;
		w = _w;
		h = _h;
		pitch = align(_w, 4);
		data = new BYTE[size];
	}

	void SetPal(PALETTEENTRY* _pal, int cnt)
	{
		memcpy(pal, _pal, cnt * sizeof(PALETTEENTRY));

		MakeTexture();
	}

	void SetData(BYTE* _data)
	{
		memcpy(data, _data, w * h);
	}

	~D3D9Bitmap()
	{
		if (data)
		{
			delete[] data;
			data = nullptr;
		}

		if (tex)
		{
			tex->Release();
			tex = nullptr;
		}
	}

	void MakeTexture()
	{
		if(tex == nullptr)
			d3d9dev->CreateTexture(w, h, 1, 0, d3dfmttex, D3DPOOL_MANAGED, &d3d9tex, nullptr);

		D3DLOCKED_RECT r;
		tex->LockRect(0, &r, nullptr, 0);

		DWORD* dst = (DWORD*)r.pBits;
		BYTE* src = data;
		for (UINT y = 0; y < h; y++)
		{
			DWORD* d = dst;
			BYTE* s = src;
			for (UINT x = 0; x < w; x++, d++, s++)
			{
				auto b = *s;
				*d = D3DCOLOR_ARGB(pal[b].peFlags,
					pal[b].peRed,
					pal[b].peGreen,
					pal[b].peBlue);
			}
			dst += r.Pitch / 4;
			src += pitch;
		}

		tex->UnlockRect(0);
	}

	BYTE* data = nullptr;
	PALETTEENTRY pal[256];
	LPDIRECT3DTEXTURE9 tex = nullptr;
	UINT w, h;
	size_t size,
		pitch;
};

int LoadBits(int w, int h, BYTE* bmp, LPCSTR filename)
{
	FILE* fp;
	if (fopen_s(&fp, filename, "rb") == 0 && fp)
	{
		BITMAPFILEHEADER head;
		fread(&head, sizeof(head), 1, fp);

		if (head.bfType != 'MB')
			return 7;

		BITMAPINFOHEADER info;
		fread(&info, sizeof(info), 1, fp);

		if (w == info.biWidth && info.biHeight == h)
		{
			auto lbytes = info.biHeight * align(info.biWidth, 4);
			fseek(fp, head.bfOffBits, SEEK_SET);
			fread(bmp, lbytes, 1, fp);
			fclose(fp);

			return 0;
		}
	}

	return 8;
}

EXTERN_C HWND hWnd;

D3DFORMAT GetBestZBuffer(D3DFORMAT adapter)
{
	static D3DFORMAT depth_fmt[]
	{
		D3DFMT_D24S8,				// 32-bit z-buffer bit depth using 24 bits for the depth channel and 8 bits for the stencil channel
		D3DFMT_D24X4S4,				// 32-bit z-buffer bit depth using 24 bits for the depth channel and 4 bits for the stencil channel
		D3DFMT_D24FS8,				// A non-lockable format that contains 24 bits of depth (in a 24-bit floating point format - 20e4) and 8 bits of stencil
		D3DFMT_D16,					// 16-bit z-buffer bit depth
		D3DFMT_D15S1,				// 16-bit z-buffer bit depth where 15 bits are reserved for the depth channel and 1 bit is reserved for the stencil channel
		D3DFMT_D32_LOCKABLE,
		D3DFMT_D32,					// 32-bit z-buffer bit depth
		D3DFMT_D24X8,				// 32-bit z-buffer bit depth using 24 bits for the depth channel
		D3DFMT_D32F_LOCKABLE,		// A lockable format where the depth value is represented as a standard IEEE floating-point number
		D3DFMT_D16_LOCKABLE,		// 16-bit z-buffer bit depth
	};

	for (int i = 0; i < _countof(depth_fmt); i++)
		if (d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, adapter, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depth_fmt[i]) == S_OK)
			return depth_fmt[i];

	return D3DFMT_UNKNOWN;
}

D3DCOLOR getCol(BYTE b)
{
	return D3DCOLOR_ARGB(255, pal_obj.pal.palPalEntry[b].peRed,
				pal_obj.pal.palPalEntry[b].peGreen,
				pal_obj.pal.palPalEntry[b].peBlue);
}

void copy_rle(DWORD* dst, BYTE* src, int w, int h, int dst_pitch)
{
	for (; h; h--)
	{
		auto d = dst;

		for (int x = 0; x < w; x++)
		{
			int len = *src++;
			if (len)
			{
				x += len;
				for (int i = 0; i < len; i++)
					*d++ = getCol(*src++);
			}
			else
			{
				x += *src;
				d += getCol(*src++);
			}
		}

		dst += dst_pitch;
	}
}

void copy_trans(DWORD* dst, BYTE* src, int w, int h, int dst_pitch, int src_pitch, byte trans)
{
	for (; h; h--)
	{
		auto s = src;
		auto d = dst;

		for (int x = 0; x < w; x++)
		{
			if (*s != trans)
				*d = getCol(*s);
			else
				*d = D3DCOLOR_ARGB(0, 0, 0, 0);
			s++;
			d++;
		}

		src += src_pitch;
		dst += dst_pitch;
	}
}

void copy_normal(DWORD* dst, BYTE* src, int w, int h, int dst_pitch, int src_pitch)
{
	for (; h; h--)
	{
		auto s = src;
		auto d = dst;

		for (int x = 0; x < w; x++)
		{
			*d = getCol(*s);
			s++;
			d++;
		}

		src += src_pitch;
		dst += dst_pitch;
	}
}

BYTE* get_src(BYTE* buf, int x, int y, int w, int h, int pitch)
{
	if (x >= 0 && y >= 0 && x < w && y < h)
		return &buf[pitch * (h - y - 1) + x];
	else
		return nullptr;
}

// this is where all 2d is rendered
int __cdecl RenderToOffScreen(int dst_x, int dst_y, int src_x, int src_y, int src_w, int src_h, int src_pitch, size_t w, int h, BYTE *src_bmp, BYTE trans_col, int mode, int type)
{
	D3DLOCKED_RECT r;
	if (FAILED(d3d9tex->LockRect(0, &r, nullptr, 0)))
		return 0;

	DWORD* dst = (DWORD*)r.pBits;
	src_bmp = get_src(src_bmp, src_x, h + src_y - 1, src_w, src_h, src_pitch);

	if (mode)
	{
		switch (type)
		{
		case 0:	// transparent
			d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
			d3d9dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			d3d9dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			copy_trans(dst, src_bmp, w, h, r.Pitch / 4, src_pitch, trans_col);
			break;
		case 3:	// rle
			d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
			copy_rle(dst, src_bmp, w, h, r.Pitch / 4); 
			break;
		}
	}
	else
	{
		d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
		copy_normal(dst, src_bmp, w, h, r.Pitch / 4, src_pitch);
	}

	if (FAILED(d3d9tex->UnlockRect(0)))
		return 0;

	float x0 = (float)dst_x - 0.5f, y0 = (float)dst_y - 0.5f, x1 = (float)(dst_x + w) - 0.5f, y1 = (float)(dst_y + h) - 0.5f;
	float u0 = 0.f, v0 = 0.f, u1 = (float)w / 512.f, v1 = (float)h / 512.f;

	fvf p[4];
	p[0].x = x0, p[0].y = y0, p[0].z = 0.5f, p[0].w = 1.f, p[0].tu = u0, p[0].tv = v1, p[0].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	p[1].x = x1, p[1].y = y0, p[1].z = 0.5f, p[1].w = 1.f, p[1].tu = u1, p[1].tv = v1, p[1].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	p[2].x = x0, p[2].y = y1, p[2].z = 0.5f, p[2].w = 1.f, p[2].tu = u0, p[2].tv = v0, p[2].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	p[3].x = x1, p[3].y = y1, p[3].z = 0.5f, p[3].w = 1.f, p[3].tu = u1, p[3].tv = v0, p[3].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	d3d9dev->SetTexture(0, d3d9tex);
	d3d9dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));

	return 1;
}

int __cdecl CreateOffScreenBitmap(RENDER_BMP* bmp, int w, int h)
{
	bmp->w = w;
	bmp->h = h;

	bmp->bmp_info = new BITMAP_DATA;
	memset(bmp->bmp_info, 0, sizeof(BITMAP_DATA));
	bmp->bmp_info->info.biSize = sizeof(bmp->bmp_info->info);
	bmp->bmp_info->info.biPlanes = 1;
	bmp->bmp_info->info.biBitCount = 8;
	bmp->bmp_info->info.biHeight = 1;
	bmp->bmp_info->info.biHeight *= h;
	bmp->bmp_info->info.biWidth = w;

	bmp->ppvbits = (RENDER_PIC*)new BYTE[w * h];

	bmp->flipped_h = 1;	// makes sure stuff is renderer in the correct vertical order
	bmp->used = 1;
	//bmp->hdc = (HDC)1;

	// resize window
	BOOL menu = (GetMenu(hWnd) != nullptr);
	auto dwStyle = GetWindowLongA(hWnd, GWL_STYLE);
	RECT rc;
	SetRect(&rc, 0, 0, RENDER_W, RENDER_H);

	//SetWindowLongA(hWnd, GWL_STYLE, dwStyle);
	POINT pm = { 0, 0 };
	AdjustWindowRectEx(&rc, dwStyle, menu, 0);
	SetWindowPos(hWnd, HWND_TOP, pm.x + 0, pm.y + 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);
	InvalidateRect(hWnd, nullptr, true);

	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	if(FAILED(d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return 0;

	D3DCAPS9 d3dCaps;
	if (FAILED(d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps)))
		return 0;

	d3dfmttex = D3DFMT_UNKNOWN;
	if (d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DYNAMIC, D3DRTYPE_TEXTURE, D3DFMT_A8B8G8R8) == S_OK)
		d3dfmttex = D3DFMT_A8B8G8R8;
	if (d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, D3DUSAGE_DYNAMIC, D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8) == S_OK)
		d3dfmttex = D3DFMT_A8R8G8B8;

	DWORD dwBehaviorFlags = D3DCREATE_PUREDEVICE | D3DCREATE_MULTITHREADED;
	if (d3dCaps.VertexProcessingCaps != 0) dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferWidth = RENDER_W;
	d3dpp.BackBufferHeight = RENDER_H;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = GetBestZBuffer(d3ddm.Format);
	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwBehaviorFlags, &d3dpp, &d3d9dev)))
		return 0;

#if USE_SOFTWARE
	d3d9dev->CreateTexture(w, h, 1, 0, d3dfmttex, D3DPOOL_MANAGED, &d3d9tex,  nullptr);
	d3d9dev->CreateTexture(w, h, 1, 0, d3dfmttex, D3DPOOL_MANAGED, &d3d9text, nullptr);

	D3DLOCKED_RECT r;
	d3d9text->LockRect(0, &r, nullptr, 0);
	memset(r.pBits, 0, r.Pitch * h);
	d3d9text->UnlockRect(0);
#else
	d3d9dev->CreateTexture(512, 512, 1, 0, d3dfmttex, D3DPOOL_MANAGED, &d3d9tex, nullptr);
#endif

	d3d9dev->SetFVF(FVF);

#if USE_SOFTWARE
	d3d9dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3d9dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
#else
	d3d9dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	d3d9dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
#endif

	return 1;
}

void __cdecl SwapBuffer(RENDER_BMP* bmp, HDC hdc)
{
	if (d3d9 == nullptr)
		return;

	static int initial = 1;

	if (initial == 1)
		initial = 0;
	else
	{
		d3d9dev->EndScene();
		d3d9dev->Present(nullptr, nullptr, nullptr, nullptr);
	}

	d3d9dev->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(255, 255, 255), 1.f, 0);
	d3d9dev->BeginScene();

#if USE_SOFTWARE
	D3DLOCKED_RECT r;
	if (FAILED(d3d9tex->LockRect(0, &r, nullptr, 0)))
		return;

	DWORD* dst = (DWORD*)r.pBits;
	BYTE* src = (BYTE*)bmp->ppvbits;
	for (int y = 0; y < 480; y++)
	{
		DWORD* l = dst;
		for (int x = 0; x < 640; x++)
		{
			BYTE b = *src++;
			if (b == 255)
				*l++ = 0xff000000;
			else *l++ = D3DCOLOR_ARGB(255, pal_obj.pal.palPalEntry[b].peRed,
				pal_obj.pal.palPalEntry[b].peGreen,
				pal_obj.pal.palPalEntry[b].peBlue);
		}
		dst += r.Pitch / 4;
	}

	if (FAILED(d3d9tex->UnlockRect(0)))
		return;

	fvf p[4];
	const float ofs = -0.5f;
	float x0 = ofs, y0 = ofs, x1 = RENDER_W + ofs, y1 = RENDER_H + ofs;
	float u0 = 0.f, v0 = 0.f, u1 = 1.f, v1 = 1.f;

	p[0].x = x0, p[0].y = y0, p[0].z = 0.5f, p[0].w = 1.f, p[0].tu = u0, p[0].tv = v0, p[0].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	p[1].x = x1, p[1].y = y0, p[1].z = 0.5f, p[1].w = 1.f, p[1].tu = u1, p[1].tv = v0, p[1].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	p[2].x = x0, p[2].y = y1, p[2].z = 0.5f, p[2].w = 1.f, p[2].tu = u0, p[2].tv = v1, p[2].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	p[3].x = x1, p[3].y = y1, p[3].z = 0.5f, p[3].w = 1.f, p[3].tu = u1, p[3].tv = v1, p[3].diffuse = D3DCOLOR_XRGB(255, 255, 255);
	d3d9dev->SetTexture(0, d3d9tex);
	d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	d3d9dev->DrawPrimitiveUP(D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));

	d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	d3d9dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
	d3d9dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3d9dev->SetTexture(0, d3d9text);
	d3d9dev->DrawPrimitiveUP(D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));
#endif

	*(DWORD*)0x423508 = 1;
}

void __cdecl HDCDrawRect(HDC a1, int x0, int y0, int x1, int y1, COLORREF col)
{
#if USE_SOFTWARE
	BYTE* dst = (BYTE*)render_bmp.ppvbits;
	size_t pitch = align(render_bmp.bmp_info->info.biWidth, 4);

	dst += y0 * pitch + x0;
	byte r = (byte)(col & 0xff);
	byte g = (byte)(col >> 8);
	byte b = (byte)(col >> 16);

	byte c = find_closest_color(r, g, b);
	for (int y = 0; y < y1 - y0; y++)
	{
		memset(dst, c, x1 - x0);
		dst += pitch;
	}
#else
	float fx0 = (float)x0, fy0 = (float)y0, fx1 = (float)x1, fy1 = (float)y1;

	byte r = (byte)(col & 0xff);
	byte g = (byte)(col >> 8);
	byte b = (byte)(col >> 16);

	fvf p[4];
	D3DCOLOR diffuse = D3DCOLOR_XRGB(r, g, b);
	p[0].x = fx0, p[0].y = fy0, p[0].z = 0.5f, p[0].w = 1.f, p[0].diffuse = diffuse;
	p[1].x = fx1, p[1].y = fy0, p[1].z = 0.5f, p[1].w = 1.f, p[1].diffuse = diffuse;
	p[2].x = fx0, p[2].y = fy1, p[2].z = 0.5f, p[2].w = 1.f, p[2].diffuse = diffuse;
	p[3].x = fx1, p[3].y = fy1, p[3].z = 0.5f, p[3].w = 1.f, p[3].diffuse = diffuse;
	d3d9dev->SetTexture(0, nullptr);
	d3d9dev->DrawPrimitiveUP(D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));
#endif
}

void __cdecl GameDrawRect(VM* game, HDC hdc, __int16 x0, __int16 y0, __int16 x1, __int16 y1, byte r, byte g, byte b)
{
#if USE_SOFTWARE
	BYTE *dst = (BYTE*)render_bmp.ppvbits;
	size_t pitch = align(render_bmp.bmp_info->info.biWidth, 4);

	dst += y0 * pitch + x0;

	byte col = find_closest_color(r, g, b);
	for (int y = 0; y < y1 - y0; y++)
	{
		memset(dst, col, x1 - x0);
		dst += pitch;
	}
#else
	float fx0 = (float)x0, fy0 = (float)y0, fx1 = (float)x1, fy1 = (float)y1;

	fvf p[4];
	D3DCOLOR diffuse = D3DCOLOR_XRGB(r, g, b);
	p[0].x = fx0, p[0].y = fy0, p[0].z = 0.5f, p[0].w = 1.f, p[0].diffuse = diffuse;
	p[1].x = fx1, p[1].y = fy0, p[1].z = 0.5f, p[1].w = 1.f, p[1].diffuse = diffuse;
	p[2].x = fx0, p[2].y = fy1, p[2].z = 0.5f, p[2].w = 1.f, p[2].diffuse = diffuse;
	p[3].x = fx1, p[3].y = fy1, p[3].z = 0.5f, p[3].w = 1.f, p[3].diffuse = diffuse;
	d3d9dev->SetTexture(0, nullptr);
	d3d9dev->DrawPrimitiveUP(D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));
#endif
}

void __cdecl GameDrawFrame(VM* a1, HDC a2, __int16 x0, __int16 y0, __int16 x1, __int16 y1, byte r, byte g, byte b)
{
#if USE_SOFTWARE
	BYTE* dst = (BYTE*)render_bmp.ppvbits;
	size_t pitch = align(render_bmp.bmp_info->info.biWidth, 4);

	dst += y0 * pitch + x0;

	byte col = find_closest_color(r, g, b);

	// first line
	memset(dst, col, x1 - x0);
	dst += pitch;

	// sides
	for (int y = 0; y < y1 - y0; y++)
	{
		dst[0] = col;
		dst[x1 - 1] = col;
		dst += pitch;
	}

	// last line
	memset(dst, col, x1 - x0);
#else
	float fx0 = (float)x0, fy0 = (float)y0, fx1 = (float)x1, fy1 = (float)y1;

	fvf p[4];
	D3DCOLOR diffuse = D3DCOLOR_XRGB(r, g, b);
	p[0].x = fx0, p[0].y = fy0, p[0].z = 0.5f, p[0].w = 1.f, p[0].diffuse = diffuse;
	p[1].x = fx1, p[1].y = fy0, p[1].z = 0.5f, p[1].w = 1.f, p[1].diffuse = diffuse;
	p[2].x = fx0, p[2].y = fy1, p[2].z = 0.5f, p[2].w = 1.f, p[2].diffuse = diffuse;
	p[3].x = fx1, p[3].y = fy1, p[3].z = 0.5f, p[3].w = 1.f, p[3].diffuse = diffuse;
	d3d9dev->SetTexture(0, nullptr);
	d3d9dev->DrawPrimitiveUP(D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));
#endif
}

LONG __cdecl GamePrintChar(VM* a1, HDC hdc, short x, short y, LPCSTR str)
{
	LPDIRECT3DSURFACE9 surf;
	d3d9text->GetSurfaceLevel(0, &surf);
	surf->GetDC(&hdc);

	SetBkColor(hdc, (LOBYTE(game->bkcol_b) << 16) | (LOBYTE(game->bkcol_g) << 8) | LOBYTE(game->bkcol_r));
	SetTextColor(hdc, (LOBYTE(game->texcol_b) << 16) | (LOBYTE(game->texcol_g) << 8) | LOBYTE(game->texcol_r));
	SelectObject(hdc, game->hFont);

	int len;
	struct tagSIZE size;

	len = strlen(str);
	TextOutA(hdc, x, y, str, len);
	len = strlen(str);
	GetTextExtentPoint32A(hdc, str, len, &size);

	surf->ReleaseDC(hdc);
	surf->Release();

	return size.cx;
}

void Inject_game()
{
#if !USE_SOFTWARE
	INJECT(0x406260, RenderToOffScreen);
#endif
	INJECT(0x40958D, CreateOffScreenBitmap);
	INJECT(0x40984F, SwapBuffer);
	INJECT(0x412985, GamePrintChar);

	INJECT(0x41280E, GameDrawRect);
	INJECT(0x4128C6, GameDrawFrame);
	INJECT(0x406537, HDCDrawRect);
}
