# blackboard_app

Blackboard App is a combination of SDL2, BGFX and IMGUI as base to create a desktop application (potentially also WebAssembly) with a user inteterface and a gpu rendering context.

Few steps are needed to create a new project using this repository as template:

 - Create your repo from this template
 - `git clone git@github.com:dashandslash/blackboard_app_template.git && cd blackboard_app_template`
 - `./create_project.sh NewProjectName`
 - `mkdir build_folder && cd build_folder`
 - `cmake .. -G [Xcode, "Visual Studio 17 2022", Ninja...]` this step will take couple of minutes as all the dependencies will be fetched and some will be built
 - `cmake --build . --target NewProjectName -j` (or by using Xcode/Visual Studio)
 
 The application has ImGui docking and multiviewport enabled out of the box.
 
 <img width="1440" alt="app_screenshot" src="https://user-images.githubusercontent.com/920858/186971987-e6d05232-c445-47b5-b4f8-a0d62f126aa6.png">

It is a Work In Progress project, but you can already create an application and start pushing pixels ;) 
