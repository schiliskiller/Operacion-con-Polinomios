#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    float *coef;
    int *exp;
    int tam;
} Polinomio;

void op1(Polinomio *);

void printArr(Polinomio);

int
main(void) {
    int i = 0;
    float *f;

    if (!(f = calloc(1, sizeof(*f)))) {
        fprintf(stderr, "ERROR: espacio no disponible");
        exit(1);
    }

    printf("Ingrese los siguientes polinomios P(x): ");
    scanf(" %f", f + i++);
    while (*(f + i - 1)) {
        f = realloc(f, (i + 1) * sizeof(*f));
        scanf(" %f", f + i++);
    }

    Polinomio p = {.tam = (i + 1) / 2};

    if (!(p.coef = calloc(p.tam, sizeof(*(p.coef)))) ||
        !(p.exp  = calloc(p.tam, sizeof(*(p.exp))))) {
        fprintf(stderr, "ERROR: espacio no disponible");
        exit(1);
    }

    for (int c = 0; c < p.tam; c++) {
        *(p.coef + c) = *(f + 2 * c);
        *(p.exp + c)  = !c ? *(f + 1) : *(f + 2 * c + 1);
    }

    free(f);

    printf("P(x) = ");
    printArr(p);
    putchar('\n');
    
    return 0;
}

void
op1(Polinomio *f) {

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
