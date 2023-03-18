#!/usr/bin/env bash
if [[ $OSTYPE == 'darwin'* ]]; then
  COPY_CLIP='pbcopy'
else
  COPY_CLIP='xclip -selection clipboard'
fi

find include -name "*.hpp" | sort | sed -r 's/include\/sneze\//\#include "/' | sed -r 's/\.hpp/\.hpp"/' | grep -v 'sneze.hpp' | $COPY_CLIP

echo "Headers copied to clipboard"