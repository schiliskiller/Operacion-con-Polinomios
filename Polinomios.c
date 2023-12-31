#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {FALSE, TRUE} Bool; // Tipo booleano
typedef enum { SUMA = 1, RESTA_A, RESTA_B, MULT, DIV } TipoEjer;

// Reserva los datos para una variable con tipo-estructura Entrada
#define RESERVAR_ENT(ent) if (!((ent).datos = calloc(1, sizeof(*(ent).datos)))) {      \
                              fprintf(stderr, "ERROR: espacio no disponible");         \
                              exit(1);                                                 \
                          }

// Reserva los datos para una variable con tipo-estructura Polinomio
#define RESERVAR_POL(pol) if (!((pol).terms = malloc((pol).tam * sizeof(*(pol).terms)))) { \
                              fprintf(stderr, "ERROR: espacio no disponible");             \
                              exit(1);                                                     \
                          }

// Reserva los datos para una variable con tipo-estructura Polinomio
#define RESERVAR_TRM(trm) if (!((trm) = malloc(2 * sizeof(*(trm))))) {         \
                              fprintf(stderr, "ERROR: espacio no disponible"); \
                              exit(1);                                         \
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

/* Tipo-estructura para una matriz */
typedef struct {
    float  **matriz;
    unsigned int m, n;
} Matriz;

/* Funciones para el manejo de matrices */
Bool crearMatriz(Matriz *, unsigned int m, unsigned int n);
void liberarMatriz(Matriz *mat);

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
void fimprPol(FILE *, Polinomio);            /* Imprime el polinomio a la pantalla */
void copiarPol(Polinomio *, Polinomio); /* Copia la informacion de un polinomio a otro */
void polarr(float **, Polinomio );
Polinomio arrpol(Polinomio *, float *, int);
void agregarReg(Polinomio *, TipoEjer);
Bool imprimirReg(FILE *arch_reg, Polinomio p, Polinomio q);
void liberarReg(void);
void liberarPol(Polinomio *);

/* --------------- Operaciones con polinomios ----------------- */
void       cmntrm(Polinomio *);               // Suma terminos comunes
Polinomio  polsum(Polinomio, Polinomio);
Polinomio  polsub(Polinomio, Polinomio);
Polinomio  negpol(Polinomio);
Polinomio  polprd(Polinomio p, Polinomio q);
Polinomio *poldiv(Polinomio p, Polinomio q);

/* --------------- Funciones de acomodo ---------------------- */
void ordenar(Polinomio *p);

/* ------------------ Variables globales ----------------------- */
int        num_ejer = 0;
Bool       pol_creado = FALSE;
Polinomio *ejercicios[10];
int tipo_ejer[10];

