# Threadily
A C++ object marshalling system across threads

One of the hardest parts of any development is handling cross thread property changes. Lets say you have an application and the user makes an edit. Well that change now needs to be sent to the service, stored in your local storage and visualized in your UI. If you do all of this on the same thread, you're likely going to block your UI and it's going to start to feel really slow (think glitchy spinning wheels).

Further, when you do implement a multithreaded system, you have to maintain multiple states of the same object in case of error scenarios and other mishaps. This is where Threadily comes in.

# How it works
Lets say you have an application that stores some basic user settings about a user, both locally and on the service. On app launch, you would let us know what threads need to exist in the system
```
auto threadManager = std::make_shared<threadily::ThreadManager>();
threadManager->getOrCreateThread(ThreadIds::UI); // UIThread -> doesn't inform anyone
threadManager->getOrCreateThread(ThreadIds::App, { ThreadIds::UI }); // App thread -> sends notifications to UI thread
threadManager->getOrCreateThread(ThreadIds::Service, { ThreadIds::App }); // Service thread -> Sends notifications to the App thread
```
Next you would create a UI object so that the UI can render and show some information
```
if (userSettingsThreadObjectManager == nullptr)
{
	userSettingsThreadObjectManager = std::make_shared<threadily::ThreadObjectManager<UserSettings>>(threadManager);
}

auto d = userSettingsThreadObjectManager->getOrCreateObject(ThreadIds::UI, /*ObjectId*/ 0);
// Continue to render your UI using this object
```

Now, depending on what design you are using (e.g. Flux, Redux, or something else) the UI Object can fire some sort of message to trigger a service download.

```
UserSettings::StartLoad()
{
	this->getObjectManager()->getThreadManager()->getOrCreateThread(ThreadIds::Service)->addWork(std::make_shared<threadily::ThreadQueueItem>([this]() {
		auto userSettings_service = userSettingsThreadObjectManager->getOrCreateObject(ThreadIds::Service, this->getId());
		userSettings_service->GetSettings(this->getId());
	}));
}
```

Upon callback since all Observable (get to this later) properties of UserSettings are synced between all of the threads, if the service gets a return, to start notifying all the other objects, it only has to do the following:

```
userSettings_service->name->set(L"The user's name!");
```

The app object will automatically get the notification and pass it onto the UI. To make the app object do something with this information, you just need to add a subscribe method onto an observable property
```
userSettings_app->name->subscribe([this](std::wstring newValue)
{
  // Save to your local storage...
}
```
And the UI object...
```
userSettings_UI->name->subscribe([this](std::wstring newValue)
{
  // Update your UI...
}
```
All of these objects run in their individual threads and each thread handles updates synchronously

##Observables
Sadly, in C++ there is no way to get the properties of each object through reflection so each ThreadableObject must implement a method called getObservableProperties
```
UserSettings::UserSettings(...)
{
  // ... general initilization
	this->name = std::make_shared<threadily::Observable<std::wstring>>();
	this->name->set(L"");
  // ... general initilization
}
std::vector<std::shared_ptr<threadily::IObservable>> UserSettings::getObservableProperties()
{
	return std::vector<std::shared_ptr<threadily::IObservable>>({
		name
	});
}
```
Now name will be automatically synced across each UserSettings object in each thread.
#Taking this to the web
This entire project is emscripten compatible (beta testing). This means that you can write the backend of your application in C++ for iOS, Andriod, Windows and Web.
Simply install emcc from emscripten and build Threadily. In the output folder you will find all the header files you need to develop along with a Threadily.bc which is needed to link with emcc.
