// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

partial interface IZSharpReplicatedObject
{
	void MarkPropertyDirty(FName propertyName) => UNetPushModelHelpers.MarkPropertyDirty((UObject)this, propertyName);
}

public static class ZSharpReplicatedObjectExtensions
{
	extension(IZSharpReplicatedObject @this)
	{
		public void MarkPropertyDirty(FName propertyName) => @this.MarkPropertyDirty(propertyName);
		public void MarkPropertyDirty(string propertyName) => @this.MarkPropertyDirty(new FName(propertyName));
	}
}


