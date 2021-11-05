# EN3D
It is a cross-platform 3D engine with minimal dependencies.
## Supported platforms
* Linux (Native)
* Windows (Native)
* Android (using Android NDK)
* Web (using WebAssembly)
## Building guide on Linux (Ubuntu / Debian)
### For platform Linux
```
apt install --no-install-recommends gcc make kconfig-frontends
make menuconfig
```
* Select "Linux" as target platform
```
make -j`nproc`
```
### For platform Android
* Install Android SDK (including Android NDK and Android Build-Tools)
* Install Java Development Kit and add it to PATH (or create symlink on "javac" and "keytool")
```
apt install --no-install-recommends make kconfig-frontends
make menuconfig
```
* Select "Android" as target platform
* Specify path to Android SDK and Android NDK
```
make -j`nproc`
```
### For platform Web
```
apt install --no-install-recommends clang llvm lld make kconfig-frontends minify
make menuconfig
```
* Select "Web" as target platform
```
make -j`nproc`
```
