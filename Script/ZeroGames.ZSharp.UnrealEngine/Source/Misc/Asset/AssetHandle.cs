// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public struct AssetHandle(string? path)
{
	public AssetHandle(UObject? asset) : this(asset?.GetPathName()){}
	
	public string Path
	{
		get => field ?? string.Empty;
	} = path;
	
	public UObject? Asset
	{
		get
		{
			if (string.IsNullOrEmpty(Path))
			{
				return null;
			}
			
			if (field is not { __IsValid: true })
			{
				field = LoadObject(UObject.StaticClass, null, Path);
			}
			
			return field?.SelfIfValid;
		}
	}
}

public struct AssetHandle<T>(string? path) where T : UObject
{
	public AssetHandle(T? asset) : this(asset?.GetPathName()){}

	public string Path
	{
		get => field ?? string.Empty;
	} = path;
	
	public T? Asset
	{
		get
		{
			if (string.IsNullOrEmpty(Path))
			{
				return null;
			}
			
			if (field is not { __IsValid: true })
			{
				field = LoadObject<T>(null, Path);
			}
			
			return field?.SelfIfValid;
		}
	}
}


