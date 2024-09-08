// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	enum class EZLoadAssemblyErrorCode : int32
	{
		Succeed = 0,
		UnknownError = -1,

		AlcUnavailable = 101,
		
		FileNotFound = 201,
		BadImage = 202,

		InvalidDllMainSignature = 301,
		UnhandledDllMainException = 302,
	};

	enum class EZInvokeMethodErrorCode : int32
	{
		Succeed = 0,
		UnknownError = -1,

		AlcUnavailable = 101,

		AssemblyNotFound = 201,
		AmbiguousAssemblyName = 202,

		TypeNotFound = 301,

		MethodNotFound = 401,
		AmbiguousMethodName = 402,
		InvalidMethodSignature = 403,
		UnhandledException = 404,
	};

	enum class EZCallErrorCode : int32
	{
		Succeed = 0,
		UnknownError = -1,

		AlcUnavailable = 101,

		InvalidHandle = 201,
		DispatcherNotFound = 202,

		DispatcherError = 301,
		BufferError = 302,
		ExternalError = 303,
	};
}


