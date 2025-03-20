// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.Contracts;
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
			if (method is null)
			{
				throw new InvalidOperationException();
			}
			
			Type? type = method.DeclaringType;
			if (type is null)
			{
				throw new InvalidOperationException();
			}

			MasterAssemblyLoadContext? result = MasterAssemblyLoadContext.Instance;
			AssemblyLoadContext? callerAlc = AssemblyLoadContext.GetLoadContext(type.Assembly);
			if (callerAlc != result && callerAlc != AssemblyLoadContext.Default)
			{
				throw new SecurityException($"Code from ALC [{callerAlc?.Name ?? "Unknown"}] has no permission to access Master ALC.");
			}
			
			return result;
		}
	}

	public static MasterAlcUnloadingRegistration RegisterUnloading(Action callback, int64 priority)
		=> MasterAssemblyLoadContext.RegisterUnloading(callback, priority);
	
	Type? GetType(RuntimeTypeUri uri);

	ZCallHandle RegisterZCall(IZCallDispatcher dispatcher);
	void RegisterZCallResolver(IZCallResolver resolver, int64 priority);
	
	EZCallErrorCode ZCall(ZCallHandle handle, ZCallBuffer* buffer);
	ZCallHandle GetZCallHandle(string name);
	IntPtr BuildConjugate(IConjugate managed, IntPtr userdata);
	void ReleaseConjugate(IConjugate conjugate);
	void PushPendingDisposeConjugate(IConjugate conjugate);
}


