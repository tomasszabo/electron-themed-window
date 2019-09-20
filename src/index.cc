#include <nan.h>

void getSystemDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info);
void getDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info);
void setDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info);

static void InitModule(v8::Local<v8::Object> exports) {
  v8::Isolate *isolate = exports->GetIsolate();
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  exports->Set(context, Nan::New("getSystemDarkMode").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(getSystemDarkMode)->GetFunction(context).ToLocalChecked());
  exports->Set(context, Nan::New("getDarkMode").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(getDarkMode)->GetFunction(context).ToLocalChecked());
  exports->Set(context, Nan::New("setDarkMode").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(setDarkMode)->GetFunction(context).ToLocalChecked());
}

NODE_MODULE(themed_window, InitModule)
