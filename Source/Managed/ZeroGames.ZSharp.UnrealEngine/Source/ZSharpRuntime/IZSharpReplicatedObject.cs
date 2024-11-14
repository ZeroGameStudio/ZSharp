// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

partial interface IZSharpReplicatedObject
{

	void MarkPropertyDirty(UnrealName propertyName) => NetPushModelHelpers.MarkPropertyDirty((UnrealObject)this, propertyName);
	void MarkPropertyDirty(string propertyName) => MarkPropertyDirty(new UnrealName(propertyName));

}


