// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public interface IAssemblyResolver
{
	ELoadAssemblyErrorCode Resolve(AssemblyLoadContext alc, string name, out Assembly? assembly);
}


