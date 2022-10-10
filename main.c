#define RENDER_IMPL
#include "render.h"

#define FOUNDATION_IMPL
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  window   *Wnd = WndInit(gfx_api_Opengl, 0, 0, -1, -1);
  renderer *Ren = RenInit(PoolReserve(0), Wnd);

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);

    RenPushVert(Ren, R32v2(-0.5, -0.5));
    RenPushVert(Ren, R32v2(0, 0.5));
    RenPushVert(Ren, R32v2(0.5, -0.5));
    RenDraw(Ren);

    WndEndFrame(Wnd);
  }

  RenEnd(Ren);
  WndEnd(Wnd);

  SysEnd();
}