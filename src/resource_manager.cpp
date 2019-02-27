
#include "resource_manager.h"

#include<sstream>
using std::stringstream;

resource_manager::resource_manager() {
  _cfg = NULL;
  _display = NULL;
  _title_font = NULL;
  _textfont = NULL;

  _rand_gen = new random_generator();

  _player = NULL;

  _log = new logger(logger::NOTE);

}//end resource_manager::resource_manager()

resource_manager::~resource_manager() {

  if (_player) {
    delete _player;
  }

  delete _log;
}

logger* resource_manager::get_logger() {
  return _log;
}

bool resource_manager::init_resources(ALLEGRO_CONFIG *ac) {
  _log->note("Initializing resources...");
  _cfg = ac;
  if (!_cfg) {
    _log->error("no config provided to resource manager!");
    return false;
  }

  if(!_init_display()) {
    _log->error("failed to initialize display!");
    return false;
  }

  if (!_init_audio()) {
    return false;
  }
  _log->note(" - audio initialized");

  if (!_init_fonts()) {
    return false;
  }
  _log->note(" - fonts initialized");

  return true;
}//end resource_manager::init()

bool resource_manager::_init_display() {
  _display = al_create_display(SCREEN_W, SCREEN_H);
  if(!_display) {
    return false;
  }

  const char *path = option("DISPLAY", "icon");
  if (path) {
    ALLEGRO_BITMAP *bm = al_load_bitmap(path);
    al_set_display_icon(_display, bm);
  }

  al_set_window_title(_display, "Invader!");
  return true;
}//end resource_manager::_init_display()

bool resource_manager::_init_audio() {

  if (!al_install_audio()) {
    _log->error("failed to initialize audio!");
    return false;
  }

  if (!al_init_acodec_addon()){
    _log->error("failed to initialize audio codecs!");
    return false;
  }

  if (!al_reserve_samples(8)) {
    _log->error("failed to reserve samples!");
    return false;
  }

  return true;
}

bool resource_manager::_init_fonts() {
  _log->note(" - initializing fonts...");
  al_init_font_addon(); // initialize the font addon
  al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

  stringstream ss;
  ss << "fontpath: " << option("FONT", "fontpath");
  _log->note(ss.str());
  _title_font = al_load_ttf_font(option("FONT", "fontpath"),
                                atoi(option("FONT", "title_fontsize")), 0);
  if (!_title_font) {
    ss.str("");
    ss << "Failed to load title font: " << option("FONT", "fontpath");
    _log->error(ss.str());
    return false;
  }

  //font = al_load_ttf_font(FONTNAME, SECONDARY_FONTSIZE, 0);
  _textfont = al_load_ttf_font(option("FONT", "fontpath"),
                          atoi(option("FONT", "secondary_fontsize")), 0);
  if (!_textfont) {
    ss.str("");
    ss << "Failed to load secondary font: " << option("FONT", "fontpath");
    _log->error(ss.str());
    return false;
  }

  return true;
}//end game::_init_fonts()


ALLEGRO_DISPLAY *resource_manager::get_display() {
  return _display;
}


fighter* resource_manager::get_player() {
  if (!_player) {
    _player = new fighter();
  }
  return _player;
}

ALLEGRO_FONT *resource_manager::get_font(fonttype f) {
  switch(f) {
    case TITLE:
      return _title_font;
      break;

    case NONE:
    case TEXT:
    default:
      return _textfont;
      break;
  };
}


const char* resource_manager::option(const char* section, const char* key) {
  //FIXME: returns null when key not found
  //DESCRIPTION: Need to find a graceful way to manage when config options are
  //not found in the config file.
  return al_get_config_value(_cfg, section, key);
}//end resource_manager::option


ALLEGRO_SAMPLE* resource_manager::get_sound(const char* option_key) {
  string path = option("MEDIA", option_key);
  stringstream ss;
  ss << "Loading sound: MEDIA::" << option_key << ":" << path;
  _log->note(ss.str());
  ALLEGRO_SAMPLE *s = al_load_sample(path.c_str());
  return s;
}//end resource_manager::get_sound()


ALLEGRO_BITMAP* resource_manager::get_sprite(const char* option_key) {
  string path = option("SPRITES", option_key);
  stringstream ss;
  ss << "Loading bitmap: SPRITES::" << option_key << ":" << path;
  _log->note(ss.str());
  ALLEGRO_BITMAP *bm = al_load_bitmap(path.c_str());
  return bm;
}//end resource_manager::get_sprite()


random_generator* resource_manager::get_random_generator() {
  return _rand_gen;
}

