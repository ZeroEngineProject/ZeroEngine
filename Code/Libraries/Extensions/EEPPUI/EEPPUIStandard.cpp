// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

// Ranges

// Enums

ZilchDefineStaticLibrary(EEPPUILibrary)
{
  builder.CreatableInScriptDefault = false;

  // Ranges

  // Enums

  // Events

  EngineLibraryExtensions::AddNativeExtensions(builder);
}

void EEPPUILibrary::Initialize()
{
  BuildStaticLibrary();
  MetaDatabase::GetInstance()->AddNativeLibrary(GetLibrary());
}

void EEPPUILibrary::Shutdown()
{
  GetLibrary()->ClearComponents();
}

} // namespace Zero
