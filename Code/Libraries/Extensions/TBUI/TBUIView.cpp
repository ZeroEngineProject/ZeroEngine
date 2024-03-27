// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIView, builder, type)
{
  type->CreatableInScript = true;
  ZeroBindDocumented();

  ZilchBindConstructor();
  ZilchBindDestructor();

  ZilchBindMethod(AddDemo);
  ZilchBindMethod(RemoveDemo);
}
TBUIView::TBUIView()
{
  mUI = TBUI::GetInstance();
  mRenderer = mUI->GetRenderer();

  // Set gravity all so we resize correctly
  SetGravity(tb::WIDGET_GRAVITY_ALL);

  // Set initial size for view
  tb::TBRect rect = mUI->GetRootWidget()->GetRect();
  SetSize(rect.w, rect.h);

  mUI->AddView(this);
}

TBUIView::~TBUIView()
{
  mUI->RemoveView(this);
}

void TBUIView::SetRenderToTexture(bool value, u32 width, u32 height)
{
  if (value && mRenderTexture == nullptr)
  {
    mRenderTexture = Texture::CreateRuntime();

    StringBuilder nameBuilder;
    nameBuilder.AppendFormat("tbui_uiview_%i_%i", width, height);
    mRenderTexture->Name = nameBuilder.ToString();

    // mRenderTexture->SetFormat(TextureFormat::RGBA8);
    mRenderTexture->SetFiltering(TextureFiltering::Enum::Bilinear);
    mRenderTexture->SetAddressingX(TextureAddressing::Enum::Clamp);
    mRenderTexture->SetAddressingY(TextureAddressing::Enum::Clamp);
    mRenderTexture->SetMipMapping(TextureMipMapping::Enum::GpuGenerated);
  }

  if (!value && mRenderTexture != nullptr)
  {
    mRenderTexture->Delete();
    mRenderTexture = nullptr;
  }

  if (mRenderTexture != nullptr)
  {
    mRenderTexture->SetSize(IntVec2(width, height));
  }
}

void TBUIView::SetSize(int width, int height)
{
  if (mRenderTexture != nullptr)
  {
    mRenderTexture->SetSize(IntVec2(width, height));
  }
  tb::TBWidget::SetSize(width, height);
}

void TBUIView::AddDemo()
{
  if (mDemo == nullptr)
  {
    mDemo = MakeUnique<TBUIDemo>(this);
  }
}

void TBUIView::RemoveDemo()
{
  if (mDemo != nullptr)
  {
    mDemo.Reset();
  }
}

