@REM Windows batch script to construct MSYS makefiles
@REM Please use argument: debug for the debug build
@REM Please use argument: regen to automatically delete the build directory and regenerate makefiles
@REM Please use argument: tests to include tests in the build

@REM If you wish to switch the target toolchain for this script, please change the "target" variable
@REM (choose from any of your available cmake generators, get a list with: cmake --help)   

@ECHO OFF

SET target="MSYS Makefiles"
SET dir=build
SET dbg=debug
SET tst=tests
SET rgn=regen

if NOT "%1" == "%rgn%" (
    if NOT "%2" == "%rgn%" (
        if NOT "%3" == "%rgn%" (
            goto :NOREGEN
        )
    )
)
RMDIR /S /Q %dir%

:NOREGEN

SET DEBUG_SETTING=
SET TEST_SETTING=
if "%1" == "%dbg%" SET DEBUG_SETTING= -DCMAKE_BUILD_TYPE=Debug
if "%2" == "%dbg%" SET DEBUG_SETTING= -DCMAKE_BUILD_TYPE=Debug
if "%3" == "%dbg%" SET DEBUG_SETTING= -DCMAKE_BUILD_TYPE=Debug

if "%2" == "%tst%" SET TEST_SETTING= -DSPECTRAL_BUILD_TESTS=ON
if "%3" == "%tst%" SET TEST_SETTING= -DSPECTRAL_BUILD_TESTS=ON
if "%1" == "%tst%" SET TEST_SETTING= -DSPECTRAL_BUILD_TESTS=ON

cmake . -B %dir% -G %target%%DEBUG_SETTING%%TEST_SETTING%

CD %dir%
make

ECHO.
ECHO Output located in %cd%