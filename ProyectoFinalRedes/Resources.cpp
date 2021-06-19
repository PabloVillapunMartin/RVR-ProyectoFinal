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
		{ PacManSprites, "resources/images/pacman_sprites.png" }, //
		{ Bullet, "resources/images/bala.png" }, //
		{ Player, "resources/images/player.png" }, //
		{ DeathBackGround, "resources/images/FondoMuerte.png" } //
};

vector<Resources::TextMsgInfo> Resources::messages_ {
//
		{ WaitingForPlayers, "Waiting for players...", { COLOR(0xaaffbbff) }, BEAR24 }, //
		{ GameOver, "Game Over!", { COLOR(0xffffffff) }, BEAR24 } //
};

vector<Resources::MusicInfo> Resources::musics_ {
//
		{ PacMan_Intro, "resources/sound/pacman_intro.wav" }, //
};

vector<Resources::SoundInfo> Resources::sounds_ {
//
		{ PacMan_Eat, "resources/sound/pacman_eat.wav" }, //
		{ PacMan_Death, "resources/sound/pacman_death.wav" }, //
		{ PacMan_Won, "resources/sound/pacman_won.wav" }, //
};

