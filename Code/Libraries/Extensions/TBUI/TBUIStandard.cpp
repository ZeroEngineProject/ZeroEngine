// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

ZilchDefineStaticLibrary(TBUILibrary)
{
  builder.CreatableInScriptDefault = false;

  ZilchInitializeType(TBUIContainer);
  ZilchInitializeType(TBUIRenderer);
  ZilchInitializeType(TBUIBitmap);
  ZilchInitializeType(TBUIFile);
  ZilchInitializeType(TBUIImageLoader);

  EngineLibraryExtensions::AddNativeExtensions(builder);
}

void TBUILibrary::Initialize()
{
  BuildStaticLibrary();
  MetaDatabase::GetInstance()->AddNativeLibrary(GetLibrary());

  TBUI::Initialize();
}

void TBUILibrary::Shutdown()
{
  TBUI::Destroy();

  GetLibrary()->ClearComponents();
}

} // namespace Zero