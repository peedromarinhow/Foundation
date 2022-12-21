#define FOUNDATION_IMPL
#include "foundation.h"

function i32 Main(str8 ArgStr) {
  wnd *Wnd = SysInitWnd();
  while (!Wnd->Finish) {
    SysWndPull(Wnd);
    SysWndPush(Wnd);
  }
  SysKillWnd(Wnd);
  return 0;
}