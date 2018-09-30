info:
	@echo "You can do: 'make run'"
	@echo "You can also do 'make clean'"

run: project.cc
	g++ -fopenmp -o run project.cc

runtacc: project.cc
	icpc -O2 -xHost -qopenmp -o run project.cc

clean:
	rm -f run
