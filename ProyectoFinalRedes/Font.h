#pragma once

#include <string>
#include <SDL2/SDL_ttf.h>

using namespace std;

class Font {
public:
	Font();
	Font(const string& fileName, int size);
	virtual ~Font();

	bool load(const string& fileName, int size);
	void close();
	SDL_Surface* renderText(const string& text, SDL_Color color) const;
private:
	TTF_Font *font_;
};
