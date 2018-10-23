#!/usr/bin/env bash


# ==================================================
# ========== Boost
# ==================================================

sudo apt-get install libboost-all-dev



# ==================================================
# ========== OpenCV
# ==================================================

sudo apt-get install -y git
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

mkdir -p ~/sources/
cd ~/sources/
if [[ ! -d ./opencv ]]; then
    git clone https://github.com/opencv/opencv.git --single-branch
fi
cd ./opencv
git checkout tags/3.4.0

mkdir -p build && cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j4
sudo make install

pkg-config --modversion opencv



# ==================================================
# ========== GTest
# ==================================================

sudo apt-get install -y libgtest-dev
sudo apt-get install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make

sudo cp *.a /usr/lib # copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder