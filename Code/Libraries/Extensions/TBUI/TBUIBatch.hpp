// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIBatch
{
public:
  ZilchDeclareType(TBUIBatch, TypeCopyMode::ReferenceType);

  TBUIBatch() = default;
  TBUIBatch(Texture* texture);

  Texture* Texture;
  Array<StreamedVertex> Vertices;
  IntRect ClipRect;
};
} // namespace Zero