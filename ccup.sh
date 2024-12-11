#!/usr/bin/env bash

# Project configuration
project_path=$(cd "$(dirname "$0")"; pwd)
project_name="${project_path##*/}"
build="build"
cpm_cache="./deps"
doc_gitbook="./docs/gitbook"
docker_image="magicbowen/ubuntu-cc-dev:v2"

if [ "$(uname)" == "Darwin" ]; then
    # Mac OS
    docker_cmd=docker
    docker_src_path="$project_path"
    docker_work_dir="/$project_name"
    cmake_generate_type=""
    cmake_extra_definations=    
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ] || [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Windows + MinGW
    docker_cmd="winpty docker"
    docker_src_path="/$project_path"
    docker_work_dir="//$project_name"
    cmake_generate_type='-GMinGW Makefiles'
    cmake_extra_definations="-DCMAKE_SH=CMAKE_SH-NOTFOUND"    
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Linux
    docker_cmd=docker
    docker_src_path="$project_path"
    docker_work_dir="/$project_name"
    cmake_generate_type=""
    cmake_extra_definations=
fi

# 输出工具
command_exists() {
    command -v "$@" >/dev/null 2>&1
}

info() {
    echo ${BOLD}"$@"${RESET} >&2
}

error() {
    echo ${RED}"ERROR: $@"${RESET} >&2
}

success() {
    echo ${GREEN}"SUCCESS: $@"${RESET} >&2
}

underline() {
    echo "$(printf '\033[4m')$@$(printf '\033[24m')"
}

splider() {
    info "==============================================="
}

start_exec() {
    info "START: $@ ${project_name}"
    splider
}

success_exec() {
    splider
    success "$@ ${project_name}!"
}

failed_exec() {
    splider
    error "$@ ${project_name}!"
}

setup_color() {
    # only use colors if connected to a terminal
    if [ -t 1 ]; then
        RED=$(printf '\033[31m')
        GREEN=$(printf '\033[32m')
        YELLOW=$(printf '\033[33m')
        BLUE=$(printf '\033[34m')
        BOLD=$(printf '\033[1m')
        RESET=$(printf '\033[m')
    else
        RED=""
        GREEN=""
        YELLOW=""
        BLUE=""
        BOLD=""
        RESET=""
    fi
}

# ccup commands
function help() {
    cat<<-EOF
Usage: ./ccup.sh [OPTIONS]

Options:
    -e               Launch project in docker environment
    -u               Update dependencies and generate makefile
    -f               Format code
    -b               Build project
    -t [ARGS...]     Execute testing with optional arguments
    -i               Install project
    -l [FILES...]    Lint code with optional file paths
    -r               Run executable
    -d               Document generation
    -c               Clean the build
    -C               Clean the build and dependent codes
    -h               Show this help message
EOF
}

function env() {
    start_exec "setup docker environment of"
    command_exists docker || {
        error "docker is not installed"
        exit 1
    }  

    $docker_cmd run -it -v "$docker_src_path":/"$project_name" -w "$docker_work_dir" "$docker_image" /bin/bash
    if [ $? -ne 0 ]; then
        failed_exec "setup docker environment"
        exit 1
    fi    
}

function update() {
    start_exec "update dependencies and generate makefile"
    command_exists cmake || {
        error "cmake is not installed"
        exit 1
    } 
    export GIT_SSL_NO_VERIFY=1
    cmake -H. "$cmake_generate_type" -B"$build" -DPROJECT_NAME="${project_name}" -DENABLE_TEST=on -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "$cmake_extra_definations"
    if [ $? -ne 0 ]; then
        failed_exec "update"
        exit 1
    fi
    success_exec "update"
}

function do_format() {
    paths=("$@")
    types=("*.h" "*.hpp" "*.c" "*.cc" "*.cpp" "*.cxx")
    for path in "${paths[@]}"; do
        for type in "${types[@]}"; do
            find "$path" -name "$type" -exec clang-format -style=file -i {} \;
        done
    done
}

function format() {
    start_exec "format code"
    command_exists clang-format || {
        error "clang-format is not installed"
        exit 1
    } 
    paths=("include" "src" "test")
    do_format "${paths[@]}"
    if [ $? -ne 0 ]; then
        failed_exec "format"
        exit 1
    fi
    success_exec "format"
}

function build() {
    start_exec "build"
    command_exists cmake || {
        error "cmake is not installed"
        exit 1
    } 
    
    cmake --build "$build"

    if [ $? -ne 0 ]; then
        failed_exec "build"
        exit 1
    fi
    success_exec "build"
}

function lint() {
    local files=()

    # if there are files, verify them
    if [ $# -gt 0 ]; then
        for file in "$@"; do
            if [ -f "$file" ]; then
                files+=("$file")
            else
                error "File is not existing: $file"
            fi
        done
    else
        # if not, find all source files
        while IFS= read -r file; do
            files+=("$file")
        done < <(find ./src ./include -type f \( -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" \))
    fi

    if [ ${#files[@]} -eq 0 ]; then
        info "Not found any lint file!"
        return 0
    fi

    start_exec "lint"

    command_exists clang-tidy || {
        error "clang-tidy is not installed"
        exit 1
    } 

    local failed=0

    for file in "${files[@]}"; do
        clang-tidy "$file" -p "$build" --
        if [ $? -ne 0 ]; then
            error "clang-tidy verify failed: $file"
            failed=1
        fi
    done

    if [ $failed -ne 0 ]; then
        failed_exec "lint"
        exit 1
    fi 

    success_exec "lint"
}

function test() {
    local test_args=("$@")
    start_exec "test"
    ./$build/test/${project_name}_test "${test_args[@]}"

    success_exec "test"
}

function install() {
    start_exec "install"
    
    cmake --install "$build"

    if [ $? -ne 0 ]; then
        failed_exec "install"
        exit 1
    fi
    success_exec "install"
}

function run() {
    start_exec "run"
    "./$build/src/${project_name}_service"
}

function doc() {
    start_exec "generate documents of gitbook"
    command_exists gitbook || {
        error "gitbook is not installed, reference : ./docs/README-gitbook.md"
        exit 1
    }      
    gitbook init "$doc_gitbook"
    gitbook build "$doc_gitbook"

    if [ $? -ne 0 ]; then
        failed_exec "document generation"
        exit 1
    fi
    success_exec "document generation"
}

function clean() {
    start_exec "clean"
    if [ -d "build" ]; then 
        cd "$build"
        ls | grep -v _deps | xargs rm -rf
        info "deleting the build targets of ${project_name}"
        cd -
    fi
    success_exec "clean"
}

function clean_all() {
    start_exec "clean all"
    info "deleting the build targets of ${project_name}"
    rm -rf build
    info "deleting the dependent codes of ${project_name}"
    rm -rf "${cpm_cache}"
    success_exec "clean all"
}

function parse_args() {
    local lint_files=()
    local test_args=()

    while [[ $# -gt 0 ]]; do
        arg="$1"
        if [[ "$arg" == "--" ]]; then
            shift
            break
        elif [[ "$arg" == -* && "$arg" != "--" ]]; then
            flags="${arg#-}"
            for (( i=0; i<${#flags}; i++ )); do
                flag="${flags:$i:1}"
                case "$flag" in
                    e)
                        env
                        ;;
                    u)
                        update
                        ;;
                    f)
                        format
                        ;;
                    b)
                        build
                        ;;
                    t)
                        # Collect -t arguments until the next option or the end
                        shift
                        while [[ $# -gt 0 && ! "$1" =~ ^- ]]; do
                            test_args+=("$1")
                            shift
                        done
                        test "${test_args[@]}"
                        test_args=()
                        ;;
                    i)
                        install
                        ;;
                    l)
                        # Collect -l file paths until the next option or the end
                        shift
                        while [[ $# -gt 0 && ! "$1" =~ ^- ]]; do
                            if [[ -f "$1" ]]; then
                                lint_files+=("$1")
                            else
                                error "File is not existing: $1"
                            fi
                            shift
                        done
                        lint "${lint_files[@]}"
                        # Clear lint_files
                        lint_files=()
                        ;;
                    r)
                        run
                        ;;
                    d)
                        doc
                        ;;
                    c)
                        clean
                        ;;
                    C)
                        clean_all
                        ;;
                    h)
                        help
                        exit 0
                        ;;
                    *)
                        error "Invalid options: -$flag"
                        help
                        exit 1
                        ;;
                esac
            done
            shift
        else
            error "Unexpected parameters: $1"
            help
            exit 1
        fi
    done
}

function main() {
    setup_color
    parse_args "$@"
}

main "$@"