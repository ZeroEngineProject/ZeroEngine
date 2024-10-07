// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

// Ranges

// Enums

ZilchDefineStaticLibrary(PrettyLibrary)
{
  builder.CreatableInScriptDefault = false;

  // Ranges

  // Enums

  // Events

  EngineLibraryExtensions::AddNativeExtensions(builder);
}

void PrettyLibrary::Initialize()
{
  BuildStaticLibrary();
  MetaDatabase::GetInstance()->AddNativeLibrary(GetLibrary());
}

void PrettyLibrary::Shutdown()
{
  GetLibrary()->ClearComponents();
}

} // namespace Zero
