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

#define MAX_ENEMIES 6
#define LEFT_EDGE 0
#define RIGHT_EDGE 320
#define ANIM_STRAIGHT 0
#define ANIM_MOVE 1

Entity enemies[MAX_ENEMIES];
u16 enemiesLeft = 0;

Entity player = {0, 0, 16, 16, 0, 0, 0, NULL, "PLAYER"};

void killEntity(Entity *e) {
  e->health = 0;
  SPR_setVisibility(e->sprite, HIDDEN);
}

void reviveEntity(Entity *e) {
  e->health = 1;
  SPR_setVisibility(e->sprite, VISIBLE);
}

void positionEnemies() {
  u16 i = 0;
  for (i = 0; i < MAX_ENEMIES; i++) {
    Entity *e = &enemies[i];
    if (e->health > 0) {
      if ((e->x + e->w) > RIGHT_EDGE) {
        e->velx = -1;
      } else if (e->x < LEFT_EDGE) {
        e->velx = 1;
      }
      e->x += e->velx;
      SPR_setPosition(e->sprite, e->x, e->y);
    }
  }
}

void positionPlayer() {
  /*Add the player's velocity to its position*/
  player.x += player.velx;

  /*Keep the player within the bounds of the screen*/
  if (player.x < LEFT_EDGE)
    player.x = LEFT_EDGE;
  if (player.x + player.w > RIGHT_EDGE)
    player.x = RIGHT_EDGE - player.w;

  /*Let the Sprite engine position the sprite*/
  SPR_setPosition(player.sprite, player.x, player.y);
}

void myJoyHandler(u16 joy, u16 changed, u16 state) {
  if (joy == JOY_1) {
    if (state & BUTTON_RIGHT) {
      player.velx = 2;
      SPR_setAnim(player.sprite, ANIM_MOVE);
      SPR_setHFlip(player.sprite, TRUE);
    } else if (state & BUTTON_LEFT) {
      player.velx = -2;
      SPR_setAnim(player.sprite, ANIM_MOVE);
      SPR_setHFlip(player.sprite, FALSE);
    } else {
      if ((changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT)) {
        player.velx = 0;
        SPR_setAnim(player.sprite, ANIM_STRAIGHT);
      }
    }
  }
}

int main() {
  int i = 0;
  int thex = 0;
  int they = 0;
  int val = 1;

  JOY_init();
  JOY_setEventHandler(&myJoyHandler);

  SYS_disableInts();
  VDP_loadTileSet(background.tileset, 1, DMA);
  SYS_enableInts();
  PAL_setPalette(PAL1, background.palette->data, DMA);
  PAL_setPalette(PAL2, background.palette->data, DMA);
  
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

  /*Create all enemy sprites*/
  Entity *e = enemies;
  for (i = 0; i < MAX_ENEMIES; i++) {
    e->x = i * 32;
    e->y = 32;
    e->w = 16;
    e->h = 16;
    e->velx = 1;
    e->health = 1;
    e->sprite = SPR_addSprite(&ship,e->x,e->y,TILE_ATTR(PAL2,0,TRUE,FALSE));
    sprintf(e->name, "En%d", i);
    enemiesLeft++;
    e++;
  }
  PAL_setColor(34, RGB24_TO_VDPCOLOR(0x0078f8));

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

    positionPlayer();
    positionEnemies();
    SPR_update();
    SYS_doVBlankProcess();
  }
  return(0);
}
