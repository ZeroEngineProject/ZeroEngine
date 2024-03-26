// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIView : public tb::TBWidget
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIView, TypeCopyMode::ReferenceType);

  TBUIView();

private:
  void UpdateBatches();
  void RenderBatches();
  void RenderBatch(const TBUIBatch& batch);
  void CreateRenderData(ViewBlock& viewBlock,
                        FrameBlock& frameBlock,
                        const Mat4& worldMatrix,
                        const IntRect& clipRect,
                        const Array<StreamedVertex>& vertices,
                        const Texture* texture,
                        PrimitiveType::Enum primitiveType);
  ViewNode& AddRenderNodes(
      ViewBlock& viewBlock, FrameBlock& frameBlock, const Mat4& worldMatrix, const IntRect& clipRect, const Texture* texture);

private:
  friend class TBUI;

  Vec3 mTranslation;
  float mAngle;
  Mat4 mWorldTx = Mat4::cIdentity;

  TBUI* mUI = nullptr;
  TBUIRenderer* mRenderer = nullptr;

  Array<TBUIBatch> mBatches;
};
} // namespace Zero