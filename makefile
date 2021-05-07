run:
	g++ DPLL.cpp parser.h
	./a.out $(filename)

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#