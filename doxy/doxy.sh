
rm -rf ../docs
doxygen
mv ../docs/html/* ../docs
rm -rf ../docs/html
mkdir -p ../docs/lib/media/sprites
cp ../lib/media/sprites/sneze.png ../docs/lib/media/sprites