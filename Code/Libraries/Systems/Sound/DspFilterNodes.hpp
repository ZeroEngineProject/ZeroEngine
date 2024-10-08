// MIT Licensed (see LICENSE.md).

#pragma once

namespace Zero
{

// Volume Node

/// Changes the volume of audio generated by its input SoundNodes
class VolumeNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(VolumeNode, TypeCopyMode::ReferenceType);

  VolumeNode(StringParam name, unsigned ID);

  /// The volume adjustment that will be applied to the node's input.
  /// A value of 1 does not affect the sound; 2 will double the sound's volume, 0.5 will halve it,
  /// and 0 will make the sound inaudible.
  float GetVolume();
  void SetVolume(float volume);
  /// Interpolates the Volume property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateVolume(float volume, float interpolationTime);
  /// The volume adjustment, in decibels, that will be applied to the node's input.
  /// A value of 0 does not affect the sound; 6 will double the sound's volume, -6 will halve it,
  /// and -100 is effectively the same as a Volume of 0.
  float GetDecibels();
  void SetDecibels(float volume);
  /// Interpolates the Decibels property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateDecibels(float volumeDB, float interpolationTime);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  float GetVolumeChangeFromOutputsThreaded() override;
  void InterpolateVolumeThreaded(float volume, float time);

  class Data
  {
  public:
    Data() : mInterpolating(false), mIndex(0)
    {
    }

    // If true, currently interpolating volume
    bool mInterpolating;
    // Current interpolation index
    unsigned mIndex;
  };

  // The volume of the modification
  Threaded<float> mVolume;
  // Used to interpolate between volumes
  InterpolatingObject Interpolator;

  Data CurrentData;
  Data PreviousData;
};

// Panning Node

/// Changes the left and right channel volumes of its input SoundNode's audio separately
class PanningNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(PanningNode, TypeCopyMode::ReferenceType);

  PanningNode(StringParam name, unsigned ID);

  /// If this property is true, the audio will be combined into a single channel
  /// before being split between the right and left channels. If it is false and the audio
  /// has more than two channels, it will be combined into only two channels before being processed.
  bool GetSumToMono();
  void SetSumToMono(bool isMono);
  /// The volume multiplier applied to audio in the left channel.
  float GetLeftVolume();
  void SetLeftVolume(float volume);
  /// Interpolates the LeftVolume property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateLeftVolume(float volume, float time);
  /// The volume multiplier applied to audio in the right channel.
  float GetRightVolume();
  void SetRightVolume(float volume);
  /// Interpolates the RightVolume property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateRightVolume(float volume, float time);
  /// Interpolates both left and right volume properties at once. The first parameter
  /// is the value to change the LeftVolume to, the second is the RightVolume,
  /// and the third is the number of seconds to use for the interpolation.
  void InterpolateVolumes(float leftVolume, float rightVolume, float time);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  float GetVolumeChangeFromOutputsThreaded() override;
  void SetVolumeThreaded(bool left, float newVolume, float time);

  class Data
  {
  public:
    Data() : mInterpolating(false), mLeftIndex(0), mRightIndex(0)
    {
    }

    // If true, currently interpolating volume
    bool mInterpolating;
    // Current interpolation index
    unsigned mLeftIndex;
    unsigned mRightIndex;
  };

  // If true, the node combines all audio into one channel before panning
  Threaded<bool> mSumToMono;
  // Current volume of the left channel
  Threaded<float> mLeftVolume;
  // Current volume of the right channel
  Threaded<float> mRightVolume;
  // Interpolates the left channel volume from one value to another
  InterpolatingObject LeftInterpolator;
  // Interpolates the right channel volume from one value to another
  InterpolatingObject RightInterpolator;
  // If true, the volume is currently modified and the node will perform its calculations
  Threaded<bool> mActive;

  Data CurrentData;
  Data PreviousData;
};

// Pitch Node

