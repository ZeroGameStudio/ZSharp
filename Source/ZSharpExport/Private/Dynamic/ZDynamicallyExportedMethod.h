// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedMethod.h"

namespace ZSharp
{
	class FZDynamicallyExportedMethod : public IZExportedMethod, public FNoncopyable
	{

	public:
		static FZDynamicallyExportedMethod* Create(const UFunction* function);

	public:
		virtual FString GetName() const override;
		virtual EZExportedMethodFlags GetFlags() const override;
		virtual FString GetZCallName() const override;
		virtual FZFullyExportedTypeName GetOwnerInterface() const override;
		virtual void ForeachParameter(TFunctionRef<void(const IZExportedParameter&)> action) const override;

	private:
		explicit FZDynamicallyExportedMethod(const UFunction* function);

	private:
		bool IsValid() const { return bValid; }

	private:
		bool bValid;
		const UFunction* Function;
		EZExportedMethodFlags Flags;
		TArray<TUniquePtr<IZExportedParameter>> Parameters;
		
	};
}


