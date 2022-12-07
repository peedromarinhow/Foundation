#define RENDER_IMPL
#include "render.h"

#define FOUNDATION_IMPL
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  window   *Wnd = WndInit(gfx_api_Opengl, 0, 0, -1, -1);
  renderer *Ren = RenInit(PoolReserve(0), Wnd);

  r32 x = 1;

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);

    RenPushVert(Ren, R32v2(-x, -x));
    RenPushVert(Ren, R32v2(-x,  x));
    RenPushVert(Ren, R32v2( x,  x));

    RenPushVert(Ren, R32v2( x,  x));
    RenPushVert(Ren, R32v2( x, -x));
    RenPushVert(Ren, R32v2(-x, -x));

    RenDraw(Ren);

    WndEndFrame(Wnd);
  }

  RenEnd(Ren);
  WndEnd(Wnd);

  SysEnd();
}