// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintEventSpecifierBase : EventSpecifierBase;

public class BlueprintImplementableEventAttribute : BlueprintEventSpecifierBase;
public class BlueprintNativeEventAttribute : BlueprintEventSpecifierBase;


