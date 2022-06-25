release:
	cmake -B build . -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target ronald

debug:
	cmake -B build . -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --target ronald

test:
	cmake -B build . -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --target tests
	./build/tests

tidy:
	clang-tidy \
		-p build \
		lib/*.cpp inc/*.hpp \
		'-checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*,performance-*,readability-*,cppcoreguidelines-*'

lint:
	cppcheck --enable=warning,style,performance,portability --template=gcc app/*.cpp inc/*.hpp lib/*.cpp


.PHONY: release debug test tidy lint
