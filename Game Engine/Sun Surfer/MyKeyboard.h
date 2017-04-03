#ifndef MYKEYBOARD_H_
#define MYKEYBOARD_H_


struct Key
{
	int vKey;
	bool wasDownLastFrame;
	bool justPressed;
	bool justReleased;
};

struct KeyToggle
{
	int vKey;
	bool* pToggle;
	bool toggleOnPress;

	void Toggle() { *(pToggle) = !*(pToggle); }
};

class MyKeyboard
{
private:
	MyKeyboard() {};
	~MyKeyboard() {};

public:
	static MyKeyboard* GetInstance();

	void Update(float dt);
	bool KeyIsDown(int vKey);	// returns true if the key is currently down
	bool KeyWasPressed(int vKey);	// return true if the key was just pressed
	bool KeyWasReleased(int vKey);	// returns true if the key was just released
	bool AddToggle(int vKey, bool* pToggle, bool fireOnPress = true); // this method will automatically toggle a bool value for us whenever the given key is pressed (or released if fireOnPress is false). Once we set this up, we don’t have to check it anymore, we simply check the current value of the bool, it will change automatically for us.
	bool AddKey(int vKey); // this tells our keyboard system to keep track of this key every frame so we can detect whether it has just been pressed or released. Ex: AddKey(‘W’);
	bool AddKeys(const char* keys/*, int numKeys*/); // lets us add multiple keys. Ex: AddKeys(“01234”);

private:
	void UpdateKeys();
	void UpdateToggles();

private:
	static const int maxKeys = 20;
	static const int maxToggles = 6;
	KeyToggle m_toggles[maxToggles]{ 0 };
	int		  m_keys[maxKeys]{ 0 };
	Key		  m_previousKeyboardState[256];
	int		  m_numkeys{ 0 };
	int		  m_numToggles{ 0 };

	static MyKeyboard* pKeyboard;
};
#endif // !MYKEYBOARD_H_