/// Changes the pitch and speed of audio generated by its input SoundNodes
class PitchNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(PitchNode, TypeCopyMode::ReferenceType);

  PitchNode(StringParam name, unsigned ID);

  /// The pitch adjustment applied to the node's input.
  /// A value of 0 will not affect the sound's pitch; 1 will raise the pitch by
  /// an octave and speed up the sound, and -1 will lower the sound by an octave and slow it down.
  /// Large pitch changes will likely affect the quality of the sound.
  float GetPitch();
  void SetPitch(float pitchRatio);
  /// Interpolates the Pitch property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolatePitch(float pitchRatio, float interpolationTime);
  /// The pitch adjustment, in semitones (or half-steps), applied to the node's input.
  /// A value of 0 will not affect the sound's pitch; 12 will raise the pitch by an octave
  /// and speed up the sound, and -12 will lower the sound by an octave and slow it down.
  /// Large pitch changes will likely affect the quality of the sound.
  float GetSemitones();
  void SetSemitones(float pitchSemitones);
  /// Interpolates the Semitones property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateSemitones(float pitchSemitones, float interpolationTime);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void SetPitchThreaded(float semitones, float interpolationTime);

  PitchChangeHandler PitchObject;

  Threaded<float> mPitchSemitones;
};

// Low Pass Node

/// Applies a low pass filter to audio generated by its input SoundNodes (removes high frequencies)
class LowPassNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(LowPassNode, TypeCopyMode::ReferenceType);

  LowPassNode(StringParam name, unsigned ID);
  ~LowPassNode();

  /// Frequencies above this number in the node's input will be attenuated.
  /// Setting this value to 20,000.00 or higher will skip all filter calculations.
  float GetCutoffFrequency();
  void SetCutoffFrequency(float frequency);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetCutoffFrequencyThreaded(float frequency);

  // The cutoff frequency for the low pass filter
  Threaded<float> mCutoffFrequency;
  // The filter used for calculations (contains history so must have separate filters per listener)
  typedef Zero::HashMap<ListenerNode*, LowPassFilter*> FilterMapType;
  FilterMapType FiltersPerListener;
};

// High Pass Node

/// Applies a high pass filter to audio generated by its input SoundNodes (removes low frequencies)
class HighPassNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(HighPassNode, TypeCopyMode::ReferenceType);

  HighPassNode(StringParam name, unsigned ID);
  ~HighPassNode();

  /// Frequencies below this number in the node's input will be attenuated.
  /// Setting this value to 20.0 or lower will skip all filter calculations.
  float GetCutoffFrequency();
  void SetCutoffFrequency(float frequency);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetCutoffFrequencyThreaded(float frequency);

  // The cutoff frequency for the high pass filter
  Threaded<float> mCutoffFrequency;
  // The filter used for calculations
  typedef Zero::HashMap<ListenerNode*, HighPassFilter*> FilterMapType;
  FilterMapType FiltersPerListener;
};

// Band Pass Node

/// Applies a band pass filter to audio generated by its input SoundNodes (removes low and high frequencies)
class BandPassNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(BandPassNode, TypeCopyMode::ReferenceType);

  BandPassNode(StringParam name, unsigned ID);
  ~BandPassNode();

  /// The center frequency of the band. Frequencies above and below this band will be attenuated.
  float GetCentralFrequency();
  void SetCentralFrequency(float frequency);
  /// The Q number of the band pass filter: higher numbers make the band smaller,
  /// while smaller numbers make it wider. The default value is 0.669.
  float GetQualityFactor();
  void SetQualityFactor(float Q);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetCentralFrequencyThreaded(float frequency);
  void SetQualityFactorThreaded(float Q);

  // The current central frequency
  Threaded<float> mCentralFrequency;
  // The Q factor (controls width of the band)
  Threaded<float> mQuality;
  // The filter used for calculations
  typedef Zero::HashMap<ListenerNode*, BandPassFilter*> FilterMapType;
  FilterMapType FiltersPerListener;
};

// Equalizer Node

