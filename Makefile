CXXFLAGS+=-std=c++20

out/%.o: src/%.cpp
	mkdir -p out
	${CXX} ${CXXFLAGS} -c -o $@ $<

out/hx711: out/main.o out/hx711.o
	${CXX} ${CXXFLAGS} -lgpiod -o $@ $^
