#!/bin/bash

# Check that there is an argument passed to the script
project_name="$1"
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 ProjectName." >&2
  exit 1
fi

# Check that the give project name argument is not a folder, as we will create ther new project under that folder
if [ -d "$project_name" ]; then
  echo "\"$project_name\" folder already exists." >&2
  exit 1
fi

# Copy the template folder and rename APP_TEMPLATE where needed with the argument containing the project name
cp -R .app_template/. $project_name
sed -i '' "s/APP_TEMPLATE/$project_name/g" "$project_name"/main.cpp
sed -i '' "s/APP_TEMPLATE/$project_name/g" "$project_name"/CMakeLists.txt

# Add the project name folder as cmake subdirectory 
subdirectory_occurencies=$(grep -o "add_subdirectory($project_name)" CMakeLists.txt | wc -l)
if [ "$subdirectory_occurencies" -eq "0" ]; then
  echo "add_subdirectory($project_name)" >> CMakeLists.txt
else
  echo "CMakeLists.txt currently contains \"add_subdirectory($project_name)\", skipping adding the same subdirectory"
fi

echo "\"$project_name\" successfully created." >&2
exit 0