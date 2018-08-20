///////////////////////////////////////////////////////////////////////////////
///
/// Author: Andrea Ellinger
/// Copyright 2017, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"

namespace Audio
{
  //----------------------------------------------------------------------------- Audio IO Interface

  //************************************************************************************************
  AudioIOInterface::AudioIOInterface() :
    MixedOutputBuffer(nullptr),
    InputBuffer(nullptr),
    mOutputStreamLatency(LatencyValues::LowLatency)
  {
    memset(OutputBufferSizePerLatency, 0, sizeof(unsigned) * LatencyValues::Size);

  }

  //************************************************************************************************
  AudioIOInterface::~AudioIOInterface()
  {
    if (MixedOutputBuffer)
      delete[] MixedOutputBuffer;
    if (InputBuffer)
      delete[] InputBuffer;
  }

  //************************************************************************************************
  bool AudioIOInterface::Initialize()
  {
    // Return false if the API is not initialized successfully
    if (AudioIO.InitializeAPI(&mApiErrorMessage) != StreamStatus::Initialized)
      return false;

    // Initialize output stream
    StreamInfo& outputInfo = StreamInfoList[StreamTypes::Output];
    outputInfo.mStatus = AudioIO.InitializeStream(StreamTypes::Output, &outputInfo.mErrorMessage);
    outputInfo.mChannels = AudioIO.GetStreamChannels(StreamTypes::Output);
    outputInfo.mSampleRate = AudioIO.GetStreamSampleRate(StreamTypes::Output);

    InitializeOutputBuffers();

    // Initialize input stream
    StreamInfo& inputInfo = StreamInfoList[StreamTypes::Input];
    inputInfo.mStatus = AudioIO.InitializeStream(StreamTypes::Input, &inputInfo.mErrorMessage);
    inputInfo.mChannels = AudioIO.GetStreamChannels(StreamTypes::Input);
    inputInfo.mSampleRate = AudioIO.GetStreamSampleRate(StreamTypes::Input);

    InitializeInputBuffers();

    return true;
  }

  //************************************************************************************************
  void AudioIOInterface::ShutDown()
  {
    AudioIO.ShutDownAPI();
  }

  //************************************************************************************************
  bool AudioIOInterface::StartStreams(bool startOutput, bool startInput)
  {
    if (!startOutput && !startInput)
      return false;

    bool returnValue = true;

    // Start output stream if requested
    if (startOutput)
    {
      // Start the stream, passing in the IOCallback function and this object as the data
      StreamInfoList[StreamTypes::Output].mStatus = AudioIO.StartStream(StreamTypes::Output, 
        &StreamInfoList[StreamTypes::Output].mErrorMessage, IOCallback, this);

      // Check if it was started successfully
      if (StreamInfoList[StreamTypes::Output].mStatus != StreamStatus::Started)
        returnValue = false;
    }

    // Start input stream if requested
    if (startInput)
    {
      // Start the stream, passing in the IOCallback function and this object as the data
      StreamInfoList[StreamTypes::Input].mStatus = AudioIO.StartStream(StreamTypes::Input, 
        &StreamInfoList[StreamTypes::Input].mErrorMessage, IOCallback, this);

      // Check if it was started successfully
      if (StreamInfoList[StreamTypes::Input].mStatus != StreamStatus::Started)
        returnValue = false;
    }

    return returnValue;
  }

  //************************************************************************************************
  bool AudioIOInterface::StopStreams(bool stopOutput, bool stopInput)
  {
    bool returnValue = true;

    // Stop output stream if requested
    if (stopOutput)
    {
      StreamInfoList[StreamTypes::Output].mStatus = AudioIO.StopStream(StreamTypes::Output,
        &StreamInfoList[StreamTypes::Output].mErrorMessage);

      // Check if it was stopped successfully
      if (StreamInfoList[StreamTypes::Output].mStatus != StreamStatus::Stopped)
        returnValue = false;
    }

    // Stop input stream if requested
    if (stopInput)
    {
      StreamInfoList[StreamTypes::Input].mStatus = AudioIO.StopStream(StreamTypes::Input,
        &StreamInfoList[StreamTypes::Input].mErrorMessage);

      // Check if it was stopped successfully
      if (StreamInfoList[StreamTypes::Input].mStatus != StreamStatus::Stopped)
        returnValue = false;
    }

    return returnValue;
  }

  //************************************************************************************************
  Audio::StreamStatus::Enum AudioIOInterface::GetStreamStatus(StreamTypes::Enum whichStream)
  {
    return StreamInfoList[whichStream].mStatus;
  }

  //************************************************************************************************
  const Zero::String& AudioIOInterface::GetStreamErrorMessage(StreamTypes::Enum whichStream)
  {
    return StreamInfoList[whichStream].mErrorMessage;
  }

  //************************************************************************************************
  const Zero::String& AudioIOInterface::GetSystemErrorMessage()
  {
    return mApiErrorMessage;
  }

  //************************************************************************************************
  unsigned AudioIOInterface::GetStreamChannels(StreamTypes::Enum whichStream)
  {
    return StreamInfoList[whichStream].mChannels;
  }

