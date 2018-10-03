# Threadily
A C++/JavaScript object marshalling system across threads

One of the hardest parts of multithreaded development is handling similar objects across multiple threads. Lets say you have an application and the user makes an edit. Well that change now needs to be sent to the service, stored in your local storage and visualized in your UI. If you do all of this on the same thread, you're likely going to block your UI and it's going to start to feel really slow (think glitchy spinning wheels).

Further, when you do implement a multithreaded system, you have to maintain multiple states of the same object in case of error scenarios and other mishaps.

Finally, once you actually implement your multithreaded system, with your app-state being properly managed, you need to duplicate all this work for JavaScript so that it works on your website as well.

This is where Threadily comes in. It is built to maintain the state of your application across multiple threads in an Observable pattern which is easy to pick up. Further, it's been built with emscripten in mind so it is very easy to transcompile into JavaScript!

# Cloning this repository
This repository makes use of submodules so it's best to use the following command

`git clone --recursive https://github.com/k2snowman69/Threadily`

or if you have already cloned the repository without --recursive, in your local repository run

`git submodule update --init --recursive`

# Dependencies
There are several dependencies to compiling this project.

## Emscripten
Emscripten is a C++ to JavaScript compiler which converts C++ code into JavaScript.
http://kripken.github.io/emscripten-site/
You'll need the SDK at minimum

## Windows only - Mingw64
Mingw64 allows for make to be run on windows.
http://mingw-w64.org/

# Setting up the CMD
Note: I've been doing this on Windows but it should compile on unix or linux with a few minor fixes. Most likely, it's the npm scripts that would give you the biggest hurdles.

# Building
## client-shared-cpp, client-shared-js, client-shared-cpp-test
On Windows
1. Open a prompt window
1. Setup the emscripten sdk by running `emsdk_env.bat` from wherever you installed
1. Set the path to include your MinGW location (e.g. `PATH = %PATH%;D:/MinGW/mingw64/bin`)
1. Run `mingw32-make`

On Unix (or Unix based system)
1. Open a terminal window
1. Setup the emscripten sdk by running `source ./emsdk_env.sh` from wherever you installed
1. Run `make`

Other notes:
 - You can also run make from the root directory, and it will recurse into all the sub directories.
 - Use the -jN feature to run multiple processors for faster builds

