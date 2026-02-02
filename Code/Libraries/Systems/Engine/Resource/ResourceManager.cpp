// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

namespace Events
{
DefineEvent(ResourceNewlyCreated);
DefineEvent(ResourceAdded);
DefineEvent(ResourceModified);
DefineEvent(ResourceRemoved);
DefineEvent(ResourceReload);
} // namespace Events

ZilchDefineType(ResourceEvent, builder, type)
{
}

ResourceEvent::ResourceEvent()
{
  Manager = nullptr;
  EventResource = nullptr;
  EventResourceLibrary = nullptr;
  RemoveMode = RemoveMode::None;
}

ResourceEvent::ResourceEvent(ResourceManager* manager, Resource* resource) : Manager(manager), EventResource(resource)
{
  EventResourceLibrary = nullptr;
  RemoveMode = RemoveMode::None;
}

void* ResourceManager::operator new(size_t size)
{
  return Resource::sHeap->Allocate(size);
}

void ResourceManager::operator delete(void* pMem, size_t size)
{
  return Resource::sHeap->Deallocate(pMem, size);
}

void AddIdAndCheckConflicts(ResourceManager::ResourceIdMapType& resourceIdMap, ResourceEntry& entry, Resource* resource)
{
  // see if there is already a resource of the given id
  ResourceManager::ResourceIdMapType::range idrange = resourceIdMap.Find(entry.mResourceId);
  if (!idrange.Empty())
  {
    if (Z::gRuntimeEditor)
      Z::gRuntimeEditor->OnResourceIdConflict(entry, idrange.Front().second);
    else
      ZPrint("Resource Id Conflict %s\n", entry.Name.c_str());
  }
  else
  {
    resourceIdMap[entry.mResourceId] = resource;
  }
}

ResourceManager::ResourceManager(BoundType* resourceType)
{
  mResourceType = nullptr;
  mCanCreateNew = false;
  mCanAddFile = false;
  mCanReload = false;
  mCanDuplicate = false;
  mNoFallbackNeeded = false;
  mSearchable = false;
  mPreview = false;
  mHidden = false;
  mExtension = String();
  mResourceType = resourceType;
  mResourceTypeName = resourceType->Name;
  Z::gResources->RegisterManager(this);
  mAddSortWeight = 100;
}

ResourceManager::~ResourceManager()
{
}

BoundType* ResourceManager::GetResourceType()
{
  return mResourceType;
}

Resource* ResourceManager::CreateNewResourceInternal(StringParam name)
{
  Resource* resource = AllocateDefaultConstructed();
  resource->Name = name;
  resource->mManager = this;
  DefaultSerializer defaultSerializer;
  resource->Serialize(defaultSerializer);
  return resource;
}

Resource* ResourceManager::CreateRuntimeInternal(StringParam name)
{
  static const String cRuntimeResourceName = "Runtime";

  String resourceName = name;
  if (resourceName.Empty())
    resourceName = cRuntimeResourceName;

  Resource* resource = CreateNewResourceInternal(resourceName);

  // Generate a new id for the runtime resource
  ResourceId id = GenerateUniqueId64();

  resource->mResourceId = id;
  String hexId = ToString(id, true);
  resource->ResourceIdName = BuildString(hexId, ":", resourceName);
  resource->mIsRuntimeResource = true;

  ResourceEntry entry;
  entry.mResourceId = id;
#if ZERO_EDITOR
  entry.mResourceSource = nullptr;
#endif

  AddIdAndCheckConflicts(ResourceIdMap, entry, resource);
  AddIdAndCheckConflicts(Z::gResources->ResourceIdMap, entry, resource);

  return resource;
}

void ResourceManager::ValidateNewName(Status& status, StringParam name, BoundType* optionalType)
{
  // Do nothing (assume its valid)
}

void ResourceManager::ValidateRawName(Status& status, StringParam name, BoundType* optionalType)
{
  // Do nothing (assume its valid)
}

void ResourceManager::AddResource(ResourceEntry& entry, Resource* resource)
{
  resource->mResourceLibrary = entry.mLibrary;

  // Make the resource string name of (includes name and guid)
  String hexId = ToString(entry.mResourceId, true);
  String resourceIdName = BuildString(hexId, ":", entry.Name);

#if ZERO_EDITOR
  // Notify the resource source of the runtime resource id
  if (entry.mResourceSource)
    entry.mResourceSource->OnResourceModified(resource);
#endif

  // Add to the main resource manager
  AddIdAndCheckConflicts(Z::gResources->ResourceIdMap, entry, resource);

  AddIdAndCheckConflicts(ResourceIdMap, entry, resource);

  // Names are local to each resource
  ResourceNameMap[entry.Name] = entry.mResourceId;

  resource->Name = entry.Name;
  resource->mManager = this;
  resource->ResourceIdName = resourceIdName;
  resource->mResourceId = entry.mResourceId;

#if ZERO_EDITOR
  resource->mResourceSource = entry.mResourceSource;
  resource->mBuilderInfo = entry.mBuilderInfo;
#endif

  // Send a resource added event on both this builder and on the resource system
  ResourceEvent event;
  event.Manager = this;
  event.EventResource = resource;
  this->DispatchEvent(Events::ResourceAdded, &event);
  Z::gResources->DispatchEvent(Events::ResourceAdded, &event);
}

