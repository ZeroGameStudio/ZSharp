// Copyright Zero Games. All Rights Reserved.

#include "ZConsole_Interop.h"

namespace ZSharp::ZConsole_Interop_Private
{
	// Forward to a singleton to use RemoveAll().
	struct FZConsoleVariableListener
	{

		using ThisClass = FZConsoleVariableListener;

		static FZConsoleVariableListener& Get()
		{
			static FZConsoleVariableListener GInstance;
			return GInstance;
		}

		void RegisterOnChanged(IConsoleVariable* cvar)
		{
			cvar->OnChangedDelegate().AddRaw(this, &ThisClass::HandleVariableChanged);
		}

		void UnregisterOnChanged(IConsoleVariable* cvar)
		{
			cvar->OnChangedDelegate().RemoveAll(this);
		}

		void HandleVariableChanged(IConsoleVariable* cvar)
		{
			FString name = IConsoleManager::Get().FindConsoleObjectName(cvar);
			FZConsole_Interop::GHandleVariableChanged(*name);
		}
		
	};
}

uint8 ZSharp::FZConsole_Interop::TryExecuteCommand(const TCHAR* command)
{
	// Migrates from UKismetSystemLibrary::ExecuteConsoleCommand().
	if (!IConsoleManager::Get().ProcessUserConsoleInput(command, *GLog, nullptr))
	{
		return GEngine->Exec(nullptr, command);
	}

	return true;
}

uint8 ZSharp::FZConsole_Interop::TryGetValue(const TCHAR* name, FString& value)
{
	IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
	if (cvar)
	{
		value = cvar->GetString();
		return true;
	}

	return false;
}

uint8 ZSharp::FZConsole_Interop::TrySetValue(const TCHAR* name, const TCHAR* value)
{
	IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
	if (cvar)
	{
		cvar->Set(value);
		return true;
	}

	return false;
}

uint8 ZSharp::FZConsole_Interop::TryRegisterOnChanged(const TCHAR* name)
{
	IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
	if (cvar)
	{
		ZConsole_Interop_Private::FZConsoleVariableListener::Get().RegisterOnChanged(cvar);
		return true;
	}

	return false;
}

uint8 ZSharp::FZConsole_Interop::TryUnregisterOnChanged(const TCHAR* name)
{
	IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
	if (cvar)
	{
		ZConsole_Interop_Private::FZConsoleVariableListener::Get().UnregisterOnChanged(cvar);
		return true;
	}

	return false;
}

uint8 ZSharp::FZConsole_Interop::TryRegisterCommand(const TCHAR* name, const TCHAR* help)
{
	// Capture name's data to a FString object to avoid dangling pointer.
	FString nameString = name;
	IConsoleCommand* command = IConsoleManager::Get().RegisterConsoleCommand(name, help, FConsoleCommandWithArgsDelegate::CreateLambda([nameString](const TArray<FString>& args)
	{
		GHandleExecuteCommand(*nameString, &args);
	}));

	return !!command;
}

uint8 ZSharp::FZConsole_Interop::TryRegisterVariable(const TCHAR* name, const TCHAR* help, const TCHAR* defaultValue)
{
	// Capture name's data to a FString object to avoid dangling pointer.
	FString nameString = name;
	IConsoleVariable* cvar = IConsoleManager::Get().RegisterConsoleVariable(name, defaultValue, help);

	return !!cvar;
}

uint8 ZSharp::FZConsole_Interop::TryUnregisterObject(const TCHAR* name)
{
	IConsoleObject* object = IConsoleManager::Get().FindConsoleObject(name);
	if (!object)
	{
		return false;
	}
	
	IConsoleManager::Get().UnregisterConsoleObject(object);
	return true;
}


