#include "asset_manager.h"

namespace AssetManager {
	static std::map<std::string, ALLEGRO_FONT *> _fonts;
	static std::map<std::string, ALLEGRO_SAMPLE *> _samples;
	static std::map<std::string, ALLEGRO_BITMAP *> _textures;

	bool initialize() {
		std::map<std::string, int> fonts = {
			{ "normal", 20 },
			{ "big", 40 },
			{ "huge", 60 }
		};

		std::map<std::string, std::string> samples = {
			{ "beginning", "assets/sounds/beginning.wav" },
			{ "end", "assets/sounds/end.wav" },
			{ "shot", "assets/sounds/fighter.wav" },
			{ "explosion", "assets/sounds/explosion.wav" },
			{ "enemy_death_1", "assets/sounds/enemy1death.wav" },
			{ "enemy_death_2", "assets/sounds/enemy2death.wav" }
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

			if (_samples[sample.first] == NULL) {
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

	ALLEGRO_FONT *getFont(std::string font) {
		return _fonts[font];
	}

	ALLEGRO_SAMPLE *getSample(std::string sample) {
		return _samples[sample];
	}

	ALLEGRO_BITMAP *getTexture(std::string texture) {
		return _textures[texture];
	}

	void playSample(std::string sample, ALLEGRO_SAMPLE_ID *sampleID) {
		if (_samples[sample] != NULL && ConfigManager::audioEnabled()) {
			al_play_sample(_samples[sample], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, sampleID);
		}
	}

	void stopSample(ALLEGRO_SAMPLE_ID *sampleID) {
		if (sampleID != NULL) {
			al_stop_sample(sampleID);
		}
	}

	void destroy() {
		for (std::pair<std::string, ALLEGRO_FONT *> font : _fonts) {
			al_destroy_font(font.second);
		}

		for (std::pair<std::string, ALLEGRO_SAMPLE *> sample : _samples) {
			al_destroy_sample(sample.second);
		}

		for (std::pair<std::string, ALLEGRO_BITMAP *> texture : _textures) {
			al_destroy_bitmap(texture.second);
		}

		_fonts.clear();
		_samples.clear();
		_textures.clear();
	}
};
