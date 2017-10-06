#include "Observable.h"

#ifdef EMSCRIPTEN
#include <string>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {

#ifdef EMSCRIPTEN


defineCallbackHandler(bool, Bool);
defineCallbackHandler(char, Char);
defineCallbackHandler(unsigned char, UnsignedChar);
defineCallbackHandler(short, Short);
defineCallbackHandler(int, Int);
defineCallbackHandler(unsigned int, UnsignedInt);
defineCallbackHandler(long, Long);
defineCallbackHandler(unsigned long, UnsignedLong);
// TODO - Bug in emscripten - https://github.com/kripken/emscripten/issues/3592
//defineCallbackHandler(float, Float);
defineCallbackHandler(double, Double);
defineCallbackHandler(std::string, String);
defineCallbackHandler(std::wstring, WString);

defineCallbackVectorHandler(int, Int);
defineCallbackVectorHandler(std::wstring, WString);

EMSCRIPTEN_BINDINGS(Observable) {
	class_<ISubscribeHandle>("ISubscribeHandle")
		.smart_ptr<std::shared_ptr<ISubscribeHandle>>("ISubscribeHandle")
		;
	enum_<ObservableActionType>("ObservableActionType")
		.value("Insert", ObservableActionType::Insert)
		.value("Erase", ObservableActionType::Erase)
		.value("Set", ObservableActionType::Set)
		;
	defineObservable(bool, Bool);
	defineObservable(char, Char);
	defineObservable(unsigned char, UnsignedChar);
	defineObservable(short, Short);
	defineObservable(int, Int);
	defineObservable(unsigned int, UnsignedInt);
	defineObservable(long, Long);
	defineObservable(unsigned long, UnsignedLong);
	// TODO - Bug in emscripten - https://github.com/kripken/emscripten/issues/3592
	//defineObservable(float, Float);
	defineObservable(double, Double);
	defineObservable(std::string, String);
	defineObservable(std::wstring, WString);

	defineObservableVector(int, Int);
	defineObservableVector(std::wstring, WString);
}
#endif
}