// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZConsole_Interop
	{
		static uint8 TryExecuteCommand(const TCHAR* command);
		static uint8 TryGetValue(const TCHAR* name, FString& value);
		static uint8 TrySetValue(const TCHAR* name, const TCHAR* value);
		static uint8 TryRegisterOnChanged(const TCHAR* name);
		static uint8 TryUnregisterOnChanged(const TCHAR* name);

		static uint8 TryRegisterCommand(const TCHAR* name, const TCHAR* help);
		static uint8 TryRegisterVariable(const TCHAR* name, const TCHAR* help, const TCHAR* defaultValue);
		static uint8 TryUnregisterObject(const TCHAR* name);
		
		inline static void(*GHandleExecuteCommand)(const TCHAR*, const TArray<FString>*) = nullptr;
		inline static void(*GHandleVariableChanged)(const TCHAR*) = nullptr;
	};
}


