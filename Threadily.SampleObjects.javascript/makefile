# Useful methods or functions
include ../makefile.sharedConstants
include ../makefile.sharedFunctions
# Dependent files
PREFIX = $(call FixPath,./src/prefix.js)
POSTFIX = $(call FixPath,./src/postfix.js)
# Dependent libraries
LIBRARIES_DEBUG = $(call FixPath,../threadily/debug/threadily.bc) $(call FixPath,../Threadily.SampleObjects/debug/threadily.sampleObjects.bc)
LIBRARIES_SHIP = $(call FixPath,../threadily/ship/threadily.bc) $(call FixPath,../Threadily.SampleObjects/ship/threadily.sampleObjects.bc)
# Outputs
TARGET_NAME_BASE = threadily.sampleobjects.javascript
TARGET_NAME_GCC = $(TARGET_NAME_BASE).js
TARGET_NAME_GCC_DEBUG = $(call FixPath,debug/$(TARGET_NAME_GCC))
TARGET_NAME_GCC_SHIP = $(call FixPath,ship/$(TARGET_NAME_GCC))
# emcc flags
EMCC_FLAGS_JS = --pre-js $(PREFIX) --post-js $(POSTFIX)

all: debug ship
debug: debug_emcc debug/$(TARGET_NAME_BASE).js debug/$(TARGET_NAME_BASE).d.ts
ship: ship_emcc ship/$(TARGET_NAME_BASE).js ship/$(TARGET_NAME_BASE).d.ts
debug_emcc: $(TARGET_NAME_GCC_DEBUG)
ship_emcc: $(TARGET_NAME_GCC_SHIP)

$(TARGET_NAME_GCC_DEBUG): $(LIBRARIES_DEBUG) $(PREFIX) $(POSTFIX)
	$(call MakeDir,$(dir $@))
	emcc $(EMCC_FLAGS_JS) --bind $(LIBRARIES_DEBUG) -o $(TARGET_NAME_GCC_DEBUG) $(EMCC_FLAGS_DEBUG)

$(TARGET_NAME_GCC_SHIP): $(LIBRARIES_SHIP) $(PREFIX) $(POSTFIX)
	$(call MakeDir,$(dir $@))
	emcc $(EMCC_FLAGS_JS) --bind $(LIBRARIES_SHIP) -o $(TARGET_NAME_GCC_SHIP) $(EMCC_FLAGS_SHIP)

# Last resort is to always copy the file
debug/%: src/%
	$(call MakeDir,$@)
	xcopy $(call FixPath,$^) $(call FixPath,$(dir $@).) /Q /Y /I

ship/%: src/%
	$(call MakeDir,$@)
	xcopy $(call FixPath,$^) $(call FixPath,$(dir $@).) /Q /Y /I

clean:
	$(call RemoveDir,debug)
	$(call RemoveDir,ship)