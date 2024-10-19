// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal static class CallerInfoHelper
{
	
	public static void Inject(ref int32 column)
	{
		if (column == default)
		{
			column = new StackFrame(2, true).GetFileColumnNumber();
		}
	}

	public static void Inject([NotNull] ref AssemblyLoadContext? context, ref int32 column)
	{
		StackFrame? frame = null;
		if (context == null)
		{
			frame ??= new(2, true);
			Assembly assembly = frame.GetMethod()!.DeclaringType!.Assembly;
			context = AssemblyLoadContext.GetLoadContext(assembly)!;
		}
		
		if (column == default)
		{
			frame ??= new(2, true);
			column = frame.GetFileColumnNumber();
		}
	}
	
}


