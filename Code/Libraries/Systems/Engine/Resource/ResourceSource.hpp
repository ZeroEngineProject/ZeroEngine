// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

// Forward declarations
class Resource;
class ResourceLibrary;

/// Resource Ids are 64 bit numbers. Stored in text files at a 16 digit hex value.
/// This typedef is duplicated from Resource.hpp to avoid circular dependencies.
typedef Guid ResourceId;

/// Abstract interface for resource source metadata.
/// This abstracts ContentItem so that Engine does not depend on the Content library.
/// The Content library implements this interface on ContentItem.
class IResourceSource
{
public:
  virtual ~IResourceSource() = default;

  /// Get the unique identifier for the resource this source produces.
  virtual ResourceId GetResourceId() const = 0;

  /// Get the full path to the source file.
  virtual String GetSourcePath() const = 0;

  /// Get the name of the library this source belongs to.
  virtual String GetLibraryName() const = 0;

  /// Check if the source's library is writable (not read-only like Core).
  virtual bool IsLibraryWritable() const = 0;

  /// Save the source content to disk.
  virtual void SaveSourceContent() = 0;

  /// Called when the associated resource has been modified.
  /// Allows the source to update its metadata or perform other bookkeeping.
  virtual void OnResourceModified(Resource* resource) = 0;

  // Tag management - sources maintain tags that are applied to resources
  virtual void GetTags(Array<String>& tags) = 0;
  virtual void GetTags(HashSet<String>& tags) = 0;
  virtual void SetTags(HashSet<String>& tags) = 0;
  virtual void RemoveTags(HashSet<String>& tags) = 0;
  virtual bool HasTag(StringParam tag) = 0;

  /// Check if this source is a resource template.
  virtual bool IsResourceTemplate() const = 0;

  /// Get/set whether this source should be shown in the editor.
  virtual bool GetShowInEditor() const = 0;
  virtual void SetShowInEditor(bool show) = 0;
};

/// Abstract interface for builder metadata.
/// This abstracts BuilderComponent so that Engine does not depend on the Content library.
/// The Content library implements this interface on BuilderComponent.
class IBuilderInfo
{
public:
  virtual ~IBuilderInfo() = default;

  /// Get the filter tag for this builder (used for resource sub-types).
  virtual String GetTag() const = 0;

  /// Get the resource owner name, if any.
  virtual String GetResourceOwner() const = 0;
};

} // namespace Zero
