# Useful methods or functions
include makefile.sharedFunctions

.PHONY: all
all: client_shared_test client_shared_javascript

.PHONY: client
client:
	cd ./Threadily $(CmdSeparator) $(Make)
	
.PHONY: client_shared
client_shared: client
	cd ./Threadily.SampleObjects $(CmdSeparator) $(Make)
	
.PHONY: client_shared_javascript
client_shared_javascript: client_shared
	cd ./Threadily.SampleObjects.javascript $(CmdSeparator) $(Make)
	
.PHONY: client_shared_test
client_shared_test: client_shared
	cd ./Threadily.test.cpp $(CmdSeparator) $(Make)

.PHONY: clean
clean:
	cd $(call FixPath,./Threadily) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./Threadily.SampleObjects) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./Threadily.SampleObjects.javascript) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./Threadily.test.cpp) $(CmdSeparator) $(Make) clean