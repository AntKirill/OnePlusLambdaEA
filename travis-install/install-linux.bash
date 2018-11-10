sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq

sudo apt-get install -qq g++-7
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 90

CMAKE_VERSION=3.11.0
CMAKE_VERSION_DIR=v3.11

CMAKE_OS=Linux-x86_64
CMAKE_TAR=cmake-$CMAKE_VERSION-$CMAKE_OS.tar.gz
CMAKE_URL=http://www.cmake.org/files/$CMAKE_VERSION_DIR/$CMAKE_TAR
CMAKE_DIR=$(pwd)/cmake-$CMAKE_VERSION

wget --quiet $CMAKE_URL
mkdir -p $CMAKE_DIR
tar --strip-components=1 -xzf $CMAKE_TAR -C $CMAKE_DIR
export PATH=$CMAKE_DIR/bin:$PATH

wget boost_1_66_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.gz/download -O boost_1_66_0.tar.gz
tar xzvf boost_1_66_0.tar.gz > /dev/null
cd boost_1_66_0
sudo ./bootstrap.sh --prefix=/usr/local --with-libraries=system,filesystem > /dev/null
echo "Installation ..."
sudo ./b2 install > /dev/null
cd ..

sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo apt-get update -qq
sudo apt-get install qtbase5-dev qtdeclarative5-dev libqt5webkit5-dev libsqlite3-dev
sudo apt-get install qt5-default qttools5-dev-tools
