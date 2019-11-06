
#pragma once

#include <nan.h>
#include <winreg.h>
// #include <iostream>
// #include <thread>

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

static LONG GetDWORDRegKey(HKEY hKey, const std::wstring &strValueName, DWORD &nValue, DWORD nDefaultValue)
{
    nValue = nDefaultValue;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
		LONG nError = ::RegQueryValueExW(hKey,
        strValueName.c_str(),
        0,
        NULL,
        reinterpret_cast<LPBYTE>(&nResult),
        &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}


static LONG GetBoolRegKey(HKEY hKey, const std::wstring &strValueName, bool &bValue, bool bDefaultValue)
{
    DWORD nDefValue((bDefaultValue) ? 1 : 0);
    DWORD nResult(nDefValue);
    LONG nError = GetDWORDRegKey(hKey, strValueName.c_str(), nResult, nDefValue);
    if (ERROR_SUCCESS == nError)
    {
        bValue = (nResult != 0) ? true : false;
    }
    return nError;
}

static bool systemDarkMode()
{
	bool result = false;

	if (getOSVersion() >= 10)
	{
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey);
		if (lRes == ERROR_SUCCESS) {
			bool lightTheme = false;
			LONG nError = GetBoolRegKey(hKey, L"AppsUseLightTheme", lightTheme, false);
			result = !lightTheme;

			RegCloseKey(hKey);
		}
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

struct Work {
  uv_work_t request;
	uv_async_t async;
  v8::Persistent<v8::Function> callback;
};

static void WorkAsync(uv_work_t *req)
{
	bool running = true;
	bool darkMode = systemDarkMode();

	while (running) {
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey);
		
		if (lRes != ERROR_SUCCESS) {
			return;
		}

		LONG nError = RegNotifyChangeKeyValue(hKey, 
																					FALSE, 
																					REG_NOTIFY_CHANGE_LAST_SET, 
																					NULL, 
																					FALSE);
			
		if (nError != ERROR_SUCCESS) {
			continue;
		}

		bool newDarkMode = systemDarkMode();

		if (darkMode != newDarkMode) {
			darkMode = newDarkMode;

			Work *work = static_cast<Work *>(req->data);
			uv_async_send(&work->async);
		}

		RegCloseKey(hKey);
	}
}

static void WorkCallback(uv_async_t* async)
{
	Nan::HandleScope scope;
	v8::Isolate * isolate = v8::Isolate::GetCurrent();
	v8::HandleScope handleScope(isolate);

	Work *work = static_cast<Work *>(async->data);
	v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, work->callback);

	callback->Call(isolate->GetCurrentContext(), isolate->GetCurrentContext()->Global(), 0, NULL);
}

static void WorkAsyncComplete(uv_work_t *req, int status)
{
	v8::Isolate * isolate = v8::Isolate::GetCurrent();
	Work *work = static_cast<Work *>(req->data);

	work->callback.Reset();

	uv_close((uv_handle_t*) &work->async, NULL);
   
  delete work;
}

void subscribeDarkMode(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate* isolate = info.GetIsolate();

	if (info.Length() == 0) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

	if (getOSVersion() < 10) {
		return;
	}

	Work * work = new Work();
  work->request.data = work;
	work->async.data = work;
	
	v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(info[0]);
	work->callback.Reset(isolate, callback);

	uv_async_init(uv_default_loop(), &work->async, WorkCallback);
	uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);

	info.GetReturnValue().Set(Undefined(isolate));
} 
