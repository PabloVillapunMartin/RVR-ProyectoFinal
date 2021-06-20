#include "Resources.h"
#include "SDL_macros.h"

vector<Resources::FontInfo> Resources::fonts_ {
//
		{ ARIAL16, "resources/fonts/ARIAL.ttf", 16 }, //
		{ ARIAL24, "resources/fonts/ARIAL.ttf", 24 }, //
		{ BEAR24, "resources/fonts/BearAndBunny.ttf", 24 } //
};

vector<Resources::ImageInfo> Resources::images_ {
//
		{ Bullet, "resources/images/bala.png" }, //
		{ Player, "resources/images/player.png" }, //
		{ DeathBackGround, "resources/images/FondoMuerte.png" }, //
		{ Suelo, "resources/images/suelo.png" }, //
		{ Vida, "resources/images/vida.png" }, //
		{ Muro, "resources/images/muro.png" }, //
		{ Muro2, "resources/images/muro2.png" }, //
		{ Waiting, "resources/images/waiting.png" } //
};

vector<Resources::TextMsgInfo> Resources::messages_ {
//
		{ GameOver, "Game Over!", { COLOR(0xffffffff) }, BEAR24 }, //
		{ Winner, "You Win!", { COLOR(0xffffffff) }, BEAR24 }

};

vector<Resources::MusicInfo> Resources::musics_ {
//

};

vector<Resources::SoundInfo> Resources::sounds_ {
//

};

