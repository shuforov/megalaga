#include <genesis.h>
#include <../res/resources.h>

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

    SYS_doVBlankProcess();
  }
  return(0);  
}  
