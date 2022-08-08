#define RENDER_IMPL
#include "render.h"

#define FOUNDATION_IMPL
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  pool *Pool = PoolReserve(0);

  window   *Wnd = WndInit(gfx_api_Opengl, 0, 0, -1, -1);
  renderer *Ren = RenInit(Pool, Wnd);

  r32 t = 0;
  r32 f = 2;

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);
    WndBeginRendering(Wnd);
    RenDraw(Ren);
    WndEndRendering(Wnd);
    WndEndFrame(Wnd);
  }

  RenEnd(Ren);
  WndEnd(Wnd);
  SysEnd();
}
