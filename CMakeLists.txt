cmake_minimum_required(VERSION 3.10)
project(Compressor)

set(SOURCES src)
set(RESOURCES res)

add_executable(${PROJECT_NAME}
               ${RESOURCES}/imagem.h
               ${SOURCES}/main.cpp
    )


#includes
target_include_directories(${PROJECT_NAME}
                           PUBLIC ${SOURCES}
                           PUBLIC ${RESOURCES})