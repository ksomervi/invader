
#include "resource_manager.h"

#include<sstream>
using std::stringstream;

resource_manager::resource_manager() {
  _cfg = nullptr;
  _display = nullptr;
  _title_font = nullptr;
  _textfont = nullptr;

  _rand_gen = new random_generator();

  _player = nullptr;

  _log = new logger(logger::NOTE);

}//end resource_manager::resource_manager()

resource_manager::~resource_manager() {

  if (_player) {
    delete _player;
  }

  delete _rand_gen;
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
  int width = atoi(option("DISPLAY", "width"));
  int height = atoi(option("DISPLAY", "height"));
  _display = al_create_display(width, height);
  if(!_display) {
    return false;
  }

  stringstream ss;
  ss << "Created display: " << width << "x" << height;
  _log->note(ss.str());

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
    _player->set_logger(_log);
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
  return al_get_config_value(_cfg, section, key);
}//end resource_manager::option


ALLEGRO_SAMPLE* resource_manager::get_sound(const char* option_key) {
  string path = option("AUDIO", option_key);
  stringstream ss;
  ss << "Loading sound: AUDIO::" << option_key << ":" << path;
  _log->note(ss.str());
  ALLEGRO_SAMPLE *s = al_load_sample(path.c_str());
  return s;
}//end resource_manager::get_sound()


ALLEGRO_BITMAP* resource_manager::get_sprite(const char* option_key) {
  return get_sprite("SPRITES", option_key);
}//end resource_manager::get_sprite()

ALLEGRO_BITMAP* resource_manager::get_sprite(const char* section, const char* option_key) {
  const char *path = option(section, option_key);
  ALLEGRO_BITMAP *bm = nullptr;
  if (path) {
    stringstream ss;
    ss << "Loading bitmap: " << section << "::" << option_key << ":" << path;
    _log->note(ss.str());
    bm = al_load_bitmap(path);
  }
  return bm;
}//end resource_manager::get_sprite()


random_generator* resource_manager::get_random_generator() {
  return _rand_gen;
}

