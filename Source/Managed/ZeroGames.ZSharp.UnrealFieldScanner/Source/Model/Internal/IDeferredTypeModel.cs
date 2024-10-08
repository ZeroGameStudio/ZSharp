// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal interface IDeferredTypeModel
{

	public void BaseInitialize();
	public void FullyInitialize();
	
	public bool IsBaseInitialized { get; }
	public bool IsFullyInitialized { get; }
	
}


