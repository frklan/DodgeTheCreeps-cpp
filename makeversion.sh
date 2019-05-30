#!/bin/bash
# Make version number
# based on work by Wolfram Rösler <wolfram@roesler-ac.de>

# Get the git repository version number
REPO=$(git describe --dirty --always)

# Get the build time stamp
WHEN=$(date +"%Y-%m-%d %H:%M:%S")

# Get the user name
WHO="$USER"

# Get the machine name
WHERE=$(hostname)

# Get the OS version
WHAT=$(uname -sr)

# Put it all together
VERSION="$REPO (built $WHEN by $WHO on $WHERE with $WHAT)"

# Output the version number to the build log
echo Build version: \"$VERSION\" >&2

# Create the version.cpp file
# NOTE: No includes to speed up compilation (remember that this file is
# re-created and compiled whenever a program is linked)
echo "// This file was created by $0 on $(date)
// Do not edit - do not add to git 
char const* version() {
  return \"$VERSION\n\";
}
" > $2