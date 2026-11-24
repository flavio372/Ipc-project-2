CC = gcc
CFLAGS = -Wall -Wextra
OBJ = main.o sistema.o equipamentos.o solicitacoes.o operacoes.o historico.o arquivos.o utils.o
lampisteria: $(OBJ)
	$(CC) $(CFLAGS) -o lampisteria $(OBJ)
%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o lampisteria
