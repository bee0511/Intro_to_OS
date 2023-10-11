#!/bin/bash

print_path=false

# Read parent pid and child pid
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      print_path=true
      shift
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi


# Check if the child's PID is in the parent's process tree
process_tree=$(pstree -p "$parent" | grep -o "$child")

if [ -n "$process_tree" ]; then
  if [ "$print_path" = true ]; then
    # Find and print the path of ancestor PIDs
    path="$child"
    while [ "$parent" -ne "$child" ]; do
    prechild=$(ps -o ppid= -p "$child" | tr -d ' ')
    path="$prechild -> $path"
    child="$prechild"
    done
    echo "Yes"
    echo "$path"
  else
    echo "Yes"
  fi
else
  echo "No"
fi
