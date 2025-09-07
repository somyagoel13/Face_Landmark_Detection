# ==========================
# Variables
# ==========================
CXX      := /home/yankee/Desktop/Final/obfuscator-llvm-4.0/build/bin/clang++
CXXFLAGS := -std=c++11 -mllvm -fla -mllvm -sub

# Project paths
SRC_DIR  := src
INC_DIR  := ./include
OPENCV   := /home/yankee/opencv-4.7.0
OPENCV_BUILD := $(OPENCV)/build

# Sources
SRCS := main.cpp \
        $(SRC_DIR)/client.cpp \
        $(SRC_DIR)/faceDetector.cpp \
        $(SRC_DIR)/faceLandmark.cpp \
        $(SRC_DIR)/FaceLandMarkDetectionManager.cpp \
        $(SRC_DIR)/jsonParser.cpp \
        $(SRC_DIR)/server.cpp \
        $(SRC_DIR)/timeUtil.cpp

# Includes
INCLUDES := \
    -I$(INC_DIR) \
    -I$(OPENCV_BUILD) \
    -I$(OPENCV)/modules/core/include \
    -I$(OPENCV)/modules/calib3d/include \
    -I$(OPENCV)/modules/features2d/include \
    -I$(OPENCV)/modules/flann/include \
    -I$(OPENCV)/modules/dnn/include \
    -I$(OPENCV)/modules/gapi/include \
    -I$(OPENCV)/modules/highgui/include \
    -I$(OPENCV)/modules/imgcodecs/include \
    -I$(OPENCV)/modules/imgproc/include \
    -I$(OPENCV)/modules/stitching/include \
    -I$(OPENCV)/modules/ts/include \
    -I$(OPENCV)/modules/video/include \
    -I$(OPENCV)/modules/videoio/include \
    -I$(OPENCV)/modules/world/include \
    -I$(OPENCV)/modules/ml/include \
    -I$(OPENCV)/modules/objdetect/include \
    -I$(OPENCV)/modules/photo/include \
    -I/usr/include \
    -I/usr/include/json-c

# Libraries
LDFLAGS := \
    -L$(OPENCV_BUILD)/lib \
    -L/usr/lib/x86_64-linux-gnu

LIBS := -lzmq -lpthread \
        -lopencv_videoio -lopencv_video -lopencv_calib3d \
        -lopencv_core -lopencv_dnn -lopencv_flann -lopencv_gapi \
        -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc \
        -lopencv_ml -lopencv_objdetect -lopencv_photo \
        -ljson-c

# Output
TARGET := faceDetectorApp 

# ==========================
# Build rules
# ==========================
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(INCLUDES) $(LDFLAGS) -o $@ $(LIBS)

clean:
	rm -f $(TARGET) *.o

