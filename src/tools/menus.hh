#ifndef MENUS_HH
#define MENUS_HH 1

#include "../entities/body.hh"
#include "../tools/ui-element.cc"
#include "../tools/asset-loader.hh"
#include "../core/types.cc"

bool g_alt_title_anim = false;

void LoadPreviewingMenu();

void LoadInsertCoinMenu();

void LoadChoosingPlayerOneMenu();
void LoadChoosingPlayerTwoMenu();

void LoadAboutToPlayMenu(Game_t *game);

#endif // MENUS_HH