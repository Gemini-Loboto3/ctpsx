#include <stdafx.h>
#include "font.h"
//#include "../encoding/encoding.h"

/*
	Setup memory handle for a font.
*/
void *FontOpen(BYTE* data, FONT_DATA *fnt)
{
	FONT_HEADER *h=(FONT_HEADER*)data;

	if(h->magic!='TNFG')
	{
		//DEBUGPRINT(("Incorrect font file.\n"));
		return 0;
	}

	fnt->data=data;
	fnt->ptr=(FONT_POINTER*)(data+sizeof(FONT_HEADER));

	//DEBUGPRINT(("Font open at %x, ptr=%x\n",fnt->data,fnt->ptr));

	return data;
}

const static FONT_GLYPH empty_glyph={0};

/*
	Obtain pointer to a character. If nothing was found, return NULL.
*/
FONT_GLYPH *FontGetGlyph(const FONT_DATA *fnt, const DWORD index)
{
	int i, size, j;
	const DWORD *sptr;
	const FONT_INDEX *idxp;
	const WORD* idx;
	const BYTE* ref;

	size=(fnt->ptr[0].ptr-sizeof(FONT_HEADER))/sizeof(FONT_POINTER);
	for(i=0; i<size; i++)
	{
		// break search, no match can be possibly found, EVER
		//if(index>fnt->ptr[i].max) break;
		// check possible ranges
		if(index>=fnt->ptr[i].min && index<=fnt->ptr[i].max)
		{
			switch(fnt->ptr[i].type)
			{
			case FT_REFERENCE:
				sptr=(DWORD*)(fnt->data+fnt->ptr[i].ptr);
				// found an empty glyph
				if(!sptr[index-fnt->ptr[i].min]) return (FONT_GLYPH*)&empty_glyph;
				// actual glyph found otherwise
				return (FONT_GLYPH*)(fnt->data+sptr[index-fnt->ptr[i].min]);
			case FT_INDEX:
				ref=(BYTE*)&fnt->data[fnt->ptr[i].ptr];
				// pointers
				idxp=(FONT_INDEX*)ref;
				// index lookup table
				idx=(WORD*)&ref[4+idxp->count*4];
				for(j=0; j<(int)idxp->count; j++)
				{
					if(idx[j]==index)
					{
						// found an empty glyph
						if(!idxp->ptr[j]) return (FONT_GLYPH*)&empty_glyph;
						// actual glyph found otherwise
						return (FONT_GLYPH*)&fnt->data[idxp->ptr[j]];
					}
				}
				break;
			default:
				//DEBUGPRINT(("Unsupported type %d\n",fnt->ptr[i].type));
				return 0;
			}
		}
	}

	//DEBUGPRINT(("Could not find symbol %x\n",index));
	return 0;
}

/*
	Initialize font reader for FontUpscaleGlyph().
*/
static __inline void FontOpenReader(FONT_BITREAD *b, WORD* data)
{
	b->data=(BYTE*)data;
	b->bit=0;
	b->byte=0;
}

/*
	Used by FontUpscaleGlyph() to retrieve glyph pixels (1 pixel=2 bits).
*/
static DWORD FontReadBits(FONT_BITREAD *b, int nb)
{
	DWORD val=0;
	int i;

	// test valid range
	if(nb>0 && nb<=32)
	{
		for(i=0; i<nb; i++)
		{
			b->bit--;
			if(b->bit<0)
			{
				b->byte=*b->data++;
				b->bit=val%256;
				b->bit=7;
			}
			// obtain a bit
			val=val*2+((b->byte>>b->bit)%2);
		}
	}
	return val;
}

/*
	Expand glyph data to framebuffer format uploadable by LoadImage().

	WARNING: 'dst' must be at least 64 bytes to hold the biggest expanded data.
*/
void FontExpandGlyph(FONT_GLYPH *g, DWORD *dst)
{
	int x, y, gw, w;
	DWORD row;
	FONT_BITREAD b;

	FontOpenReader(&b,g->data);

	// convert glyph
	for(y=0; y<g->h; y++)
	{
		// upscale an entire scanline
		for(gw=g->w; gw>0; gw-=8)
		{
			w=(gw>8 ? 8 : gw);
			for(x=0, row=0; x<w; x++)
			{
				row>>=4;
				row|=FontReadBits(&b,2)<<28;
			}
			// skip remaining bits
			for(; x<8; x++) row>>=4;
			// flush to dest buffer
			*dst++=row;
		}
	}
}
