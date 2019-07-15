#include <nan.h>
#include <winrt/Windows.UI.ViewManagement.h>

using namespace winrt::Windows::UI::ViewManagement;

static bool *systemDarkMode()
{
	bool result = false;

	if (RUNNING_ON_WINDOWS_10)
	{
		UISettings settings;
		auto background = settings.GetColorValue(UIColorType::Background);
		auto foreground = settings.GetColorValue(UIColorType::Foreground);

		result = foreground == #FFFFFFFF && background == #FF000000;
	}

	return result;
}

void getSystemDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
	auto bool result = systemDarkMode();
	info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void getDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
	auto bool result = systemDarkMode();
	info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void setDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
}