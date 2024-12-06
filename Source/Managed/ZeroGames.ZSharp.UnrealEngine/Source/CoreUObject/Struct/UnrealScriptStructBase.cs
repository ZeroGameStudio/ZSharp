// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(2)]
public abstract class UnrealScriptStructBase : UnrealExportedObjectBase
{

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
	    return this.ZCall(MasterAlcCache.Instance, zcallName, true, index, value);
    }

    public DynamicZCallResult WriteUnrealProperty<T>(string name, T value) => WriteUnrealProperty(name, 0, value);
    
    protected UnrealScriptStructBase(){}
    protected UnrealScriptStructBase(IntPtr unmanaged) : base(unmanaged){}
    
    protected unsafe void Copy(UnrealScriptStructBase other) => UnrealScriptStruct_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
    protected unsafe bool Identical(UnrealScriptStructBase other) => UnrealScriptStruct_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;

}


