// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/IZPropertyVisitor.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	class FZFunctionVisitor
	{

		friend class FZFunctionVisitorRegistry;
	
	public:
		int32 Invoke_Black(FZCallBuffer* buffer) const;
		int32 Invoke_Red(void* param) const;

	private:
		explicit FZFunctionVisitor(UFunction* function);

	private:
		bool IsAvailable() const { return bAvailable; }

	private:
		TWeakObjectPtr<UFunction> Function;

		bool bAvailable;

		bool bIsStatic;
		TArray<TUniquePtr<IZPropertyVisitor>> ParameterProperties;
		TUniquePtr<IZPropertyVisitor> ReturnProperty;
		TArray<int32> OutParamIndices;
		
	};
}


