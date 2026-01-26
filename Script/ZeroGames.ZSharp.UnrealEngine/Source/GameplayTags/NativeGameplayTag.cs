// Copyright Zero Games. All Rights Reserved.

#if UE_MODULE_GAMEPLAYTAGS

namespace ZeroGames.ZSharp.UnrealEngine.GameplayTags;

public class NativeGameplayTag : IDisposable
{
	
	public NativeGameplayTag(string tagName, string comment, string pluginName, string moduleName)
	{
		if (string.IsNullOrWhiteSpace(tagName))
		{
			throw new ArgumentOutOfRangeException(tagName);
		}
		
		TagName = tagName;
		Comment = comment;
		PluginName = pluginName;
		ModuleName = moduleName;

		unsafe
		{
			fixed (char* tagNameBuffer = tagName)
			fixed (char* commentBuffer = comment)
			fixed (char* pluginNameBuffer = pluginName)
			fixed (char* moduleNameBuffer = moduleName)
			{
				_unmanaged = GameplayTag_Interop.TryRegisterNativeTag(tagNameBuffer, commentBuffer, pluginNameBuffer, moduleNameBuffer);
			}
		}

		Tag = GameplayTag.Request(TagName);
		
		MasterAlcCache.Instance.RegisterAutoDisposedResource(this);
	}
	
	public NativeGameplayTag(string tagName, string comment, string moduleName) : this(tagName, comment, string.Empty, moduleName){}
	public NativeGameplayTag(string tagName, string moduleName) : this(tagName, string.Empty, string.Empty, moduleName){}
	
	~NativeGameplayTag()
	{
		InternalDispose();
	}

	public static implicit operator GameplayTag(NativeGameplayTag value) => value.Tag;
	
	public void Dispose()
	{
		InternalDispose();
		
		GC.SuppressFinalize(this);
	}
	
	private unsafe void InternalDispose()
	{
		if (_disposed)
		{
			return;
		}

		_disposed = true;

		if (_unmanaged != IntPtr.Zero)
		{
			ensureAlways(GameplayTag_Interop.TryUnregisterNativeTag(_unmanaged) > 0);
			_unmanaged = IntPtr.Zero;
		}
	}
	
	public string TagName { get; }
	public string Comment { get; }
	public string PluginName { get; }
	public string ModuleName { get; }

	public GameplayTag Tag { get; }

	private IntPtr _unmanaged;
	private bool _disposed;

}

#endif


