# Useful methods or functions
include makefile.sharedFunctions

all: client client_shared client_shared_javascript client_shared_test

client:
	cd ./Threadily $(CmdSeparator) $(Make)
	
client_shared:
	cd ./Threadily.SampleObjects $(CmdSeparator) $(Make)
	
client_shared_javascript:
	cd ./Threadily.SampleObjects.javascript $(CmdSeparator) $(Make)
	
client_shared_test:
	cd ./Threadily.test.cpp $(CmdSeparator) $(Make)

clean:
	cd $(call FixPath,./Threadily) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./Threadily.SampleObjects) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./Threadily.SampleObjects.javascript) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./Threadily.test.cpp) $(CmdSeparator) $(Make) clean