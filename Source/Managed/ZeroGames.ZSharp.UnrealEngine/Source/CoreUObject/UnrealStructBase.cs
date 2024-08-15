// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(2)]
public abstract class UnrealStructBase : UnrealExportedObjectBase, IStaticStruct
{
	
    public UnrealStructBase(){}
    protected UnrealStructBase(IntPtr unmanaged) : base(unmanaged){}

    public static UnrealScriptStruct SStaticStruct => throw new NotSupportedException();
    
    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name, int32 index)
    {
	    string zcallName = $"up:/{UnrealFieldPath}:{name}";
	    return this.ZCall(zcallName, false, index, typeof(T));
    }

    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name) => ReadUnrealPropertyEx<T>(name, 0);

    public T? ReadUnrealProperty<T>(string name, int32 index) => (T?)ReadUnrealPropertyEx<T>(name, index)[3].Object;

    public T? ReadUnrealProperty<T>(string name) => ReadUnrealProperty<T>(name, 0);

    public DynamicZCallResult WriteUnrealProperty<T>(string name, int32 index, T value)
    {
	    string zcallName = $"up:/{UnrealFieldPath}:{name}";
	    return this.ZCall(zcallName, true, index, value);
    }

    public DynamicZCallResult WriteUnrealProperty<T>(string name, T value) => WriteUnrealProperty(name, 0, value);
    
}


