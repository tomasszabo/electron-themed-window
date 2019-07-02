#include <nan.h>

void getDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info);
void setDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info);

static void InitModule(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("getDarkMode").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(getDarkMode)->GetFunction());
  exports->Set(Nan::New("setDarkMode").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(setDarkMode)->GetFunction());
}

NODE_MODULE(themed_window, InitModule)