/// Applied volume changes to specific frequency bands in the audio generated by its input SoundNodes
class EqualizerNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(EqualizerNode, TypeCopyMode::ReferenceType);

  EqualizerNode(StringParam name, unsigned ID);
  ~EqualizerNode();

  /// The volume adjustment applied to frequencies below 80 Hz in the node's input.
  /// Values above 1.0 will boost these frequencies while values less than 1.0 will reduce them.
  float GetLowPassGain();
  void SetLowPassGain(float gain);
  /// The volume adjustment applied to frequencies above 5000 Hz in the node's input.
  /// Values above 1.0 will boost these frequencies while values less than 1.0 will reduce them.
  float GetHighPassGain();
  void SetHighPassGain(float gain);
  /// The volume adjustment applied to frequencies within the band centered at 150 Hz in the node's input.
  /// Values above 1.0 will boost these frequencies while values less than 1.0 will reduce them.
  float GetBand1Gain();
  void SetBand1Gain(float gain);
  /// The volume adjustment applied to frequencies within the band centered at 600 Hz in the node's input.
  /// Values above 1.0 will boost these frequencies while values less than 1.0 will reduce them.
  float GetBand2Gain();
  void SetBand2Gain(float gain);
  /// The volume adjustment applied to frequencies within the band centered at 2500 Hz in the node's input.
  /// Values above 1.0 will boost these frequencies while values less than 1.0 will reduce them.
  float GetBand3Gain();
  void SetBand3Gain(float gain);
  /// Sets the volume adjustment of all bands (the parameters in order are low pass, band 1, band 2,
  /// band 3, high pass) over the number of seconds passed in as the final parameter.
  void
  InterpolateAllBands(float lowPass, float band1, float band2, float band3, float highPass, float timeToInterpolate);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetBandGainThreaded(EqualizerBands::Enum whichBand, float gain);
  void InterpolateAllThreaded(float* values, float timeToInterpolate);

  // volume adjustment applied to sounds below 80 Hz
  Threaded<float> mLowPassGain;
  // volume adjustment applied to sounds above 5000 Hz
  Threaded<float> mHighPassGain;
  // volume adjustment applied to the band centered at 150 Hz
  Threaded<float> mBand1Gain;
  // volume adjustment applied to the band centered at 600 Hz
  Threaded<float> mBand2Gain;
  // volume adjustment applied to the band centered at 2500 Hz
  Threaded<float> mBand3Gain;
  // The filters used for calculations
  typedef Zero::HashMap<ListenerNode*, Equalizer*> FilterMapType;
  FilterMapType FiltersPerListener;
};

// Reverb Node

/// Applies a simple reverb filter to audio generated by its input SoundNodes
class ReverbNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(ReverbNode, TypeCopyMode::ReferenceType);

  ReverbNode(StringParam name, unsigned ID);
  ~ReverbNode();

  /// The length of the reverb tail, in seconds. The default value is 0.1.
  float GetLength();
  void SetLength(float time);
  /// DEPRECATED The WetValue property should be used instead.
  float GetWetPercent();
  void SetWetPercent(float percent);
  /// The percentage of the node's output (0 - 1.0) which has the reverb filter applied to it.
  /// Setting this property to 0 will stop all reverb calculations.
  float GetWetValue();
  void SetWetValue(float value);
  /// DEPRECATED The InterpolateWetValue method should be used instead.
  void InterpolateWetPercent(float percent, float time);
  /// Interpolates the WetValue property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateWetValue(float value, float time);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetLengthMsThreaded(float time);
  void SetWetValueThreaded(float value);
  void InterpolateWetValueThreaded(float value, float time);

  // The current length of reverb in seconds
  Threaded<float> mTimeSec;
  // The current wet level (0 - 1.0f)
  Threaded<float> mWetLevelValue;
  // Whether the reverb tail has finished
  bool mOutputFinishedThreaded;
  // The filter used for calculations
  typedef Zero::HashMap<ListenerNode*, Reverb*> FilterMapType;
  FilterMapType FiltersPerListener;
};

// Delay Node

