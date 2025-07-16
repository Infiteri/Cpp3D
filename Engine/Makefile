ASSEMBLY := Engine
EXTENSION := .dll
INCLUDE_FLAGS := -IEngine/Source $(VENDOR_INCLUDE)
DEFINES += -DCE_BUILD_DLL
FLAGS := -g -shared  -fno-lto -L$(BUILD_DIR) -lglfw3 -lglad -lImGui -lcomdlg32 -lShlwapi -lYAML -lImGuizmo
LINKER_FLAGS := $(FLAGS)

#Recursive wildcard function to get all .cpp files
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)

SRC_FILES := $(call rwildcard,$(ASSEMBLY)/Source/,*.cpp)
OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: scaffold compile link

.PHONY: scaffold
scaffold:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(dir $(OBJ_FILES))

.PHONY: compile
compile:
	@echo Building $(ASSEMBLY)...

.PHONY: link
link: $(OBJ_FILES)
	@echo Linking $(ASSEMBLY)...
	@g++ $(OBJ_FILES) -o $(BUILD_DIR)/$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)/$(ASSEMBLY)$(EXTENSION)
	@rm -rf $(OBJ_DIR)/$(ASSEMBLY)

.PHONY: manual_link
manual_link:
	@echo $(OBJ_FILES) 
	@echo "Manually linking with ld for Win32 (ucrt64)..." 
	@ld -o $(BUILD_DIR)/$(ASSEMBLY)$(EXTENSION) -LD:/SYS/msys64/ucrt64/lib -LD:\SYS\msys64\ucrt64\lib\gcc\x86_64-w64-mingw32\14.2.0 -lgcc -LD:/SYS/msys65/ucrt64/x86_64-w64-mingw32/lib -LD:/SYS/msys64/ucrt64/lib/gcc/x86_64-w64-mingw32/13.2.0 -L$(BUILD_DIR) $(OBJ_FILES) -lstdc++ -lkernel32 -luser32 -lgdi32 -lopengl32 -lEngine -lImGui -lglad -lglfw3 -lucrt -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -loleaut32 -lshell32 -luuid -lcomdlg32 -lws2_32 -lYAML -lImGuizmo  



$(OBJ_DIR)/%.o: %.cpp
	@echo Compiling $<...
	@mkdir -p $(dir $@)
	@g++ $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS) -g -shared -L$(BUILD_DIR)
