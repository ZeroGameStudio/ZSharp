// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

partial interface IZSharpReplicatedObject
{
	void MarkPropertyDirty(FName propertyName) => UNetPushModelHelpers.MarkPropertyDirty((UObject)this, propertyName);
}

public static class ZSharpReplicatedObjectExtensions
{
	public static void MarkPropertyDirty(this IZSharpReplicatedObject @this, FName propertyName) => @this.MarkPropertyDirty(propertyName);
	public static void MarkPropertyDirty(this IZSharpReplicatedObject @this, string propertyName) => @this.MarkPropertyDirty(new FName(propertyName));
}


