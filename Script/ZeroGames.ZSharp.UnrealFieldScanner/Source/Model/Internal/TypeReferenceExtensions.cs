// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class TypeReferenceExtensions
{
	extension(TypeReference @this)
	{
		// Remove reference and generic arguments.
		// Example:
		// int32 -> int32
		// int32& -> int32
		// List<int32> -> List<T>
		// List<int32>& -> List<T>
		public TypeReference DecayedType
		{
			get
			{
				TypeReference result = @this;
		
				/*
				 * Remove reference.
				 * TypeReference.GetElementType() returns wrong value for generic instance type
				 * and we have to use ByReferenceType.ElementType to get the correct GenericInstanceType...
				 */
				if (result is ByReferenceType byRefType)
				{
					result = byRefType.ElementType;
				}

				/*
				 * Remove generic arguments.
				 * For generic instance type, TypeReference.GetElementType() returns the generic type of that instance.
				 * For non-generic instance type, TypeReference.GetElementType() just returns itself.
				 */
				return result.GetElementType();
			}
		}
	}
}


