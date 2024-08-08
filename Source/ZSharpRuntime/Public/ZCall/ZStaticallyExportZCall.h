// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"
#include "ZCallBufferSlotEncoder.h"

namespace ZSharp
{
	struct FZCallBuffer;

	struct ZSHARPRUNTIME_API FZStaticallyExportZCall
	{
		FZStaticallyExportZCall(const FString& name, const TFunction<EZCallErrorCode(FZCallBuffer*)>& function);
	};

	template <typename T, T>
	struct TZStaticallyExportZCall;

	template <typename TRet, typename... TArgs, TRet(*Function)(TArgs...)>
	struct TZStaticallyExportZCall<decltype(Function), Function>
	{
		TZStaticallyExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticallyExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices])... };
				if constexpr (std::is_same_v<TRet, void>)
				{
					(*Function)(std::forward<TArgs>(args.template Get<Indices>())...);
				}
				else
				{
					TZCallBufferSlotEncoder<std::decay_t<TRet>>::EncodeRet((*Function)(std::forward<TArgs>(args.template Get<Indices>())...), buf[-1]);
				}
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeStatic(args, buf);
				
				return EZCallErrorCode::Succeed;
			}};
		}
	};

	template <typename TRet, typename TThis, typename... TArgs, TRet(TThis::*Function)(TArgs...)>
	struct TZStaticallyExportZCall<decltype(Function), Function>
	{
		TZStaticallyExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}
		
		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticallyExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				TThis* pThis = TZCallBufferSlotEncoder<TThis>::DecodePointer(buf[0]);
				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices + 1])... };
				if constexpr (std::is_same_v<TRet, void>)
				{
					(pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...);
				}
				else
				{
					TZCallBufferSlotEncoder<std::decay_t<TRet>>::EncodeRet((pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...), buf[-1]);
				}
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeInstance(args, buf);
				
				return EZCallErrorCode::Succeed;
			}};
		}
	};

	template <typename TRet, typename TThis, typename... TArgs, TRet(TThis::*Function)(TArgs...) const>
	struct TZStaticallyExportZCall<decltype(Function), Function>
	{
		TZStaticallyExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticallyExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				TThis* pThis = TZCallBufferSlotEncoder<TThis>::DecodePointer(buf[0]);
				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices + 1])... };
				if constexpr (std::is_same_v<TRet, void>)
				{
					(pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...);
				}
				else
				{
					TZCallBufferSlotEncoder<std::decay_t<TRet>>::EncodeRet((pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...), buf[-1]);
				}
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeInstance(args, buf);
					
				return EZCallErrorCode::Succeed;
			}};
		}
	};

	namespace ZStaticallyExportZCall_Private
	{
		ZSHARPRUNTIME_API FString MakeZCallName(const FString& functionName);
	}
	
}


