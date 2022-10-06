#include <t5wrapper/Glasses.h>

namespace T5Wrapper {

T5_Result Glasses::Create(Context context, std::string glassesID)
{
	return t5CreateGlasses((T5_Context)context, glassesID.c_str(), &handle);
}

void Glasses::Delete()
{
	if(handle != 0)
		t5DestroyGlasses(&handle);
}

Glasses::~Glasses()
{
	if(isAcquired)
		Release();
}

T5_Result Glasses::Acquire(std::string displayName)
{
	T5_Result err = t5AcquireGlasses(handle, displayName.c_str());
	isAcquired = (err == T5_SUCCESS);
	return err;
}

T5_Result Glasses::Release()
{
	isAcquired = false;
	return t5ReleaseGlasses(handle);
}

T5_Result Glasses::EnsureReady()
{
	return t5EnsureGlassesReady(handle);
}

T5_Result Glasses::InitGlassesOpenGLContext()
{
	return t5InitGlassesGraphicsContext(handle, kT5_GraphicsApi_Gl, nullptr);
}

T5_Result Glasses::SendFrameToGlasses(const T5_FrameInfo& info)
{
	return t5SendFrameToGlasses(handle,&info);
}

OptGlassesPose Glasses::GetGlassesPose()
{
	T5_GlassesPose pose;
	auto err = t5GetGlassesPose(handle, &pose);
	return err ? OptGlassesPose(err) : OptGlassesPose(pose);
}

OptConnectionState Glasses::GetConnectionState()
{
	T5_ConnectionState connectionState;
	T5_Result err = t5GetGlassesConnectionState(handle, &connectionState);

	return err ? OptConnectionState(err) : OptConnectionState(connectionState);
}

OptFloatParam Glasses::GetGlassesFloatParam(T5_ParamGlasses param)
{
	double paramValue;
	auto err = t5GetGlassesFloatParam(handle, 0, param, &paramValue);

	return err == T5_SUCCESS ? OptFloatParam(paramValue) : OptFloatParam(err);
}

OptIntegerParam Glasses::GetGlassesIntegerParam(T5_ParamGlasses param)
{
	int64_t paramValue;
	auto err = t5GetGlassesIntegerParam(handle, 0, param, &paramValue);

	return err == T5_SUCCESS ? OptIntegerParam(paramValue) : OptIntegerParam(err);
}

float Glasses::GetIpd()
{
	auto ipd = GetGlassesFloatParam(kT5_ParamGlasses_Float_IPD);
	return ipd.IsValid() ? (float)ipd.GetValue() : DefaultIPD;
}

std::string GetConnectionText(OptConnectionState connectionState)
{
	T5_ConnectionState state;

	if (connectionState.TryGet(state))
	{
		switch (state)
		{
		case kT5_ConnectionState_ExclusiveConnection:
		return "Exclusive Connection";

		case kT5_ConnectionState_ExclusiveReservation:
		return "Exclusive Reservation";

		case kT5_ConnectionState_NotExclusivelyConnected:
		return "Not Connected";

		case kT5_ConnectionState_Disconnected:
		return "Disconnected";

		default:
		return "Unknown";
		}
	}

	return 	connectionState.GetErrorMessage();
}


}



