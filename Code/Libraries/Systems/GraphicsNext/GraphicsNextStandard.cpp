// MIT Licensed (see LICENSE.md).

#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineStaticLibrary(GraphicsNextLibrary)
{
  builder.CreatableInScriptDefault = false;

  // Ranges

  // Enums

  // Meta Components

  ZilchInitializeType(GraphicsNextEngine);
  ZilchInitializeType(GraphicsNextSpace);

  EngineLibraryExtensions::AddNativeExtensions(builder);
}

void GraphicsNextLibrary::Initialize()
{
  BuildStaticLibrary();
  MetaDatabase::GetInstance()->AddNativeLibrary(GetLibrary());
}

void GraphicsNextLibrary::Shutdown()
{
  GetLibrary()->ClearComponents();
}

} // namespace Zero
