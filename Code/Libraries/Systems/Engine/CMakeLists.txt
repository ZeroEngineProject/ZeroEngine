add_library(Engine)

zero_setup_library(Engine ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(Engine ${CMAKE_CURRENT_LIST_DIR})

set(Engine_Action_Sources
    ${CMAKE_CURRENT_LIST_DIR}/Action/Action.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/Action.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ActionEase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ActionEase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ActionGenerator.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ActionSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ActionSystem.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/BasicActions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/BasicActions.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/MetaAction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/MetaAction.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ZilchAction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Action/ZilchAction.hpp
)

set(Engine_Animation_Sources
    ${CMAKE_CURRENT_LIST_DIR}/Animation/Animation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/Animation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationGraph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationGraph.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationGraphEvents.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationGraphEvents.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationNode.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/AnimationNode.inl
    ${CMAKE_CURRENT_LIST_DIR}/Animation/PropertyTrack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/PropertyTrack.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Animation/PropertyTrack.inl
)

set(Engine_Platform_Sources
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/GamePadSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/GamePadSystem.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/JoystickSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/JoystickSystem.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/Keyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/Keyboard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/Mouse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/Input/Mouse.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/OsShell.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/OsShell.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/OsWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Platform/OsWindow.hpp
)

set(Engine_Resource_Sources
    ${CMAKE_CURRENT_LIST_DIR}/Resource/Resource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/Resource.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourceLibrary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourceLibrary.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourceManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourceManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourcePropertyOperations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourcePropertyOperations.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourceSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Resource/ResourceSystem.hpp
)

set(Engine_Scripting_Sources
    ${CMAKE_CURRENT_LIST_DIR}/Scripting/ScriptEvents.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Scripting/ScriptEvents.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Scripting/ZilchManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Scripting/ZilchManager.hpp
)

target_sources(Engine
  PRIVATE
    ${Engine_Action_Sources}
    ${Engine_Animation_Sources}
    ${Engine_Platform_Sources}
    ${Engine_Resource_Sources}
    ${Engine_Scripting_Sources}
    ${CMAKE_CURRENT_LIST_DIR}/Archetype.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Archetype.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ArchetypeRebuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ArchetypeRebuilder.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Area.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Area.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AsyncProcess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AsyncProcess.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Cog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Cog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogHelpers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogHelpers.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogId.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogId.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogInitializer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogInitializer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogMeta.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogMeta.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogMetaComposition.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogMetaComposition.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogOperations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogOperations.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogPath.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogPath.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogRange.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogRange.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogRestoreState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogRestoreState.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogSelection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogSelection.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CogSerialization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CogSerialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ColorGradient.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ColorGradient.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Component.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Component.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ComponentHierarchy.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ComponentMeta.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ComponentMeta.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Configuration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Configuration.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CopyOnWrite.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DataSource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DataSource.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DebugDraw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DebugDraw.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DocumentResource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DocumentResource.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Documentation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Documentation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EditorSettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EditorSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EditorSupport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EditorSupport.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Engine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Engine.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineBindingExtensions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineBindingExtensions.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineContainers.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineEvents.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineEvents.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineLibraryExtensions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineLibraryExtensions.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineMath.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineObject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineObject.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EngineStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Environment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Environment.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ErrorContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ErrorContext.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EventDirectoryWatcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EventDirectoryWatcher.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Factory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Factory.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Game.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Game.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Gradient.hpp
    ${CMAKE_CURRENT_LIST_DIR}/HeightMap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HeightMap.hpp
    ${CMAKE_CURRENT_LIST_DIR}/HeightMapSource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HeightMapSource.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Hierarchy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Hierarchy.hpp
    ${CMAKE_CURRENT_LIST_DIR}/HierarchyRange.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HierarchyRange.hpp
    ${CMAKE_CURRENT_LIST_DIR}/HierarchySpline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HierarchySpline.hpp
    ${CMAKE_CURRENT_LIST_DIR}/JobSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JobSystem.hpp
    ${CMAKE_CURRENT_LIST_DIR}/LauncherConfiguration.cpp
    ${CMAKE_CURRENT_LIST_DIR}/LauncherConfiguration.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Level.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Level.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MetaOperations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MetaOperations.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Noise.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Noise.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectLink.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectLink.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectLoader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectLoader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectSaver.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectSaver.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectStore.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectStore.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Operation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Operation.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Project.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Project.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ProxyObject.hpp
    ${CMAKE_CURRENT_LIST_DIR}/RaycastProvider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RaycastProvider.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Rectangle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Rectangle.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ResourceTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ResourceTable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SampleCurve.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SampleCurve.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SceneGraph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SceneGraph.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ShortcutsDoc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ShortcutsDoc.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SimpleResourceFactory.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Space.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Space.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Spline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Spline.hpp
    ${CMAKE_CURRENT_LIST_DIR}/System.cpp
    ${CMAKE_CURRENT_LIST_DIR}/System.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SystemObjectManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SystemObjectManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TextResource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TextResource.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ThreadDispatch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ThreadDispatch.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Time.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Time.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Tracker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Tracker.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Transform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Transform.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TransformSupport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TransformSupport.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Tweakables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Tweakables.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Tweakables.inl
    ${CMAKE_CURRENT_LIST_DIR}/ZilchResource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ZilchResource.hpp
)

target_link_libraries(Engine
  PUBLIC
    Common
    Geometry
    Meta
    Platform
    Serialization
    SpatialPartition
    Zilch
)

target_include_directories(Engine
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../Systems/
    ${CMAKE_CURRENT_LIST_DIR}/../../Renderer/
)
