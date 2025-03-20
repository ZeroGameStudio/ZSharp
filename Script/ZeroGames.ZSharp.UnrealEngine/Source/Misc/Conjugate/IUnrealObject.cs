// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

// This exists for generic arguments which means the object has static property StaticClass.
// Because StaticClass can also exist in interface type and interface cannot "implement" IStaticClass.
// So if we use IStaticClass as generic arguments and give an interface type, compiler complains.
// So we use IUnrealObject as generic arguments and there is a convention that if you implement/inherit IUnrealObject
// then you must have a static property StaticClass in your class/interface.
public interface IUnrealObject;


