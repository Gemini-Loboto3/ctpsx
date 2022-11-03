#pragma once

void GameDrawRects(VM* vm, int rect_no);
void GameDrawFromRects(VM* vm, int rect_no);

void Game_BgDispTrnRect(VM* vm);
void Game_BgDispTrn_1(VM* vm);
void Game_BgDispTrn_0(VM* vm);
void Game_RedrawAll(VM* vm);
void Game_BgDispTrn(VM* vm, int x, int y, int w, int h, __int16 a6, __int16 a7, WORD id, int type, int a10);
void Game_RedrawScene(VM* vm);

void Render_bank(int x, int y, int w, int h, int srcx, int srcy, CTexture* bank, int mode);
