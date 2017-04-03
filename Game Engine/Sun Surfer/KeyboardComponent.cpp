#include "KeyboardComponent.h"



bool KeyboardComponent::Initialize()
{
	m_keyboard = MyKeyboard::GetInstance();
	return true;
}

bool KeyboardComponent::KeyIsDown(int vKey)
{
	return m_keyboard->KeyIsDown(vKey);
}

bool KeyboardComponent::KeyWasPressed(int vKey)
{
	return m_keyboard->KeyWasPressed(vKey);
}

bool KeyboardComponent::KeyWasReleased(int vKey)
{
	return m_keyboard->KeyWasReleased(vKey);
}

bool KeyboardComponent::AddToggle(int vKey, bool * pToggle, bool fireOnPress)
{
	return m_keyboard->AddToggle(vKey, pToggle, fireOnPress);
}

bool KeyboardComponent::AddKey(int vKey)
{
	return m_keyboard->AddKey(vKey);
}

bool KeyboardComponent::AddKeys(const char * keys)
{
	return m_keyboard->AddKeys(keys);
}
