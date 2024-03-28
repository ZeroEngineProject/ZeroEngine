// MIT Licensed (see LICENSE.md).

#pragma once

// Other projects
#include "Common/CommonStandard.hpp"
#include "Gpu/GpuStandard.hpp"
#include "Platform/PlatformStandard.hpp"
#include "Engine/EngineStandard.hpp"
#include "Meta/MetaStandard.hpp"
#include "SpatialPartition/SpatialPartitionStandard.hpp"
// #include "ZilchShaders/ZilchShadersStandard.hpp"

namespace Zero
{

class GraphicsNextEngine;
class GraphicsNextSpace;

// Graphics library
class ZeroNoImportExport GraphicsNextLibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(GraphicsNextLibrary, "ZeroEngine");

  static void Initialize();
  static void Shutdown();
};

} // namespace Zero

#include "GraphicsNextSpace.hpp"

#include "GraphicsNextEngine.hpp"
