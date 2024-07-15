// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(1)]
public abstract class UnrealObjectBase : UnrealExportedObjectBase, IStaticClass
{

    public static UnrealClass SStaticClass => throw new NotSupportedException();
    
    public abstract UnrealClass __Class { get; }
    
    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name, int32 index)
    {
        string zcallName = $"up:/{_classPath}:{name}";
        return this.ZCall(zcallName, false, index, typeof(T));
    }

    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name) => ReadUnrealPropertyEx<T>(name, 0);

    public T? ReadUnrealProperty<T>(string name, int32 index) => (T?)ReadUnrealPropertyEx<T>(name, index)[3].Object;

    public T? ReadUnrealProperty<T>(string name) => ReadUnrealProperty<T>(name, 0);

    public DynamicZCallResult WriteUnrealProperty<T>(string name, int32 index, T value)
    {
        string zcallName = $"up:/{_classPath}:{name}";
        return this.ZCall(zcallName, true, index, value);
    }

    public DynamicZCallResult WriteUnrealProperty<T>(string name, T value) => WriteUnrealProperty(name, 0, value);

    public DynamicZCallResult CallUnrealFunctionEx<T>(string name, params object?[] parameters)
    {
        string zcallName = $"uf:/{_classPath}:{name}";
        object?[] parametersRet = new object?[parameters.Length + 1];
        parameters.CopyTo(parametersRet, 0);
        parametersRet[^1] = typeof(T);
        return this.ZCall(zcallName, parametersRet);
    }

    public T? CallUnrealFunction<T>(string name, params object?[] parameters) => (T?)CallUnrealFunctionEx<T>(name, parameters)[-1].Object;

    public DynamicZCallResult CallUnrealFunction(string name, params object?[] parameters)
    {
        string zcallName = $"uf:/{_classPath}:{name}";
        return this.ZCall(zcallName, parameters);
    }
    
    protected UnrealObjectBase(IntPtr unmanaged) : base(unmanaged){}

    private string _classPath
    {
        get
        {
            if (_cachedClassPath is null)
            {
                _cachedClassPath = __Class.__PathName.ToString();
            }

            return _cachedClassPath;
        }
    }

    private string? _cachedClassPath;
    
}