/// Applies a delay filter to audio generated by its input SoundNodes
class DelayNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(DelayNode, TypeCopyMode::ReferenceType);

  DelayNode(StringParam name, unsigned ID);
  ~DelayNode();

  /// The length of the delay, in seconds.
  float GetDelay();
  void SetDelay(float seconds);
  /// DEPRECATED The FeedbackValue property should be used instead.
  float GetFeedbackPercent();
  void SetFeedbackPercent(float feedback);
  /// The percentage of output (from 0 to 1.0f) which is fed back into the filter as input,
  /// creating an echo-like effect.
  float GetFeedbackValue();
  void SetFeedbackValue(float feedback);
  /// DEPRECATED The WetValue property should be used instead.
  float GetWetPercent();
  void SetWetPercent(float wetLevel);
  /// The percentage of the node's output (0 - 1.0) which has the delay filter applied to it.
  float GetWetValue();
  void SetWetValue(float wetLevel);
  /// DEPRECATED The InterpolateWetValue method should be used instead.
  void InterpolateWetPercent(float wetPercent, float time);
  /// Interpolates the WetValue property from its current value to the value passed in
  /// as the first parameter, over the number of seconds passed in as the second parameter.
  void InterpolateWetValue(float wetPercent, float time);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetDelayMsThreaded(float time);
  void SetFeedbackThreaded(float value);
  void SetWetValueThreaded(float value);
  void InterpolateWetValueThreaded(float value, float time);

  // Length of delay
  Threaded<float> mDelaySec;
  // Feedback value (0 - 1.0)
  Threaded<float> mFeedbackValue;
  // Wet level value (0 - 1.0)
  Threaded<float> mWetValue;
  // The filter used for calculations
  typedef Zero::HashMap<ListenerNode*, DelayLine*> FilterMapType;
  FilterMapType FiltersPerListener;
};

// Flanger Node

/// Applies a flanger filter to audio generated by its input SoundNodes
class FlangerNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(FlangerNode, TypeCopyMode::ReferenceType);

  FlangerNode(StringParam name, unsigned ID);
  ~FlangerNode();

  /// The maximum delay reached by the modulation. It will oscillate between 0 and this value
  /// at the frequency set by the ModulationFrequency property.
  float GetMaxDelayMillisec();
  void SetMaxDelayMillisec(float delay);
  /// The frequency of the oscillator which varies the modulation.
  float GetModulationFrequency();
  void SetModulationFrequency(float frequency);
  /// DEPRECATED The FeedbackValue property should be used instead.
  float GetFeedbackPercent();
  void SetFeedbackPercent(float percent);
  /// The percentage of output (0 - 1.0) which is fed back into the filter as input.
  float GetFeedbackValue();
  void SetFeedbackValue(float value);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetModFreqThreaded(float frequency);
  void SetFeedbackThreaded(float value);
  void SetMaxDelayMSecThreaded(float delay);

  class Data
  {
  public:
    Data(float frequency, float feedback, float maxDelayMs);

    // Delay filter for calculations
    DelayLine Delay;
    // Oscillator filter for calculations
    Oscillator LFO;
  };

  typedef Zero::HashMap<ListenerNode*, Data*> FilterMapType;
  FilterMapType FiltersPerListener;

  // Maximum delay allowed
  Threaded<float> mMaxDelayMS;
  // Modulation frequency
  Threaded<float> mModFrequency;
  // Feedback value (0 - 1.0)
  Threaded<float> mFeedback;
};

// Chorus Node

