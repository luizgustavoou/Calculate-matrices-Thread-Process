auxiliar:
	g++ auxiliar.cpp funcoesMatrix.cpp -o auxiliar
	./auxiliar

sequencial:
	g++ matriz.cpp funcoesMatrix.cpp -o matriz
	./matriz

processo:
	g++ matriz_processo.cpp -o matriz_processo
	./matriz_processo

thread:
	g++ matriz_thread.cpp -o matriz_thread
	./matriz_thread