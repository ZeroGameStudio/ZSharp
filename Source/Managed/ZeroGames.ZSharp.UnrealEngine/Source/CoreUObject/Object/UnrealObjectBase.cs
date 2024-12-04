// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(1)]
public abstract class UnrealObjectBase : UnrealExportedObjectBase
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

    public DynamicZCallResult CallUnrealFunctionEx<T>(string name, params ReadOnlySpan<object?> parameters)
    {
        string zcallName = $"uf:/{UnrealFieldPath}:{name}";
        return this.ZCall(MasterAlcCache.Instance, zcallName, [ ..parameters, typeof(T) ]);
    }

    public T? CallUnrealFunction<T>(string name, params ReadOnlySpan<object?> parameters) => (T?)CallUnrealFunctionEx<T>(name, parameters)[-1].Object;

    public DynamicZCallResult CallUnrealFunction(string name, params ReadOnlySpan<object?> parameters)
    {
        string zcallName = $"uf:/{UnrealFieldPath}:{name}";
        return this.ZCall(MasterAlcCache.Instance, zcallName, parameters);
    }

    protected UnrealObjectBase(IntPtr unmanaged) : base(unmanaged){}

}


