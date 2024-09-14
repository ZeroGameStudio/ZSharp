// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/ZCallHandle.h"

namespace ZSharp
{
	struct FZSharpClass
	{
	
	};

	struct FZSharpFunction
	{

		friend class FZSharpFieldRegistry;

	public:
		FZCallHandle GetZCallHandle() const;

	public:
		FString ZCallName;
		
	private:
		mutable FZCallHandle ZCallHandle{};
		
	};

	/**
	 * There are too many places violate the fucking LSP in engine code, i.e. Assuming that native class is exact UClass but not subclass...
	 * So we have no way to make a subclass like UZSharpClass inherits from UClass and have to use proxy...
	 */
	class ZSHARPRUNTIME_API IZSharpFieldRegistry
	{
	public:
		static IZSharpFieldRegistry& Get();
	public:
		virtual ~IZSharpFieldRegistry(){}
	public:
		virtual const FZSharpClass* GetClass(const UClass* cls) const = 0;
		virtual const FZSharpFunction* GetFunction(const UFunction* function) const = 0;
	public:
		bool IsZSharpClass(const UClass* cls) const { return !!GetClass(cls); }
		bool IsZSharpFunction(const UFunction* function) const { return !!GetFunction(function); }
	};
}


