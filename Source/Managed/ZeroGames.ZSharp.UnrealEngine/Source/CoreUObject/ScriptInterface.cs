// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class ScriptInterface<T> : ScriptInterfaceBase, IConjugate<ScriptInterface<T>> where T : IUnrealInterface
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

		if (!obj.Implements<T>())
		{
			throw new NotSupportedException();
		}

		Object = obj;
	}
	
}


