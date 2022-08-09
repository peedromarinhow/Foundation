# Foundation
This is the scaffold for making real-time and single execution programs in C in windows (no other OSs suported yet). This projects reuses and adapts code from https://github.com/Mr-4th-Programming/mr4th, https://github.com/vurtun/tau and https://youtu.be/NG_mUhc8LRw.

Example program:
```C
#define RENDER_IMPL
#include "render.h"

#define FOUNDATION_IMPL
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);

  pool *Pool = PoolReserve(0);

  window   *Wnd = WndInit(gfx_api_Opengl, 0, 0, -1, -1);
  renderer *Ren = RenInit(Pool, Wnd);

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
```

![output.gif](media/output.gif)