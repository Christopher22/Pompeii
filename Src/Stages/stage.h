#pragma once

namespace Engine {
	class BackBuffer;
}

union SDL_Event;
class Font;

class Stage {
public:
	Stage(Font* font);
	virtual ~Stage();
	virtual void Process(float deltaTime) {}
	virtual void Draw(Engine::BackBuffer& backBuffer) = 0;
	virtual void Input(SDL_Event* input) = 0;
	Font* GetFont() const;
private:
	Font* m_pFont;
};