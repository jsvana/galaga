#ifndef GALAGA_ASSET_MANAGER
	#define GALAGA_ASSET_MANAGER

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_audio.h>
  #include <allegro5/allegro_font.h>

	#include <iostream>
	#include <list>
	#include <map>

	#include "config_manager.h"

	namespace AssetManager {
		bool initialize();

		ALLEGRO_FONT *getFont(std::string font);
		ALLEGRO_SAMPLE *getSample(std::string sample);
		ALLEGRO_BITMAP *getTexture(std::string texture);

		void playSample(std::string sample, ALLEGRO_SAMPLE_ID *sampleID);
		void stopSample(ALLEGRO_SAMPLE_ID *sampleID);

		void destroy();
	};
#endif
