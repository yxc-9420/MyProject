project(opencvPro) #设置项目名称，参数任意指定
aux_source_directory(./ SRC_CUR) #搜索当前目录下的源文件，将结果存到SRC_CUR
aux_source_directory(src SRC_SUB)#搜索src目录下的源文件，将结果存到SRC_CUR


find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(inc) #设置包含的头文件

add_executable(OPENCV ${SRC_CUR} ${SRC_SUB}) #指定源文件，生成可执行文件opencvPro

target_link_libraries(OPENCV ${OpenCV_LIBS})




