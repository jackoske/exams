#!/bin/bash

# Function to recursively find and add git submodules
add_submodules() {
  local current_dir=$1
  local parent_dir=$2

  # Loop through all items in the current directory
  for item in "$current_dir"/*; do
    if [ -d "$item" ]; then
      # Check if the item is a git repository
      if [ -d "$item/.git" ]; then
        # Compute the relative path from the main repository and ensure it starts with ./
        relative_path="./$(realpath --relative-to="$parent_dir" "$item")"
        
        # Check if the submodule already exists in .gitmodules
        if ! git config --file .gitmodules --get-regexp path | grep -q "$relative_path"; then
          echo "Adding submodule: $relative_path"
          git submodule add "$relative_path" "$relative_path"
        else
          echo "Submodule $relative_path already exists."
        fi
      else
        # Recursively check the subdirectories
        add_submodules "$item" "$parent_dir"
      fi
    fi
  done
}

# Start the recursive search and addition from the current directory
add_submodules "$(pwd)" "$(pwd)"

# Initialize and update all submodules
echo "Initializing and updating submodules..."
git submodule init
git submodule update

