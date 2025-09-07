To run project with CMake
1. change the OpenCV_DIR (line 45) to your opencv path. (Application created and Tested with 4.7.0)
2. change the OLLVM_BIN (line 16) to your path.
NOTE :: The cmake file is not runing with the CMake. However, I was able to run it with makefile , and to run in obfuscation mode, use the makefile attached in the folder. You need to change these to paths in the makefile as well.

Commands :

mkdir build
cd build
cmake ..
cmake --build .

To run MakeFile , only supports obfuscation:

make
mv faceDetectorApp build/

./faceDetectorApp


-Compiled version of obfuscation tool is attached with code.
-Models are also available in path.
-SampleImage consists of a live example.

