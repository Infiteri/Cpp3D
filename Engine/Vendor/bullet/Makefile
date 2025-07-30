VP=Engine/Vendor
BUILD_DIR?=Bin

SRC_FILES := $(VP)/bullet/src/btBulletCollisionAll.cpp $(VP)/bullet/src/btBulletDynamicsAll.cpp $(VP)/bullet/src/btLinearMathAll.cpp 

.PHONY: All

All:
	g++ -g -shared -I$(VP)/bullet/src -L$(BUILD_DIR) $(SRC_FILES) -o $(VP)/bullet/Bullet.dll
	cp $(VP)/bullet/Bullet.dll $(BUILD_DIR)/Bullet.dll
