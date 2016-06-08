#include "Observable.h"

#ifdef EMSCRIPTEN
#include <string>
#include <emscripten/bind.h>

using namespace emscripten;
#endif

namespace threadily {
#ifdef EMSCRIPTEN

	class ISubscribeHandleCallbackWStringWrapper : public wrapper<ISubscribeHandleCallback<std::wstring>>
	{
	public:
		EMSCRIPTEN_WRAPPER(ISubscribeHandleCallbackWStringWrapper);
		void onChange(std::wstring work) {
			return call<void>("onChange", work);
		}
	};

EMSCRIPTEN_BINDINGS(Observable) {
	class_<ISubscribeHandle>("ISubscribeHandle")
		.smart_ptr<std::shared_ptr<ISubscribeHandle>>("ISubscribeHandle")
		;
	class_<Observable<bool>>("ObservableBool")
		.smart_ptr_constructor("ObservableBool", &std::make_shared<Observable<bool>>)
		.function("get", &Observable<bool>::get)
		.function("set", &Observable<bool>::set)
		;
	class_<Observable<char>>("ObservableChar")
		.smart_ptr_constructor("ObservableChar", &std::make_shared<Observable<char>>)
		.function("get", &Observable<char>::get)
		.function("set", &Observable<char>::set)
		;
	class_<Observable<signed char>>("ObservableSignedChar")
		.smart_ptr_constructor("ObservableSignedChar", &std::make_shared<Observable<signed char>>)
		.function("get", &Observable<signed char>::get)
		.function("set", &Observable<signed char>::set)
		;
	class_<Observable<unsigned char>>("ObservableUnsignedChar")
		.smart_ptr_constructor("ObservableUnsignedChar", &std::make_shared<Observable<unsigned char>>)
		.function("get", &Observable<unsigned char>::get)
		.function("set", &Observable<unsigned char>::set)
		;
	class_<Observable<short>>("ObservableShort")
		.smart_ptr_constructor("ObservableShort", &std::make_shared<Observable<short>>)
		.function("get", &Observable<short>::get)
		.function("set", &Observable<short>::set)
		;
	class_<Observable<int>>("ObservableInt")
		.smart_ptr_constructor("ObservableInt", &std::make_shared<Observable<int>>)
		.function("get", &Observable<int>::get)
		.function("set", &Observable<int>::set)
		;
	class_<Observable<unsigned int>>("ObservableUnsignedInt")
		.smart_ptr_constructor("ObservableUnsignedInt", &std::make_shared<Observable<unsigned int>>)
		.function("get", &Observable<unsigned int>::get)
		.function("set", &Observable<unsigned int>::set)
		;
	class_<Observable<long>>("ObservableLong")
		.smart_ptr_constructor("ObservableLong", &std::make_shared<Observable<long>>)
		.function("get", &Observable<long>::get)
		.function("set", &Observable<long>::set)
		;
	class_<Observable<unsigned long>>("ObservableUnsignedLong")
		.smart_ptr_constructor("ObservableUnsignedLong", &std::make_shared<Observable<unsigned long>>)
		.function("get", &Observable<unsigned long>::get)
		.function("set", &Observable<unsigned long>::set)
		;
	class_<Observable<float>>("ObservableFloat")
		.smart_ptr_constructor("ObservableFloat", &std::make_shared<Observable<float>>)
		.function("get", &Observable<float>::get)
		.function("set", &Observable<float>::set)
		;
	class_<Observable<double>>("ObservableDouble")
		.smart_ptr_constructor("ObservableDouble", &std::make_shared<Observable<double>>)
		.function("get", &Observable<double>::get)
		.function("set", &Observable<double>::set)
		;
	class_<Observable<std::string>>("ObservableString")
		.smart_ptr_constructor("ObservableString", &std::make_shared<Observable<std::string>>)
		.function("get", &Observable<std::string>::get)
		.function("set", &Observable<std::string>::set)
		;
	class_<Observable<std::wstring>>("ObservableWString")
		.smart_ptr_constructor("ObservableWString", &std::make_shared<Observable<std::wstring>>)
		.function("get", &Observable<std::wstring>::get)
		.function("set", &Observable<std::wstring>::set)
		.function("subscribe", select_overload<std::shared_ptr<ISubscribeHandle>(ISubscribeHandleCallback<std::wstring>*)>(&Observable<std::wstring>::subscribe), allow_raw_pointers())
		;

	class_<ISubscribeHandleCallback<std::wstring>>("ISubscribeHandleCallbackWString")
		.smart_ptr<std::shared_ptr<ISubscribeHandleCallback<std::wstring>>>("ISubscribeHandleCallbackWString")
		.function("onChange", &ISubscribeHandleCallback<std::wstring>::onChange, pure_virtual())
		.allow_subclass<ISubscribeHandleCallbackWStringWrapper>("ISubscribeHandleCallbackWStringWrapper")
		;
	/*
	class_<ISubscribeHandleCallback<T>>("ISubscribeHandleCallback")
		.smart_ptr<std::shared_ptr<ISubscribeHandleCallback<T>>>("ISubscribeHandleCallback")
		.function("onChange", &ISubscribeHandleCallback<T>::run, pure_virtual())
		.allow_subclass<ISubscribeHandleCallbackWrapper<T>>("ISubscribeHandleCallbackWrapper")
		;
	*/
}
#endif
}