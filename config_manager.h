#ifndef GALAGA_CONFIG_MANAGER
	#define GALAGA_CONFIG_MANAGER

	#include <allegro5/allegro.h>
	#include <iostream>
	#include <map>
	#include <string.h>

	namespace ConfigManager {
		bool initialize();

		bool audioEnabled();

		void destroy();
	};
#endif
