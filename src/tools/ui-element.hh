#ifndef UI_ELEMENT_HH
#define UI_ELEMENT_HH 1

#include "../core/cirno.hh"
#include "../entities/body.hh"
#include "asset-loader.hh"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

struct UIElement_t {
    bool active = false, flashing = false;
    Body_t body = { false };
    long long int queued_tick;
};

const int kMaxAnimationExtraMasks = 3;

struct AnimatedUIElement_t {
    bool active;
    Vector2_t delta;
    UIElement_t element;
    Mask_t extra_masks[kMaxAnimationExtraMasks];
    int frame_count = 1;
    int frame_rate = 4;
    long long int end_tick;
};

struct WipeScreen {
    Vector2_t position;
    Vector2_t size;
    int step_size = 16, current_step = -1;
    long long int queued_tick = 0;
};

const unsigned int UIElement_kRegistryCapacity = 200;
extern unsigned int UIElement_registry_active_count;
extern UIElement_t UIElement_registry[UIElement_kRegistryCapacity];

const unsigned int AnimatedUIElement_kRegistryCapacity = 5;
extern unsigned int AnimatedUIElement_registry_active_count;
extern AnimatedUIElement_t AnimatedUIElement_registry[AnimatedUIElement_kRegistryCapacity];

const unsigned int ProgressiveTextTickRate = 6;

const Vector2_t kCreditsNumberPosition = {200,240};
const Vector2_t kScore1NumberPosition = {10 + 8*2 , 9 + 8*2};
const Vector2_t kScore2NumberPosition = {10 + 8*20, 9 + 8*2};
const Vector2_t kHiScoreNumberPosition = {10 + 8*10, 9 + 8*2};
const Vector2_t kLivesNumberPosition = {10, 240};
const Vector2_t kTaitoCopPosition = {82,32};

extern WipeScreen g_wipe_screen;

extern Body_t g_bottomline;


UIElement_t* UIElement_Construct();
void Destruct(UIElement_t* self);

void GenerateBottomLine();

char TranslateCharToIndex(char c);

Mask_t CharMask(char c);

void Destruct(UIElement_t* self);
void Destruct(AnimatedUIElement_t* self);
void EraseUIElement(Vector2_t position, int exluded_index = -1);
void EraseText(Vector2_t position, int length = 1);

void GenerateUIElement(Body_t b, long long int tick = 0, bool flashing = false);
void QueuedEraseText(Vector2_t position, int length, long long int tick);

void GenerateAnimatedUIElement(
  Body_t b, 
  long long int start_tick, 
  long long int end_tick, 
  Vector2_t delta, 
  Mask_t extra_masks[3] = {0}, 
  int fr_count = 1, 
  int fr_rate = 4, 
  bool flashing = false
);

// Clear
void ClearUIContent();

// Texto
void GenerateText(
  const char *text, 
  Vector2_t position, 
  int start_tick = 0, 
  int wait_rate = 0, 
  bool flashing = false, 
  bool ignore_space = false
);

void GenerateNumber(
  int value, 
  int digits, 
  Vector2_t position, 
  int start_tick = 0, 
  bool flashing = false
);


//Vidas

void GenerateLives(int lives);

//Wipe Screen

void AdvanceWipeScreen(WipeScreen *wp);
void WipeScreenToContent(WipeScreen *wp);
void WipeScreenToFull(WipeScreen *wp);

#endif // UI_ELEMENT_HH