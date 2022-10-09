#pragma once

void GameDrawRects(VM* game, unsigned __int16 rect_no);
void GameDrawRects(VM* game, unsigned __int16 rect_no);
void GameDrawFromRects(VM* game, unsigned __int16 rect_no);

void Game_BgDispTrnRect(VM* a1);
void Game_BgDispTrn_1(VM* game);
void Game_BgDispTrn_0(VM* a1);
void Game_RedrawAll(VM* g);
void Game_BgDispTrn(VM* game, int x, int y, int w, int h, __int16 a6, __int16 a7, WORD id, int type, int a10);
void Game_RedrawScene(VM* a1);

void Render_bank(int x, int y, int w, int h, int srcx, int srcy, CTim* bank, int mode);
