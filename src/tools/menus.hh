#ifndef MENUS_HH
#define MENUS_HH 1

#include "../entities/body.hh"
#include "../tools/ui-element.hh"
#include "../tools/asset-loader.hh"

struct Game_t;

extern bool g_alt_title_anim;

void LoadPreviewingMenu();

void LoadInsertCoinMenu();

void LoadChoosingPlayerOneMenu();
void LoadChoosingPlayerTwoMenu();

void LoadAboutToPlayMenu(Game_t *game);

#endif // MENUS_HH