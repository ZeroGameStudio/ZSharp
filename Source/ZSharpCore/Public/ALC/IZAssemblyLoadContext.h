// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	/**
	 * Represents an active System.Runtime.Loader.AssemblyLoadContext.
	 *
	 * This is a base interface provides mechanisms shared across all kinds of ALC implementation.
	 *
	 * [Library Interface] - you should just consume this and have no reason to implement this.
	 */
	class IZAssemblyLoadContext : public FNoncopyable
	{
		
	public:
		/**
		 * Unloads the ALC.
		 * This is idempotent, so it is safe to call multiple times.
		 */
		virtual void Unload() = 0;
		
	public:
		/**
		 * Loads an assembly into this ALC and execute DllMain if exists.
		 * See EZLoadAssemblyErrorCode for more details.
		 * 
		 * @param assemblyName The name of the assembly, i.e. ZeroGames.ZSharp.Core.
		 *                     Assembly resolution logic depends on ZeroGames.ZSharp.Core.IAssemblyResolver.
		 * @param args A transparent argument passed to DllMain.
		 *             If there is no DllMain, this parameter is ignored.
		 * @return The error code of this operation.
		 */
		virtual EZLoadAssemblyErrorCode LoadAssembly(const FString& assemblyName, void* args = nullptr) = 0;

		/**
		 * Invokes a method in this ALC.
		 * See EZInvokeMethodErrorCode for more details.
		 * 
		 * @param assemblyName The name of the assembly, i.e. ZeroGames.ZSharp.Core.
		 *                     The assembly must have already been loaded into this ALC or Default ALC.
		 * @param typeName The full name of the type, i.e. ZeroGames.ZSharp.Core.IConjugate.
		 * @param methodName The name of the method.
		 * @param args A transparent argument passed to the method.
		 * @return The error code of this operation.
		 */
		virtual EZInvokeMethodErrorCode InvokeMethod(const FString& assemblyName, const FString& typeName, const FString& methodName, void* args = nullptr) = 0;

	public:
		virtual ~IZAssemblyLoadContext() = default;
		
	};
}


