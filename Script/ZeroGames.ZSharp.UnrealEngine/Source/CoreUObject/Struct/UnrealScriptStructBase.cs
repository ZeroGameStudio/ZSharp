// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(2)]
public abstract class UnrealScriptStructBase : UnrealConjugateBase, IUnrealFieldPath
{

	public UnrealScriptStruct GetScriptStruct()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetScriptStruct();
	}
	
	public bool IsA(UnrealScriptStruct scriptStruct)
	{
		MasterAlcCache.GuardInvariant();
		return InternalIsA(scriptStruct);
	}
	public bool IsA(Type scriptStruct) => IsA(UnrealScriptStruct.FromType(scriptStruct));
	public bool IsA<T>() where T : IStaticStruct => IsA(UnrealScriptStruct.FromType<T>());

	public DynamicZCallResult ReadUnrealPropertyEx<T>(string name, int32 index)
    {
	    string zcallName = $"up:/{UnrealFieldPath}:{name}";
	    return this.ZCall(MasterAlcCache.Instance, zcallName, false, index, typeof(T));
    }

    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name) => ReadUnrealPropertyEx<T>(name, 0);

    public T ReadUnrealProperty<T>(string name, int32 index) => (T)ReadUnrealPropertyEx<T>(name, index)[3].Object!;

    public T ReadUnrealProperty<T>(string name) => ReadUnrealProperty<T>(name, 0);

    public DynamicZCallResult WriteUnrealProperty<T>(string name, int32 index, T value)
    {
	    string zcallName = $"up:/{UnrealFieldPath}:{name}";
	    return this.ZCall(MasterAlcCache.Instance, zcallName, [ true, index, value ]);
    }

    public DynamicZCallResult WriteUnrealProperty<T>(string name, T value) => WriteUnrealProperty(name, 0, value);
    
    public abstract string UnrealFieldPath { get; }
    
    protected UnrealScriptStructBase(){}
    protected UnrealScriptStructBase(IntPtr unmanaged) : base(unmanaged){}
    
    protected unsafe void Copy(UnrealScriptStructBase other)
	    => UnrealScriptStructBase_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
    
    protected unsafe bool Identical(UnrealScriptStructBase other)
	    => UnrealScriptStructBase_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
    
    private unsafe UnrealScriptStruct InternalGetScriptStruct()
	    => UnrealScriptStructBase_Interop.GetScriptStruct(ConjugateHandle.FromConjugate(this)).GetTargetChecked<UnrealScriptStruct>();
    
    private unsafe bool InternalIsA(UnrealScriptStruct scriptStruct)
	    => UnrealScriptStructBase_Interop.IsA(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(scriptStruct)) > 0;

}


