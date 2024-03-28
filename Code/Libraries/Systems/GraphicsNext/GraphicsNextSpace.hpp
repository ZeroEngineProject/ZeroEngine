// MIT Licensed (see LICENSE.md).

#pragma once

namespace Zero
{

/// Core space component that manages all interactions between graphics related
/// objects.
class GraphicsNextSpace : public Component
{
public:
  ZilchDeclareType(GraphicsNextSpace, TypeCopyMode::ReferenceType);

  // Component Interface
  void Initialize(CogInitializer& initializer) override;
  void Serialize(Serializer& stream) override;

  void OnSpaceDestroyed(ObjectEvent* event);

  

  void OnLogicUpdate(UpdateEvent* event);
  // void OnFrameUpdate(UpdateEvent* updateEvent);
  void OnFrameUpdate(float frameDt);

  Link<GraphicsNextSpace> EngineLink;
  GraphicsNextEngine* mGraphicsEngine;



  /// If graphics for this Space should be running.
  bool mActive;

  float mFrameTime;
  float mLogicTime;

  /// If the random number generator used by graphics objects should be seeded
  /// randomly.
  bool mRandomSeed;
  /// Value to seed the random number generator with.
  uint mSeed;
  Math::Random mRandom;
};

} // namespace Zero
