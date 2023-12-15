#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {FALSE, TRUE} Bool; // Tipo booleano

// Reserva los datos para una variable con tipo-estructura Entrada
#define RESERVAR_ENT(ent) if (!((ent).datos = calloc(1, sizeof(*(ent).datos)))) {      \
                              fprintf(stderr, "ERROR: espacio no disponible"); \
                              exit(1);                                         \
                          }

// Reserva los datos para una variable con tipo-estructura Polinomio
#define RESERVAR_POL(pol) if (!((pol).terms = malloc((pol).tam * sizeof(*(pol).terms)))) { \
                              fprintf(stderr, "ERROR: espacio no disponible"); \
                              exit(1); \
                          }

// Estructura para polinomios
typedef struct {
    float **terms; // Terminos del polinomio
    int tam;       // Numero de terminos del polinomio
} Polinomio;

// Estructura para la entrada de datos
typedef struct {
    float *datos;    // Arreglo que guarda todos los datos ingresados
    int tam;         // Tamanio del arreglo f
} Entrada;

/* --------------------- Opciones -------------------------- */
void capt(Polinomio *, Polinomio *);  // Opcion 1
void suma(Polinomio *, Polinomio *);  // Opcion 2
void rest(Polinomio *, Polinomio *);  // Opcion 3
void prod(Polinomio *, Polinomio *);  // Opcion 4
void divi(Polinomio *, Polinomio *);  // Opcion 5
void impr(Polinomio *, Polinomio *);  // Opcion 6

/* ------------- Funciones para el programa ------------------ */
void menu(Polinomio, Polinomio);
#ifndef _WIN32
void pause(void);
#endif
void ingresarEnt(Entrada *);        /* El programa le pasa un prompt al usuario
                                     * para ingresar los datos a una variable
                                     * tipo Entrada. */
void entapol(Entrada, Polinomio *); /* Pasa los datos de la variable tipo Entrada
                                     * a otra de tipo Polinomio */
void imprPol(Polinomio);            /* Imprime el polinomio a la pantalla */
void copiarPol(Polinomio *, Polinomio); /* Copia la informacion de un polinomio a otro */
void liberarReg(void);

/* --------------- Operaciones con polinomios ----------------- */
void      cmntrm(Polinomio *);               // Suma terminos comunes
Polinomio polsum(Polinomio, Polinomio);
Polinomio polsub(Polinomio, Polinomio);
Polinomio negpol(Polinomio);
Polinomio polprd(Polinomio p, Polinomio q);

/* --------------- Funciones de acomodo ---------------------- */
void ordenar(Polinomio *p);

/* ------------------ Variables globales ----------------------- */
int       num_ejer = 0;
Bool      pol_creado = FALSE;
Polinomio *ejercicios[10];

/* --------------------- Programa principal ----------------------- */
int
main(void) {
    void (*opciones[6])(Polinomio *, Polinomio *) = {
        capt,
        suma,
        rest,
        prod,
        /* divi, */
        /* impr */
    };

    Polinomio p, q;
    int op;

    do {	
        system("cls||clear");
        menu(p, q);
        scanf("%d",&op);
        if (op > 0 && op <= 6) {
            (*opciones[op - 1])(&p, &q);
#ifdef _WIN32
            system("pause");
#else
            pause();
#endif
        }
    } while (op);

    puts("\nGracias por usar el programa!");

    return 0;
}

/* ---------------------- Funciones del programa ---------------------- */

void
pause(void) {
    while (getchar() != '\n');
    printf("Presione Enter para continuar...");
    getchar();
}