void ResourceManager::AddLoader(StringParam name, ResourceLoader* loader)
{
  Z::gResources->AddLoader(name, loader);
}

String ResourceManager::GetTemplateSourceFile(ResourceAdd& resourceAdd)
{
#if ZERO_EDITOR
  if (resourceAdd.Template->mResourceSource)
    return resourceAdd.Template->mResourceSource->GetSourcePath();
#endif
  return String();
}

ResourceManager::ResourceRange ResourceManager::AllResources()
{
  return ResourceIdMap.Values();
}

void ResourceManager::Remove(Resource* resource, RemoveMode::Enum removeMode)
{
  // Remove the resource so that is can not be found with EnumerateResources or by name

  // Remove from Name map
  ResourceNameMap.Erase(resource->Name);

  // Remove from local Id map
  if (resource->mResourceId != 0)
    ResourceIdMap.Erase(resource->mResourceId);

  // Signal that the resource has been removed
  ResourceEvent event(this, resource);
  event.RemoveMode = removeMode;
  this->DispatchEvent(Events::ResourceRemoved, &event);
  Z::gResources->DispatchEvent(Events::ResourceRemoved, &event);

  // Now invalidate the id after ResourceRemoved has been sent so
  // handles are still valid during Dispatch

  // Remove from global id map invalidating all handles
  Z::gResources->ResourceIdMap.Erase(resource->mResourceId);
}

void ResourceManager::EnumerateResources(Array<String>& values)
{
  // Append the names of all resource items that should be shown in the editor
  forRange (Resource* resource, AllResources())
  {
#if ZERO_EDITOR
    IResourceSource* source = resource->mResourceSource;
    if (source && source->GetShowInEditor())
      values.PushBack(resource->Name);
#else
    values.PushBack(resource->Name);
#endif
  }

  // Sort alphabetically
  Sort(values.All());
}

void ResourceManager::EnumerateResources(Array<Resource*>& values)
{
  forRange (Resource* resource, AllResources())
    values.PushBack(resource);
}

Resource* ResourceManager::GetResource(StringParam resourceString, ResourceNotFound::Enum notFound)
{
  /// Standard resource string is 16 digit hex and a resource name separated by a ':'
  /// Example 0123456789ABCDEF:SomeName
  // The reason we're using 'SizeInBytes' instead of 'ComputeRuneCount' because we can guarantee that each rune is 1
  // byte for the hex portion of the resource string
  if (resourceString.SizeInBytes() > cHex64Size + 1 && resourceString.Data()[cHex64Size] == ':')
  {
    /// Isolate Hex sub string
    StringRange hexString = resourceString.SubStringFromByteIndices(0, cHex64Size);
    ResourceId resourceId = ReadHexString(hexString);

    /// Isolate Name sub string
    StringRange stringName = resourceString.SubStringFromByteIndices(cHex64Size + 1, resourceString.SizeInBytes());

    Resource* resource = GetResourceNameOrId(stringName, resourceId);
    if (resource)
      return resource;
  }

  // Old resource id is 16 digit hex followed by (name)
  // Example 0123456789ABCDEF(SomeName)
  if (resourceString.SizeInBytes() > cHex64Size + 2 && resourceString.Data()[resourceString.SizeInBytes() - 1] == ')' &&
      resourceString.Data()[cHex64Size] == '(')
  {
    // StringRange delimiter = resourceString.FindFirstOf('(');
    // StringRange delimiterEnd = resourceString.FindFirstOf(')');
    // Hex part
    // StringRange hexString = resourceString.SubString(resourceString.Begin(), delimiter.Begin());
    StringRange hexString = resourceString.SubStringFromByteIndices(0, cHex64Size);
    Guid resourceId = ReadHexString(hexString);

    // String part
    StringRange stringName = resourceString.SubStringFromByteIndices(cHex64Size + 1, resourceString.SizeInBytes() - 1);
    Resource* resource = GetResourceNameOrId(stringName, resourceId);
    if (resource)
      return resource;
  }

  /// Try to find Resource as a simple string
  ResourceId resourceId = ResourceNameMap.FindValue(resourceString, 0);
  if (resourceId)
  {
    Resource* resource = ResourceIdMap.FindValue(resourceId, NULL);
    if (resource)
      return resource;
  }

  // Resource is missing handle failure
  if (notFound == ResourceNotFound::ReturnNull)
    return nullptr;

  if (notFound == ResourceNotFound::ReturnDefault)
    return GetDefaultResource();

  // Empty string use default resource
  if (resourceString.Empty())
    return GetDefaultResource();

  // Signal the error
  MissingResource(resourceString, 0);

  return GetFallbackResource();
}

