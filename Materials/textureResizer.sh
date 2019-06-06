#!/bin/bash
for f in /"$@"/*.png
	convert -resize 1024X1024  f.png f.jpg
