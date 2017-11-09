#pragma once

#include "stage.h"

#include "../Engine/backbuffer.h"

#include <vector>

class Font;
class Label;

class Menu : public Stage {
public:
	using OnAction = void(*)(Menu*);

	class MenuEntry {
	public:
		MenuEntry(Label* label, OnAction onAction, unsigned short ySpace = 0);
		~MenuEntry();
		Label* GetLabel() const;
		bool IsSelectable() const;
		OnAction GetEvent() const;
		unsigned short GetYSpace();
	private:
		MenuEntry(MenuEntry& entry);

		Label* m_pLabel;
		OnAction m_action;
		unsigned short m_space;
	};

	Menu(Font* font);
	~Menu();
	void Initialize(Engine::BackBuffer& backBuffer);
	virtual void Draw(Engine::BackBuffer& backBuffer);
	void Input(SDL_Event* input);
	MenuEntry* GetCurrentSelection();

protected:
	virtual void addElements() = 0;
	void addElement(MenuEntry* entry);
private:
	bool Menu::nextElement(std::vector<MenuEntry*>::iterator& start, bool forward);
	std::vector<MenuEntry*> m_entries;
	std::vector<MenuEntry*>::iterator m_selectedIndex;
};