#include "cTextureResource.h"

cTextureResource::~cTextureResource(void)
{
	unload();
}

void cTextureResource::load()
{
	D3DXCreateTextureFromFile(p_Device,StringToLPCWSTR(m_FileName),
		&p_Texture);
}
	
void cTextureResource::unload()
{
	if(p_Texture)
	{
		p_Texture->Release();
		p_Texture = NULL;
	}
}

LPCWSTR cTextureResource::StringToLPCWSTR(std::string s_Variable)
{
	std::wstring ws_Temp = std::wstring(s_Variable.begin(), s_Variable.end());
	return ws_Temp.c_str();
}