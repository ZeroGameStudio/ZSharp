﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedType.h"

namespace ZSharp
{
	class IZExportedMethod;
	
	class ZSHARPEXPORT_API IZExportedClass : public IZExportedType
	{
	public:
		virtual FString GetBaseType() const = 0;
		virtual TArray<FString> GetInterfaces() const = 0;
		virtual void ForeachMethod(TFunctionRef<void(IZExportedMethod&)> action) const = 0;
	private:
		virtual bool IsStatic() const override { return false; }
		virtual EZCallBufferSlotType GetSlotType() const override { return EZCallBufferSlotType::Conjugate; }
	};
}

