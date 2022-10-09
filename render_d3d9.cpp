#include <stdafx.h>
#include <d3d9.h>
#include "game.h"

RENDER_BMP render_bmp;

#define RENDER_W		1280
#define RENDER_H		960

LPDIRECT3D9 d3d9 = nullptr;
LPDIRECT3DDEVICE9 d3d9dev = nullptr;
LPDIRECT3DTEXTURE9 d3d9rend = nullptr;
LPDIRECT3DTEXTURE9 d3d9text = nullptr;
D3DFORMAT d3dfmttex;

const UINT FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

typedef struct fvf
{
	float x, y, z, w;
	D3DCOLOR diffuse;
	float tu, tv;
} fvf;

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

int CreateOffScreenBitmap(RENDER_BMP* bmp, int w, int h)
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

	// resize window
	BOOL menu = (GetMenu(prog.hWnd) != nullptr);
	auto dwStyle = GetWindowLongA(prog.hWnd, GWL_STYLE);
	RECT rc;
	SetRect(&rc, 0, 0, w, h);

	//SetWindowLongA(hWnd, GWL_STYLE, dwStyle);
	POINT pm = { 0, 0 };
	AdjustWindowRectEx(&rc, dwStyle, menu, 0);
	SetWindowPos(prog.hWnd, HWND_TOP, pm.x + 0, pm.y + 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	SetWindowPos(prog.hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE);
	InvalidateRect(prog.hWnd, nullptr, true);

	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	if (FAILED(d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
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
	d3dpp.hDeviceWindow = prog.hWnd;
	d3dpp.BackBufferWidth = w;
	d3dpp.BackBufferHeight = h;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = GetBestZBuffer(d3ddm.Format);
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, prog.hWnd, dwBehaviorFlags, &d3dpp, &d3d9dev)))
		return 0;

	d3d9dev->CreateTexture(GAME_W, GAME_H, 1, D3DUSAGE_RENDERTARGET, d3dfmttex, D3DPOOL_DEFAULT, &d3d9rend, nullptr);

	d3d9dev->SetFVF(FVF);

	d3d9dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	d3d9dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	d3d9dev->SetRenderState(D3DRS_ZWRITEENABLE, 0);
	d3d9dev->SetRenderState(D3DRS_ZENABLE, 0);

	return 1;
}

void InitRender()
{
	CreateOffScreenBitmap(&render_bmp, 640, 480);
}

LPDIRECT3DSURFACE9 targ;
LPDIRECT3DSURFACE9 old_targ;

void SwapBuffer()
{
	if (d3d9 == nullptr)
		return;

	static int initial = 1;

	if (initial == 1)
		initial = 0;
	else
	{
		d3d9dev->SetRenderTarget(0, old_targ);

		fvf p[4];
		const float ofs = -0.5f;
		float x0 = ofs, y0 = ofs,
			x1 = (float)render_bmp.w + ofs, y1 = (float)render_bmp.h + ofs;
		float u0 = 0.f, v0 = 0.f,
			u1 = 1.f, v1 = 1.f;
		DWORD diffuse = D3DCOLOR_XRGB(255, 255, 255);

		d3d9dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		d3d9dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

		p[0].x = x0, p[0].y = y0, p[0].z = 0.5f, p[0].w = 1.f, p[0].tu = u0, p[0].tv = v0, p[0].diffuse = diffuse;
		p[1].x = x1, p[1].y = y0, p[1].z = 0.5f, p[1].w = 1.f, p[1].tu = u1, p[1].tv = v0, p[1].diffuse = diffuse;
		p[2].x = x0, p[2].y = y1, p[2].z = 0.5f, p[2].w = 1.f, p[2].tu = u0, p[2].tv = v1, p[2].diffuse = diffuse;
		p[3].x = x1, p[3].y = y1, p[3].z = 0.5f, p[3].w = 1.f, p[3].tu = u1, p[3].tv = v1, p[3].diffuse = diffuse;
		d3d9dev->SetTexture(0, d3d9rend);
		d3d9dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));

		d3d9dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		d3d9dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

		d3d9dev->EndScene();
		d3d9dev->Present(nullptr, nullptr, nullptr, nullptr);

		targ->Release();
	}

	d3d9dev->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
	d3d9dev->BeginScene();

	d3d9rend->GetSurfaceLevel(0, &targ);
	d3d9dev->GetRenderTarget(0, &old_targ);
	d3d9dev->SetRenderTarget(0, targ);

	d3d9dev->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	d3d9dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3d9dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void RenderRect(CTim* tim, int x, int y, int w, int h, int u, int v, BYTE r, BYTE g, BYTE b)
{
	const float ofs = -0.5f;
	float x0 = (float)x + ofs, y0 = (float)y + ofs,
		x1 = x0 + (float)w, y1 = y0 + (float)h;

	float uw = 1.f / tim->real_w;
	float vh = 1.f / tim->pix_h;

	float u0 = (float)u * uw, v0 = (float)v * vh,
		u1 = u0 + w * uw, v1 = v0 + h * vh;

	DWORD diffuse = D3DCOLOR_XRGB(r, g, b);

	fvf p[4];
	p[0].x = x0, p[0].y = y0, p[0].z = 0.5f, p[0].w = 1.f, p[0].tu = u0, p[0].tv = v0, p[0].diffuse = diffuse;
	p[1].x = x1, p[1].y = y0, p[1].z = 0.5f, p[1].w = 1.f, p[1].tu = u1, p[1].tv = v0, p[1].diffuse = diffuse;
	p[2].x = x0, p[2].y = y1, p[2].z = 0.5f, p[2].w = 1.f, p[2].tu = u0, p[2].tv = v1, p[2].diffuse = diffuse;
	p[3].x = x1, p[3].y = y1, p[3].z = 0.5f, p[3].w = 1.f, p[3].tu = u1, p[3].tv = v1, p[3].diffuse = diffuse;

	d3d9dev->SetTexture(0, tim->tex);
	d3d9dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));
}

void RenderTile(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b)
{
	const float ofs = -0.5f;
	float x0 = (float)x + ofs, y0 = (float)y + ofs,
		x1 = x0 + (float)w, y1 = y0 + (float)h;

	DWORD diffuse = D3DCOLOR_XRGB(r, g, b);

	fvf p[4];
	p[0].x = x0, p[0].y = y0, p[0].z = 0.5f, p[0].w = 1.f, p[0].diffuse = diffuse;
	p[1].x = x1, p[1].y = y0, p[1].z = 0.5f, p[1].w = 1.f, p[1].diffuse = diffuse;
	p[2].x = x0, p[2].y = y1, p[2].z = 0.5f, p[2].w = 1.f, p[2].diffuse = diffuse;
	p[3].x = x1, p[3].y = y1, p[3].z = 0.5f, p[3].w = 1.f, p[3].diffuse = diffuse;

	d3d9dev->SetTexture(0, nullptr);
	d3d9dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, p, sizeof(fvf));
}