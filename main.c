#define FOUNDATION_IMPLEMENTATION
#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);
  window *Window = SysCreateWindowWithOpenGL();
  opengl_api *Gl = cast(opengl_api*, Window->GfxApi);
  r32 RedCycle = 0.f;
  while (!Window->Finish) {
    SysGetWindowInput(Window);
    SysBeginRenderingWithOpengl(Window);
    Gl->ClearColor(RedCycle = (RedCycle >= 0.9)? 0 : RedCycle+.05f, 0, 0, 1);
    Gl->Clear(GL_COLOR_BUFFER_BIT);
    SysEndRenderingWithOpengl(Window);
  }
  SysDestroyWindowWithOpenGL(Window);
}