void
menu(Polinomio p, Polinomio q) {
    if (!pol_creado) {
        printf("Sin polinomios:\n\n\n");
    } else {
        printf("Polinomios:\n"
               "P(x) = ");
        imprPol(p);
        printf("G(x) = ");
        imprPol(q);
    }
    puts("--------------------------------");
    puts("** CALCULADORA DE POLINOMIOS **\n");
    puts("[1] Capturar polinomios");
    puts("[2] Suma de polinomios");
    puts("[3] Resta de polinomios");
    puts("[4] Multiplicacion de polinomios");
    puts("[5] Dividicion de polinomios");
    puts("[6] Imprimir ejercicio");
    puts("[0] Salir");
    printf("\n> ");
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
        // Crea el espacio para meter el termino en el polinomio
        p->terms[i] = malloc(2 * sizeof(**p->terms));

        // Asignacion de coeficiente
        p->terms[i][0] = *(f.datos + 2 * i); 
        // Asignacion de exponente
        p->terms[i][1]  = !i ? round(*(f.datos + 1)) : round(*(f.datos + 2 * i + 1));
    }
}

void
imprPol(Polinomio f) {
    for (int i = 0; i < f.tam; i++) {
        if (!f.terms[i][1]) {            // Si el exponente es 0, imprimir solamente el coeficiente
            if (f.terms[i][0])
                printf("%s%g", f.terms[i][0] < 0 ? " - " : " + ",
                               fabs(f.terms[i][0]));
        } else if (fabs(f.terms[i][0]) == 1) { // Si el coeficiente es 1, no imprimir el coeficiente
            if (fabs(f.terms[i][1]) == 1)
                printf("%sx", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "));
            else
                printf("%sx^%g", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "), f.terms[i][1]);
        } else if (!f.terms[i][0]) {     // Si el coeficiente es 0, no imprimir con exponente; unicamente para restas
        } else {                         // De lo contrario, imprimir tanto el coeficiente como el exponente
            if (fabs(f.terms[i][1]) == 1)
                printf("%s%gx", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "),
                                fabs(f.terms[i][0]));
            else
                printf("%s%gx^%g", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "),
                                   fabs(f.terms[i][0]), f.terms[i][1]);
        }
    }
    putchar('\n');
}

void
ordenar(Polinomio *p) {
    for (int i = 0; i < p->tam; i++) {
    	float *aux;

        for (int j = i + 1; j < p->tam; j++)
            if (p->terms[i][1] < p->terms[j][1]){
                aux         = p->terms[i];
                p->terms[i] = p->terms[j];
                p->terms[j] = aux;
            }
    }
}

/* Recursiva */
void
copiarPol(Polinomio *c, Polinomio p) {
    static int i = 0;
    
    c->terms[i] = malloc(2 * sizeof(*p.terms[i]));
    memcpy(c->terms[i], p.terms[i], 2 * sizeof(**p.terms));

    if (++i == p.tam) {
        i = 0;
        return;
    }

    copiarPol(c, p);
}

void
liberarReg(void) {

}

/* ----------------------------- Opciones ------------------------------ */
void
capt(Polinomio *p, Polinomio *q) {
    Entrada f = {.tam = 0},
            g = {.tam = 0};

    RESERVAR_ENT(f);
    RESERVAR_ENT(g);

    printf("Ingrese los siguientes polinomios P(x) (0 para terminar): ");
    ingresarEnt(&f);
    printf("Ingrese los siguientes polinomios Q(x) (0 para terminar): ");
    ingresarEnt(&g);

    p->tam = f.tam / 2;
    q->tam = g.tam / 2;

    RESERVAR_POL(*p);
    RESERVAR_POL(*q);

    entapol(f, p);
    entapol(g, q);

    free(f.datos);
    free(g.datos);

    puts("\nLos datos introducidos fueron:");

    printf("\nP(x) = ");
    imprPol(*p);

    printf("Q(x) = ");
    imprPol(*q);
    putchar('\n');

    ordenar(p);
    ordenar(q);

    cmntrm(p);
    cmntrm(q);

    puts("\nAcomodado: ");

    printf("P(x) = ");
    imprPol(*p);

    printf("Q(x) = ");
    imprPol(*q);
    putchar('\n');

    pol_creado = TRUE;
}

void
suma(Polinomio *p, Polinomio *q) {
    Polinomio *suma = malloc(sizeof(*suma));

    *suma = polsum(*p, *q);

    printf("\n(P + Q)(x) = ");
    imprPol(*suma);
    ejercicios[num_ejer++] = suma;
}

void
rest(Polinomio *p, Polinomio *q) {
    Polinomio *resta = malloc(sizeof(*resta));
    int op;
    printf("\nElija el orden:\n"
           "[1] P - Q\n"
           "[2] Q - P\n"
           "[0] Cancelar\n"
           "\n> ");
    scanf(" %d", &op);

    switch (op) {
        case 1:
            printf("\n(P - Q)(x) = ");
            imprPol(polsub(*p, *q));
            break;
        case 2:
            printf("\n(Q - P)(x) = ");
            imprPol(polsub(*q, *p));
            break;
        default:
            break;
    }
}

void
prod(Polinomio *p, Polinomio *q) {
    Polinomio *prdct = malloc(sizeof(*prod));

    *prdct = polprd(*p, *q);

    printf("\n(P * Q)(x) = ");
    imprPol(*prdct);
    if (num_ejer < 10)
    ejercicios[num_ejer++] = prdct;
}

/* ------------------ Op. polinomios ----------------- */
void
cmntrm(Polinomio *p) { // Suma terminos comunes
    for (int i = 0; i < p->tam; i++)
        if (i + 1 < p->tam && p->terms[i][1] == p->terms[i + 1][1]) {
            p->terms[i][0] += p->terms[i + 1][0];
            p->terms[i + 1][1] = 0;

            ordenar(p);
            free(p->terms[p->tam - 1]);
            p->tam--;
        }
}

Polinomio
negpol(Polinomio p) {
    Polinomio n = { .tam = p.tam };

    RESERVAR_POL(n);

    copiarPol(&n, p);
 
    for (int i = 0; i < p.tam; i++)
        n.terms[i][0] = 0 - p.terms[i][0];

    return n;
}

/* Recursiva */
Polinomio
polsum(Polinomio p, Polinomio q) {
    static int i = 0;
    static Polinomio r;
    Polinomio max = { .tam = p.tam }, // Polinomio con mas monomios
              men = { .tam = q.tam }; // Polinomio con menos monomios

    if (max.tam == men.tam) {
        max = p;
        men = q;
    } else {
        max = p.tam > q.tam ? p : q,
        men = p.tam < q.tam ? p : q;
    }
        
    if (!i) {
        r.tam = max.tam;

        RESERVAR_POL(r);
        copiarPol(&r, max);
    } 

    for (int d = 0; d <= max.tam && i < men.tam; d++) {
        if (d == max.tam) {
            r.terms = realloc(r.terms, ++r.tam * sizeof(*r.terms));

            r.terms[r.tam - 1] = malloc(2 * sizeof(**r.terms));
            memcpy(r.terms[r.tam - 1], men.terms[i], 2 * sizeof(*r.terms[0]));
            break;
        }

        if (men.terms[i][1] == max.terms[d][1]) {
            r.terms[i][0] += men.terms[i][0];
            break;
        }    }

    if (++i == max.tam) {
        i = 0;
        ordenar(&r);
        return r;
    }

    return polsum(p, q);
}

Polinomio
polsub(Polinomio p, Polinomio q) {
    return polsum(p, negpol(q));
}

/* Recursiva */
Polinomio
polprd(Polinomio p, Polinomio q) {
    static int i = 0, j = 0;
    static Polinomio r;

    if (!i) {
        r.tam   = p.tam * q.tam;
        r.terms = malloc(p.tam * q.tam * sizeof(*r.terms));
    }

    for (int d = 0; d < q.tam; d++) {
        r.terms[j] = malloc(2 * sizeof(**r.terms));

        r.terms[j][0] = p.terms[i][0] * q.terms[d][0];
        r.terms[j++][1] = p.terms[i][1] + q.terms[d][1];
    }
    
    if (++i == p.tam) {
        i = j = 0;
        ordenar(&r);
        cmntrm(&r);
        return r;
    }

    return polprd(p, q);
}
