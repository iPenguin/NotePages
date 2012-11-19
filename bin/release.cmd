

cmake ..\notePages -DDOCS=ON -G "MinGW Makefiles"
mingw32-make.exe
cpack -G NSIS
