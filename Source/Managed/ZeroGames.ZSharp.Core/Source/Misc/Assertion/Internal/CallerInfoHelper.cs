// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal static class CallerInfoHelper
{
	
	public static void Inject(ref int32 column)
	{
		GCHandle? dummy = default;
		Inject(ref dummy, ref column);
	}

	public static void Inject(ref GCHandle? context, ref int32 column)
	{
		StackFrame? frame = null;
		if (context == null)
		{
			frame ??= new(2, true);
			Assembly assembly = frame.GetMethod()!.DeclaringType!.Assembly;
			AssemblyLoadContext alc = AssemblyLoadContext.GetLoadContext(assembly)!;
			context = GCHandle.Alloc(alc, GCHandleType.WeakTrackResurrection);
		}
		
		if (column == default)
		{
			frame ??= new(2, true);
			column = frame.GetFileColumnNumber();
		}
	}
	
}


