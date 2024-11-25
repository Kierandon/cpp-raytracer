CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
CXXFLAGS += -O3 -march=native -flto
CXXFLAGS += -fopenmp
CXXFLAGS += -I./include
MAKEFLAGS += -j8

# Debug/Profile flags (comment out for release)
# CXXFLAGS += -g -pg

TARGET = raytracer.exe

SRCS = src/core/Ray.cpp \
       src/core/Vec3.cpp \
       src/core/Image.cpp \
       src/core/Camera.cpp \
       src/core/Utils.cpp \
       src/core/PhongPathtracer.cpp \
	src/core/Pathtracer.cpp \
       src/geometry/Sphere.cpp \
       src/geometry/Rectangle.cpp \
       src/geometry/Box.cpp \
       src/geometry/HittableList.cpp \
       src/materials/BlinnPhongMaterial.cpp \
       src/materials/Metal.cpp \
       src/materials/Dielectric.cpp \
	src/materials/Emissive.cpp \
	src/Materials/Diffuse.cpp \
	src/materials/CosinePDF.cpp \
       src/geometry/Plane.cpp \
       src/geometry/Cylinder.cpp \
       src/geometry/Triangle.cpp \
       src/scene/SceneLoader.cpp \
       src/scene/SceneRenderer.cpp \
       src/textures/ImageTexture.cpp \
       src/geometry/AABB.cpp \
       src/geometry/BVHNode.cpp \
       src/postprocess/BilateralDenoiser.cpp \
       src/core/ImportanceSampler.cpp \
       main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lgdi32

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /F /Q *.o src\core\*.o src\geometry\*.o src\materials\*.o src\scene\*.o src\postprocess\*.o src\textures\*.o $(TARGET) output.ppm
