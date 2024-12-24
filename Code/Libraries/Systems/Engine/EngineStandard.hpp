// MIT Licensed (see LICENSE.md).
#pragma once

#include "Common/CommonStandard.hpp"
#include "Platform/PlatformStandard.hpp"
#include "Serialization/SerializationStandard.hpp"
#include "Meta/MetaStandard.hpp"
#include "Support/SupportStandard.hpp"

namespace Zero
{
// Forward declarations
class Cog;
class Event;
class Component;
class Serializer;
class Level;
class Space;
class GameSession;
class GameWidget;
class Transform;
class ContentLibrary;
class ResourcePackage;
class ResourceLibrary;
class ProjectSettings;
class Hierarchy;
class KeyboardEvent;
class UpdateEvent;
class ActionSpace;
class SavingEvent;
class DocumentResource;

// Engine library
class ZeroNoImportExport EngineLibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(EngineLibrary, "ZeroEngine");

  static bool Initialize();
  static void Shutdown();

private:
};

} // namespace Zero

#include "Rectangle.hpp"
#include "Resource/Resource.hpp"
#include "EngineBindingExtensions.hpp"
#include "EngineObject.hpp"
#include "EngineContainers.hpp"
#include "EngineMath.hpp"
#include "World/CogId.hpp"
#include "World/HierarchyRange.hpp"
#include "World/Cog.hpp"
#include "World/Component.hpp"
#include "World/ComponentMeta.hpp"
#include "World/CogMetaComposition.hpp"
#include "World/CogMeta.hpp"
#include "World/Space.hpp"
#include "DocumentResource.hpp"
#include "ZilchResource.hpp"
#include "Resource/ResourceLibrary.hpp"
#include "JobSystem.hpp"
#include "EngineEvents.hpp"
#include "System.hpp"
#include "Time.hpp"
#include "Engine.hpp"
#include "ThreadDispatch.hpp"
#include "World/Game.hpp"
#include "Factory.hpp"
#include "World/ArchetypeRebuilder.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Resource/ResourcePropertyOperations.hpp"
#include "ErrorContext.hpp"
#include "World/Tracker.hpp"
#include "World/Hierarchy.hpp"
#include "World/TransformSupport.hpp"
#include "World/Transform.hpp"
#include "Action/Action.hpp"
#include "Action/ActionSystem.hpp"
#include "Action/ActionEase.hpp"
#include "Action/MetaAction.hpp"
#include "Action/BasicActions.hpp"
#include "Action/ActionGenerator.hpp"
#include "EditorSupport.hpp"
#include "World/CogSerialization.hpp"
#include "World/ObjectLoader.hpp"
#include "World/ObjectSaver.hpp"
#include "World/CogInitializer.hpp"
#include "World/CogPath.hpp"
#include "World/ObjectLink.hpp"
#include "Platform/Input/Keyboard.hpp"
#include "Environment.hpp"
#include "SystemObjectManager.hpp"
#include "Platform/OsWindow.hpp"
#include "Platform/OsShell.hpp"
#include "Resource/ResourceManager.hpp"
#include "World/Archetype.hpp"
#include "Platform/Input/Mouse.hpp"
#include "World/Level.hpp"
#include "Operation.hpp"
#include "Resource/ResourceListOperation.hpp"
#include "MetaOperations.hpp"
#include "World/CogRestoreState.hpp"
#include "World/CogOperations.hpp"
#include "Animation/AnimationNode.hpp"
#include "Animation/PropertyTrack.hpp"
#include "Animation/AnimationGraph.hpp"
#include "Animation/AnimationGraphEvents.hpp"
#include "Animation/Animation.hpp"
#include "World/CogSelection.hpp"
#include "Configuration.hpp"
#include "LauncherConfiguration.hpp"
#include "DebugDraw.hpp"
#include "Scripting/ScriptEvents.hpp"
#include "TextResource.hpp"
#include "EditorSettings.hpp"
#include "Documentation.hpp"
#include "ShortcutsDoc.hpp"
#include "World/ObjectStore.hpp"
#include "ResourceTable.hpp"
#include "SampleCurve.hpp"
#include "Noise.hpp"
#include "HeightMap.hpp"
#include "HeightMapSource.hpp"
#include "Gradient.hpp"
#include "ColorGradient.hpp"
#include "Area.hpp"
#include "Tweakables.hpp"
#include "SceneGraph.hpp"
#include "ProxyObject.hpp"
#include "Project.hpp"
#include "RaycastProvider.hpp"
#include "SimpleResourceFactory.hpp"
#include "Platform/Input/GamePadSystem.hpp"
#include "Platform/Input/JoystickSystem.hpp"
#include "EventDirectoryWatcher.hpp"
#include "World/CogRange.hpp"
#include "World/CogHelpers.hpp"
#include "World/ComponentHierarchy.hpp"
#include "DataSource.hpp"
#include "EngineLibraryExtensions.hpp"
#include "Scripting/ZilchManager.hpp"
#include "Spline.hpp"
#include "HierarchySpline.hpp"
#include "AsyncProcess.hpp"
#include "CopyOnWrite.hpp"

// Co-dependent libraries
#include "Content/ContentStandard.hpp"
