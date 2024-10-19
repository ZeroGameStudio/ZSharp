// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	class FZSharpFunctionFailureDelegate
	{

		friend struct FZHookZSharpFunctionFailure;

	public:
		static FZSharpFunctionFailureDelegate& Get();

	public:
		void Execute(EZCallErrorCode error, UObject* object, FFrame& stack, RESULT_DECL);

	private:
		void Hook(TFunction<void(EZCallErrorCode, UObject*, FFrame&, void* const)> hook);

	private:
		TFunction<void(EZCallErrorCode, UObject*, FFrame&, void* const)> HookFunction;
		
	};
}


