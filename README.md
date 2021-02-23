# Yuatja

- [Yuatja](#yuatja)
  - [Initial Plan](#initial-plan)
  - [Install Conan packages, generate CMake files and build](#install-conan-packages-generate-cmake-files-and-build)

## Initial Plan

- [x] Put CMake build so we can compile
- [x] Put Conan in place to get dependencies
- [ ] Replace GWIN with SFML
- [ ] Get the old team together maybe push this further than before?

## Install Conan packages, generate CMake files and build

For now update profile.txt to contain the Build Config mode [`Debug` or `Release`]

```Bash
# cd into build location
cd build/<build-confiuration> -> so: cd build/x64-Debug

# Install Deps
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add skypjack https://api.bintray.com/conan/skypjack/conan
conan install --build=missing --profile ../profile.txt ..     

# then run cmake
cmake ..
```

then build
