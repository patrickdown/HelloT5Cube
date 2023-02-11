#pragma once


typedef enum {
	/// \breif Treat ::T5_FrameInfo.leftTexHandle and ::T5_FrameInfo.rightTexHandle as a pair of
	/// GL_TEXTURE_2D.
	kT5_GraphicsApi_GL_TextureMode_Pair = 1,

	/// \breif Treat ::T5_FrameInfo.leftTexHandle as a GL_TEXTURE_2D_ARRAY.
	/// ::T5_FrameInfo.rightTexHandle is unused.
	///
	/// Left/Right array index should be specified in
	kT5_GraphicsApi_GL_TextureMode_Array = 2,
} T5_GraphicsApi_GL_TextureMode;

typedef struct {
	/// \brief Specify the interpretation of the texture handles in ::T5_FrameInfo
	T5_GraphicsApi_GL_TextureMode textureMode;

	/// \brief In kT5_GraphicsApi_GL_TextureMode_Array, specify the array index of the left eye
	uint32_t leftEyeArrayIndex;

	/// \brief In kT5_GraphicsApi_GL_TextureMode_Array, specify the array index of the right eye
	uint32_t rightEyeArrayIndex;
} T5_GraphicsContextGL;

