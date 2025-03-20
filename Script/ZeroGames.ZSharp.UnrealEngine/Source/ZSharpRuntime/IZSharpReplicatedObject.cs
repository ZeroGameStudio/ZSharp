// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

partial interface IZSharpReplicatedObject
{
	void MarkPropertyDirty(UnrealName propertyName) => NetPushModelHelpers.MarkPropertyDirty((UnrealObject)this, propertyName);
}

public static class ZSharpReplicatedObjectExtensions
{
	public static void MarkPropertyDirty(this IZSharpReplicatedObject @this, UnrealName propertyName) => @this.MarkPropertyDirty(propertyName);
	public static void MarkPropertyDirty(this IZSharpReplicatedObject @this, string propertyName) => @this.MarkPropertyDirty(new UnrealName(propertyName));
}


