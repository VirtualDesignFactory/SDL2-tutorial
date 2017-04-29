# lazyFoo SDL2 Tutorial Code
Introduction to the SDL2 library for C++ programming.  

This repository is the lesson history for the turorials provided by lazyFoo.

Find the tutorials at this url:  [http://lazyfoo.net/tutorials/SDL/index.php](http://lazyfoo.net/tutorials/SDL/index.php)

## Repo structure
This repo is structured as [GitFlow](https://datasift.github.io/gitflow/IntroducingGitFlow.html), for practicing the use of GitFlow.

## What to do
- make a feature branch for each lesson
- commit lesson instructions to the feature branch
- when finished wiht a lesson, merge the feature branch into develop
- repeat for next lesson

## Building the project
use the makefile to build the project

```bash
make
```
This will produce an executable file called 'MyProgram' which can then be run from the command line

```bash
./MyProgram
```
### Make files will be different for different platforms

Linux - Unbuntu

```bash
#OBJS specifies which files to compile as part of the project
OBJS = source/*.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = MyProgram

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
```

Mac - Osx

```bash
```