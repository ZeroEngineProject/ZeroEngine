// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIRenderer, builder, type)
{
  ZeroBindDocumented();
}
tb::TBBitmap* TBUIRenderer::CreateBitmap(int width, int height, uint32* data)
{
  TBUIBitmap* bitmap = new TBUIBitmap(this);
  if (!bitmap->Init(width, height, data))
  {
    delete bitmap;
    return nullptr;
  }

  return bitmap;
}
void TBUIRenderer::RenderBatch(Batch* tbBatch)
{
  Texture* texture = nullptr;
  TBUIBitmap* bitmap = (TBUIBitmap*)tbBatch->bitmap;
  if (bitmap)
    texture = reinterpret_cast<Texture*>(bitmap->GetTexture());

  TBUIBatch batch(texture);
  batch.ClipRect = mClipRect;

  for (int i = 0; i < tbBatch->vertex_count; i++)
  {
    {
      tb::TBRendererBatcher::Vertex tbVertex = tbBatch->vertex[i];
      StreamedVertex vertex =
          StreamedVertex(Vec3(tbVertex.x, tbVertex.y, 0), Vec2(tbVertex.u, tbVertex.v), ToFloatColor(tbVertex.col));
      batch.Vertices.PushBack(vertex);
    }
  }

  if (mBatches)
    mBatches->Append(batch);
}
void TBUIRenderer::SetClipRect(const tb::TBRect& rect)
{
  Vec2 size = Vec2{(real)rect.w, (real)rect.h};
  Vec2 pos = Vec2{(real)rect.x, (real)rect.y};

  mClipRect = IntRect(pos.x, pos.y, size.x, size.y);
}
} // namespace Zero