#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX_NOMBRE 50
#define MAX_SINTOMA 100

typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
    char sintoma[MAX_SINTOMA];
    time_t hora_registro;
} Paciente;

typedef struct Nodo {
    Paciente paciente;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *inicio;
} ListaPacientes;

typedef struct {
    ListaPacientes alta;
    ListaPacientes media;
    ListaPacientes baja;
} ListaEspera;

void inicializarLista(ListaPacientes *lista);
void registrarPaciente(ListaEspera *lista);
void mostrarPacientesEnEspera(ListaEspera *lista);
void cambiarPrioridadPaciente(ListaEspera *lista);
void mostrarMenuPrincipal(ListaEspera *lista);
void mostrarListaEspera(ListaEspera *lista);
void mostrarLista(ListaPacientes *lista); // Declaración de la función mostrarLista
int validarNombre(const char *nombre);
int validarEdad(int edad);
void agregarPaciente(ListaPacientes *lista, Paciente paciente);

int main() {
    ListaEspera lista_espera;
    inicializarLista(&lista_espera.alta);
    inicializarLista(&lista_espera.media);
    inicializarLista(&lista_espera.baja);

    mostrarMenuPrincipal(&lista_espera);

    return 0;
}

void inicializarLista(ListaPacientes *lista) {
    lista->inicio = NULL;
}

void agregarPaciente(ListaPacientes *lista, Paciente paciente) {
    Nodo *nuevo_nodo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo_nodo == NULL) {
        printf("Error al asignar memoria para el paciente.\n");
        return;
    }
    nuevo_nodo->paciente = paciente;
    nuevo_nodo->siguiente = lista->inicio;
    lista->inicio = nuevo_nodo;
}

