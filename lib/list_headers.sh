#!/usr/bin/env bash
find include -name "*.hpp" | sort | sed -r 's/include\/sneze\//\#include "/' | sed -r 's/\.hpp/\.hpp"/' | grep -v 'sneze.hpp'ch | xclip -selection clipboard
