// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.Loader;
using System.Security;
using System.Threading;

namespace ZeroGames.ZSharp.Core;

public readonly struct RuntimeTypeLocator
{
	public RuntimeTypeLocator(string assemblyName, string typeName)
	{
		AssemblyName = assemblyName;
		TypeName = typeName;
	}
	public unsafe RuntimeTypeLocator(InteropRuntimeTypeLocator* locator)
	{
		AssemblyName = new(locator->AssemblyName);
		TypeName = new(locator->TypeName);
		TypeParameters = new RuntimeTypeLocator[locator->NumTypeParameters];
		for (int32 i = 0; i < TypeParameters.Length; ++i)
		{
			TypeParameters[i] = new(locator->TypeParameters + i);
		}
	}
	public string AssemblyName { get; }
	public string TypeName { get; }
	public RuntimeTypeLocator[]? TypeParameters { get; }
}

public unsafe interface IMasterAssemblyLoadContext : IZSharpAssemblyLoadContext
{

	public static IMasterAssemblyLoadContext? Instance
	{
		get
		{
			StackFrame stack = new(1);
			MethodBase? method = stack.GetMethod();
			if (method is null)
			{
				throw new SecurityException("Code has no permission to access Master ALC.");
			}

			Type? type = method!.DeclaringType;
			if (type is null)
			{
				throw new SecurityException("Code has no permission to access Master ALC.");
			}

			MasterAssemblyLoadContext? result = MasterAssemblyLoadContext.Instance;
			AssemblyLoadContext? callerAlc = AssemblyLoadContext.GetLoadContext(type!.Assembly);
			if (callerAlc != result && callerAlc != AssemblyLoadContext.Default)
			{
				throw new SecurityException("Code has no permission to access Master ALC.");
			}
			
			return result;
		}
	}
	
	Type? GetType(ref readonly RuntimeTypeLocator locator);

	ZCallHandle RegisterZCall(IZCallDispatcher dispatcher);
	void RegisterZCallResolver(IZCallResolver resolver, uint64 priority);
	
	EZCallErrorCode ZCall(ZCallHandle handle, ZCallBuffer* buffer);
	ZCallHandle GetZCallHandle(string name);
	IntPtr BuildConjugate(IConjugate managed, IntPtr userdata);
	void ReleaseConjugate(IntPtr unmanaged);
	void PushPendingDisposeConjugate(IConjugate conjugate);
}


