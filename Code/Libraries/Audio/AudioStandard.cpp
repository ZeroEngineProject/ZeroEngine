// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"
#include "opus.h"

namespace Zero
{

// cSystemSampleRate is only used in this file for encoder initialization
namespace { const unsigned cSystemSampleRate = 48000; }

PacketEncoder::~PacketEncoder()
{
  if (Encoder)
    opus_encoder_destroy(Encoder);
}

void PacketEncoder::InitializeEncoder()
{
  if (Encoder)
    opus_encoder_destroy(Encoder);

  int error;
  Encoder = opus_encoder_create(cSystemSampleRate, cChannels, OPUS_APPLICATION_VOIP, &error);
}

void PacketEncoder::EncodePacket(const float* dataBuffer, const unsigned samples, Array<::byte>& encodedData)
{
  ReturnIf(!Encoder, , "Tried to encode packet without initializing encoder");
  ReturnIf(samples != AudioConstants::cPacketFrames, , "Tried to encode packet with incorrect number of samples");

  encodedData.Resize(AudioConstants::cMaxPacketSize);
  unsigned encodedDataSize =
      opus_encode_float(Encoder, dataBuffer, samples, encodedData.Data(), AudioConstants::cMaxPacketSize);
  encodedData.Resize(encodedDataSize);
}

} // namespace Zero
