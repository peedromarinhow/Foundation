#define FOUNDATION_IMPLEMENTATION
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);
  window *Wnd = WndInit(gfx_api_Opengl, 0, 0, -1, -1);

  while (!Wnd->Finish) {
    WndBeginFrame(Wnd);
      cast(opengl_api*, Wnd->GfxApi)->ClearColor(0, 0, 0, 1);
      cast(opengl_api*, Wnd->GfxApi)->Clear(GL_COLOR_BUFFER_BIT);
    WndEndFrame(Wnd);
  }

  WndEnd(Wnd);
  SysEnd();
}
