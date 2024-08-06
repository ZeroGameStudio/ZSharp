// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public enum ELoadAssemblyErrorCode : int32
{
	Succeed = 0,
	UnknownError = -1,

	AlcUnavailable = 101,

	BadImage = 201,

	InvalidDllMainSignature = 301,
	UnhandledDllMainException = 302,
}

public enum ECallMethodErrorCode : int32
{
	Succeed = 0,
	UnknownError = -1,

	AlcUnavailable = 101,

	AssemblyNotFound = 201,
	AmbiguousAssemblyName = 202,

	TypeNotFound = 301,

	MethodNotFound = 401,
	AmbiguousMethodName = 402,
	AmbiguousMethodSignature = 403,
	UnhandledException = 404,
}

public interface IZSharpAssemblyLoadContext : IGCHandle
{
	
}


