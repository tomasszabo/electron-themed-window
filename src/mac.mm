#include <nan.h>
#import <Cocoa/Cocoa.h>


static NSWindow *windowFromBuffer(const v8::Local<v8::Value>& buffer) {
  auto data = (NSView **)node::Buffer::Data(buffer);
  auto view = data[0];
  return view.window;
}

void getDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }
  if (!node::Buffer::HasInstance(info[0]) || node::Buffer::Length(info[0]) < 4) {
    Nan::ThrowTypeError("Pointer buffer is invalid");
    return;
  }

  bool result = false;

  if (@available(macOS 10.14, *)) {
    auto win = windowFromBuffer(info[0]);
    auto appearance = win.appearance;

    NSAppearanceName basicAppearance = [appearance bestMatchFromAppearancesWithNames:@[
        NSAppearanceNameAqua,
        NSAppearanceNameDarkAqua
    ]];

    result = [basicAppearance isEqualToString:NSAppearanceNameDarkAqua];
  } 

  info.GetReturnValue().Set(Nan::New<v8::Boolean>(result));
}

void setDarkMode(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }
  if (!node::Buffer::HasInstance(info[0]) || node::Buffer::Length(info[0]) < 4) {
    Nan::ThrowTypeError("Pointer buffer is invalid");
    return;
  }
  if (!info[1]->IsBoolean()) {
    Nan::ThrowTypeError("DarkMode must be boolean");
  }

  if (@available(macOS 10.14, *)) {
    auto win = windowFromBuffer(info[0]);
    bool darkMode = info[1]->BooleanValue();

    if (darkMode == true) {
      win.appearance = [NSAppearance appearanceNamed:NSAppearanceNameDarkAqua];
    } else {
      win.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    }
  }
}
