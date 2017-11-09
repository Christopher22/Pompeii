// COMP710 GP 2D Framework 2017

// This include:
#include "texturemanager.h"

// Local includes:
#include "texture.h"
#include "logmanager.h"

// Library includes:
#include <SDL.h>

namespace Engine
{
	TextureManager::TextureManager()
		: m_pRenderer(0)
	{

	}

	TextureManager::~TextureManager()
	{
		for (const auto& t : m_pLoadedTextures)
		{
			delete t.second;
		}
	}

	bool
		TextureManager::Initialise(SDL_Renderer* pRenderer)
	{
		m_pRenderer = pRenderer;

		return (true);
	}

	Texture*
		TextureManager::GetTexture(const char* pcFilename)
	{
		Texture* pTexture = 0;

		if (m_pLoadedTextures.find(pcFilename) == m_pLoadedTextures.end())
		{
			// Not already loaded... so load...
			pTexture = new Texture();
			if (!pTexture->Initialise(pcFilename, m_pRenderer))
			{
				delete pTexture;
				LogManager::GetInstance().Log("Texture failed to initialise!");
				pTexture = nullptr;
			} 
			else
			{
				m_pLoadedTextures[pcFilename] = pTexture;
			}			
		}
		else
		{
			// Is already loaded...
			pTexture = m_pLoadedTextures[pcFilename];
		}

		return (pTexture);
	}
}