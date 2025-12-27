// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public struct AssetHandle(string? path)
{
	public AssetHandle(UObject? asset) : this(asset?.GetPathName()){}
	
	public UObject? Asset
	{
		get
		{
			if (string.IsNullOrEmpty(_path))
			{
				return null;
			}
			
			if (field is not { __IsValid: true })
			{
				field = LoadObject(UObject.StaticClass, null, _path);
			}
			
			return field?.SelfIfValid;
		}
	}
	
	private readonly string? _path = path;
}

public struct AssetHandle<T>(string? path) where T : UObject
{
	public AssetHandle(T? asset) : this(asset?.GetPathName()){}
	
	public T? Asset
	{
		get
		{
			if (string.IsNullOrEmpty(_path))
			{
				return null;
			}
			
			if (field is not { __IsValid: true })
			{
				field = LoadObject<T>(null, _path);
			}
			
			return field?.SelfIfValid;
		}
	}
	
	private readonly string? _path = path;
}