# How to run tests
C++ tests can be run using the output executable `Threadily.test.cpp\debug\threadily.test.exe`. You can change the reporter status to get more details from the tests. `--reporter=spec` is a pretty good start, more information can be found via [banditcpp's documentation](https://github.com/banditcpp/bandit)

Javascript tests can be run by running `npm test` in Threadily.test.js

You can also run `make test` from the root directory and it will run all tests

# Last checked versions
This example code was last verified on Windows on October 1st, 2018
| Tool          | Version |
| ------------- | -------- |
| Windows | 10 Version 1803 |
| EMCC | 1.38.12 (commit 0d8576c0e8f5ee09a36120b9d44184b5da2f2e7a) |
| Node | 8.9.1 |
| mingw32-make | v5.0.0 |

This example code was last verified on OSX on October 1st, 2018
| Tool          | Version |
| ------------- | -------- |
| macOS | Version 10.13.6 |
| EMCC | 1.38.12 (commit 0d8576c0e8f5ee09a36120b9d44184b5da2f2e7a) |
| Node | 8.12.0 |
| make | GNU Make 3.81 |

# How it works - by example
Lets use an example to help understand how to get started. Let say you have an application of which shows some basic details about a user.
### Initialize the threads that are used in the application
First, on app launch, you would let us know what threads need to exist in the system
```
// Create a thread manager
auto threadManager = std::make_shared<threadily::ThreadManager>();
// Create the UI Thread
threadManager->getOrCreateThread(ThreadIds::ThreadId_UI);
// Create the Storage thread, which notifies the UI thread of changes
threadManager->getOrCreateThread(ThreadIds::ThreadId_Storage, { ThreadIds::ThreadId_UI });
// Create a Servie thread which notifies the Storage thread
threadManager->getOrCreateThread(ThreadIds::ThreadId_Service, { ThreadIds::ThreadId_Storage });
```
So now whenever a Threadily object is updated in our system by the service thread, it will automatically send an update to the object on the storage thread which will send an update to the UI thread.

### Define the object
We need to do some basic coding to define our objects in the system
```
// UserDetails.h
class UserDetails : public threadily::ThreadObject
{
public:
	std::shared_ptr<threadily::Observable<std::wstring>> firstName;
	std::shared_ptr<threadily::Observable<std::wstring>> lastName;
	std::shared_ptr<threadily::Observable<std::wstring>> emailAddress;
public:
	UserDetails(std::shared_ptr<threadily::IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id);

	virtual std::vector<std::shared_ptr<threadily::IObservable>> getObservableProperties() override;
};

// UserDetails.cpp
UserDetails::UserDetails(std::shared_ptr<IThreadObjectManager> objectManager, unsigned int threadId, unsigned int id) :
	ThreadObject(objectManager, threadId, id)
{
	firstName = std::make_shared<Observable<std::wstring>>();
	lastName = std::make_shared<Observable<std::wstring>>();
	emailAddress = std::make_shared<Observable<std::wstring>>();
}

std::vector<std::shared_ptr<IObservable>> PrimativesThreadObject::getObservableProperties()
{
	return std::vector<std::shared_ptr<IObservable>>({
		firstName,
		lastName,
		emailAddress
	});
}
```

### Initialize the UI
Next you would create a UI object so that the UI can render and show some information
```
// Get the manager for the user details object
if (userDetailsThreadObjectManager == nullptr)
{
	userDetailsThreadObjectManager = std::make_shared<threadily::ThreadObjectManager<UserDetails>>(threadManager);
}

auto userDetails_UI = userDetailsThreadObjectManager->getOrCreateObject(ThreadIds::ThreadId_UI, /*ObjectId*/ 0);

// Now render the page with this object's properties
page.firstNameLabel = userDetails_UI->firstName->get();
page.lastNameLabel = userDetails_UI->lastName->get();
page.emailAddressLabel = userDetails_UI->emailAddress->get();

// finally register for any changes to these properties and update the UI accordingly
userDetails_UI->firstName->subscribe([&page](std::wstring newValue)
{
	page.firstNameLabel = newValue;
});
userDetails_UI->lastName->subscribe([&page](std::wstring newValue)
{
	page.lastNameLabel = newValue;
});
userDetails_UI->emailAddress->subscribe([&page](std::wstring newValue)
{
	page.emailAddressLabel = newValue;
});

```
### Invoke a network request
Now, depending on what design you are using (e.g. Flux, Redux, or something else) the UI Object can fire some sort of message to trigger a service download.

```
UserDetails::StartLoad()
{
	// if we aren't on the service thread, just push the work to that thread
    if (this->getThreadId() != ThreadIds::ThreadId_Service)
	{
		this->runOnPeer(ThreadIds::ThreadId_Service, [](std::shared_ptr<IThreadObject> sibling) {
			auto serviceObject = std::static_pointer_cast<UserDetails>(sibling);
			sericeObject->StartLoad();
		});
	}
	// else do the actual network call
	else 
	{
		Network.ajax(...)
			.done([&userDetails_service](response)
			{
				userDetails_service->firstName->set(response.firstName);
				userDetails_service->lastName->set(response.lastName);
				userDetails_service->emailAddress->set(response.emailAddress);
			};
	}
}
```
As you can see, in our callback function, we are setting the properties on the UserDetails on the Service Thread. This `set(...)` automatically fires a notification to the Storage Thread for it to update it's property which does the same to the UI Thread.

### Listen to changes as the notifications go through the stack

Each UserDetails object in each thread can be listened to and can pick up on changes. The following is how the storage thread is informed of changes to the firstName property. We used this same scenario earlier when we first created the Page that contains all the UserDetails.
```
userDetails_app->name->subscribe([this](std::wstring newValue)
{
  // Save update to local storage...
}
```
All of these objects run in their individual threads and each thread handles updates synchronously. This means the Storage Thread and the UI Thread can update simultaneously but never get updates out of order.

# Coding examples
We will be continuously adding new tests to `sample-object-js-test` and `sample-object-cpp-test` so feel free to look in those projects for examples!
