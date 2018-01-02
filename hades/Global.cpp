#include "SDK.h"

bool				G::Aimbotting;
bool				G::Return;
HMODULE				G::Dll;
HWND				G::Window;
bool				G::PressedKeys[256] = {};
bool				G::d3dinit = false;
float				G::FOV;
char				G::AnimatedClantag[16];
bool				G::SendPacket = true;
int					G::BestTarget = -1;
char				G::buf3[128];
char				G::extra_flags;
float				G::hitmarkeralpha;