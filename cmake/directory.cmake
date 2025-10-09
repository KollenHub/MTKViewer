function(get_direct_subdirectories base_dir result_var)
    # 获取当前目录中的所有文件或文件夹
    file(GLOB entries RELATIVE ${base_dir} ${base_dir}/*)

    set(subdirectories "")

    foreach(entry ${entries})
        if(IS_DIRECTORY "${base_dir}/${entry}")
            list(APPEND subdirectories "${base_dir}/${entry}")
        endif()
    endforeach()

    # 返回直接子文件夹
    set(${result_var} ${subdirectories} PARENT_SCOPE)
endfunction()


#获取多级目录
# 获取第N级父目录，带默认级别
# 用法: get_nth_parent_directory(<input_path> [<levels>] [<output_variable>])
function(get_nth_parent_directory)
    # 解析参数
    set(INPUT_PATH ${ARGV0})
    
    if(ARGC EQUAL 1)
        # 只有一个参数：只有输入路径，使用默认级别1和默认输出变量
        set(LEVELS 1)
        set(OUTPUT_VAR "PARENT_DIR")
    elseif(ARGC EQUAL 2)
        # 两个参数：输入路径 + 级别 或 输入路径 + 输出变量
        if(ARGV1 MATCHES "^[0-9]+$")
            set(LEVELS ${ARGV1})
            set(OUTPUT_VAR "PARENT_DIR")
        else()
            set(LEVELS 1)
            set(OUTPUT_VAR ${ARGV1})
        endif()
    elseif(ARGC EQUAL 3)
        # 三个参数：输入路径 + 级别 + 输出变量
        set(LEVELS ${ARGV1})
        set(OUTPUT_VAR ${ARGV2})
    else()
        message(FATAL_ERROR "get_nth_parent_directory: 参数错误")
    endif()
    
    # 执行获取逻辑
    set(current_path ${INPUT_PATH})
    while(LEVELS GREATER 0)
        get_filename_component(current_path ${current_path} DIRECTORY)
        if("${current_path}" STREQUAL "")
            message(WARNING "无法获取 ${LEVELS} 级父目录，已到达根目录")
            break()
        endif()
        math(EXPR LEVELS "${LEVELS} - 1")
    endwhile()
    
    set(${OUTPUT_VAR} ${current_path} PARENT_SCOPE)
endfunction()