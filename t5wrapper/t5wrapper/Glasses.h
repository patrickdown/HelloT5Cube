#pragma once
#include <util/Wrapper.h>
#include <TiltFiveNative.h>
#include <variant>
#include <t5Wrapper/Context.h>
#include <t5Wrapper/OptError.h>


namespace T5Wrapper
{
	using OptConnectionState = OptError<T5_ConnectionState>;
	using OptFloatParam = OptError<double>;
	using OptIntegerParam = OptError<int64_t>;
	using OptGlassesPose = OptError<T5_GlassesPose>;
	

	class Glasses 
	{
		protected:
		T5_Glasses handle = 0;
		bool isAcquired = false;

		inline static float DefaultIPD = 59;

		public:
		Glasses() = default;
		Glasses(const Glasses&) = default;
		~Glasses();


		T5_Result Create(Context context, std::string glassesID);

		void Delete();


		T5_Result Acquire(std::string displayName);
		T5_Result Release();
		T5_Result EnsureReady();
		T5_Result InitGlassesOpenGLContext(bool layered = false);
		T5_Result SendFrameToGlasses(const T5_FrameInfo& info);

		OptConnectionState GetConnectionState();
		OptGlassesPose GetGlassesPose();
		OptFloatParam GetGlassesFloatParam(T5_ParamGlasses param);
		OptIntegerParam GetGlassesIntegerParam(T5_ParamGlasses param);

		float GetIpd();
	};

	std::string GetConnectionText(OptConnectionState connectionState);
}
