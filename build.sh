# Bash script to construct Unix makefiles
# Please use argument: debug for the debug build
# Please use argument: regen to automatically delete the build directory and regenerate makefiles
# Please use argument: tests to include tests in the build

# If you wish to switch the target toolchain for this script, please change the "target" variable
# (choose from any of your available cmake generators, get a list with: cmake --help)   

target="Unix Makefiles"
dir="build"
dbg="debug"
tst="tests"
rgn="regen"

if [ "$1" == "$rgn" ] || [ "$2" == "$rgn" ]; then
    rm -rf $dir
fi

options=""
if [ "$1" == "$dbg" ] || [ "$2" == "$dbg" ] || [ "$3" == "$dbg" ]; then
    options+=" -DCMAKE_BUILD_TYPE=Debug"
fi
if [ "$1" == "$tst" ] || [ "$2" == "$tst" ] || [ "$3" == "$tst" ]; then
    options+=" -DINCLUDE_TESTS=ON"
fi

cmake . -B $dir -G "$target"$options

cd $dir
make
echo -e "\nExecutable located in $PWD/bin"