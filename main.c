#include "foundation.h"

int main(int Argc, char **Argv) {
  SysInit(Argc, Argv);
  window *Window = SysCreateWindowWithOpenGL();
  r32 RedCycle = 0;
  while (!Window->Quit) {
    SysGetInput(Window);
    SysBeginRenderingWithOpengl(Window);
    Window->GlClearColor(RedCycle, 0, 0, 1);
    Window->GlClear(GL_COLOR_BUFFER_BIT);
    SysEndRenderingWithOpengl(Window);
    RedCycle += 0.05f;
    if (RedCycle >= 0.9f)
      RedCycle = 0;
  }
  SysEnd();
}
