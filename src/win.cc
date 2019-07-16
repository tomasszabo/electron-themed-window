
#pragma once

#include <nan.h>
#include <roerrorapi.h>
#include <winrt/Windows.UI.ViewManagement.h>

using namespace winrt::Windows::UI::ViewManagement;

static double getOSVersion()
{
    int ret = 0.0;
    NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osInfo;

    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (NULL != RtlGetVersion)
    {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);
        ret = osInfo.dwMajorVersion;
    }

    return ret;
}

static bool systemDarkMode()
{
	bool result = false;

	if (getOSVersion() >= 10)
	{
		UISettings settings;
		auto background = settings.GetColorValue(UIColorType::Background);
		auto foreground = settings.GetColorValue(UIColorType::Foreground);

		result = (
			foreground.A == 255 && 
			foreground.R == 255 && 
			foreground.G == 255 && 
			foreground.B == 255
		) && (
			background.A == 255 &&
			background.R == 0 &&
			background.G == 0 &&
			background.B == 0
		);
	}

	return result;
}

void getSystemDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
	bool result = systemDarkMode();
	info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void getDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
	bool result = systemDarkMode();
	info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void setDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
}