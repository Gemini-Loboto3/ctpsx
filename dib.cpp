#include <stdafx.h>
#include "game.h"

CTim *faces[87];

void LoadFace(const char* filename, CTim *tim)
{
	tim->Open(filename);
}

void LoadFaces()
{
	CTim fjenn, fmary1, fmary2,
		flaura, flotte, fanne,
		fsimon;

	LoadFace("FACE\\FACEJ.TIM",  &fjenn);
	LoadFace("FACE\\FACEM1.TIM", &fmary1);
	LoadFace("FACE\\FACEM2.TIM", &fmary2);
	LoadFace("FACE\\FACER.TIM",  &flaura);
	LoadFace("FACE\\FACEA.TIM",  &flotte);
	LoadFace("FACE\\FACEL.TIM",  &fanne);
	LoadFace("FACE\\FACEB.TIM",  &fsimon);

	for (int face = 1; face <= 87; face++)
	{
		int u = 0, v = 0, id = 0;
		CTim* src = nullptr;

		// jennifer
		if (face >=1 && face <= 24)
		{
			src = &fjenn;
			id = face - 1;
		}
		// mary
		else if (face >= 31 && face <= 39)
		{
			src = &fmary1;
			id = face - 31;
		}
		// mary sus
		else if (face >= 40 && face <= 45)
		{
			src = &fmary2;
			id = face - 40;
		}
		// laura
		else if (face >= 51 && face <= 56)
		{
			src = &flaura;
			id = face - 51;
		}
		// lotte
		else if (face >= 61 && face <= 66)
		{
			src = &flotte;
			id = face - 61;
		}
		// anne
		else if (face >= 71 && face <= 76)
		{
			src = &fanne;
			id = face - 71;
		}
		// simon
		else if(face >= 81 && face <= 87)
		{
			src = &fsimon;
			id = face - 81;
		}

		if (src)
		{
			u = (id % 6) * 40;
			v = (id / 6) * 40;

			faces[face - 1] = new CTim();
			faces[face - 1]->Copy(src, u, v, 40, 40);
		}
	}
}

CTexture* LoadDIB(const char* path)
{
	auto tex = MakeTexture();

	CTim tim;

	if (strncmp("FACE", path, 4) == 0)
	{
		int face = atoi(&path[10]);
		tex->Create(faces[face - 1]->clut, faces[face - 1]->pixel, faces[face - 1]->bpp, faces[face - 1]->real_w, faces[face - 1]->pix_h);
	}
	else
	{
		char temp[MAX_PATH];
		strncpy_s(temp, MAX_PATH, path, strrchr(path, '.') - path);
		strcat_s(temp, MAX_PATH, ".TIM");

		tim.Open(temp);
		tex->Create(tim.clut, tim.pixel, tim.bpp, tim.real_w, tim.pix_h);
	}

	return tex;
}

CTexture* MakeBankFromDIB(const char* path)
{
	return LoadDIB(path);
}