#pragma once
#include "..\font.h"
#include "..\texture.h"

#define GAME_W		256
#define GAME_H		240

#define GAME_REALW	640
#define GAME_REALH	480

#define GAME_X		((GAME_REALW / 2 - GAME_W) / 2)
#define GAME_Y		0

#define GETX(x)		((x) / 2 - GAME_X)	// center

#define GETY(y)		((y) / 2 - GAME_Y)
#define GETXR(x)	((x) / 2 - 8)	// push the right

// map/sprite rendering region
#define WINDOW_W	240
#define WINDOW_H	152
#define WINDOW_X	((GAME_W - WINDOW_W) / 2)
#define WINDOW_Y	8

enum
{
	RENDTARG_BACK,
	RENDTARG_SPRITE,
	RENDTARG_FRONT
};

extern LPDIRECT3D9 d3d9;
extern LPDIRECT3DDEVICE9 d3d9dev;
extern LPDIRECT3DTEXTURE9 d3d9rend;
extern LPDIRECT3DTEXTURE9 d3d9text;
extern D3DFORMAT d3dfmttex;

class CTextureD3D9 : public CTexture
{
public:
	CTextureD3D9() : tex(nullptr)
	{}

	virtual int Create(WORD* clut, BYTE* data, int bpp, int w, int h, int u, int v);
	virtual void Release();

	LPDIRECT3DTEXTURE9 tex;
};

void InitRender();
void SwapBuffer();
void RenderRect(CTim* tim, int x, int y, int w, int h, int u, int v, BYTE r, BYTE g, BYTE b);
void RenderRect(LPDIRECT3DTEXTURE9 tex, int x, int y, int real_w, int real_h, int w, int h, int u, int v, BYTE r, BYTE g, BYTE b);
void RenderTile(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a = 255);
void RenderGlyph(int x, int y, FONT_GLYPH* g, DWORD* gfx);

int GetX(int x, int double_w);
