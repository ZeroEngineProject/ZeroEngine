// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIBatch, builder, type)
{
  ZeroBindDocumented();
}
TBUIBatch::TBUIBatch(Zero::Texture* texture) : Texture(texture)
{
}
} // namespace Zero