INCLUDE_INSTALL_PATH=/usr/local/include
LIBRARY_INSTALL_PATH=/usr/local/lib

LIBRARY_NAME=visone

CFLAGS=-Os -mtune=native -std=c++17

PLATFORM := $(shell uname)

ifeq "$(PLATFORM)" "Linux"
	INCLUDE=-I./include `pkg-config --cflags opencv4 freetype2 astro`
	LIBS=`pkg-config --libs opencv4` -lglad -lsoil2 `pkg-config --libs --static glfw3` -lfreetype -lGL -lassimp -ltiff -lcoordFK5 -last2Body -lastTime -lastMath -lSGP4 -lcurl -lEopSpw -lastIOD -lastUtils
	COMPILER=g++
endif

ifeq "$(PLATFORM)" "Darwin"
	INCLUDE=-I./include `pkg-config --cflags opencv4 freetype2 astro`
	LIBS=`pkg-config --libs opencv4` -lglad `pkg-config --libs --static glfw3` -lfreetype -framework OpenGL -lassimp -lSOIL2 -ltiff -lcoordFK5 -last2Body -lastTime -lastMath -lSGP4 -lcurl -lEopSpw -lastIOD -lastUtils
	COMPILER=g++	
endif


all: example


setup:


development:
	@rm -rf $(INCLUDE_INSTALL_PATH)/$(LIBRARY_NAME)
	@ln -s $(shell pwd)/include $(INCLUDE_INSTALL_PATH)/$(LIBRARY_NAME)


install:
	@rm -rf $(INCLUDE_INSTALL_PATH)/$(LIBRARY_NAME)
	@mkdir -p $(INCLUDE_INSTALL_PATH)/$(LIBRARY_NAME)
	@cp -R ./include/* $(INCLUDE_INSTALL_PATH)/$(LIBRARY_NAME)

uninstall:
	@rm -rf $(INCLUDE_INSTALL_PATH)/$(LIBRARY_NAME)

clean:

example:
	@mkdir -p ~/bin
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o ~/bin/$(LIBRARY_NAME) ./main.cpp $(LIBS)
	
