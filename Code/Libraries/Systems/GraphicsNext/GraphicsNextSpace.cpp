// MIT Licensed (see LICENSE.md).

#include "Precompiled.hpp"

namespace Zero
{

ZilchDefineType(GraphicsNextSpace, builder, type)
{
  ZeroBindComponent();
  ZeroBindDocumented();
  ZeroBindSetup(SetupMode::DefaultSerialization);
  type->AddAttribute(ObjectAttributes::cCore);

  ZeroBindDependency(Space);

  ZilchBindFieldProperty(mActive);
  ZilchBindFieldProperty(mRandomSeed)->AddAttribute(PropertyAttributes::cInvalidatesObject);
  ZilchBindFieldProperty(mSeed)->ZeroFilterEquality(mRandomSeed, bool, false);
}

void GraphicsNextSpace::Serialize(Serializer& stream)
{
  SerializeNameDefault(mActive, true);
  SerializeNameDefault(mRandomSeed, false);
  SerializeNameDefault(mSeed, 0u);
}

void GraphicsNextSpace::Initialize(CogInitializer& initializer)
{
  mFrameTime = 0.0f;
  mLogicTime = 0.0f;

  if (!mRandomSeed)
    mRandom.SetSeed(mSeed);

  mGraphicsEngine = Z::gEngine->has(GraphicsNextEngine);
  mGraphicsEngine->AddSpace(this);

  ConnectThisTo(this, Events::SpaceDestroyed, OnSpaceDestroyed);
  ConnectThisTo(this, Events::SystemLogicUpdate, OnLogicUpdate);
  // ConnectThisTo(GetOwner(), Events::GraphicsFrameUpdate, OnFrameUpdate);
}

void GraphicsNextSpace::OnSpaceDestroyed(ObjectEvent* event)
{
  mGraphicsEngine->RemoveSpace(this);
}




void GraphicsNextSpace::OnLogicUpdate(UpdateEvent* event)
{
  mLogicTime += event->Dt;
}

// currently considering keeping this as a part of graphics update and not frame
// update
void GraphicsNextSpace::OnFrameUpdate(float frameDt)
{
}
} // namespace Zero
