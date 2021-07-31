#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "MapAttr.h"

CMapAttr::CMapAttr()
{
	m_size[0] = m_size[1] = 0;
	m_attr = NULL;
	m_height = NULL;
}

CMapAttr::~CMapAttr()
{
	if (m_attr)
	{
		int i;
		for (i = 0; i < m_size[0]; i++)
			delete[] m_attr[i];
		delete[] m_attr;
		m_attr = NULL;
	}

	if (m_height)
	{
		int i;
		for (i = 0; i < m_size[0]; i++)
			delete[] m_height[i];
		delete[] m_height;
		m_height = NULL;
	}
}

bool CMapAttr::Load(int zone, int ylayer, int w, int h, const char* attrmap, const char* heightmap)
{
	GAMELOG << init("SYSTEM")
			<< "Attribute File Reading "
			<< ylayer
			<< end;
	
	FILE* fpAttr;
	FILE* fpHeight;
	CLCString filename(1024);
	filename.Format("%s%s", gserver.m_serverpath, attrmap);
	fpAttr = fopen(attrmap, "rb");
	if (!fpAttr)
		return false;
	filename.Format("%s%s", gserver.m_serverpath, heightmap);
	fpHeight = fopen(heightmap, "rb");
	if (!fpHeight)
	{
		fclose(fpAttr);
		return false;
	}
	
	int x, z;

	unsigned char** tmp = new unsigned char*[w];
	float** tmp2 = new float*[w];
	for (x = 0; x < w; x++)
	{
		tmp[x] = new unsigned char[h];
		tmp2[x] = new float[h];
	}
	
	unsigned char att;
	unsigned short hm;
	bool bFail = false;
	
	for (z = 0; !bFail && z < h; z++)
	{
		for (x = 0; !bFail && x < w; x++)
		{
			if (fread(&att, sizeof(unsigned char), 1, fpAttr) <= 0
				|| fread(&hm, sizeof(unsigned short), 1, fpHeight) <= 0)
			{
				bFail =  true;
				break;
			}
			
			switch (att)
			{
			case MAPATT_FIELD:
				tmp[x][z] = MAPATT_FIELD;
				break;

			case MAPATT_PEACEZONE:
				tmp[x][z] = MAPATT_PEACEZONE;
				break;

			case MAPATT_PRODUCT_PUBLIC:
				tmp[x][z] = MAPATT_PRODUCT_PUBLIC;
				break;

			case MAPATT_PRODUCT_PRIVATE:
				tmp[x][z] = MAPATT_PRODUCT_PRIVATE;
				break;
				
			case MAPATT_STAIR_UP:
				tmp[x][z] = MAPATT_STAIR_UP;
				break;
				
			case MAPATT_STAIR_DOWN:
				tmp[x][z] = MAPATT_STAIR_DOWN;
				break;

			case MAPATT_WARZONE:
				tmp[x][z] = MAPATT_WARZONE;
				break;

			case MAPATT_FREEPKZONE:
				tmp[x][z] = MAPATT_FREEPKZONE;
				break;
		
			default:
				tmp[x][z] = MAPATT_BLOCK;
				break;
			}
			
			tmp2[x][z] = ntohs(hm) / MULTIPLE_HEIGHTMAP;
		}
	}
	
	fclose(fpAttr);
	fclose(fpHeight);
	
	if (bFail)
	{
		for (x = 0; x < w; x++)
		{
			if (tmp[x])
				delete[] tmp[x];
			if (tmp2[x])
				delete[] tmp2[x];
		}
		delete[] tmp;
		delete[] tmp2;
		return false;
	}
	
	m_size[0] = w;
	m_size[1] = h;
	m_attr = tmp;
	m_height = tmp2;
	return true;
}
