#define FOUNDATION_IMPLEMENTATION
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);
  window     *Wnd = WndInit(gfx_api_Opengl);
  opengl_api *Gl  = cast(opengl_api*, Wnd->GfxApi);

  r32 Red = 0.f;

  while (!Wnd->Finish) {
    WndGetInput(Wnd);

    WndBeginRendering(Wnd);
      Gl->ClearColor(Red = (Red >= 0.9)? 0 : Red+.05f, 0, 0, 1);
      Gl->Clear(GL_COLOR_BUFFER_BIT);
    WndEndRendering(Wnd);
  }

  WndEnd(Wnd);
  SysEnd();
}
