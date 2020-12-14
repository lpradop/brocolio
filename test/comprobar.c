/****************************************************************/
/*   Descripci�n:                                               */
/*      Este programa lee la descripci�n del grafo desde el     */
/*      fichero entrada.txt, y lee el ciclo encontrado desde    */
/*      la entrada est�ndar.                                    */
/*      Comprueba si el ciclo encontrado es correcto, visita    */
/*      todos los nodos y el coste est� bien calculado. En caso */
/*      afirmativo escribe: El ciclo encontrado es correcto. En */
/*      caso contrario indica el error encontrado.              */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>


#define COMPRUEBA(A, B)                                                        \
  if (!(A)) {                                                                  \
    printf("Error ");                                                          \
    printf(B);                                                                 \
    printf("\n");                                                              \
    liberagrafo();                                                             \
    exit(1);                                                                   \
  }
#define COMPRUEBA1(A, B, C)                                                    \
  if (!(A)) {                                                                  \
    printf("Error ");                                                          \
    printf(B, C);                                                              \
    printf("\n");                                                              \
    liberagrafo();                                                             \
    exit(1);                                                                   \
  }
#define COMPRUEBA2(A, B, C, D)                                                 \
  if (!(A)) {                                                                  \
    printf("Error ");                                                          \
    printf(B, C, D);                                                           \
    printf("\n");                                                              \
    liberagrafo();                                                             \
    exit(1);                                                                   \
  }
#define COMPRUEBA3(A, B, C, D, E)                                              \
  if (!(A)) {                                                                  \
    printf("Error ");                                                          \
    printf(B, C, D, E);                                                        \
    printf("\n");                                                              \
    liberagrafo();                                                             \
    exit(1);                                                                   \
  }
#define ERROR_ENT                                                              \
  ": el fichero de definici�n del grafo es incorrecto, use el original."
#define ARISTA(U, V) mat[numnodos * ((U)-1) + (V)-1]

FILE* entrada = NULL;
int numnodos, numaristas, nodoinic;
int* visitados = NULL;
int* aristas = NULL;
int* mat = NULL;

void liberagrafo(void) {
  if (entrada) fclose(entrada);
  if (visitados) free(visitados);
  if (aristas) free(aristas);
  if (mat) free(mat);
}

void leegrafo(void) {
  int i, aux;
  COMPRUEBA(entrada = fopen("entrada.txt", "r"),
            "no se puede abrir el fichero de definici�n del grafo entrada.txt");
  COMPRUEBA(fscanf(entrada, "%d %d %d", &numnodos, &numaristas, &nodoinic) == 3,
            ERROR_ENT);
  COMPRUEBA(visitados = calloc(sizeof(int), numnodos + 1),
            " reservando memoria.");
  for (i = 0; i < numnodos * 2; i++)
    COMPRUEBA(fscanf(entrada, "%d", &aux) == 1, ERROR_ENT);
  COMPRUEBA(aristas = calloc(sizeof(int), numaristas * 3),
            " reservando memoria.");
  COMPRUEBA(mat = calloc(sizeof(int), numnodos * numnodos),
            " reservando memoria.");
  for (i = 0; i < numaristas; i++) {
    COMPRUEBA(fscanf(entrada, "%d %d %d", aristas + i * 3, aristas + i * 3 + 1,
                     aristas + i * 3 + 2) == 3,
              ERROR_ENT);
    COMPRUEBA(aristas[i * 3] >= 1 && aristas[i * 3] <= numnodos &&
                  aristas[i * 3 + 1] >= 1 && aristas[i * 3 + 1] <= numnodos,
              ERROR_ENT);
    ARISTA(aristas[i * 3], aristas[i * 3 + 1]) = aristas[i * 3 + 2];
    ARISTA(aristas[i * 3 + 1], aristas[i * 3]) = aristas[i * 3 + 2];
  }
  fclose(entrada);
  entrada = NULL;
}

void compruebaciclo(void) {
  int costeciclo, longciclo;
  int i, nodo1, nodo2, coste, costeacum = 0;
  COMPRUEBA(
      scanf("%d", &costeciclo) == 1,
      "en la l�nea 1: no se puede leer el coste total del ciclo encontrado.");
  COMPRUEBA(scanf("%d", &longciclo) == 1,
            "en la l�nea 2: no se puede leer el n�mero de nodos del ciclo "
            "encontrado.");
  COMPRUEBA1(
      longciclo > numnodos,
      "en la l�nea 2: el n�mero de nodos del ciclo no puede ser menor que %d.",
      numnodos + 1);
  COMPRUEBA(scanf("%d", &nodo1) == 1,
            "en la l�nea 3: no se puede leer el nodo n�mero 1 del ciclo.");
  COMPRUEBA1(nodo1 == nodoinic,
             "en la l�nea 3: el nodo inicial del ciclo debe ser %d.", nodoinic);
  visitados[nodo1] = 1;
  for (i = 1; i < longciclo; i++) {
    COMPRUEBA2(scanf("%d", &nodo2) == 1,
               "en la l�nea %d: no se puede leer el nodo n�mero %d del ciclo.",
               i + 3, i + 1);
    COMPRUEBA3(nodo2 >= 1 && nodo2 <= numnodos,
               "en la l�nea %d: nodo %d incorrecto, debe estar entre 1 y %d.",
               i + 3, nodo2, numnodos);
    COMPRUEBA3(
        coste = ARISTA(nodo1, nodo2),
        "en la l�nea %d: el ciclo pasa por la arista (%d, %d) que no existe.",
        i + 3, nodo1, nodo2);
    costeacum += coste;
    visitados[nodo2] = 1;
    nodo1 = nodo2;
  }
  COMPRUEBA2(nodo1 == nodoinic,
             "en la l�nea %d: el nodo final del ciclo debe ser %d.", i + 2,
             nodoinic);
  COMPRUEBA1(costeacum == costeciclo,
             ": el coste total calculado es incorrecto, debe ser %d.",
             costeacum);
  for (i = 1; i <= numnodos; i++)
    COMPRUEBA1(visitados[i], ": el ciclo no pasa por el nodo %d del grafo.", i);
}

int main(void) {
  leegrafo();
  compruebaciclo();
  printf("El ciclo encontrado es correcto.\n");
  liberagrafo();
  return 0;
}