void registrarPaciente(ListaEspera *lista) {
    system(CLEAR);
    Paciente nuevo_paciente;
    printf("Ingrese el nombre del paciente: ");
    if (scanf("%s", nuevo_paciente.nombre) != 1 || !validarNombre(nuevo_paciente.nombre)) {
        printf("Error: El nombre debe contener solo letras.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Ingrese la edad del paciente: ");
    if (scanf("%d", &nuevo_paciente.edad) != 1 || !validarEdad(nuevo_paciente.edad)) {
        printf("Error: La edad debe ser un número de hasta 3 dígitos.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Ingrese el síntoma del paciente: ");
    if (scanf("%s", nuevo_paciente.sintoma) != 1) {
        printf("Error al leer el síntoma del paciente.\n");
        while (getchar() != '\n');
        return;
    }
    nuevo_paciente.hora_registro = time(NULL);
    agregarPaciente(&lista->baja, nuevo_paciente);
    system(CLEAR);
    printf("*** Paciente registrado exitosamente ***\n");
    sleep(2);
    system(CLEAR);
}

void mostrarPacientesEnEspera(ListaEspera *lista) {
    system(CLEAR);
    mostrarListaEspera(lista);
    int opcion;
    printf("\n1. Eliminar Paciente\n");
    printf("2. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    if (scanf("%d", &opcion) != 1) {
        printf("Error al leer la entrada. Inténtelo de nuevo.\n");
        while (getchar() != '\n');
        mostrarPacientesEnEspera(lista);
        return;
    }
    switch (opcion) {
        case 1: {
            system(CLEAR);
            mostrarListaEspera(lista);
            printf("\nIngrese el nombre del paciente a eliminar: ");
            char nombre[MAX_NOMBRE];
            if (scanf("%s", nombre) != 1) {
                printf("Error al leer el nombre del paciente.\n");
                while (getchar() != '\n');
                return;
            }
            ListaPacientes *listas_prioridad[3] = {&lista->alta, &lista->media, &lista->baja};
            for (int i = 0; i < 3; ++i) {
                Nodo *actual = listas_prioridad[i]->inicio;
                Nodo *anterior = NULL;
                while (actual != NULL && strcmp(actual->paciente.nombre, nombre) != 0) {
                    anterior = actual;
                    actual = actual->siguiente;
                }
                if (actual != NULL) {
                    if (anterior != NULL) {
                        anterior->siguiente = actual->siguiente;
                    } else {
                        listas_prioridad[i]->inicio = actual->siguiente;
                    }
                    free(actual);
                    system(CLEAR);
                    printf("*** Paciente eliminado exitosamente ***\n");
                    sleep(2);
                    system(CLEAR);
                    mostrarPacientesEnEspera(lista);
                    return;
                }
            }
            printf("El paciente no se encontró en la lista.\n");
            sleep(2);
            system(CLEAR);
            mostrarPacientesEnEspera(lista);
            break;
        }
        case 2:
            mostrarMenuPrincipal(lista);
            break;
        default:
            printf("Opción no válida. Por favor, seleccione 1 o 2.\n");
            mostrarPacientesEnEspera(lista);
            break;
    }
}

void mostrarListaEspera(ListaEspera *lista) {
    system(CLEAR);
    printf("Lista de pacientes en espera:\n\n");
    printf("--- Alta prioridad ---\n");
    mostrarLista(&lista->alta);
    printf("\n--- Media prioridad ---\n");
    mostrarLista(&lista->media);
    printf("\n--- Baja prioridad ---\n");
    mostrarLista(&lista->baja);
}

void mostrarLista(ListaPacientes *lista) {
    Nodo *actual = lista->inicio;
    while (actual != NULL) {
        printf("Nombre: %s, Edad: %d, Síntoma: %s\n", actual->paciente.nombre, actual->paciente.edad, actual->paciente.sintoma);
        actual = actual->siguiente;
    }
}

void mostrarMenuPrincipal(ListaEspera *lista) {
    system(CLEAR);
    int opcion;
    do {
        printf("Menu Principal\n");
        printf("1. Registrar nuevo paciente\n");
        printf("2. Ver lista de pacientes en espera\n");
        printf("3. Cambiar prioridad de un paciente\n");
        printf("4. Atender al siguiente paciente\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Error al leer la entrada. Inténtelo de nuevo.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (opcion) {
            case 1:
                registrarPaciente(lista);
                break;
            case 2:
                mostrarPacientesEnEspera(lista);
                break;
            case 3:
                cambiarPrioridadPaciente(lista);
                break;
            case 4:
                // Lógica para atender al siguiente paciente
                break;
            case 5:
                system(CLEAR);
                printf("Saliendo del programa...\n");
                sleep(1);
                system(CLEAR);
                return;
            default:
                printf("Opción no válida. Por favor, seleccione una opción del 1 al 5.\n");
                break;
        }
    } while (opcion != 5);
}

void cambiarPrioridadPaciente(ListaEspera *lista) {
    system(CLEAR);
    mostrarListaEspera(lista);
    int opcion;
    printf("\n1. Cambiar prioridad de un paciente\n");
    printf("2. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    if (scanf("%d", &opcion) != 1) {
        printf("Error al leer la entrada. Inténtelo de nuevo.\n");
        while (getchar() != '\n');
        cambiarPrioridadPaciente(lista);
        return;
    }
    switch (opcion) {
        case 1: {
            system(CLEAR);
            mostrarListaEspera(lista);
            printf("\nIngrese el nombre del paciente: ");
            char nombre[MAX_NOMBRE];
            if (scanf("%s", nombre) != 1) {
                printf("Error al leer el nombre del paciente.\n");
                while (getchar() != '\n');
                return;
            }
            printf("Ingrese la nueva prioridad (a - alta, m - media, b - baja): ");
            char prioridad;
            if (scanf(" %c", &prioridad) != 1 || (prioridad != 'a' && prioridad != 'm' && prioridad != 'b')) {
                system(CLEAR);
                printf("Error: Ingrese una prioridad válida (a, m, b).\n");
                while (getchar() != '\n');
                sleep(2);
                system(CLEAR);
                cambiarPrioridadPaciente(lista);
                return;
            }
            ListaPacientes *lista_origen = NULL;
            switch (prioridad) {
                case 'a':
                    lista_origen = &lista->alta;
                    break;
                case 'm':
                    lista_origen = &lista->media;
                    break;
                case 'b':
                    lista_origen = &lista->baja;
                    break;
                default:
                    printf("Error: Prioridad no válida.\n");
                    return;
            }
            Nodo *actual;
            Nodo *anterior = NULL;
            // Buscar al paciente en todas las listas
            for (int i = 0; i < 3; ++i) {
                actual = NULL;
                switch (i) {
                    case 0:
                        actual = lista->alta.inicio;
                        break;
                    case 1:
                        actual = lista->media.inicio;
                        break;
                    case 2:
                        actual = lista->baja.inicio;
                        break;
                }
                while (actual != NULL && strcmp(actual->paciente.nombre, nombre) != 0) {
                    anterior = actual;
                    switch (i) {
                        case 0:
                            actual = actual->siguiente;
                            break;
                        case 1:
                            actual = actual->siguiente;
                            break;
                        case 2:
                            actual = actual->siguiente;
                            break;
                    }
                }
                if (actual != NULL) {
                    if (anterior != NULL) {
                        anterior->siguiente = actual->siguiente;
                    } else {
                        switch (i) {
                            case 0:
                                lista->alta.inicio = actual->siguiente;
                                break;
                            case 1:
                                lista->media.inicio = actual->siguiente;
                                break;
                            case 2:
                                lista->baja.inicio = actual->siguiente;
                                break;
                        }
                    }
                    switch (prioridad) {
                        case 'a':
                            actual->siguiente = lista->alta.inicio;
                            lista->alta.inicio = actual;
                            break;
                        case 'm':
                            actual->siguiente = lista->media.inicio;
                            lista->media.inicio = actual;
                            break;
                        case 'b':
                            actual->siguiente = lista->baja.inicio;
                            lista->baja.inicio = actual;
                            break;
                    }
                    system(CLEAR);
                    printf("*** Cambio de prioridad exitoso ***\n");
                    sleep(2);
                    cambiarPrioridadPaciente(lista);
                }
            }
            system(CLEAR);
            printf("Error: El paciente no se encontró en la lista.\n");
            sleep(2);
            system(CLEAR);
            break;
        }
        case 2:
            mostrarMenuPrincipal(lista);
            break;
        default:
            system(CLEAR);
            printf("Opción no válida. Por favor, seleccione 1 o 2.\n");
            sleep(2);
            system(CLEAR);
            cambiarPrioridadPaciente(lista);
            break;
    }
}


int validarNombre(const char *nombre) {
    while (*nombre) {
        if (!isalpha(*nombre)) {
            return 0;
        }
        nombre++;
    }
    return 1;
}

int validarEdad(int edad) {
    return (edad >= 0 && edad <= 999);
}
