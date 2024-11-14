// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

partial interface IZSharpReplicatedObject
{

	void MarkPropertyDirty(string propertyName) => NetPushModelHelpers.MarkPropertyDirty((UnrealObject)this, propertyName);

}


