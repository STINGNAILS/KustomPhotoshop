#pragma once

// Safe disable dx default warnings
#pragma warning(push)
#pragma warning(disable:4005) // allows to define macros twice

#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma warning(pop)

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "d3dcompiler.lib")