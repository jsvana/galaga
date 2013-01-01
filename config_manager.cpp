#include "config_manager.h"

namespace ConfigManager {
	const char *CONFIG_FILENAME = "galaga.ini";

	ALLEGRO_PATH *_configPath;
	ALLEGRO_CONFIG *_config;

	bool initialize() {
		_configPath = al_get_standard_path(ALLEGRO_USER_SETTINGS_PATH);
		al_make_directory(al_path_cstr(_configPath, ALLEGRO_NATIVE_PATH_SEP));
		al_set_path_filename(_configPath, CONFIG_FILENAME);
		_config = al_load_config_file(al_path_cstr(_configPath, ALLEGRO_NATIVE_PATH_SEP));

		if (_config == NULL) {
			_config = al_create_config();
		}

		return true;
	}

	bool audioEnabled() {
		const char *enabled = al_get_config_value(_config, "audio", "enabled");

		if (enabled == NULL) {
			al_add_config_section(_config, "audio");
			al_set_config_value(_config, "audio", "enabled", "true");
		}

		return strncmp(enabled, "true", 4) == 0;
	}

	void destroy() {
		al_set_path_filename(_configPath, CONFIG_FILENAME);
		if (!al_save_config_file(al_path_cstr(_configPath, ALLEGRO_NATIVE_PATH_SEP), _config)) {
			std::cerr << "[ERROR] Error saving config file" << std::endl;
		}
		al_destroy_config(_config);
		al_destroy_path(_configPath);
	}
};
