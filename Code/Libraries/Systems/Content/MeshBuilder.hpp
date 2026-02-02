// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
// FixedVertexDescription, MeshHeader, and mesh file constants moved to RendererBase/Renderer.hpp

/// Geometry content item that builds meshes.
class MeshBuilder : public BuilderComponent
{
public:
  ZilchDeclareType(MeshBuilder, TypeCopyMode::ReferenceType);

  MeshBuilder();

  bool mCombineMeshes;
  bool mGenerateSmoothNormals;
  float mSmoothingAngleDegreesThreshold;
  bool mGenerateTangentSpace;
  bool mInvertUvYAxis;
  bool mFlipWindingOrder;
  bool mFlipNormals;

  Array<GeometryResourceEntry> Meshes;

  // BuilderComponent Interface
  bool NeedsBuilding(BuildOptions& options) override;
  void Generate(ContentInitializer& initializer) override;
  void Serialize(Serializer& stream) override;
  void BuildListing(ResourceListing& listing) override;
};

} // namespace Zero
