// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

public interface IAssemblyResolver
{
	ELoadAssemblyErrorCode Resolve(string name, out Assembly? assembly);
}