Resource* ResourceManager::GetResource(ResourceId resourceId, ResourceNotFound::Enum notFound)
{
  // Try the id
  Resource* resource = ResourceIdMap.FindValue(resourceId, nullptr);
  if (resource)
    return resource;

  // Resource can not be found
  if (notFound == ResourceNotFound::ReturnNull)
    return nullptr;

  // Resource Id 0 returns the default resource
  if (resourceId == 0)
    return GetDefaultResource();

  if (notFound == ResourceNotFound::ReturnDefault)
    return GetDefaultResource();

  // Signal the error
  MissingResource(String(), resourceId);

  return GetFallbackResource();
}

Resource* ResourceManager::GetDefaultResource()
{
  return GetResourceByName(DefaultResourceName);
}

Resource* ResourceManager::GetResourceById(ResourceId id)
{
  Resource* resource = ResourceIdMap.FindValue(id, nullptr);
  if (resource == nullptr)
    return GetFallbackResource();
  else
    return resource;
}

Resource* ResourceManager::GetResourceByName(StringParam name)
{
  ResourceId resourceId = ResourceNameMap.FindValue(name, 0);
  return GetResourceById(resourceId);
}

void ResourceManager::MissingResource(StringParam resourceName, ResourceId resourceId)
{
  String nameOfResource = resourceName;

  if (resourceName.Empty())
    nameOfResource = ToString(resourceId);

  String errorMessage = String::Format("Could not find %s resource with name and id "
                                       "'%s'. Using default.",
                                       mResourceTypeName.c_str(),
                                       nameOfResource.c_str());
  DoNotifyErrorWithContext(errorMessage);
}

Resource* ResourceManager::GetFallbackResource()
{
  // If no fallback resource is set use the DefaultResource instead
  String fallback = FallbackResourceName.Empty() ? DefaultResourceName : FallbackResourceName;

  // No fall back or default return null
  if (fallback.Empty())
    return nullptr;

  // Get the fallback resource by name
  Resource* resource = this->GetResource(fallback, ResourceNotFound::ReturnNull);
  // RESOURCEREFACTOR Should this check for no fallback needed?
  if (resource == nullptr /* && mNoFallbackNeeded == false*/)
  {
    ZPrint("No fallback '%s' name %s.\n", mResourceTypeName.c_str(), fallback.c_str());
  }

  return resource;
}

Resource* ResourceManager::GetResourceNameOrId(StringRange resourceName, ResourceId resourceId)
{
  // Try the resource Id
  Resource* resource = ResourceIdMap.FindValue(resourceId, nullptr);
  if (resource)
    return resource;

  // Try to get a resource id for this resource name
  resourceId = ResourceNameMap.FindValue(resourceName, 0);

  // Either name was not found or the id is zero which is never used
  // so just return what was found in the id map
  return ResourceIdMap.FindValue(resourceId, nullptr);
}

const String cNullResource = "null";

Resource* ResourceManager::ResolveResourceStream(cstr fieldName,
                                                 Serializer& stream,
                                                 Resource* resourcePtr,
                                                 ResourceNotFound::Enum ifNotFound,
                                                 cstr defaultResourceName)
{
  if (stream.GetMode() == SerializerMode::Saving)
  {
    if (stream.GetType() == SerializerType::Binary)
    {
      if (resourcePtr != nullptr && resourcePtr->IsRuntime() == false)
      {
        // In binary, save resource by id
        ResourceId id = resourcePtr->mResourceId;
        stream.SerializeField(fieldName, id);
        return resourcePtr;
      }
      else
      {
        // No resource, serialize empty resource id
        ResourceId id = 0;
        stream.SerializeField(fieldName, id);
        return resourcePtr;
      }
    }
    else
    {
      if (resourcePtr != nullptr && resourcePtr->IsRuntime() == false)
      {
        // In text, save resource by Id and name
        StringRange name = resourcePtr->ResourceIdName.All();
        stream.StringField("string", fieldName, name);
        return resourcePtr;
      }
      else
      {
        // No resource, serialize empty string
        StringRange empty = cNullResource.All();
        stream.StringField("string", fieldName, empty);
        return resourcePtr;
      }
    }
  }
  else
  {
    if (stream.GetType() == SerializerType::Binary)
    {
      ResourceId resourceId;
      stream.SerializeField(fieldName, resourceId);
      return GetResource(resourceId, ifNotFound);
    }
    else
    {
      StringRange resourceIdName;

      bool valid = stream.StringField("string", fieldName, resourceIdName);

      // Attempt to read the old type
      if (!valid)
      {
        String oldName = BuildString(fieldName, "Name");
        valid = stream.StringField("string", oldName.c_str(), resourceIdName);
      }

      // Return the default if we didn't find anything
      if (valid == false)
        resourceIdName = defaultResourceName;

      // Return null if it was explicitly set to null
      if (resourceIdName == cNullResource && ifNotFound == ResourceNotFound::ReturnNull)
        return nullptr;

      Resource* newResource = GetResource(resourceIdName, ifNotFound);

      // We have to handle if the Resource specified in the file was removed (or doesn't exist).
      // It will return null, but we still want to set it to the default
      // We need to check if resourceIdName is empty, because empty is valid for nullable properties
      if (newResource == nullptr && valid && resourceIdName.Empty() == false)
        newResource = GetResource(defaultResourceName, ResourceNotFound::ErrorFallback);

      // If it wasn't in the file, just return the current value
      if (!valid && stream.mPatching)
        return resourcePtr;

      return newResource;
    }
  }
}

