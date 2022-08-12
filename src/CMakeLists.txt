set(LIB_NAME "PoolStrategy")

file(GLOB LIB_SOURCES *.cxx)
file(GLOB LIB_HEADERS *.hxx)

add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(Main Main.cpp)
target_link_libraries(Main PoolStrategy)