// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.Loader;
using System.Security;

namespace ZeroGames.ZSharp.Core;

public unsafe interface IMasterAssemblyLoadContext : IZSharpAssemblyLoadContext
{
	public static IMasterAssemblyLoadContext? Instance
	{
		get
		{
			StackFrame stack = new(1);
			MethodBase? method = stack.GetMethod();
			check(method is not null);
			
			Type? type = method.DeclaringType;
			check(type is not null);

			MasterAssemblyLoadContext? result = MasterAssemblyLoadContext.Instance;
			AssemblyLoadContext? callerAlc = AssemblyLoadContext.GetLoadContext(type.Assembly);
			if (callerAlc != result && callerAlc != AssemblyLoadContext.Default)
			{
				throw new SecurityException($"Code from ALC [{callerAlc?.Name ?? "Unknown"}] has no permission to access Master ALC.");
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
	void ReleaseConjugate(IConjugate conjugate);
	void PushPendingDisposeConjugate(IConjugate conjugate);
}


