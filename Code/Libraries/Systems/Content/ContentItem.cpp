// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(ContentItem, builder, type)
{
  type->HandleManager = ZilchManagerId(ContentItemHandleManager);
  type->Add(new ContentItemMetaOperations());

  ZilchBindGetterProperty(Name);
}

void ContentItemHandleManager::ObjectToHandle(const ::byte* object, BoundType* type, Handle& handleToInitialize)
{
  if (object == nullptr)
    return;

  ContentItem* item = (ContentItem*)object;
  handleToInitialize.HandleU32 = item->Id;
}

::byte* ContentItemHandleManager::HandleToObject(const Handle& handle)
{
  return (::byte*)Z::gContentSystem->LoadedItems.FindValue(handle.HandleU32, nullptr);
}

ContentItem::ContentItem()
{
  mLibrary = nullptr;
  mRuntimeResource = 0;
  ShowInEditor = true;
  EditMode = ContentEditMode::NoEdit;
  mResourceIsContentItem = false;
  mIgnoreMultipleResourcesWarning = false;

  // Generate an Id for this Content Item
  ++Z::gContentSystem->mIdCount;
  Id = Z::gContentSystem->mIdCount;

  // Store the handle
  Z::gContentSystem->LoadedItems[Id] = this;
}

ContentItem::~ContentItem()
{
  Z::gContentSystem->LoadedItems.Erase(Id);
}

String ContentItem::GetName()
{
  return FilePath::GetFileNameWithoutExtension(Filename);
}

// IResourceSource interface implementation
ResourceId ContentItem::GetResourceId() const
{
  return mRuntimeResource;
}

String ContentItem::GetSourcePath() const
{
  return FilePath::Combine(mLibrary->SourcePath, Filename);
}

String ContentItem::GetLibraryName() const
{
  if (mLibrary)
    return mLibrary->Name;
  return String();
}

bool ContentItem::IsLibraryWritable() const
{
  if (mLibrary)
    return mLibrary->GetWritable();
  return false;
}

void ContentItem::SaveSourceContent()
{
  SaveContent();
}

void ContentItem::OnResourceModified(Resource* resource)
{
  // Update the runtime resource id when a resource is created/modified
  if (resource && EditMode == ContentEditMode::ResourceObject)
    mRuntimeResource = resource->mResourceId;
}

bool ContentItem::IsResourceTemplate() const
{
  // Check if this content item has a ResourceTemplate component
  // Note: Using const_cast because QueryComponentId is not const
  ContentItem* self = const_cast<ContentItem*>(this);
  return self->Has<ResourceTemplate>() != nullptr;
}

bool ContentItem::GetShowInEditor() const
{
  return ShowInEditor;
}

void ContentItem::SetShowInEditor(bool show)
{
  ShowInEditor = show;
}

void ContentItem::SaveContent()
{
  ErrorIf(EditMode == ContentEditMode::NoEdit, "Can not save. No edit type.");

  if (EditMode == ContentEditMode::ResourceObject)
  {
    // Ask the resource to save itself
    Resource* resource = Z::gResources->GetResource(mRuntimeResource);
    ErrorIf(resource == nullptr, "Resource has been removed but this content item still refers to it.");

    if (resource)
    {
      if (Z::gContentSystem->mHistoryEnabled)
      {
        String backUpPath = Z::gContentSystem->GetHistoryPath(mLibrary);
        BackUpFile(backUpPath, GetFullPath());
      }
      resource->Save(this->GetFullPath());
    }
  }

  // Save the meta file
  SaveMetaFile();
}

void ContentItem::SaveMetaFile()
{
  String metaFilePath = GetMetaFilePath();
  SaveToDataFile(*this, metaFilePath);
}

String ContentItem::GetMetaFilePath()
{
  return BuildString(GetFullPath(), ".meta");
}

String ContentItem::GetFullPath()
{
  return FilePath::Combine(mLibrary->SourcePath, Filename);
}

void ContentItem::GetTags(Array<String>& tags)
{
  ContentTags* contentTags = this->has(ContentTags);
  if (contentTags == nullptr)
    return;

  forRange (String currTag, contentTags->mTags.All())
  {
    tags.PushBack(currTag);
  }
}

void ContentItem::GetTags(HashSet<String>& tags)
{
  ContentTags* contentTags = this->has(ContentTags);
  if (contentTags == nullptr)
    return;

  forRange (String currTag, contentTags->mTags.All())
  {
    tags.Insert(currTag);
  }
}

void ContentItem::SetTag(StringParam tag)
{
  ContentTags* contentTags = this->has(ContentTags);
  if (contentTags == nullptr)
  {
    contentTags = new ContentTags();
    AddComponent(contentTags);
  }

  // Insert tag
  contentTags->mTags.Insert(tag);
}

void ContentItem::SetTags(HashSet<String>& tags)
{
  if (tags.Empty())
    return;

  ContentTags* contentTags = this->has(ContentTags);
  if (contentTags == nullptr)
  {
    contentTags = new ContentTags();
    AddComponent(contentTags);
  }

  // Insert each tag
  forRange (String tag, tags.All())
  {
    contentTags->mTags.Insert(tag);
  }
}

void ContentItem::RemoveTags(HashSet<String>& tags)
{
  ContentTags* contentTags = this->has(ContentTags);
  if (contentTags == nullptr)
    return;

  forRange (String& tag, tags.All())
  {
    contentTags->mTags.Erase(tag);
  }
}

bool ContentItem::HasTag(StringParam tag)
{
  ContentTags* contentTags = this->has(ContentTags);
  if (contentTags)
    return contentTags->mTags.Contains(tag);
  return false;
}

Object* ContentItem::GetEditingObject(Resource* resource)
{
  ErrorIf(EditMode == ContentEditMode::NoEdit, "Can not edit. No edit type.");
  if (EditMode == ContentEditMode::ResourceObject)
  {
    if (resource == nullptr)
      return Z::gResources->GetResource(mRuntimeResource);

    BoundType* resourceType = ZilchVirtualTypeId(resource);

    forRange (Property* prop, resourceType->GetProperties())
    {
      if (prop->HasAttribute(PropertyAttributes::cProperty))
        return resource;
    }

    return this;
  }
  else
  {
    return this;
  }
}

void ContentItem::BuildContentItem(bool useJob)
{
  ProfileScopeFunctionArgs(Filename);
  BuildOptions options(mLibrary);
  BuildContentItem(options);
}

void ContentItem::BuildListing(ResourceListing& listing)
{
}

void ContentItem::Serialize(Serializer& stream)
{
}

void ContentItem::Initialize(ContentLibrary* library)
{
  mLibrary = library;
  if (library)
    library->AddContentItem(this);
  OnInitialize();
}

ContentComponent* ContentItem::QueryComponentId(BoundType* typeId)
{
  return nullptr;
}

void ContentItem::OnInitialize()
{
}

// Resource Meta Operations
ZilchDefineType(ContentItemMetaOperations, builder, type)
{
}

void ContentItemMetaOperations::ObjectModified(HandleParam object, bool intermediateChange)
{
  MetaOperations::ObjectModified(object, intermediateChange);

  if (!intermediateChange)
  {
    ContentItem* contentItem = object.Get<ContentItem*>(GetOptions::AssertOnNull);
    Z::gContentSystem->mModifiedContentItems.Insert(contentItem->Id);
  }
}

} // namespace Zero
