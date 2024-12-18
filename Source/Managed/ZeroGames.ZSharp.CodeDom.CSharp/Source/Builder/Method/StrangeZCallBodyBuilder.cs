// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class StrangeZCallBodyBuilder(string zcallReplace, TypeReference? returnType, bool needsUnsafeBlock, params ParameterDeclaration[]? parameters) : ZCallMethodBodyBuilder(string.Empty, returnType, needsUnsafeBlock, parameters)
{

	public new MethodBody Build()
	{
		StringBuilder sb = new();
		
		int32 numSlots = (IsStatic ? 0 : 1) + (Parameters?.Count ?? 0) + (ReturnType is null ? 0 : 1);
		bool needsBuffer = numSlots > 0;
		if (needsBuffer)
		{
			string setupBuffer = 
$@"Thrower.ThrowIfNotInGameThread();

const int32 NUM_SLOTS = {numSlots};
ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[NUM_SLOTS]
{{
{MakeSlots().Indent()}
}};
ZCallBuffer buffer = new(slots, NUM_SLOTS);";

			sb.Append(setupBuffer);
		}

		sb.AppendLine();
		sb.AppendLine();
		string bufferParameter = needsBuffer ? "&buffer" : "null";
		sb.Append(
$@"if ({ZCallReplace}({bufferParameter}) != EZCallErrorCode.Succeed)
{{
	throw new InvalidOperationException();
}}");
	
		if (needsBuffer)
		{
			sb.Append(MakeCopyOutsAndReturn());
		}

		Block body = NeedsUnsafeBlock ?
			$@"unsafe
{{
{sb.ToString().Indent()}
}}" : sb.ToString();

		return new(body);
	}

	public string ZCallReplace { get; } = zcallReplace;

}


