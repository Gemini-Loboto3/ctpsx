#pragma once

enum ENCODING_TYPE
{
	ENCTYPE_DIRECT,	// direct add/sub, no lookup necessary
	ENCTYPE_RANGE,	// correspondences
	ENCTYPE_MATCH	// lookup tables
};

enum FONT_TYPE
{
	FT_REFERENCE,
	FT_INDEX
};

typedef struct tagFontHeader
{
	DWORD magic;
	WORD version;
	WORD count;
} FONT_HEADER;

typedef struct tagFontPointer
{
	WORD min, max;
	DWORD ptr:24;
	DWORD type:8;
} FONT_POINTER;

typedef struct tagFontData
{
	BYTE *data;
	FONT_POINTER *ptr;
} FONT_DATA;

typedef struct tagFontIndex
{
	DWORD count;
	DWORD ptr[1];
} FONT_INDEX;

typedef struct tagFontGlyph
{
	BYTE w, h;
	BYTE x, y;
	WORD data[1];
} FONT_GLYPH;

typedef struct tagFontBitread
{
	BYTE* data;
	int bit;
	DWORD byte;
} FONT_BITREAD;

/* create a memory handle to the font */
void *FontOpen(BYTE* data, FONT_DATA *fnt);

/* retrieve glyph data */
FONT_GLYPH *FontGetGlyph(const FONT_DATA *fnt, const DWORD index);

/* expand glyph data */
void FontExpandGlyph(FONT_GLYPH *g, DWORD *dst);

