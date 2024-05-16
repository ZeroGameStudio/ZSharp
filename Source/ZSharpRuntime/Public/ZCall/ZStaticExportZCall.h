// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCallBufferSlotEncoder.h"
#include "ZFullyDecay.h"

namespace ZSharp
{
	struct FZCallBuffer;

	struct ZSHARPRUNTIME_API FZStaticExportZCall
	{
		FZStaticExportZCall(const FString& name, const TFunction<int32(FZCallBuffer*)>& function);
	};

	// ================================================= BEGIN EXPERIMENTAL =================================================
	template <typename T, T>
	struct TZStaticExportZCall;

	template <typename... TArgs, void(*Function)(TArgs...)>
	struct TZStaticExportZCall<decltype(Function), Function>
	{
		TZStaticExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices])... };
				(*Function)(std::forward<TArgs>(args.template Get<Indices>())...);
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeStatic(args, buf);
				
				return 0;
			}};
		}
	};

	template <typename TRet, typename... TArgs, TRet(*Function)(TArgs...)>
	struct TZStaticExportZCall<decltype(Function), Function>
	{
		TZStaticExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices])... };
				TZCallBufferSlotEncoder<std::decay_t<TRet>>::Encode(std::forward<TRet>((*Function)(std::forward<TArgs>(args.template Get<Indices>())...)), buf[sizeof...(TArgs)]);
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeStatic(args, buf);
				
				return 0;
			}};
		}
	};

	template <typename TThis, typename... TArgs, void(TThis::*Function)(TArgs...)>
	struct TZStaticExportZCall<decltype(Function), Function>
	{
		TZStaticExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				auto pThis = TZCallBufferSlotEncoder<TThis>::DecodeThis(buf[0]);
				TTuple<TArgs...> args{TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices + 1])...};
				(pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...);
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeInstance(args, buf);

				return 0;
			}};
		}
	};

	template <typename TRet, typename TThis, typename... TArgs, TRet(TThis::*Function)(TArgs...)>
	struct TZStaticExportZCall<decltype(Function), Function>
	{
		TZStaticExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}
		
		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				auto pThis = TZCallBufferSlotEncoder<TThis>::DecodeThis(buf[0]);
				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices + 1])... };
				TZCallBufferSlotEncoder<std::decay_t<TRet>>::Encode(std::forward<TRet>((pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...)), buf[sizeof...(TArgs) + 1]);
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeInstance(args, buf);
				
				return 0;
			}};
		}
	};

	template <typename TThis, typename... TArgs, void(TThis::*Function)(TArgs...) const>
	struct TZStaticExportZCall<decltype(Function), Function>
	{
		TZStaticExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				auto pThis = TZCallBufferSlotEncoder<TThis>::DecodeThis(buf[0]);
				TTuple<TArgs...> args { TZCallBufferSlotEncoder<std::decay_t<TArgs>>::Decode(buf[Indices + 1])... };
				(pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...);
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeInstance(args, buf);
					
				return 0;
			}};
		}
	};

	template <typename TRet, typename TThis, typename... TArgs, TRet(TThis::*Function)(TArgs...) const>
	struct TZStaticExportZCall<decltype(Function), Function>
	{
		TZStaticExportZCall(const FString& name)
		{
			Invoke(name, TMakeIntegerSequence<int32, sizeof...(TArgs)>{});
		}

		template <int32... Indices>
		void Invoke(const FString& name, TIntegerSequence<int32, Indices...>&&)
		{
			FZStaticExportZCall { name, [](FZCallBuffer* buffer)
			{
				FZCallBuffer& buf = *buffer;

				auto pThis = TZCallBufferSlotEncoder<TThis>::DecodeThis(buf[0]);
				TTuple<TArgs...> args { TZCallBufferSlotEncoder<TZFullyDecay_T<TArgs>>::Decode(buf[Indices + 1])... };
				TZCallBufferSlotEncoder<std::decay_t<TRet>>::Encode(std::forward<TRet>((pThis->*Function)(std::forward<TArgs>(args.template Get<Indices>())...)), buf[sizeof...(TArgs) + 1]);
				TZTryDecodeZCallBufferSlot<TArgs...>::template EncodeInstance(args, buf);
					
				return 0;
			}};
		}
	};
	// =================================================  END EXPERIMENTAL  =================================================

	namespace ZStaticExportZCall_Private
	{
		ZSHARPRUNTIME_API FString MakeZCallName(const FString& functionName);
	}
	
}


