cmake_minimum_required(VERSION 3.10)
project(Compressor)

set(SOURCES src)

add_executable(${PROJECT_NAME}
               ${SOURCES}/imagem.hpp
               ${SOURCES}/estruturas.hpp
               ${SOURCES}/main.cpp
    )


#includes
target_include_directories(${PROJECT_NAME}
                           PUBLIC ${SOURCES}
                           )
                           )