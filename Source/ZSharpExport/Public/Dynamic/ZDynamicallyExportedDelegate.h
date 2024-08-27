// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedDelegate.h"
#include "IZExportedParameter.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API FZDynamicallyExportedDelegate : public IZExportedDelegate, public FNoncopyable
	{

	public:
		static FZDynamicallyExportedDelegate* Create(const UDelegateFunction* signature);

	public:
		virtual FString GetName() const override;
		virtual FString GetModule() const override;
		virtual FString GetUnrealFieldPath() const override;
		virtual EZExportedDelegateFlags GetFlags() const override;
		virtual void ForeachParameter(TFunctionRef<void(const IZExportedParameter&)> action) const override;

	private:
		explicit FZDynamicallyExportedDelegate(const UDelegateFunction* signature);

	private:
		bool IsValid() const { return bValid; }

	private:
		bool bValid;
		const UDelegateFunction* Signature;
		EZExportedDelegateFlags Flags;
		TArray<TUniquePtr<IZExportedParameter>> Parameters;
		
	};
}


