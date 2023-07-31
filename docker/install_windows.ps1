# Run this as Admin
# Requires CHOCOLAT package manager


# mingw-w64
choco install mingw --yes

# Add MINGW_HOME env variable
Set-ItemProperty -Path 'HKCU:\Environment' -Name 'MINGW_HOME' -Value 'C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64'

# Also add to PATH
# %MINGW_HOME%\bin
# %MINGW_HOME%\mingw32\bin

# gcc test
gcc --version

# CMake
choco install cmake

# Add CMake to environments PATH 
# C:\Program Files\CMake\bin

#CMake test
cmake --version

# VS Code 
# 1. add properties.json
# 2. add extension code runner
# 3. precc ctrl + alt + n to compile and run

# gstreamer
#https://gstreamer.freedesktop.org/download/


# opencv