/// Applies a chorus filter to audio generated by its input SoundNodes
class ChorusNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(ChorusNode, TypeCopyMode::ReferenceType);

  ChorusNode(StringParam name, unsigned ID);
  ~ChorusNode();

  /// The maximum delay reached by the modulation. It will oscillate between the MinDelayMillisec
  /// value and this value at the frequency set by the ModulationFrequency property.
  float GetMaxDelayMillisec();
  void SetMaxDelayMillisec(float delay);
  /// The minimum delay reached by the modulation. It will oscillate between the this value and the
  /// MaxDelayMillisec value at the frequency set by the ModulationFrequency property.
  float GetMinDelayMillisec();
  void SetMinDelayMillisec(float delay);
  /// The frequency of the oscillator which varies the modulation.
  float GetModulationFrequency();
  void SetModulationFrequency(float frequency);
  /// DEPRECATED The FeedbackValue property should be used instead.
  float GetFeedbackPercent();
  void SetFeedbackPercent(float percent);
  /// The percentage of output (0 - 1.0) which is fed back into the filter as input.
  float GetFeedbackValue();
  void SetFeedbackValue(float value);
  /// The offset value of the chorus filter, in milliseconds.
  float GetOffsetMillisec();
  void SetOffsetMillisec(float offset);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetModFreqThreaded(float frequency);
  void SetFeedbackThreaded(float value);

  class Data
  {
  public:
    Data(float frequency, float minDelay, float feedback);

    // Delay filter for calculations
    DelayLine Delay;
    // Oscillator filter for calculations
    Oscillator LFO;
  };

  typedef Zero::HashMap<ListenerNode*, Data*> FilterMapType;
  FilterMapType FiltersPerListener;

  // Minimum delay amount
  Threaded<float> mMinDelayMS;
  // Maximum delay amount
  Threaded<float> mMaxDelayMS;
  // Modulation frequency
  Threaded<float> mModFrequency;
  // Feedback value (0 - 1.0f)
  Threaded<float> mFeedback;
  // Chorus offset value
  Threaded<float> mChorusOffsetMS;
};

// Compressor Node

class CompressorNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(CompressorNode, TypeCopyMode::ReferenceType);

  CompressorNode(StringParam name, unsigned ID);

  /// The volume adjustment applied to the audio input, in decibels.
  float GetInputGainDecibels();
  void SetInputGainDecibels(float gain);
  /// The threshold, in decibels, at which the volume of the input is affected by the compressor.
  float GetThresholdDecibels();
  void SetThresholdDecibels(float dB);
  /// The time for the compressor to ramp to full effect after the input reaches the threshold.
  float GetAttackMillisec();
  void SetAttackMillisec(float attack);
  /// The time for the compressor to ramp from full effect to off after the input drops below the threshold.
  float GetReleaseMillisec();
  void SetReleaseMillisec(float release);
  /// The ratio of the volume reduction applied by the compressor.
  float GetRatio();
  void SetRatio(float ratio);
  /// The volume adjustment applied to the compressor output, in decibels.
  float GetOutputGainDecibels();
  void SetOutputGainDecibels(float gain);
  /// The knee width of the compressor, in decibels.
  float GetKneeWidth();
  void SetKneeWidth(float knee);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;

  // Filter for calculations
  DynamicsProcessor Filter;
  // Input gain value in decibels
  float mInputGainDB;
  // Threshold value in decibels
  float mThresholdDB;
  // Attack time in milliseconds
  float mAttackMSec;
  // Release time in milliseconds
  float mReleaseMSec;
  // Filter ratio value
  float mRatio;
  // Output gain in decibels
  float mOutputGainDB;
  // Filter knee width value
  float mKneeWidth;
};

// Expander Node

class ExpanderNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(ExpanderNode, TypeCopyMode::ReferenceType);

  ExpanderNode(StringParam name, unsigned ID);

  /// The volume adjustment applied to the audio input, in decibels.
  float GetInputGainDecibels();
  void SetInputGainDecibels(float gain);
  /// The threshold, in decibels, at which the volume of the input is affected by the expander.
  float GetThresholdDecibels();
  void SetThresholdDecibels(float dB);
  /// The time for the expander to ramp to full effect after the input reaches the threshold.
  float GetAttackMillisec();
  void SetAttackMillisec(float attack);
  /// The time for the expander to ramp from full effect to off after the input goes above the threshold.
  float GetReleaseMillisec();
  void SetReleaseMillisec(float release);
  /// The ratio of the volume reduction applied by the expander.
  float GetRatio();
  void SetRatio(float ratio);
  /// The volume adjustment applied to the expander output, in decibels.
  float GetOutputGainDecibels();
  void SetOutputGainDecibels(float gain);
  /// The knee width of the expander, in decibels.
  float GetKneeWidth();
  void SetKneeWidth(float knee);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;

  // Filter for calculations
  DynamicsProcessor Filter;
  // Input gain value in decibels
  float mInputGainDB;
  // Threshold value in decibels
  float mThresholdDB;
  // Attack time in milliseconds
  float mAttackMSec;
  // Release time in milliseconds
  float mReleaseMSec;
  // Filter ratio value
  float mRatio;
  // Output gain in decibels
  float mOutputGainDB;
  // Filter knee width value
  float mKneeWidth;
};

