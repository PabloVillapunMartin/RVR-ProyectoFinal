#pragma once

#include <string>
#include <vector>
#include <SDL.h>

using namespace std;

class Resources {
public:

	enum TextureId : std::size_t {
		// images
		Bullet,
		Player,
		DeathBackGround,
		Suelo,
		Vida,
		Muro,
		Muro2,
		Waiting,

		// text
		GameOver,
		Winner,
	};

	enum AudioId : std::size_t {
	};

	enum FontId : std::size_t {
		ARIAL16, ARIAL24,BEAR24,
	};

	struct FontInfo {
		FontId id;
		string fileName;
		int size;
	};

	struct ImageInfo {
		TextureId id;
		string fileName;
	};

	struct TextMsgInfo {
		TextureId id;
		string msg;
		SDL_Color color;
		FontId fontId;
	};

	struct MusicInfo {
		AudioId id;
		string fileName;
	};

	struct SoundInfo {
		AudioId id;
		string fileName;

	};

	static vector<FontInfo> fonts_; // initialized in .cpp
	static vector<ImageInfo> images_; // initialized in .cpp
	static vector<TextMsgInfo> messages_; // initialized in .cpp
	static vector<MusicInfo> musics_; // initialized in .cpp
	static vector<SoundInfo> sounds_; // initialized in .cpp

};
