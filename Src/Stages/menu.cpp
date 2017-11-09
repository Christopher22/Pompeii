#include "menu.h"

#include "Helper/label.h"

#include <SDL.h>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <cassert>
#include <complex>

static const Label::Color SELECTION_COLOR = Label::Color(255, 0, 0);

Menu::Menu(Font* font) : Stage(font), m_entries(), m_selectedIndex(m_entries.end()){}

Menu::~Menu() {
	std::for_each(m_entries.begin(), m_entries.end(), [&](MenuEntry* entry) {
		delete entry;
		entry = nullptr;
	});
}

void Menu::Initialize(Engine::BackBuffer& backBuffer) {
	this->addElements();

	backBuffer.SetClearColour(0, 0, 0);

	const unsigned short height = std::accumulate(m_entries.begin(), m_entries.end(), 0, [&](unsigned short current, MenuEntry* entry) {
		return current + entry->GetLabel()->GetHeight() + entry->GetYSpace();
	});

	unsigned short y = backBuffer.GetHeight() / 2 - height / 2;
	std::for_each(m_entries.begin(), m_entries.end(), [&](MenuEntry* entry) {
		entry->GetLabel()->SetX(std::max(backBuffer.GetWidth() / 2 - entry->GetLabel()->GetWidth() / 2, 0));
		entry->GetLabel()->SetY(y);
		y += entry->GetLabel()->GetHeight() + entry->GetYSpace();
	});

	m_selectedIndex = m_entries.begin();
	this->nextElement(m_selectedIndex, true);
}

void Menu::addElement(MenuEntry* entry) {
	m_entries.push_back(entry);
}

void Menu::Draw(Engine::BackBuffer& backBuffer) {
	std::for_each(m_entries.begin(), m_entries.end(), [&](MenuEntry* entry) {
		entry->GetLabel()->Draw(backBuffer);
	});
}

void Menu::Input(SDL_Event* input) {
	if (input->type == SDL_JOYHATMOTION && (input->jhat.value == SDL_HAT_DOWN || input->jhat.value == SDL_HAT_UP)) {
		(*m_selectedIndex)->GetLabel()->ResetColor();
		bool forward = input->jhat.value == SDL_HAT_DOWN;

		if (!forward && m_selectedIndex == m_entries.begin()) {
			m_selectedIndex = m_entries.end();
		}
		if (!this->nextElement((forward ? ++m_selectedIndex : --m_selectedIndex), forward)) {
			m_selectedIndex = forward ? m_entries.begin() : m_entries.end() - 1;
			this->nextElement(m_selectedIndex, forward);
		}
	} else if (input->type == SDL_JOYBUTTONDOWN) {
		if (m_selectedIndex != m_entries.end()) {
			((*m_selectedIndex)->GetEvent())(this);
		}
	}
}

bool Menu::nextElement(std::vector<MenuEntry*>::iterator& start, bool forward) {
	if (!forward && m_selectedIndex == m_entries.begin()) {
		m_selectedIndex = m_entries.end() - 1;
	}

	auto end = (forward ? m_entries.end() : m_entries.begin());
	for (; start != end; forward ? ++start : --start) {
		if ((*start)->IsSelectable()) {
			(*start)->GetLabel()->SetColor(SELECTION_COLOR);
			return true;
		}
	}

	if (!forward && start == m_entries.begin()) {
		return (*start)->IsSelectable();
	} else {
		return false;
	}
}

Menu::MenuEntry::MenuEntry(Label* label, OnAction onAction, unsigned short ySpace) : m_pLabel(label), m_action(onAction), m_space(ySpace) {
	assert(label);
}

unsigned short Menu::MenuEntry::GetYSpace() {
	return m_space;
}

Menu::MenuEntry::~MenuEntry() {
	delete m_pLabel;
	m_pLabel = nullptr;
}

Label* Menu::MenuEntry::GetLabel() const {
	return m_pLabel;
}

bool Menu::MenuEntry::IsSelectable() const {
	return m_action != nullptr;
}

Menu::OnAction Menu::MenuEntry::GetEvent() const {
	return m_action;
}

Menu::MenuEntry* Menu::GetCurrentSelection() {
	return (m_selectedIndex != m_entries.end() ? *m_selectedIndex : nullptr);
}