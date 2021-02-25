# Yautja

- [Yautja](#yautja)
  - [Initial Plan](#initial-plan)
  - [Install Conan packages](#install-conan-packages)
  - [You need to build this project using Visual Studio 2019](#you-need-to-build-this-project-using-visual-studio-2019)


![](./src/assets/demo.gif)

## Initial Plan

- [x] Put CMake build so we can compile
- [x] Put Conan in place to get dependencies
- [x] Replace GWIN with SFML
- [ ] Aggressively Refactor the code
- [ ] Get the old team together maybe push this further than before?

## Install Conan packages

For now update profile.txt to contain the Build Config mode [`Debug` or `Release`]

```Bash
mkdir build/<build-confiuration>  # so: mkdir build/x64-Debug

# cd into build location
cd build/<build-confiuration> # so: cd build/x64-Debug

# Install Deps
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add skypjack https://api.bintray.com/conan/skypjack/conan
conan install --build=missing --profile ../../profile.txt ../..

```

## You need to build this project using Visual Studio 2019

```bash
# Generate CMake cache using VS

# Build executable and test binary
```
