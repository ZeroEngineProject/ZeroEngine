// MIT Licensed (see LICENSE.md).
#pragma once

#include "Engine/EngineStandard.hpp"
#include "Graphics/GraphicsStandard.hpp"

namespace Zero
{
// Forward declarations

// Widget library
class ZeroNoImportExport PrettyLibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(PrettyLibrary, "ZeroEngine");

  static void Initialize();
  static void Shutdown();
};

} // namespace Zero

// Indcludes
#include "Node.hpp"
