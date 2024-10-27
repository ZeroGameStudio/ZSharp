// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class ScriptInterface<T> : ScriptInterfaceBase, IConjugate<ScriptInterface<T>> where T : IUnrealInterface
{
	
	public static ScriptInterface<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public ScriptInterface() : base(typeof(T)){}
	public ScriptInterface(IntPtr unmanaged) : base(typeof(T), unmanaged){}

	public ScriptInterface(UnrealObject? obj) : this()
	{
		if (obj is null)
		{
			return;
		}

		if (!ensure(obj.Implements<T>()))
		{
			return;
		}

		_Object = obj;
	}

	public UnrealObject? Object
	{
		get => _Object;
		set => _Object = value;
	}
	
}


