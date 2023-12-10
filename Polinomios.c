#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Reserva los datos para una variable con tipo-estructura Entrada
#define RESERVAR_ENT(ent) if (!((ent).datos = calloc(1, sizeof(*(ent).datos)))) {      \
                              fprintf(stderr, "ERROR: espacio no disponible"); \
                              exit(1);                                         \
                          }
// Reserva los datos para una variable con tipo-estructura Polinomio
#define RESERVAR_POL(pol) if (!((pol).coef = calloc((pol).tam, sizeof(*((pol).coef)))) || \
                              !((pol).exp  = calloc((pol).tam, sizeof(*((pol).exp))))) { \
                              fprintf(stderr, "ERROR: espacio no disponible"); \
                              exit(1); \
                          }

// Estructura para polinomios
typedef struct {
    float *coef; // Guarda los coeficientes
    int *exp;    // Guarda los exponentes
    int tam;     // Numero de terminos del polinomio
} Polinomio;

// Estructura para la entrada de datos
typedef struct {
    float *datos;    // Arreglo que guarda todos los datos ingresados
    int tam;     // Tamanio del arreglo f
} Entrada;

/* void op1(Polinomio *); */

void ingresarEnt(Entrada *);
void entapol(Entrada, Polinomio *); // Pasa los datos de la variable tipo Entrada
                // a otra de tipo Polinomio
void printArr(Polinomio);

int
main(void) {
    Entrada f = {.tam = 0};

    RESERVAR_ENT(f);

    printf("Ingrese los siguientes polinomios P(x): ");
    ingresarEnt(&f);

    Polinomio p = {.tam = (f.tam + 1) / 2};

    RESERVAR_POL(p);

    entapol(f, &p);

    free(f.datos);

    printf("P(x) = ");
    printArr(p);
    putchar('\n');
    
    return 0;
}

void
op1(Polinomio *f) {

}

void
ingresarEnt(Entrada *ent) {
    scanf(" %f", ent->datos + ent->tam++);
    while (*(ent->datos + ent->tam - 1)) {
        ent->datos = realloc(ent->datos, (ent->tam + 1) * sizeof(*ent->datos));
        scanf(" %f", ent->datos + ent->tam++);
    }
}

void
entapol(Entrada f, Polinomio *p) {
    for (int i = 0; i < p->tam; i++) {
        *(p->coef + i) = *(f.datos + 2 * i);
        *(p->exp + i)  = !i ? *(f.datos + 1) : *(f.datos + 2 * i + 1);
    }
}

void
printArr(Polinomio f) {
    for (int i = 0; i < f.tam; i++) {
        if (!*(f.exp + i)) {
            if (*(f.coef + i))
                printf("%s%g", f.coef[i] < 0 ? " - " : " + ",
                               fabs(f.coef[i]));
        } else {
            printf("%s%gx^%d", f.coef[i] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "),
                               fabs(f.coef[i]), f.exp[i]);
        }
    }
}
