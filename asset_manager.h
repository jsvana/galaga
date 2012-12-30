#ifndef GALAGA_ASSET_MANAGER
	#define GALAGA_ASSET_MANAGER

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_audio.h>
  #include <allegro5/allegro_font.h>

	#include <iostream>
	#include <list>
	#include <map>

	class AssetManager {
		static std::map<std::string, ALLEGRO_FONT *> _fonts;
		static std::map<std::string, ALLEGRO_SAMPLE *> _samples;
		static std::map<std::string, ALLEGRO_BITMAP *> _textures;

	public:
		static bool init();

		static ALLEGRO_FONT *getFont(std::string font);
		static ALLEGRO_SAMPLE *getSample(std::string sample);
		static ALLEGRO_BITMAP *getTexture(std::string texture);

		static void cleanup();
	};
#endif
