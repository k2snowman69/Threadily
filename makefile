# Useful methods or functions
include makefile.sharedFunctions

.PHONY: all
all: sample_objects_cpp_test sample_objects_js_test

.PHONY: threadily
threadily:
	cd ./threadily $(CmdSeparator) $(Make)
	
.PHONY: sample_objects_cpp
sample_objects_cpp: threadily
	cd ./sample-objects-cpp $(CmdSeparator) $(Make)
	
.PHONY: sample_objects_js
sample_objects_js: sample_objects_cpp
	cd ./sample-objects-js $(CmdSeparator) $(Make)
	
.PHONY: sample_objects_js_test
sample_objects_js_test: sample_objects_js
	cd ./sample-objects-js-test $(CmdSeparator) $(Make)
	
.PHONY: sample_objects_cpp_test
sample_objects_cpp_test: sample_objects_cpp
	cd ./sample-objects-cpp-test $(CmdSeparator) $(Make)

.PHONY: test
test: sample_objects_js_test sample_objects_cpp_test
	cd ./sample-objects-cpp-test $(CmdSeparator) $(Make) test
	cd ./sample-objects-js-test $(CmdSeparator) $(Make) test

.PHONY: clean
clean:
	cd $(call FixPath,./threadily) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./sample-objects-cpp) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./sample-objects-cpp-test) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./sample-objects-js) $(CmdSeparator) $(Make) clean
	cd $(call FixPath,./sample-objects-js-test) $(CmdSeparator) $(Make) clean