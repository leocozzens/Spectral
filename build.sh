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
ex="examples"
rgn="regen"

if [ "$1" == "$rgn" ] || [ "$2" == "$rgn" ] || [ "$3" == "$rgn" ] || [ "$4" == "$rgn" ]; then
    rm -rf $dir
fi

options=""
if [ "$1" == "$dbg" ] || [ "$2" == "$dbg" ] || [ "$3" == "$dbg" ] || [ "$4" == "$dbg" ]; then
    options+=" -DCMAKE_BUILD_TYPE=Debug"
fi
if [ "$1" == "$tst" ] || [ "$2" == "$tst" ] || [ "$3" == "$tst" ] || [ "$4" == "$tst"]; then
    options+=" -DSPECTRAL_BUILD_TESTS=ON"
fi
if [ "$1" == "$ex" ] || [ "$2" == "$ex" ] || [ "$3" == "$ex" ] || [ "$4" == "$ex"]; then
    options+=" -DSPECTRAL_BUILD_EXAMPLES=ON"
fi

cmake . -B $dir -G "$target"$options

cd $dir
make
echo -e "\nOutput located in $PWD"