void TBUIView::UpdateBatches()
{
  mBatches.Clear();

  tb::TBRect clipRect = GetRect();
  mRenderer->SetClipRect(clipRect);
  mRenderer->mBatches = &mBatches;

  tb::g_renderer->BeginPaint(clipRect.w, clipRect.h);

  InvokePaint(tb::TBWidget::PaintProps());

  tb::g_renderer->EndPaint();
}
void TBUIView::RenderBatches()
{
  if (mBatches.Size() == 0)
    return;

  ///////////////////////////////////////

  ColorTransform colorTx = {Vec4(1, 1, 1, 1)};
  Mat4 localTx;
  this->mTranslation = Vec3{0, 0, 0};
  this->mAngle = 0;
  Build2dTransform(localTx, this->mTranslation, this->mAngle);
  mWorldTx = localTx * Mat4::cIdentity;

  tb::TBRect rect = GetRect();

  // IntVec2 size = IntVec2{rect.w, rect.h};
  // IntVec2 pos = IntVec2{rect.x, rect.y};
  Vec2 size = Vec2{(real)rect.w, (real)rect.h};
  Vec2 pos = Vec2{(real)rect.x, (real)rect.y};

  if (mRenderTexture != nullptr)
  {
    size.x = mRenderTexture->GetSize().x;
    size.y = mRenderTexture->GetSize().y;
  }

  IntRect clipRect = IntRect(pos.x, pos.y, size.x, size.y);

  Vec4 clearColor = Vec4{0.5f, 0.7f, 0.3f, 0.1f};

  GraphicsEngine* graphics = Z::gEngine->has(GraphicsEngine);
  RenderTasks& renderTasks = *graphics->mRenderTasksBack;
  RenderQueues& renderQueues = *graphics->mRenderQueuesBack;
  renderQueues.mRenderTasks = &renderTasks;

  FrameBlock& frameBlock = renderQueues.mFrameBlocks.PushBack();
  ViewBlock& viewBlock = renderQueues.mViewBlocks.PushBack();
  frameBlock.mRenderQueues = &renderQueues;

  Mat4 translation;
  translation.Translate(size.x * -0.5f, size.y * -0.5f, 0.0f);
  Mat4 scale;
  scale.Scale(1.0f, -1.0f, 1.0f);
  viewBlock.mWorldToView = scale * translation;
  BuildOrthographicTransformZero(viewBlock.mViewToPerspective, size.y, size.x / size.y, -1.0f, 1.0f);

  Mat4 apiPerspective;
  Z::gRenderer->BuildOrthographicTransform(apiPerspective, size.y, size.x / size.y, -1.0f, 1.0f);
  viewBlock.mZeroPerspectiveToApiPerspective = apiPerspective * viewBlock.mViewToPerspective.Inverted();

  forRange (const TBUIBatch& batch, mBatches.All())
  {
    // RenderBatch(batch);

    CreateRenderData(
        viewBlock, frameBlock, mWorldTx, clipRect, batch.Vertices, batch.Texture, PrimitiveType::Triangles);
  }

  IndexRange& indexRange = viewBlock.mRenderGroupRanges.PushBack();
  indexRange.start = 0;
  indexRange.end = viewBlock.mViewNodes.Size();

  RenderTaskRange& renderTaskRange = renderTasks.mRenderTaskRanges.PushBack();
  renderTaskRange.mFrameBlockIndex = renderQueues.mFrameBlocks.Size() - 1;
  renderTaskRange.mViewBlockIndex = renderQueues.mViewBlocks.Size() - 1;
  renderTaskRange.mTaskIndex = renderTasks.mRenderTaskBuffer.mCurrentIndex;
  renderTaskRange.mTaskCount = 0;

  HandleOf<RenderTarget> renderTarget;

  if (mRenderTexture != nullptr)
  {
    renderTarget = Z::gEngine->has(GraphicsEngine)->GetRenderTarget(mRenderTexture);
  }
  else
  {
    renderTarget = Z::gEngine->has(GraphicsEngine)->GetRenderTarget((uint)size.x, (uint)size.y, TextureFormat::RGBA8);
  }

  GraphicsRenderSettings renderSettings;
  renderSettings.SetColorTarget(renderTarget);
  renderSettings.mBlendSettings[0].SetBlendAlpha();
  renderSettings.mScissorMode = ScissorMode::Enabled;

  BoundType* defaultRenderPass = MetaDatabase::GetInstance()->FindType("ColorOutput");
  ReturnIf(defaultRenderPass == nullptr, , "We expected to have a type defined called ColorOutput");

  HandleOf<MaterialBlock> renderPassHandle = ZilchAllocate(MaterialBlock, defaultRenderPass);
  MaterialBlock& renderPass = renderPassHandle;

  Material* spriteMaterial = MaterialManager::FindOrNull("AlphaSprite");
  uint shaderInputsId = 0;

  {
    Pair<u64, uint> key((u64)spriteMaterial->mResourceId, shaderInputsId);
    IndexRange range = spriteMaterial->AddShaderInputs(renderTasks.mShaderInputs, renderTasks.mShaderInputsVersion);
    renderTasks.mShaderInputRanges.Insert(key, range);
  }
  {
    Pair<u64, uint> key(cFragmentShaderInputsId, shaderInputsId);
    IndexRange range = renderPass.AddShaderInputs(renderTasks.mShaderInputs);
    renderTasks.mShaderInputRanges.Insert(key, range);
  }

  RenderTaskHelper helper(renderTasks.mRenderTaskBuffer);
  // helper.AddRenderTaskClearTarget(renderSettings, clearColor, 0, 0, 0xFF);
  helper.AddRenderTaskRenderPass(renderSettings, 0, defaultRenderPass->Name, shaderInputsId);

  ScreenViewport viewport = {pos.x, pos.y, (int)size.x, (int)size.y};
  helper.AddRenderTaskBackBufferBlit(renderTarget, viewport);

  renderTaskRange.mTaskCount = 2;

  Z::gEngine->has(GraphicsEngine)->ClearRenderTargets();

  ///////////////////////////////////////
}
void TBUIView::RenderBatch(const TBUIBatch& batch)
{
  // CreateRenderData(viewBlock, frameBlock, mWorldTx, clipRect, batch.Vertices, batch.Texture,
  // PrimitiveType::Triangles);
}

