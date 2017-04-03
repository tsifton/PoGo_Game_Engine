#ifndef KEYBOARDCOMPONENT_H_
#define KEYBOARDCOMPONENT_H_

#include "Component.h"

#include "MyKeyboard.h"

class KeyboardComponent : public Component
{
public:
	KeyboardComponent() {};
	~KeyboardComponent() {};

public:
	bool Initialize() override;

	bool KeyIsDown(int vKey);
	bool KeyWasPressed(int vKey);
	bool KeyWasReleased(int vKey);
	bool AddToggle(int vKey, bool* pToggle, bool fireOnPress = true);
	bool AddKey(int vKey);
	bool AddKeys(const char* keys/*, int numKeys*/);

private:
	MyKeyboard* m_keyboard;
};
#endif // !KEYBOARDCOMPONENT_H_
