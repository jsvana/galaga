#include "asset_manager.h"

bool AssetManager::init() {
	std::map<std::string, int> fonts = {
		{ "normal", 20 },
		{ "big", 40 },
		{ "huge", 60 }
	};

	std::map<std::string, std::string> samples = {
		{ "beginning", "assets/sounds/beginning.wav" },
		{ "end", "assets/sounds/end.wav" },
		{ "shot", "assets/sounds/fighter.wav" },
		{ "explosion", "assets/sounds/explosion.wav" }
	};

	std::map<std::string, std::string> textures = {
		{ "ship", "assets/images/galaga.png" },
		{ "explosion", "assets/images/explosion.png" },
		{ "enemies", "assets/images/enemies.png" },
		{ "bullet", "assets/images/galaga_bullet.png" },
		{ "powerups", "assets/images/powerups.png" }
	};

	for (std::pair<std::string, int> font : fonts) {
		_fonts[font.first] = al_load_font("assets/fonts/arcade.ttf", font.second, NULL);

		if (_fonts[font.first] == NULL) {
			return false;
		}
	}

	for (std::pair<std::string, std::string> sample : samples) {
		_samples[sample.first] = al_load_sample(sample.second.c_str());

		if (_samples[samples.first] == NULL) {
			return false;
		}
	}

	for (std::pair<std::string, std::string> texture : textures) {
		_textures[texture.first] = al_load_bitmap(texture.second.c_str());

		if (_textures[texture.first] == NULL) {
			return false;
		}
	}

	return true;
}