void TBUIView::CreateRenderData(ViewBlock& viewBlock,
                                FrameBlock& frameBlock,
                                const Mat4& worldMatrix,
                                const IntRect& clipRect,
                                const Array<StreamedVertex>& vertices,
                                const Texture* texture,
                                PrimitiveType::Enum primitiveType)
{
  if (vertices.Empty())
    return;

  StreamedVertexArray& streamedVertices = frameBlock.mRenderQueues->mStreamedVertices;

  ViewNode& viewNode = AddRenderNodes(viewBlock, frameBlock, worldMatrix, clipRect, texture);

  for (uint i = 0; i < vertices.Size(); ++i)
  {
    StreamedVertex vertex = vertices[i];
    vertex.mPosition = Math::TransformPoint(viewNode.mLocalToView, vertex.mPosition);
    streamedVertices.PushBack(vertex);
  }

  viewNode.mStreamedVertexType = primitiveType;
  viewNode.mStreamedVertexCount = streamedVertices.Size() - viewNode.mStreamedVertexStart;

  // viewNode.mStreamedVertexStart = offset;
  // viewNode.mStreamedVertexCount = count;
}

ViewNode& TBUIView::AddRenderNodes(ViewBlock& viewBlock,
                                   FrameBlock& frameBlock,
                                   const Mat4& worldMatrix,
                                   const IntRect& clipRect,
                                   const Texture* texture)
{
  FrameNode& frameNode = frameBlock.mFrameNodes.PushBack();
  ViewNode& viewNode = viewBlock.mViewNodes.PushBack();

  frameNode.mGraphicalEntry = nullptr;
  viewNode.mGraphicalEntry = nullptr;

  frameNode.mBorderThickness = 1.0f;
  frameNode.mBlendSettingsOverride = false;
  frameNode.mRenderingType = RenderingType::Streamed;
  frameNode.mCoreVertexType = CoreVertexType::Streamed;
  frameNode.mLocalToWorld = worldMatrix.Transposed();
  viewNode.mFrameNodeIndex = frameBlock.mFrameNodes.Size() - 1;
  viewNode.mLocalToView = viewBlock.mWorldToView * frameNode.mLocalToWorld;

  frameNode.mClip = Vec4(clipRect.Left(), clipRect.Top(), clipRect.Right(), clipRect.Bottom());

  // maybe cache this lookup on root
  Material* spriteMaterial = nullptr;

  spriteMaterial = MaterialManager::FindOrNull("AlphaSprite");

  frameNode.mMeshRenderData = nullptr;
  frameNode.mMaterialRenderData = spriteMaterial->mRenderData;
  frameNode.mTextureRenderData = texture->mRenderData;

  // default setup for adding streamed data
  viewNode.mStreamedVertexType = PrimitiveType::Triangles;
  viewNode.mStreamedVertexStart = frameBlock.mRenderQueues->mStreamedVertices.Size();
  viewNode.mStreamedVertexCount = 0;

  return viewNode;
}
} // namespace Zero