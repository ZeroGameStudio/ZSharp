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
	extension(ISpecifierProvider @this)
	{
		public bool HasSpecifier(Type attributeType) => @this.HasSpecifier(attributeType, false);
		public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType) => @this.GetSpecifier(attributeType, false);
		public bool HasSpecifier<T>(bool exactType) where T : IUnrealReflectionSpecifier => @this.HasSpecifier(typeof(T), exactType);
		public T? GetSpecifier<T>(bool exactType) where T : IUnrealReflectionSpecifier => (T?)@this.GetSpecifier(typeof(T), exactType);
		public bool HasSpecifier<T>() where T : IUnrealReflectionSpecifier => @this.HasSpecifier(typeof(T), false);
		public T? GetSpecifier<T>() where T : IUnrealReflectionSpecifier => (T?)@this.GetSpecifier(typeof(T), false);
	}
}


