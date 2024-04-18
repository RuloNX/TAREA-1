#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOMBRE 50
#define MAX_SINTOMA 100

// Estructura del paciente
typedef struct {
  char nombre[MAX_NOMBRE];
  int edad;
  char sintoma[MAX_SINTOMA];
  time_t hora_registro;
} Paciente;

// Nodo para la lista de pacientes
typedef struct Nodo {
  Paciente paciente;
  struct Nodo *siguiente;
} Nodo;

// Lista para almacenar pacientes de cada prioridad
typedef struct {
  Nodo *inicio;
} ListaPacientes;

// Estructura para manejar las listas de pacientes de cada prioridad
typedef struct {
  ListaPacientes alta;
  ListaPacientes media;
  ListaPacientes baja;
} ListaEspera;

// Prototipos de funciones
void inicializarLista(ListaPacientes *lista);
void registrarPaciente(ListaEspera *lista);
void mostrarPacientesEnEspera(ListaEspera *lista);
void mostrarMenuPrincipal();
void mostrarLista(ListaPacientes *lista); // Prototipo añadido aquí

// Función principal
int main() {
  ListaEspera lista_espera;
  inicializarLista(&lista_espera.alta);
  inicializarLista(&lista_espera.media);
  inicializarLista(&lista_espera.baja);

  int opcion;

  do {
    mostrarMenuPrincipal();
    // Cambiar scanf() por un getchar() adicional para consumir el carácter de
    // nueva línea
    if (scanf("%d", &opcion) != 1) {
      printf("Error al leer la entrada. Inténtelo de nuevo.\n");
      while (getchar() != '\n')
        ; // Limpiar el buffer de entrada
      continue;
    }

    switch (opcion) {
    case 1:
      registrarPaciente(&lista_espera);
      break;
    case 2:
      mostrarPacientesEnEspera(&lista_espera);
      break;
    case 3:
      // Lógica para mostrar prioridad de los pacientes
      break;
    case 4:
      // Lógica para atender al siguiente paciente
      break;
    case 5:
      printf("Saliendo del programa...\n");
      break;
    default:
      printf(
          "Opción no válida. Por favor, seleccione una opción del 1 al 5.\n");
      break;
    }

  } while (opcion != 5);

  return 0;
}

// Función para inicializar una lista de pacientes
void inicializarLista(ListaPacientes *lista) { lista->inicio = NULL; }

// Función para agregar un paciente a la lista de espera según su prioridad
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

// Función para registrar un nuevo paciente
void registrarPaciente(ListaEspera *lista) {
  Paciente nuevo_paciente;
  printf("Ingrese el nombre del paciente: ");
  if (scanf("%s", nuevo_paciente.nombre) != 1) {
    printf("Error al leer el nombre del paciente.\n");
    return;
  }
  printf("Ingrese la edad del paciente: ");
  if (scanf("%d", &nuevo_paciente.edad) != 1) {
    printf("Error al leer la edad del paciente.\n");
    return;
  }
  printf("Ingrese el síntoma del paciente: ");
  if (scanf("%s", nuevo_paciente.sintoma) != 1) {
    printf("Error al leer el síntoma del paciente.\n");
    return;
  }

  // Obtener la hora actual y asignarla al paciente
  nuevo_paciente.hora_registro = time(NULL);

  // Agregar el paciente a la lista de espera de acuerdo a su prioridad
  // Aquí se podría implementar la lógica para determinar la prioridad del
  // paciente Por ahora, todos los pacientes se agregan a la lista de prioridad
  // baja
  agregarPaciente(&lista->baja, nuevo_paciente);

  printf("\n*** Paciente registrado exitosamente ***\n");
}

// Función para mostrar los pacientes en espera
void mostrarPacientesEnEspera(ListaEspera *lista) {
  // Mostrar pacientes de alta prioridad
  printf("\nPacientes de alta prioridad:\n");
  mostrarLista(&lista->alta);

  // Mostrar pacientes de media prioridad
  printf("\nPacientes de media prioridad:\n");
  mostrarLista(&lista->media);

  // Mostrar pacientes de baja prioridad
  printf("\nPacientes de baja prioridad:\n");
  mostrarLista(&lista->baja);
}

// Función para mostrar una lista de pacientes
void mostrarLista(ListaPacientes *lista) {
  Nodo *actual = lista->inicio;
  while (actual != NULL) {
    printf("Nombre: %s, Edad: %d, Síntoma: %s\n", actual->paciente.nombre,
           actual->paciente.edad, actual->paciente.sintoma);
    actual = actual->siguiente;
  }
}

// Función para mostrar el menú principal
void mostrarMenuPrincipal() {
  printf("\nMenu Principal\n");
  printf("1. Registrar nuevo paciente\n");
  printf("2. Ver lista de pacientes en espera\n");
  printf("3. Mostrar prioridad de los pacientes\n");
  printf("4. Atender al siguiente paciente\n");
  printf("5. Salir\n");
  printf("Seleccione una opción: ");
}