/* --------------------- Programa principal ----------------------- */
int
main(void) {
    void (*opciones[6])(Polinomio *, Polinomio *) = {
        capt,
        suma,
        rest,
        prod,
        divi,
        impr
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

    liberarReg();

    liberarPol(&p);
    liberarPol(&q);

    return 0;
}

/* ---------------------- Funciones del programa ---------------------- */

void
pause(void) {
    while (getchar() != '\n');
    printf("\nPresione Enter para continuar...");
    getchar();
}

void
menu(Polinomio p, Polinomio q) {
    if (!pol_creado) {
        printf("Sin polinomios:\n\n\n");
    } else {
        printf("Polinomios: [Ejercicios hechos: %d]\n", num_ejer);
        printf("P(x) = ");
        imprPol(p);
        printf("Q(x) = ");
        imprPol(q);
    }
    puts("--------------------------------");
    puts("** CALCULADORA DE POLINOMIOS **\n");
    puts("[1] Capturar polinomios");
    puts("[2] Suma de polinomios");
    puts("[3] Resta de polinomios");
    puts("[4] Multiplicacion de polinomios");
    puts("[5] Division de polinomios");
    puts("[6] Imprimir ejercicio");
    puts("[0] Salir");
    printf("\n> ");
}

Bool
crearMatriz(Matriz *mat, unsigned int m, unsigned int n) {
    if (mat->matriz) liberarMatriz(mat);

    if (!(mat->matriz = malloc(m * sizeof(*mat->matriz)))) {
        printf("ERROR: espacio no disponible\n");
        return FALSE;
    }

    for (unsigned int i = 0; i < m; i++)
        *(mat->matriz + i) = calloc(n, sizeof(**mat->matriz));

    mat->m = m;
    mat->n = n;
    
    return TRUE;
}

void
liberarMatriz(Matriz *mat) {
    for (unsigned int i = 0; i < mat->m; i++)
        free(mat->matriz[i]);
    free(mat->matriz);
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
        RESERVAR_TRM(p->terms[i]);
        /* p->terms[i] = malloc(2 * sizeof(**p->terms)); */

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
            if (f.terms[i][1] == 1)
                printf("%sx", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "));
            else
                printf("%sx^%g", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "), f.terms[i][1]);
        } else if (!f.terms[i][0]) {     // Si el coeficiente es 0, no imprimir con exponente; unicamente para restas
        } else {                         // De lo contrario, imprimir tanto el coeficiente como el exponente
            if (f.terms[i][1] == 1)
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
fimprPol(FILE *arch, Polinomio f) {
    for (int i = 0; i < f.tam; i++) {
        if (!f.terms[i][1]) {            // Si el exponente es 0, imprimir solamente el coeficiente
            if (f.terms[i][0])
                fprintf(arch, "%s%g", f.terms[i][0] < 0 ? " - " : " + ",
                               fabs(f.terms[i][0]));
        } else if (fabs(f.terms[i][0]) == 1) { // Si el coeficiente es 1, no imprimir el coeficiente
            if (f.terms[i][1] == 1)
                fprintf(arch, "%sx", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "));
            else
                fprintf(arch, "%sx^%g", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "), f.terms[i][1]);
        } else if (!f.terms[i][0]) {     // Si el coeficiente es 0, no imprimir con exponente; unicamente para restas
        } else {                         // De lo contrario, imprimir tanto el coeficiente como el exponente
            if (f.terms[i][1] == 1)
                fprintf(arch, "%s%gx", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "),
                                      fabs(f.terms[i][0]));
            else
                fprintf(arch, "%s%gx^%g", f.terms[i][0] < 0 ? (!i ? "-" : " - ") : (!i ? "" : " + "),
                                   fabs(f.terms[i][0]), f.terms[i][1]);
        }
    }
    fputc('\n', arch);
}

/* Recursiva */
void
copiarPol(Polinomio *c, Polinomio p) {
    static int i = 0;
    
    /* c->terms[i] = malloc(2 * sizeof(**c->terms)); */
    RESERVAR_TRM(c->terms[i]);
    memcpy(c->terms[i], p.terms[i], 2 * sizeof(**p.terms));

    if (++i == p.tam) {
        i = 0;
        return;
    }

    copiarPol(c, p);
}

void
polarr(float **arr, Polinomio p) {
    int j = 0;
    *arr = calloc(p.terms[0][1] + 1, sizeof(*arr));

    for (int i = p.terms[0][1]; i >= 0; i--)
        if (j < p.tam && p.terms[j][1] == i) {
            *(*arr + (int) (p.terms[0][1] - i)) = p.terms[j][0];
            i = p.terms[0][1];
            j++;
            continue;
        }
}

Polinomio
arrpol(Polinomio *p, float *arr, int size) {
    int exp = size - 1;
    for (int i = 0; i < size; i++) {
        RESERVAR_TRM(p->terms[i]);

        p->terms[i][0] = arr[i];
        p->terms[i][1] = exp--;
    }
    ordenar(p);

    for (int i = 0; i < size; i++)
        if (!p->terms[i][0]) {
            p->terms[i][1] = 0;
            ordenar(p);
            cmntrm(p);
            size--;
        }
    ordenar(p);
    
    return *p;
}

void
agregarReg(Polinomio *p, TipoEjer tipo) {
    if (num_ejer < 10) {
        ejercicios[num_ejer] = p;
        tipo_ejer[num_ejer++] = tipo;
    } else
        fprintf(stderr, "Registro lleno\n");
}

Bool
imprimirReg(FILE *arch_reg, Polinomio p, Polinomio q) {
    if (ftell(arch_reg) == EOF) {
        fprintf(stderr, "ERROR: archivo no pudo abrirse\n");
        return FALSE;
    }
    fputs("Experiencia Educativa: Programacion Estructurada\n", arch_reg);
    fputs("Docente:               Ceron Alvarez Carlos Arturo\n\n", arch_reg);
    fprintf(arch_reg, "Favor de resolver lo siguiente:\n\n");

    fprintf(arch_reg, "P(x) = ");
    fimprPol(arch_reg, p);
    fprintf(arch_reg, "Q(x) = ");
    fimprPol(arch_reg, q);
    fputs("\n\n", arch_reg);

    for (int i = 0; i < num_ejer; i++) {
        fprintf(arch_reg, "%d ", i + 1);
        switch (tipo_ejer[i]) {
            case SUMA:
                fprintf(arch_reg, "(P + Q)(x) = ");
                break;
            case RESTA_A:
                fprintf(arch_reg, "(P - Q)(x) = ");
                break;
            case RESTA_B:
                fprintf(arch_reg, "(Q - P)(x) = ");
                break;
            case MULT:
                fprintf(arch_reg, "(P * Q)(x) = ");
                break;
            case DIV:
                fprintf(arch_reg, "C(x) = ");
                fimprPol(arch_reg, ejercicios[i][0]);
                fprintf(arch_reg, "  R(x) = ");
                fimprPol(arch_reg, ejercicios[i][1]);
                fputc('\n', arch_reg);
                break;
        }
        if (tipo_ejer[i] != DIV) {
            fimprPol(arch_reg, *ejercicios[i]);
            fputc('\n', arch_reg);
        }
    }

    return TRUE;
}

void
liberarReg(void) {
    for (int i = 0; i < num_ejer; i++)
        free(ejercicios[i]);
}

void
liberarPol(Polinomio *p) {
    for (int i = 0; i < p->tam; i++)
        free(p->terms[i]);
    free(p->terms);
}

/* ----------------------- Funciones de acomodo ------------------------ */
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

/* ----------------------------- Opciones ------------------------------ */
void
capt(Polinomio *p, Polinomio *q) {
    Entrada f = {.tam = 0},
            g = {.tam = 0};

    if (pol_creado) {
        printf("Hay polinomios registrados.\n"
               "Desea proceder a crear otros polinomios (s/n)? ");

        while (getchar() != '\n');
        if (getchar() == 'n') return;

        liberarPol(p);
        liberarPol(q);

        pol_creado = FALSE;
    }

    RESERVAR_ENT(f);
    RESERVAR_ENT(g);

    printf("Ingrese los siguientes polinomios P(x) (0 para terminar): ");
    ingresarEnt(&f);
    if (!f.tam) return;

    printf("Ingrese los siguientes polinomios Q(x) (0 para terminar): ");
    ingresarEnt(&g);
    if (!g.tam) return;

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

    if (!pol_creado) {
        printf("Polinomios no creados");
        free(suma);
        return;
    }

    if (num_ejer == 10) return;

    *suma = polsum(*p, *q);

    printf("\n(P + Q)(x) = ");
    imprPol(*suma);

    agregarReg(suma, SUMA);
}

void
rest(Polinomio *p, Polinomio *q) {
    Polinomio *resta = malloc(sizeof(*resta));
    int op;

    if (!pol_creado) {
        printf("Polinomios no creados");
        free(resta);
        return;
    }

    if (num_ejer == 10) return;

    printf("\nElija el orden:\n"
           "[1] P - Q\n"
           "[2] Q - P\n"
           "[0] Cancelar\n"
           "\n> ");
    scanf(" %d", &op);

    switch (op) {
        case 1:
            printf("\n(P - Q)(x) = ");
            *resta = polsub(*p, *q);
            imprPol(*resta);
            agregarReg(resta, RESTA_A);
            break;
        case 2:
            printf("\n(Q - P)(x) = ");
            *resta = polsub(*q, *p);
            imprPol(*resta);
            agregarReg(resta, RESTA_B);
            break;
    }
}

void
prod(Polinomio *p, Polinomio *q) {
    Polinomio *prdct = malloc(sizeof(*prdct));

    if (!pol_creado) {
        printf("Polinomios no creados");
        free(prdct);
        return;
    }

    if (num_ejer == 10) return;

    *prdct = polprd(*p, *q);

    printf("\n(P * Q)(x) = ");
    imprPol(*prdct);

    agregarReg(prdct, MULT);
}

void
divi(Polinomio *p, Polinomio *q) {
    Polinomio *div = malloc(2 * sizeof(*div));

    if (!pol_creado) {
        printf("Polinomios no creados");
        free(div);
        return;
    }

    if (p->tam < q->tam || p->terms[p->tam - 1][1] < 0) {
        printf("Division no existe\n");
        return;
    }

    if (num_ejer == 10) return;

    div = poldiv(*p, *q);

    printf("\nCociente: ");
    printf("\nC(x) = ");
    imprPol(div[0]);

    printf("\nResiduo: ");
    printf("\nR(x) = ");
    imprPol(div[1]);

    agregarReg(div, DIV);
}

void
impr(Polinomio *p, Polinomio *q) {
    FILE *registro;   

    if (!num_ejer) {
        printf("Ejercicios inexistentes.\n"
               "Haga operaciones de polinomios para registrar los ejercicios.\n");
        return;
    }

    if (!(registro = fopen("Ejercicios.txt", "w"))) {
        fprintf(stderr, "ERROR: archivo no se pudo abrir\n");
        return;
    }

    if (imprimirReg(registro, *p, *q))
        printf("Impresion exitosa.\n");
    else
        printf("ERROR: impresion no exitosa\n");

    fclose(registro);
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
        n.terms[i][0] = -p.terms[i][0];

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

            RESERVAR_TRM(r.terms[r.tam - 1]);
            memcpy(r.terms[r.tam - 1], men.terms[i], 2 * sizeof(*r.terms[0]));
            break;
        }

        if (men.terms[i][1] == r.terms[d][1]) {
            r.terms[d][0] += men.terms[i][0];
            break;
        }
    }

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
        RESERVAR_TRM(r.terms[j]);

        r.terms[j][0] = p.terms[i][0] * q.terms[d][0];
        r.terms[j++][1] = p.terms[i][1] + q.terms[d][1];
    }
    
    if (++i == p.tam) {
        i = j = 0;
        ordenar(&r);
        cmntrm(&r);
        cmntrm(&r);
        return r;
    }

    return polprd(p, q);
}

// Se uso el metodo de division de Horner
Polinomio
*poldiv(Polinomio p, Polinomio q) {
    Matriz     mat = {NULL};
    Polinomio *r    = malloc(2 * sizeof(*r));
    int        it = 1;
    float     *pval = NULL, *qval = NULL,
              *quo  = calloc(p.terms[0][1] - q.terms[0][1] + 1, sizeof(*quo)),
              *res  = calloc(q.terms[0][1], sizeof(*res));

    polarr(&pval, p);
    polarr(&qval, q);

    r[0].tam = p.terms[0][1] - q.terms[0][1] + 1; // Tamanio del cociente
    r[1].tam = q.terms[0][1];                     // Tamanio del residuo

    RESERVAR_POL(r[0]);                           // Polinomio del cociente
    RESERVAR_POL(r[1]);                           // Polinomio del residuo

    crearMatriz(&mat, q.terms[0][1], p.terms[0][1] + 1);

    for (; it <= p.terms[0][1] - q.terms[0][1] + 1; it++) {
        float sum = pval[it - 1];

        for (int i = 0; i < mat.m; i++)
            sum += mat.matriz[i][it - 1];

        sum /= *qval;

        for (int i = 0; i < q.terms[0][1]; i++)
            mat.matriz[it - 1][i + it] = sum * (-qval[i + 1]);

        quo[it - 1] = sum;
    }

    for (; it <= p.terms[0][1] + 1; it++) {
        float sum = pval[it - 1];

        for (int i = 0; i < mat.m; i++)
            sum += mat.matriz[i][it - 1];

        res[it - (r[0].tam + 1)] = sum;
    }

    r[0] = arrpol(&r[0], quo, r[0].tam);   
    r[1] = arrpol(&r[1], res, r[1].tam);   

    free(pval);
    free(qval);

    liberarMatriz(&mat);

    /* free(quo); */
    /* free(res); */

    return r;
}
