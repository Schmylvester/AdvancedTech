#pragma once
#include <vector>

enum class KeyBind
{
	Esc = 27,
	Space = 32,
	Bar0 = 48, Bar1, Bar2, Bar3, Bar4, Bar5, Bar6, Bar7, Bar8, Bar9,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
};

enum class KeyState
{
	DOWN,
	HELD,
	UP,
};

class KeyboardInput
{
public:
	KeyboardInput() = default;
	~KeyboardInput() = default;

	void keyDown(KeyBind key_id);
	void keyUp(KeyBind key_id);
	bool searchInputs(KeyBind key_id, KeyState type);
	void tick();

private:
	std::vector<KeyBind> key_down;
	std::vector<KeyBind> key;
	std::vector<KeyBind> key_up;
};

