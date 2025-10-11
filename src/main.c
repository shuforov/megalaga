#include <genesis.h>
#include <../res/resources.h>

typedef struct {
  int x;
  int y;
  int w;
  int h;
  int velx;
  int vely;
  int health;
  Sprite *sprite;
  char name[6];
} Entity;

Entity player = {0, 0, 16, 16, 0, 0, 0, NULL, "PLAYER"};

void killEntity(Entity *e) {
  e->health = 0;
  SPR_setVisibility(e->sprite, HIDDEN);
}

void reviveEntity(Entity *e) {
  e->health = 1;
  SPR_setVisibility(e->sprite, VISIBLE);
}

int main() {
  int i = 0;
  int thex = 0;
  int they = 0;
  int val = 1;

  SYS_disableInts();
  VDP_loadTileSet(background.tileset, 1, DMA);
  SYS_enableInts();
  PAL_setPalette(PAL1, background.palette->data, DMA);

  for (i = 0; i < 1280; i++) {
    thex = i % 40;
    they = i / 40;
    val = (random() % (10 - 1 + 1)) + 1;
    if (val > 3) {
      val = 1;
    }
    SYS_disableInts();
    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, 0, 0, val), thex, they);
    SYS_enableInts();
  }

  SPR_init();

  /*Add the player*/
  player.x = 152;
  player.y = 192;
  player.health = 1;
  player.sprite = SPR_addSprite(&ship, player.x, player.y,
                                TILE_ATTR(PAL1, 0, FALSE, FALSE));
  SPR_update();

  int offset = 0;
  SYS_disableInts();
  VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
  SYS_enableInts();
  while (1) {
    SYS_disableInts();
    VDP_setVerticalScroll(BG_B, offset -= 2);
    SYS_enableInts();
    if (offset <= -256) {
      offset = 0;
    }

    SPR_update();
    SYS_doVBlankProcess();
  }
  return(0);  
}  
