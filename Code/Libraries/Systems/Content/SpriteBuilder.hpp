// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

// SpriteSampling, SpriteFill, SpriteDataMembers macro are in RendererBase/Renderer.hpp
// SpriteData class is in Graphics/SpriteTypes.hpp

// Sprite source builder outputs a png to be composited at runtime by the engine.
class SpriteSourceBuilder : public DirectBuilderComponent
{
public:
  ZilchDeclareType(SpriteSourceBuilder, TypeCopyMode::ReferenceType);
  SpriteDataMembers();

  void Serialize(Serializer& stream) override;

  SpriteSourceBuilder();

  SpriteFill::Enum GetFill();
  void SetFill(SpriteFill::Enum fill);

  int GetLeft();
  void SetLeft(int value);

  int GetRight();
  void SetRight(int value);

  int GetTop();
  void SetTop(int value);

  int GetBottom();
  void SetBottom(int value);

  // BuilderComponent Interface
  void Generate(ContentInitializer& initializer) override;
  void BuildContent(BuildOptions& buildOptions) override;
  bool NeedsBuilding(BuildOptions& options) override;
  void BuildListing(ResourceListing& listing) override;
  void SetDefaults();
};

} // namespace Zero
