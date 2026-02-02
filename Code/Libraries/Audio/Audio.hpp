// MIT Licensed (see LICENSE.md).

// Forward declaration for Opus encoder (from Opus library)
struct OpusEncoder;

namespace Zero
{

DeclareEnum2(LatencyValues, LowLatency, HighLatency);
DeclareEnum2(StreamTypes, Output, Input);
DeclareEnum6(StreamStatus, Uninitialized, Initialized, Started, Stopped, ApiProblem, DeviceProblem);

// This function type will be used to either request audio output data or to provide audio input
// data. It will be called on the audio device thread.
typedef void IOCallbackType(float* outputBuffer, float* inputBuffer, unsigned framesPerBuffer, void* data);

// Audio Input Output

class AudioInputOutput
{
public:
  AudioInputOutput();
  ~AudioInputOutput();

  // Initializes the underlying audio API
  StreamStatus::Enum InitializeAPI(Zero::String* resultMessage);
  // Initializes the specified audio stream
  StreamStatus::Enum InitializeStream(StreamTypes::Enum whichStream, Zero::String* resultMessage);
  // Starts the specified audio stream, which will use the provided callback function to
  // either request or provide audio data.
  StreamStatus::Enum
  StartStream(StreamTypes::Enum whichStream, Zero::String* resultMessage, IOCallbackType* callback, void* callbackData);
  // Stops the specified audio stream
  StreamStatus::Enum StopStream(StreamTypes::Enum whichStream, Zero::String* resultMessage);
  // Shuts down the underlying audio API
  void ShutDownAPI();
  // Returns the number of channels in the specified audio stream
  unsigned GetStreamChannels(StreamTypes::Enum whichStream);
  // Returns the sample rate of the specified audio stream
  unsigned GetStreamSampleRate(StreamTypes::Enum whichStream);
  // Used to calculate the size for the input and output ring buffers
  float GetBufferSizeMultiplier();

private:
  OsHandle PlatformData;
};

DeclareEnum7(MidiEventType, MidiNoteOn, MidiNoteOff, MidiPitchWheel, MidiVolume, MidiModWheel, MidiControl, NotSet);

// MIDI Data

class MidiData
{
public:
  MidiEventType::Enum mEventType;
  int mData1;
  float mData2;
  float mData3;
};

class MidiInput;
typedef void (*MidiDataCallback)(MidiData* data, MidiInput* input);

// MIDI Input

class MidiInput
{
public:
  MidiInput();
  ~MidiInput();

  MidiDataCallback mOnMidiData;
  void* mUserData;
  OsHandle mHandle;
};

// Audio file encoding constants
namespace AudioConstants
{
// 20 ms of audio data at 48000 samples per second
const unsigned cPacketFrames = 960;
// Recommended max packet size
const unsigned cMaxPacketSize = 4000;
} // namespace AudioConstants

/// The choices for how to load and play an audio file.
/// <param name="StreamFromFile">The audio data will be read from the file and decompressed as it plays.</param>
/// <param name="StreamFromMemory">The compressed audio data will be read into memory when the
/// Sound resource is loaded and will be decompressed as it plays.</param>
/// <param name="Uncompressed">The audio data will be decompressed and held in memory when the Sound resource is
/// loaded.</param>
/// <param name="Auto">This will choose whether to stream a file depending on its length. Files longer
/// than 30 seconds will be streamed from memory, and those longer than 1 minute will be streamed from file.</param>
DeclareEnum4(AudioFileLoadType, StreamFromFile, StreamFromMemory, Uncompressed, Auto);

struct FileHeader
{
  const char Name[4] = {'Z', 'E', 'R', 'O'};
  short Channels;
  unsigned SamplesPerChannel;
};

struct PacketHeader
{
  PacketHeader() : Channel(0), Size(0)
  {
  }

  const char Name[4] = {'p', 'a', 'c', 'k'};
  short Channel;
  unsigned Size;
};

// Packet Encoder
class PacketEncoder
{
public:
  PacketEncoder() : Encoder(nullptr)
  {
  }
  ~PacketEncoder();

  // Initializes encoder for use with EncodePacket.
  // If the encoder already exists, it will be destroyed and re-created.
  void InitializeEncoder();
  // Encodes a single packet of data and allocates a buffer for the encoded data.
  // Number of samples must be the same as PacketFrames
  void EncodePacket(const float* dataBuffer, const unsigned samples, Zero::Array<::byte>& encodedData);

  static const unsigned cChannels = 1;

private:
  // Used for repeated calls to EncodePacket
  ::OpusEncoder* Encoder;
};

} // namespace Zero
