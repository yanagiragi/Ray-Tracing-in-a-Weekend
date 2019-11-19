main:
	g++ main.cpp
run:
	./a.out > output.ppm && magick.exe convert output.ppm output.png
