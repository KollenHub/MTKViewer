# CMake模板项目
## 使用方式:  
1. 使用pkg-config,也就是常见的后缀为.pc的配置文件
- 下载pkg-config-lite[https://sourceforge.net/projects/pkgconfiglite]
- 配置环境变量PKG_CONFIG_EXECUTABLE=xxx/xxx/xxx/pkg-config.exe
- 在PackagePath.config中加入形如LIB_AVUIIL_PC=H:/Package/ffmpeg7_1/lib/pkgconfig/libavutil.pc的配置行
- 在cmake中，如下配置
```cmake
#这一句放在project之后，两种配置都要使用到的
include(cmake/package.cmake)
#打开详细信息
showconfiginfo()
find_package_by_pkgconfig(${LIB_AVUTIL_PC} LIBAVUTIL)
#成功的话可以调用到
message(STATUS ${LIBAVUTIL_INCLUDE_DIRS})
message(STATUS ${LIBAVUTIL_LIBRARIES})
```
2. 使用config的方式
- 需要在PackagePath.config中加入形如Qt5_DIR=C:/Qt/Qt5.14.2/5.14.2/msvc2017_64/lib/cmake/Qt5，其中Qt5对应文件夹中XXXConfig.cmake【xxx-config.cmake】中的Qt5,自己根据库名称去配置
- 在cmake中使用如下配置
```cmake
include(cmake/package.cmake)
find_package(Qt5 CONFIG REQUIRED Widgets)
#具体根据xxxConfig.cmake中的内容去配置target或者对应的目标
```


