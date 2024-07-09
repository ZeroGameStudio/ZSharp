// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class UnrealExportedObjectBase : ExportedObjectBase, IUnrealFieldPath
{
	
	public static string SUnrealFieldPath => throw new NotSupportedException();

	public UnrealExportedObjectBase(){}
	protected UnrealExportedObjectBase(IntPtr unmanaged) : base(unmanaged){}

	public DynamicZCallResult ReadUnrealPropertyEx<T>(string name, int32 index)
	{
		string zcallName = $"up:/{UnrealFieldPath}:{name}";
		return this.ZCall(zcallName, false, index, typeof(T));
	}

	public DynamicZCallResult ReadUnrealPropertyEx<T>(string name) => ReadUnrealPropertyEx<T>(name, 0);

	public T? ReadUnrealProperty<T>(string name, int32 index) => (T?)ReadUnrealPropertyEx<T>(name, index)[3].ReadObject();

	public T? ReadUnrealProperty<T>(string name) => ReadUnrealProperty<T>(name, 0);

	public DynamicZCallResult WriteUnrealProperty<T>(string name, int32 index, T value)
	{
		string zcallName = $"up:/{UnrealFieldPath}:{name}";
		return this.ZCall(zcallName, true, index, value);
	}

	public DynamicZCallResult WriteUnrealProperty<T>(string name, T value) => WriteUnrealProperty(name, 0, value);

	public DynamicZCallResult CallUnrealFunctionEx<T>(string name, params object?[] parameters)
	{
		string zcallName = $"uf:/{UnrealFieldPath}:{name}";
		object?[] parametersRet = new object?[parameters.Length + 1];
		parameters.CopyTo(parametersRet, 0);
		parametersRet[^1] = typeof(T);
		return this.ZCall(zcallName, parametersRet);
	}

	public T? CallUnrealFunction<T>(string name, params object?[] parameters) => (T?)CallUnrealFunctionEx<T>(name, parameters)[-1].ReadObject();

	public DynamicZCallResult CallUnrealFunction(string name, params object?[] parameters)
	{
		string zcallName = $"uf:/{UnrealFieldPath}:{name}";
		return this.ZCall(zcallName, parameters);
	}
	
	public abstract string UnrealFieldPath { get; }
	
}


