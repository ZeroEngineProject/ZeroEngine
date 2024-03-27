// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIContainer, builder, type)
{
  ZeroBindDocumented();
  ZeroBindComponent();
  ZeroBindInterface(Graphical);
  ZeroBindSetup(SetupMode::DefaultSerialization);

  ZilchBindFieldProperty(mVertexColor);
  ZilchBindFieldProperty(mGeometryMode);
  ZilchBindFieldProperty(mFlipX);
  ZilchBindFieldProperty(mFlipY);
}

void TBUIContainer::Serialize(Serializer& stream)
{
  Graphical::Serialize(stream);
  SerializeNameDefault(mVertexColor, Vec4(1.0f));
  SerializeEnumNameDefault(SpriteGeometryMode, mGeometryMode, SpriteGeometryMode::ZPlane);
  SerializeNameDefault(mFlipX, false);
  SerializeNameDefault(mFlipY, false);
}

void TBUIContainer::Initialize(CogInitializer& initializer)
{
  Graphical::Initialize(initializer);
  mView = MakeUnique<TBUIView>();
  mView->SetRenderToTexture(true, 300, 600);
  mView->AddDemo();
}
void TBUIContainer::ComponentAdded(BoundType* typeId, Component* component)
{
  Graphical::ComponentAdded(typeId, component);
  if (typeId == ZilchTypeId(Area))
    UpdateBroadPhaseAabb();
}
void TBUIContainer::ComponentRemoved(BoundType* typeId, Component* component)
{
  Graphical::ComponentRemoved(typeId, component);
  if (typeId == ZilchTypeId(Area))
    UpdateBroadPhaseAabb();
}
void TBUIContainer::DebugDraw()
{

}
String TBUIContainer::GetDefaultMaterialName()
{
  return "AlphaSprite";
}
Aabb TBUIContainer::GetLocalAabb()
{
  Aabb localAabb = Aabb(Vec3(GetLocalCenter(), 0.0f), Vec3(GetLocalWidths(), 0.0f));

  if (mGeometryMode == SpriteGeometryMode::ViewPlane)
    return GetViewPlaneAabb(localAabb);

  return localAabb;
}
void TBUIContainer::ExtractFrameData(FrameNode& frameNode, FrameBlock& frameBlock)
{
  frameNode.mBorderThickness = 1.0f;
  frameNode.mBlendSettingsOverride = false;
  frameNode.mRenderingType = RenderingType::Streamed;
  frameNode.mCoreVertexType = CoreVertexType::Streamed;

  frameNode.mMaterialRenderData = mMaterial->mRenderData;
  frameNode.mMeshRenderData = nullptr;
  frameNode.mTextureRenderData = mView->GetTexture()->mRenderData;

  frameNode.mLocalToWorld = mTransform->GetWorldMatrix();
  frameNode.mLocalToWorldNormal = Mat3::cIdentity;

  frameNode.mObjectWorldPosition = Vec3::cZero;

  frameNode.mBoneMatrixRange = IndexRange(0, 0);
}
void TBUIContainer::ExtractViewData(ViewNode& viewNode, ViewBlock& viewBlock, FrameBlock& frameBlock)
{
  FrameNode& frameNode = frameBlock.mFrameNodes[viewNode.mFrameNodeIndex];

  ComputeLocalToViewMatrix(viewNode.mLocalToView, frameNode.mLocalToWorld, viewBlock.mWorldToView);

  Vec2 center = GetLocalCenter();
  Vec2 widths = GetLocalWidths();

  Vec3 pos0 = Vec3(center, 0.0f) + Vec3(-widths.x, widths.y, 0.0f);
  Vec3 pos1 = Vec3(center, 0.0f) + Vec3(widths.x, -widths.y, 0.0f);

  //UvRect rect = mSpriteSource->GetUvRect(mCurrentFrame);

  //Vec2 uv0 = rect.TopLeft;
  //Vec2 uv1 = rect.BotRight;
  Vec2 uv0 = Vec2(0, 0);
  Vec2 uv1 = Vec2(mView->GetTexture()->GetSize().x, mView->GetTexture()->GetSize().y);

  Vec2 uvAux0 = Vec2(0.0f);
  Vec2 uvAux1 = Vec2(1.0f);

  if (mFlipX)
  {
    Math::Swap(uv0.x, uv1.x);
    Math::Swap(uvAux0.x, uvAux1.x);
  }
  if (mFlipY)
  {
    Math::Swap(uv0.y, uv1.y);
    Math::Swap(uvAux0.y, uvAux1.y);
  }

  viewNode.mStreamedVertexType = PrimitiveType::Triangles;
  viewNode.mStreamedVertexStart = frameBlock.mRenderQueues->mStreamedVertices.Size();
  viewNode.mStreamedVertexCount = 0;

  bool hasArea = mOwner->has(Area) != nullptr;

  //frameBlock.mRenderQueues->AddStreamedQuad(viewNode, pos0, pos1, uv0, uv1, mVertexColor, uvAux0, uvAux1);
}
Aabb TBUIContainer::GetViewPlaneAabb(Aabb localAabb)
{
  Vec3 center = localAabb.GetCenter();
  Vec3 widths = localAabb.GetHalfExtents();
  Vec3 worldSize = mTransform->GetScale();
  float maxX = Math::Max(Math::Abs(center.x - widths.x), Math::Abs(center.x + widths.x)) * worldSize.x;
  float maxY = Math::Max(Math::Abs(center.y - widths.y), Math::Abs(center.y + widths.y)) * worldSize.y;
  float radius = Math::Sqrt(maxX * maxX + maxY * maxY);
  return Aabb(Vec3(0.0f), Vec3(1.0f) * radius / worldSize);
}
void TBUIContainer::ComputeLocalToViewMatrix(Mat4& localToView, Mat4& localToWorld, Mat4& worldToView)
{
  if (mGeometryMode == SpriteGeometryMode::ViewPlane)
    MakeLocalToViewAligned(localToView, localToWorld, worldToView, mTransform->GetWorldTranslation());
  else
    localToView = worldToView * localToWorld;
}
Vec2 TBUIContainer::GetLocalCenter()
{
  if (Area* area = mOwner->has(Area))
  {
    Vec2 size = area->GetSize();
    Vec2 offset = -Location::GetDirection(area->GetOrigin());

    return offset * size;
  }
  else
  {
    IntVec2 iSize = mView->GetTexture()->GetSize();
    Vec2 size = Vec2(iSize.x, iSize.y);
    //Vec2 origin = mSpriteSource->GetOrigin();
    Vec2 origin = Vec2(0,0);
    // Origin is in uv coordinate direction, so y needs to be flipped for
    // position
    Vec2 offset = size * 0.5f - origin;
    offset.y = -offset.y;

    //return offset / mSpriteSource->PixelsPerUnit;
    return offset;
  }
}
Vec2 TBUIContainer::GetLocalWidths()
{
  if (Area* area = mOwner->has(Area))
    return area->GetSize() * 0.5;
  else
  {

    // return mView->GetTexture()->GetSize() * 0.5f / mSpriteSource->PixelsPerUnit;
    IntVec2 size = mView->GetTexture()->GetSize() * 0.5f;
    return Vec2(size.x, size.y);
  }
}
} // namespace Zero