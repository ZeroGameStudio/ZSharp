// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public static class ParameterExtensions
{

	public static string GetKindText(this ParameterDeclaration @this) => @this.Kind switch
	{
		EParameterKind.Ref => "ref ",
		EParameterKind.Out => "out ",
		_ => string.Empty
	};

}


