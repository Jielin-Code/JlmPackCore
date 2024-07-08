#pragma once
/**************************************************************************
 * @作者 王杰林 2024-05-08
 * @功能 配置信息，主要方便调整SDK中部分参数
 * @版权 湖南遥昇通信技术有限公司
 * @版本 1.0
 * @邮件 254908447@qq.com
 **************************************************************************/
#ifndef JLMPACKCONFIG_H  
#define JLMPACKCONFIG_H

 // 版本号定义
#define JLMPACK_VERSION 1.0

/**************************************************************************
采用预处理指令自定义跨平台的函数接口
**************************************************************************/
#if defined(_WIN32) || defined(_WIN64)

#include "Windows.h"
#include "direct.h"

// windows下删除目录函数为RemoveDirectoryA
#define removeEmptyFolder RemoveDirectoryA
// windows下创建目录函数为_mkdir
#define JlmPack_CreateFolder(path) _mkdir(path)
// windows下设置字符串格式
#define JlmPack_Format_head "LC_ALL="
#define JlmPack_LC_ALL_Format(format) _putenv(format) // 用法JlmPack_LC_ALL_Format("en_US.UTF-8")
// 通过JLMPACK_API导出函数
#define JLMPACK_API __declspec(dllexport)

#else

#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"

// linux和unix下的删除目录函数为rmdir
#define removeEmptyFolder rmdir
// linux和unix下的创建目录函数为mkdir
#define JlmPack_CreateFolder(path) mkdir(path, S_IRWXU)

// linux和unix下设置字符串格式
#define JlmPack_Format_head ""
#define JlmPack_LC_ALL_Format(format) setenv("LC_ALL", format, 1) // 用法JlmPack_LC_ALL_Format("en_US.UTF-8")
// 因编译时采用-fvisibility = hidden，通过JLMPACK_API导出函数
#define JLMPACK_API __attribute__ ((visibility ("default")))

#endif


/**************************************************************************
 JLMPACK的一些核心宏定义
 **************************************************************************/
 // 随机读取文件中READ_BYTE_LIMIT_TO_COMPUTE_HASH个字节用于计算哈希值
#define READ_BYTE_LIMIT_TO_COMPUTE_HASH 1024
// 随机加密的随机数长度，随机数越大安全性越高
#define RANDOM_BYTE_LIMIT 8
// 当前文件的哈希值长度，建议8到16字节
#define HASH_BYTE_LIMIT 8
// 统一规范文件名（不含路径，带后缀）的最大字节数
#define NAME_BYTE_LIMIT 200
// 统一规范路径（带文件名和后缀）的最大字节数
#define URL_BYTE_LIMIT 1000
// 最大允许的目录数，即所有的目录和子目录、文件的总和不能超过CATALOG_MAX_LIMIT，可在此处调整
#define CATALOG_MAX_LIMIT 10000
// 目录嵌套深度，即文件夹下的文件夹，最大嵌套深度
#define FOLDERS_NESTED_MAX_LIMIT 1000


// 哈希值的长度限制
#define JLM_FILE_HASH_BYTE_LIMIT 64
#define PACK_FILE_HASH_BYTE_LIMIT 32

// JLM_EXTRA中部分对象的字节长度限制
#define EXTRA_BYTE_LIMIT 100

// JlmPack_JLMSE函数的两个状态：加解密模式
#define ENCODE_MODEL 0
#define DECODE_MODEL 1

// 日志描述的字节大小
#define NOTE_BYTE_LIMIT 200

// 限制二进制文件读写的最大缓存为10MB，可以设置和调整以适应最佳效率，不能低于4MB，不能太大占内存过多
#define BINARY_FILE_MAX_LIMIT 10485760  //10485760  16777216

// 设置包的最大个数，一般来讲打包的目的是把大量的文件和文件夹打包成为几个压缩包，这里设置为100，能满足很多情况下，注意配合PACK_BYTES_LIMIT设置
#define MAX_NUMBER_PACKS 100

// 将JLM压缩包切割成多个包，每个包最大字节数为PACK_BYTES_LIMIT，通过JLM_FILE.packBytesLength自定义
#define JLMPACK_BYTES_LIMIT 4227858432ULL

#endif