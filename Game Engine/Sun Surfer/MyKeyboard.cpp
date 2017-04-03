#include "MyKeyboard.h"
#include "MyWindow.h"
#include "GameLogger.h"

MyKeyboard* MyKeyboard::pKeyboard;

MyKeyboard * MyKeyboard::GetInstance()
{
	if (!pKeyboard) pKeyboard = new MyKeyboard();
	return pKeyboard;
}

void MyKeyboard::Update(float /*m_dt*/)
{
	UpdateKeys();
	UpdateToggles();
}

bool MyKeyboard::KeyIsDown(int vKey)
{
	return (0 != GetAsyncKeyState(vKey));
}

bool MyKeyboard::KeyWasPressed(int vKey)
{
	return m_previousKeyboardState[vKey].justPressed;
}

bool MyKeyboard::KeyWasReleased(int vKey)
{
	return m_previousKeyboardState[vKey].justReleased;
}

bool MyKeyboard::AddToggle(int vKey, bool* pToggle, bool fireOnPress)
{
	if (m_numToggles >= maxToggles)
	{
		GameLogger::Log(MsgType::cFatalError, "MyKeyboard::AddToggle() is full at %d toggles.\n", maxToggles);
		return false;
	}
	if (!AddKey(vKey)) return false;
	m_toggles[m_numToggles++] = { vKey, pToggle, fireOnPress };
	return true;
}

bool MyKeyboard::AddKey(int vKey)
{
	if (m_numkeys >= maxKeys)
	{
		GameLogger::Log(MsgType::cFatalError, "MyKeyboard::AddKey() - Key buffer is full at %d keys\n", maxKeys);
		return false;
	}
	m_keys[m_numkeys++] = vKey;
	m_previousKeyboardState[vKey] = { vKey, KeyIsDown(vKey), false, false };
	return true;
}

bool MyKeyboard::AddKeys(const char * keys)
{
	for (const char* p = keys; *p; ++p)
	{
		if (!AddKey(*p)) return false;
	}
	return true;
}

void MyKeyboard::UpdateKeys()
{
	for (int i = 0; i < m_numkeys; i++)
	{
		int vKey = m_keys[i];
		Key *k = &m_previousKeyboardState[vKey];
		bool isDown = KeyIsDown(vKey);
		bool wasDown = m_previousKeyboardState[vKey].wasDownLastFrame;
		if (isDown) k->justPressed = !wasDown;
		else        k->justReleased = wasDown;
		m_previousKeyboardState[vKey].wasDownLastFrame = isDown;
	}
}

void MyKeyboard::UpdateToggles()
{
	for (int i = 0; i < m_numToggles; i++)
	{
		KeyToggle* toggle = &m_toggles[i];
		int vkey = toggle->vKey;
		Key* key = &m_previousKeyboardState[vkey];
		if (key->justPressed && toggle->toggleOnPress) toggle->Toggle();
		else if (key->justReleased && !toggle->toggleOnPress) toggle->Toggle();
	}
}
