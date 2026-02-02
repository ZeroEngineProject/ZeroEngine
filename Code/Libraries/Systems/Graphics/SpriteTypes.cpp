// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

// Zilch binding for SpriteData (class defined in RendererBase/Renderer.hpp)
ZilchDefineExternalBaseType(SpriteData, TypeCopyMode::ReferenceType, builder, type)
{
  type->AddAttribute(ObjectAttributes::cHidden);
}

} // namespace Zero
