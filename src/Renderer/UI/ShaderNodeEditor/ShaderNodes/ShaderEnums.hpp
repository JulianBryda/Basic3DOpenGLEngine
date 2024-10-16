#pragma once

namespace ShaderEnums
{
	enum ShaderVarPrefix
	{
		None,
		In,
		Out
	};

	enum ShaderNodeVisibility
	{
		Visible,
		Hidden,
		AttributeHidden
	};

	enum ShaderNodeCategory
	{
		Input,
		Output,
		Color,
		Converter,
		Shader,
		Texture
	};

	enum ShaderVarNodeType
	{
		Var,
		Function,
		Uniform
	};

	enum ShaderVarType
	{
		NONE = 1 << 0,

		INT = 1 << 1,
		FLOAT = 1 << 2,
		DOUBLE = 1 << 3,

		VEC_2 = 1 << 4,
		VEC_3 = 1 << 5,
		VEC_4 = 1 << 6,

		SAMPLER_2D = 1 << 7,

		VEC = VEC_2 | VEC_3 | VEC_4
	};
}