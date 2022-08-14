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

  table(i32) Ints = {0};
  i32 N = 11;
  ItrNum (i, N)
    TableAdd(&Ints, _U64Hash(256*(i+1)), 42+i);
  ItrNum (i, N)
    printf("%d\n", TableGet(&Ints, _U64Hash(256*(i+1))));

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);
    WndBeginRendering(Wnd);
  
    RenPushVert(Ren, R32v2(-0.5f,  0.5f));
    RenPushVert(Ren, R32v2( 0.5f,  0.5f));
    RenPushVert(Ren, R32v2( 0.5f, -0.5f));

    RenDraw(Ren);

    WndEndRendering(Wnd);
    WndEndFrame(Wnd);
  }

  RenEnd(Ren);
  WndEnd(Wnd);
  SysEnd();
}
