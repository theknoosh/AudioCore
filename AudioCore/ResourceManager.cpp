#include "ResourceManager.h"


//-------------------------------------------------------------------------------------
//=====================================================================================
//--------------------Load Resource from XML File -------------------------------------
//=====================================================================================
bool cResourceManager::loadFromXMLFile(std::string Filename)
{
	TiXmlDocument doc(Filename.c_str());

	if(doc.LoadFile())
	{
		//find topmost resources node in XML file
		TiXmlNode* ResourceTree = doc.FirstChild("resources");

		if(ResourceTree)
		{
			//Enumerate resource objects
			for(TiXmlNode* child = ResourceTree->FirstChild();child; child = child->NextSibling())
			{
				TiXmlElement *Element = child->ToElement();

				if(Element)
				{
					cResource *Resource = NULL;

					for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib = ElementAttrib->Next())
					{
						//examine our resource object
						std::string AttribName = ElementAttrib->Name();
						std::string AttribValue = ElementAttrib->Value();

						//detect resource type. Graphic, Texture, Audio
						if(AttribName=="type")
						{
							//Allow managers to implement derived versions of cResource.
							//Managers will create resources and pass a cResource point to be
							//added to resource list.  
							if(AttribValue=="graphic")
							{
								//Resource = g_RenderManager->loadResourceFromXML(Element);  //as in CGraphicResource = etc
							}

							if(AttribValue=="texture")
							{
								//Resource = g_TextureManager->loadResourceFromXML(Element);  CTextureResource
							}

							if(AttribValue=="audio")
							{
								//Resource = g_AudioManager->loadResourceFromXML(Element);  CAudioResource
							}
						}

						if(Resource)
						{
							if(AttribName=="UID")
							{
								Resource->m_ResourceID = atoi(AttribValue.c_str());
							}

							if(AttribName=="filename")
							{
								Resource->m_FileName = AttribValue;
							}

							if(AttribName=="scenescope")
							{
								Resource->m_Scope = atoi(AttribValue.c_str());
							}
						}
					}

					if(Resource)
					{
						//Resources are added to map here
						m_Resources[Resource->m_Scope].push_back(Resource);
						m_ResourceCount++;
					}
				}
			}
			return true;
		}
	}
	return false;
}