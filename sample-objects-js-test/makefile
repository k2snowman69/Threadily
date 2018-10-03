# Useful methods or functions
include ../makefile.sharedConstants
include ../makefile.sharedFunctions

all: node_modules

node_modules: package.json
	npm install

.PHONY: test
test: node_modules
	npm run test

.PHONY: clean
clean:
	$(call RemoveDir,coverage)
