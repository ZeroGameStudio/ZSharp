// Copyright Zero Games. All Rights Reserved.

#include "ZConsole_Interop.h"

#include "ALC/ZRedFrameScope.h"
#include "Interop/ZInteropExceptionHelper.h"

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
			FZRedFrameScope scope;
			
			FString name = IConsoleManager::Get().FindConsoleObjectName(cvar);
			FZConsole_Interop::GHandleVariableChanged(*name);
		}
		
	};

	static uint8 TryGetValue(const TCHAR* name, FString& value)
	{
		IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
		if (cvar)
		{
			value = cvar->GetString();
			return true;
		}

		return false;
	}

	static uint8 TryRegisterCommand(const TCHAR* name, const TCHAR* help)
	{
		// Capture name's data to a FString object to avoid dangling pointer.
		FString nameString = name;
		IConsoleCommand* command = IConsoleManager::Get().RegisterConsoleCommand(name, help, FConsoleCommandWithArgsDelegate::CreateLambda([nameString](const TArray<FString>& args)
		{
			FZRedFrameScope scope;
			FZConsole_Interop::GHandleExecuteCommand(*nameString, &args);
		}));

		return !!command;
	}

	static uint8 TryRegisterVariable(const TCHAR* name, const TCHAR* help, const TCHAR* defaultValue)
	{
		// Capture name's data to a FString object to avoid dangling pointer.
		FString nameString = name;
		IConsoleVariable* cvar = IConsoleManager::Get().RegisterConsoleVariable(name, defaultValue, help);

		return !!cvar;
	}
}

uint8 ZSharp::FZConsole_Interop::TryExecuteCommand(const TCHAR* command)
{
	TRY
	{
		// Migrates from UKismetSystemLibrary::ExecuteConsoleCommand().
		if (!IConsoleManager::Get().ProcessUserConsoleInput(command, *GLog, nullptr))
		{
			return GEngine->Exec(nullptr, command);
		}

		return true;
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TryGetValue(const TCHAR* name, FString& value)
{
	TRY
	{
		return ZConsole_Interop_Private::TryGetValue(name, value);
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TrySetValue(const TCHAR* name, const TCHAR* value)
{
	TRY
	{
		IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
		if (cvar)
		{
			cvar->Set(value);
			return true;
		}

		return false;
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TryRegisterOnChanged(const TCHAR* name)
{
	TRY
	{
		IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
		if (cvar)
		{
			ZConsole_Interop_Private::FZConsoleVariableListener::Get().RegisterOnChanged(cvar);
			return true;
		}

		return false;
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TryUnregisterOnChanged(const TCHAR* name)
{
	TRY
	{
		IConsoleVariable* cvar = IConsoleManager::Get().FindConsoleVariable(name);
		if (cvar)
		{
			ZConsole_Interop_Private::FZConsoleVariableListener::Get().UnregisterOnChanged(cvar);
			return true;
		}

		return false;
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TryRegisterCommand(const TCHAR* name, const TCHAR* help)
{
	TRY
	{
		return ZConsole_Interop_Private::TryRegisterCommand(name, help);
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TryRegisterVariable(const TCHAR* name, const TCHAR* help, const TCHAR* defaultValue)
{
	TRY
	{
		return ZConsole_Interop_Private::TryRegisterVariable(name, help, defaultValue);
	}
	CATCHR(false)
}

uint8 ZSharp::FZConsole_Interop::TryUnregisterObject(const TCHAR* name)
{
	TRY
	{
		IConsoleObject* object = IConsoleManager::Get().FindConsoleObject(name);
		if (!object)
		{
			return false;
		}
	
		IConsoleManager::Get().UnregisterConsoleObject(object);
		return true;
	}
	CATCHR(false)
}


