// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public static class UObjectGlobals
{

	public static unsafe UnrealObject? FindObject(UnrealClass cls, UnrealObject? outer, string path, bool exactClass)
	{
		IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
		const int32 numSlots = 5;
		ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
		{
			ZCallBufferSlot.FromConjugate(cls),
			ZCallBufferSlot.FromConjugate(outer),
			ZCallBufferSlot.FromConjugate(new UnrealString(path)),
			ZCallBufferSlot.FromBool(exactClass),
			ZCallBufferSlot.FromConjugate(),
		};
		ZCallBuffer buffer = new(slots, numSlots);
		ZCallHandle handle = alc.GetZCallHandle("ex://UObjectGlobals.FindObject");
		alc.ZCall(handle, &buffer);
		
		return slots[4].ReadConjugate<UnrealObject>();
	}
	
	public static T? FindObject<T>(string path) where T : UnrealObject => FindObject<T>(null, path, false);

	public static T? FindObject<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject => (T?)FindObject((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!, outer, path, exactClass);

	public static T FindObjectChecked<T>(string path) where T : UnrealObject => FindObjectChecked<T>(null, path, false);
	
	public static T FindObjectChecked<T>(UnrealObject? outer, string path, bool exactClass) where T : UnrealObject
	{
		T? res = FindObject<T>(outer, path, exactClass);
		if (res is null)
		{
			throw new InvalidOperationException();
		}

		return res;
	}
	
	public static unsafe UnrealObject? LowLevelFindObject(string path)
	{
		IMasterAssemblyLoadContext alc = IMasterAssemblyLoadContext.Get()!;
		const int32 numSlots = 2;
		ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
		{
			ZCallBufferSlot.FromConjugate(new UnrealString(path)),
			ZCallBufferSlot.FromConjugate(),
		};
		ZCallBuffer buffer = new(slots, numSlots);
		ZCallHandle handle = alc.GetZCallHandle("ex://UObjectGlobals.LowLevelFindObject");
		alc.ZCall(handle, &buffer);
		
		return slots[1].ReadConjugate<UnrealObject>();
	}

	public static T? LowLevelFindObject<T>(string path) where T : UnrealObject => LowLevelFindObject(path) as T;

}