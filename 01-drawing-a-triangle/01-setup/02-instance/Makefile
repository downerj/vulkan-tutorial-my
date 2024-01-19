.PHONY: test clean

CXX_STD = -std=c++17
OPTIMIZE =
DEBUG =
WARNINGS = -Wall -Wextra -Werror -Wpedantic -pedantic-errors
LINK_LIBS = -lglfw -lvulkan # -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
EXE_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
INCL_DIR = src
EXECUTABLE = ${EXE_DIR}/VulkanTest
MAIN_CXX = ${SRC_DIR}/main.cxx
MAIN_OBJ = ${OBJ_DIR}/main.o
APP_CXX = ${SRC_DIR}/app.cxx
APP_HXX = ${INCL_DIR}/app.hxx
APP_OBJ = ${OBJ_DIR}/app.o

release: OPTIMIZE = -O2
release: DEBUG = -DNDEBUG
release: ${EXECUTABLE}
debug: OPTIMIZE = -Og
debug: DEBUG = -DDEBUG -g
debug: ${EXECUTABLE}

${EXECUTABLE}: ${MAIN_OBJ} ${APP_OBJ}
	mkdir -p ${EXE_DIR}
	${CXX} -o $@ $^ ${LINK_LIBS}

${MAIN_OBJ}: ${MAIN_CXX}
	mkdir -p ${OBJ_DIR}
	${CXX} -c -o $@ $< ${CXX_STD} ${WARNINGS} ${OPTIMIZE} ${DEBUG}

${APP_OBJ}: ${APP_CXX}
	mkdir -p ${OBJ_DIR}
	${CXX} -c -o $@ $< ${CXX_STD} ${WARNINGS} ${OPTIMIZE} ${DEBUG}

test: ${EXECUTABLE}
	${EXECUTABLE}

clean:
	rm -f ${EXECUTABLE} **/*.o
	rmdir ${EXE_DIR}
	rmdir ${OBJ_DIR}
