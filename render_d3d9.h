#pragma once

#define GAME_W		256
#define GAME_H		240

#define GAME_REALW	640
#define GAME_REALH	480

#define GAME_X		((GAME_REALW / 2 - GAME_W) / 2)
#define GAME_Y		0

//#define GETX(x)		((x) / 2 - GAME_X)	// center

int GETX(int x);

#define GETY(y)		((y) / 2 - GAME_Y)
#define GETXR(x)	((x) / 2 - 8)	// push the right

extern LPDIRECT3D9 d3d9;
extern LPDIRECT3DDEVICE9 d3d9dev;
extern LPDIRECT3DTEXTURE9 d3d9rend;
extern LPDIRECT3DTEXTURE9 d3d9text;
extern D3DFORMAT d3dfmttex;

void InitRender();
void SwapBuffer();
void RenderRect(CTim* tim, int x, int y, int w, int h, int u, int v, BYTE r, BYTE g, BYTE b);
void RenderTile(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b);
