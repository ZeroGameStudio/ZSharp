// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
	 * See ZeroGames.ZSharp.Core.EZLoadAssemblyErrorCode.
	 */
	enum class EZLoadAssemblyErrorCode : int32
	{
		Succeed = 0,
		UnknownError = -1,

		AlcUnavailable = 101,
		
		FileNotFound = 201,
		BadImage = 202,

		DllMainNotFound = 301,
		InvalidDllMainSignature = 302,
		UnhandledDllMainException = 303,
	};

	/**
 	 * See ZeroGames.ZSharp.Core.EZInvokeMethodErrorCode.
 	 */
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

	/**
 	 * See ZeroGames.ZSharp.Core.EZCallErrorCode.
 	 */
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