// Add Noise Node

/// Adds random noise (static) to audio generated by its input SoundNodes
class AddNoiseNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(AddNoiseNode, TypeCopyMode::ReferenceType);

  AddNoiseNode(StringParam name, unsigned ID);

  /// The gain of the additive noise component, in decibels.
  float GetAdditiveGain();
  void SetAdditiveGain(float decibels);
  /// The gain of the multiplicative noise component, in decibels.
  float GetMultiplicativeGain();
  void SetMultiplicativeGain(float decibels);
  /// The cutoff frequency used for the additive noise component, in Hz.
  float GetAdditiveCutoff();
  void SetAdditiveCutoff(float frequency);
  /// The cutoff frequency used for the multiplicative noise component, in Hz.
  float GetMultiplicativeCutoff();
  void SetMultiplicativeCutoff(float frequency);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;

  // Gain of additive noise in decibels (default is -40)
  float mAdditiveNoiseDB;
  // Gain of multiplicative noise in decibels (default is -10)
  float mMultipleNoiseDB;
  // Cutoff frequency of additive noise in Hz (default is 2000)
  float mAdditiveNoiseCutoffHz;
  // Cutoff frequency of multiplicative noise in Hz (default is 50)
  float mMultipleNoiseCutoffHz;

  int mAddCountThreaded;
  int mMultiplyCountThreaded;
  float mAddNoiseThreaded;
  float mMultiplyNoiseThreaded;
  Math::Random mRandomValuesThreaded;
  float mAddPeriodThreaded;
  float mMultiplyPeriodThreaded;
  float mAddGainThreaded;
  float mMultiplyGainThreaded;
};

// Modulation Node

/// Applies either ring or amplitude modulation to audio generated by its input SoundNodes
class ModulationNode : public SimpleCollapseNode
{
public:
  ZilchDeclareType(ModulationNode, TypeCopyMode::ReferenceType);

  ModulationNode(StringParam name, unsigned ID);
  ~ModulationNode();

  /// If this property is true, the node will apply amplitude modulation (multiply the audio input
  /// with a unipolar sine wave with values from 0.0 to 1.0). If false, the node will apply
  /// ring modulation (multiply the input with a bipolar sine wave with values from -1.0 to 1.0).
  bool GetUseAmplitudeModulation();
  void SetUseAmplitudeModulation(bool useAmplitude);
  /// The frequency of the sine wave used for the modulation.
  float GetFrequency();
  void SetFrequency(float frequency);
  /// DEPRECATED The WetValue property should be used instead.
  float GetWetPercent();
  void SetWetPercent(float percent);
  /// The percentage of the input (0 - 1.0) which should have the modulation applied to it.
  float GetWetValue();
  void SetWetValue(float value);

private:
  bool GetOutputSamples(BufferType* outputBuffer,
                        const unsigned numberOfChannels,
                        ListenerNode* listener,
                        const bool firstRequest) override;
  void RemoveListenerThreaded(SoundEvent* event) override;
  void SetUseAmplitudeThreaded(bool useAmplitude);
  void SetFrequencyThreaded(float frequency);

  Threaded<bool> mAmplitude;
  Threaded<float> mFrequency;
  Threaded<float> mWetLevelValue;

  typedef Zero::HashMap<ListenerNode*, Oscillator*> OscMapType;
  OscMapType OscillatorsPerListenerThreaded;
};

} // namespace Zero
