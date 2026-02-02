// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
// AudioFileLoadType moved to Audio/Audio.hpp

class AudioContent : public ContentComposition
{
public:
  ZilchDeclareType(AudioContent, TypeCopyMode::ReferenceType);

  AudioContent();
};

const String SoundExtension = ".snd";

class SoundBuilder : public DirectBuilderComponent
{
public:
  ZilchDeclareType(SoundBuilder, TypeCopyMode::ReferenceType);

  /// If Streamed is selected, or if Auto is selected and the file is longer than one minute,
  /// the sound file will be streamed from disk at runtime instead of loaded into memory.
  /// Streaming files can't be played multiple times simultaneously and can't use loop tails.
  AudioFileLoadType::Enum mFileLoadType;
  /// If true, the audio will be normalized when loaded so that the highest volume peak matches
  /// the MaxVolume value.
  bool mNormalize;
  /// The volume of the sound will be altered so that the highest volume peak matches this value.
  /// All audio samples will be adjusted equally.
  float mMaxVolume;

  SoundBuilder() :
      DirectBuilderComponent(0, SoundExtension, "Sound"),
      mFileLoadType(AudioFileLoadType::Auto),
      mNormalize(false),
      mMaxVolume(0.9f),
      mStreamed(false)
  {
  }

  // BuilderComponent Interface
  void Generate(ContentInitializer& initializer) override;
  void Serialize(Serializer& stream) override;
  void BuildContent(BuildOptions& options) override;
  bool NeedsBuilding(BuildOptions& options) override;
  void BuildListing(ResourceListing& listing) override;

  // This should be removed at the next major version
  bool mStreamed;
};

} // namespace Zero
