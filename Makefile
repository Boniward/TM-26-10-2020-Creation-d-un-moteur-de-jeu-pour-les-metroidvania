build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	./src/Managers/*.cpp \
	./src/Components/*.cpp \
	./src/Quests/*.cpp \
	./src/Rewards/*.cpp \
	./src/Kaiser/Kaiser.cpp \
	-o bin/game.exe \
	-I"lib\SDL2\include\SDL2" \
	-L"lib\SDL2\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lSDL2 \
	./lib/pugixml/pugixml.cpp

clear:
	del bin/game.exe

run:
	bin/game.exe

debugg:
	gdb bin/game.exe
