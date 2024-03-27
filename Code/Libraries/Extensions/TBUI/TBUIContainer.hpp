// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIContainer : public Graphical
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIContainer, TypeCopyMode::ReferenceType);

  void AddDemo();
  
  /// Component Interface.
  void Serialize(Serializer& stream) override;
  void Initialize(CogInitializer& initializer) override;
  void ComponentAdded(BoundType* typeId, Component* component) override;
  void ComponentRemoved(BoundType* typeId, Component* component) override;
  void DebugDraw() override;

  // Graphical Interface

  String GetDefaultMaterialName() override;
  Aabb GetLocalAabb() override;
  void ExtractFrameData(FrameNode& frameNode, FrameBlock& frameBlock) override;
  void ExtractViewData(ViewNode& viewNode, ViewBlock& viewBlock, FrameBlock& frameBlock) override;

    // Properties

  /// Color attribute of the generated vertices accessible in the vertex shader.
  Vec4 mVertexColor;

  /// How the Sprite should be oriented in 3D space.
  SpriteGeometryMode::Enum mGeometryMode;

    /// Flips the X axis of the Sprite's image (left/right).
  bool mFlipX;

  /// Flips the Y axis of the Sprite's image (top/bottom).
  bool mFlipY;

  // Internal

  Aabb GetViewPlaneAabb(Aabb localAabb);
  void ComputeLocalToViewMatrix(Mat4& localToView, Mat4& localToWorld, Mat4& worldToView);

  Vec2 GetLocalCenter();
  Vec2 GetLocalWidths();

private:
  UniquePointer<TBUIView> mView = nullptr;
};
} // namespace Zero