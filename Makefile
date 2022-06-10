release:
	cmake -B build . -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target path_tracer

debug:
	cmake -B build . -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --target path_tracer

test:
	cmake -B build . -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --target tests
	./build/tests

lint:
	clang-tidy \
		-p build \
		lib/*.cpp inc/*.hpp \
		'-checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*,performance-*,readability-*,cppcoreguidelines-*'


.PHONY: release debug test lint