void ResourceManager::DestroyResources()
{
  ErrorIf(!ResourceIdMap.Empty(), "Resources that still have Ids are left!");
}

void AddGeometryFileFilters(ResourceManager* manager)
{
  Array<FileDialogFilter>& filters = manager->mOpenFileFilters;

  uint allMeshesIndex = filters.Size();
  filters.PushBack(FileDialogFilter("All Meshes", ""));
  filters.PushBack(FileDialogFilter("*.fbx"));
  filters.PushBack(FileDialogFilter("*.obj"));
  filters.PushBack(FileDialogFilter("*.dae"));
  filters.PushBack(FileDialogFilter("*.glb"));
  filters.PushBack(FileDialogFilter("*.gltf"));
  filters.PushBack(FileDialogFilter("*.3ds"));
  filters.PushBack(FileDialogFilter("*.blend"));
  filters.PushBack(FileDialogFilter("*.ase"));
  filters.PushBack(FileDialogFilter("*.ifc"));
  filters.PushBack(FileDialogFilter("*.xgl"));
  filters.PushBack(FileDialogFilter("*.zgl"));
  filters.PushBack(FileDialogFilter("*.ply"));
  filters.PushBack(FileDialogFilter("*.dxf"));
  filters.PushBack(FileDialogFilter("*.lwo"));
  filters.PushBack(FileDialogFilter("*.lws"));
  filters.PushBack(FileDialogFilter("*.lxo"));
  filters.PushBack(FileDialogFilter("*.stl"));
  filters.PushBack(FileDialogFilter("*.x"));
  filters.PushBack(FileDialogFilter("*.ac"));
  filters.PushBack(FileDialogFilter("*.ms3d"));
  filters.PushBack(FileDialogFilter("*.cob"));
  filters.PushBack(FileDialogFilter("*.scn"));
  filters.PushBack(FileDialogFilter("*.csm"));
  filters.PushBack(FileDialogFilter("*.xml"));
  filters.PushBack(FileDialogFilter("*.mdl"));
  filters.PushBack(FileDialogFilter("*.md2"));
  filters.PushBack(FileDialogFilter("*.md3"));
  filters.PushBack(FileDialogFilter("*.md5mesh"));
  filters.PushBack(FileDialogFilter("*.smd"));
  filters.PushBack(FileDialogFilter("*.ogex"));
  filters.PushBack(FileDialogFilter("*.b3d"));
  filters.PushBack(FileDialogFilter("*.q3o"));
  filters.PushBack(FileDialogFilter("*.q3s"));
  filters.PushBack(FileDialogFilter("*.nff"));
  filters.PushBack(FileDialogFilter("*.off"));
  filters.PushBack(FileDialogFilter("*.raw"));
  filters.PushBack(FileDialogFilter("*.ter"));
  filters.PushBack(FileDialogFilter("*.hmp"));
  filters.PushBack(FileDialogFilter("*.sib"));
  filters.PushBack(FileDialogFilter("*.amf"));
  filters.PushBack(FileDialogFilter("*.x3d"));
  filters.PushBack(FileDialogFilter("*.mmd"));

  // The first filter should contain the extensions of all other filters
  FileDialogFilter& allFilter = filters[allMeshesIndex];

  StringBuilder filterBuilder;

  // Skip the first filter (it's the one we're building)
  for (uint i = allMeshesIndex + 1; i < filters.Size(); ++i)
  {
    FileDialogFilter& currFilter = filters[i];
    filterBuilder.Append(currFilter.mFilter);

    if (i < filters.Size() - 1)
      filterBuilder.Append(";");
  }

  allFilter.mFilter = filterBuilder.ToString();
}

} // namespace Zero
