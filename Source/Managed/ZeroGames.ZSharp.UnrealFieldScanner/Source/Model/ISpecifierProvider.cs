// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface ISpecifierProvider
{
	bool HasSpecifier(Type attributeType, bool exactType);
	IUnrealReflectionSpecifier? GetSpecifier(Type attributeType, bool exactType);
	IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers { get; }
}

public static class SpecifierProviderExtensions
{
	
	public static bool HasSpecifier(this ISpecifierProvider @this, Type attributeType) => @this.HasSpecifier(attributeType, false);
	public static IUnrealReflectionSpecifier? GetSpecifier(this ISpecifierProvider @this, Type attributeType) => @this.GetSpecifier(attributeType, false);

	public static bool HasSpecifier<T>(this ISpecifierProvider @this, bool exactType) where T : IUnrealReflectionSpecifier => @this.HasSpecifier(typeof(T), exactType);
	public static T? GetSpecifier<T>(this ISpecifierProvider @this, bool exactType) where T : IUnrealReflectionSpecifier => (T?)@this.GetSpecifier(typeof(T), exactType);
	public static bool HasSpecifier<T>(this ISpecifierProvider @this) where T : IUnrealReflectionSpecifier => @this.HasSpecifier(typeof(T), false);
	public static T? GetSpecifier<T>(this ISpecifierProvider @this) where T : IUnrealReflectionSpecifier => (T?)@this.GetSpecifier(typeof(T), false);
	
}