  //************************************************************************************************
  unsigned AudioIOInterface::GetStreamSampleRate(StreamTypes::Enum whichStream)
  {
    return StreamInfoList[whichStream].mSampleRate;
  }

  //************************************************************************************************
  void AudioIOInterface::WaitUntilOutputNeededThreaded()
  {
    MixThreadSemaphore.WaitAndDecrement();
  }

  //************************************************************************************************
  void AudioIOInterface::GetInputDataThreaded(Zero::Array<float>& buffer, unsigned howManySamples)
  {
    unsigned samplesAvailable = InputRingBuffer.GetReadAvailable();

    if (samplesAvailable < howManySamples)
      howManySamples = samplesAvailable;
    
    buffer.Resize(howManySamples);

    unsigned samplesRead = InputRingBuffer.Read(buffer.Data(), howManySamples);

    if (samplesRead != howManySamples)
      buffer.Resize(samplesRead);
  }

  //************************************************************************************************
  void AudioIOInterface::SetOutputLatency(LatencyValues::Enum newLatency)
  {
    // If the setting is the same, don't do anything
    if (newLatency == mOutputStreamLatency)
      return;

    // Set the latency variable
    mOutputStreamLatency = newLatency;
    // Shut down the output stream so we can change the buffer size
    AudioIO.StopStream(StreamTypes::Output, nullptr);
    // Set up the output buffer with the current latency setting
    InitializeRingBuffer(OutputRingBuffer, MixedOutputBuffer, OutputBufferSizePerLatency[mOutputStreamLatency]);
    // Restart the audio output
    AudioIO.InitializeStream(StreamTypes::Output, nullptr);
    StreamInfoList[StreamTypes::Output].mStatus = AudioIO.StartStream(StreamTypes::Output,
      &StreamInfoList[StreamTypes::Output].mErrorMessage, IOCallback, this);
  }

  //************************************************************************************************
  void AudioIOInterface::InitializeOutputBuffers()
  {
    unsigned size = GetBufferSize(AudioIO.GetStreamSampleRate(StreamTypes::Output),
      AudioIO.GetStreamChannels(StreamTypes::Output));

    OutputBufferSizePerLatency[LatencyValues::LowLatency] = size;
    OutputBufferSizePerLatency[LatencyValues::HighLatency] = size * 4;

    InitializeRingBuffer(OutputRingBuffer, MixedOutputBuffer, OutputBufferSizePerLatency[mOutputStreamLatency]);
  }

  //************************************************************************************************
  void AudioIOInterface::InitializeInputBuffers()
  {
    unsigned size = GetBufferSize(AudioIO.GetStreamSampleRate(StreamTypes::Input),
      AudioIO.GetStreamChannels(StreamTypes::Input));

    InitializeRingBuffer(InputRingBuffer, InputBuffer, size * 2);
  }

  //************************************************************************************************
  unsigned AudioIOInterface::GetBufferSize(unsigned sampleRate, unsigned channels)
  {
    // Start at the BufferSizeStartValue
    unsigned size = BufferSizeStartValue;
    // We need to get close to a value that accounts for the sample rate and channels
    float checkValue = AudioIO.GetBufferSizeMultiplier() * sampleRate * channels;
    // Continue multiplying by 2 until we get close enough (buffer must be multiple of 2)
    while (size < checkValue)
      size *= 2;

    return size;
  }

  //************************************************************************************************
  void AudioIOInterface::InitializeRingBuffer(RingBuffer& ringBuffer, float* buffer, unsigned size)
  {
    // If the buffer already exists, delete it
    if (buffer)
      delete[] buffer;

    // Create the buffer
    buffer = new float[size];
    // Initialize the RingBuffer with the new buffer
    ringBuffer.Initialize(sizeof(float), size, buffer);
  }

  //************************************************************************************************
  void AudioIOInterface::GetMixedOutputSamples(float* outputBuffer, const unsigned howManySamples)
  {
    // Copy the samples from the OutputRingBuffer
    unsigned samplesRead = OutputRingBuffer.Read((void*)outputBuffer, howManySamples);

    // If there weren't enough available, set the rest to 0
    if (samplesRead < howManySamples)
      memset(outputBuffer + samplesRead, 0, (howManySamples - samplesRead) * sizeof(float));

    // Signal the semaphore for the mix thread
    MixThreadSemaphore.Increment();
  }

  //************************************************************************************************
  void AudioIOInterface::SaveInputSamples(const float* inputBuffer, unsigned howManySamples)
  {
    // Note: remember this function is called asynchronously from the audio device thread

    InputRingBuffer.Write(inputBuffer, howManySamples);
  }

  //------------------------------------------------------------------------------------------------

  //************************************************************************************************
  void IOCallback(float* outputBuffer, float* inputBuffer, unsigned framesPerBuffer, void* data)
  {
    AudioIOInterface* io = ((AudioIOInterface*)data);

    if (outputBuffer)
      io->GetMixedOutputSamples(outputBuffer, framesPerBuffer *
        io->GetStreamChannels(StreamTypes::Output));
    if (inputBuffer)
      io->SaveInputSamples(inputBuffer, framesPerBuffer *
        io->GetStreamChannels(StreamTypes::Input));
  }


}
