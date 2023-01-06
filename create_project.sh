#!/bin/bash

# Check that there is an argument passed to the script
project_name="$1"
blackboard_project_name="$1""_project"

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 ProjectName." >&2
  exit 1
fi

# Check that the give project name argument is not a folder, as we will create ther new project under that folder
if [ -d "projects/$project_name" ]; then
  echo "\"projects/$project_name\" folder already exists." >&2
  exit 1
fi

# Copy the template folder and rename APP_TEMPLATE where needed with the argument containing the project name
platform_name=$(uname)
cp -R .app_template/. projects/$project_name
if [ $platform_name == "Darwin" ]; then
  sed -i '' "s/APP_TEMPLATE/$project_name/g" "projects/$project_name"/main.cpp
  sed -i '' "s/APP_TEMPLATE/$project_name/g" "projects/$project_name"/CMakeLists.txt
else
  sed -i "s/APP_TEMPLATE/$project_name/g" "projects/$project_name"/main.cpp
  sed -i "s/APP_TEMPLATE/$project_name/g" "projects/$project_name"/CMakeLists.txt
fi

# Add the project name folder as cmake subdirectory
add_subdirectory_str="add_subdirectory(${project_name} "
add_subdirectory_str+='${CMAKE_CURRENT_BINARY_DIR}/'
add_subdirectory_str+="${project_name})"
subdirectory_occurencies=$(grep -o "add_subdirectory(${project_name}" projects/CMakeLists.txt | wc -l)
if [ "$subdirectory_occurencies" -eq "0" ]; then
  echo $add_subdirectory_str >> projects/CMakeLists.txt
else
  echo "CMakeLists.txt currently contains \"$add_subdirectory_str\", skipping adding the same subdirectory"
fi

echo "\"$project_name\" successfully created." >&2
exit 0
