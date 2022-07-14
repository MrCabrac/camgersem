uint8_t screen_state = 0;    // El estado actual de la pantalla
uint8_t menu_state = 0;      // El estado actual del menu
uint8_t statusBar_state = 0; // El estado actual de la barra de estado
uint8_t window_state = 0;    // El estado de la ventana abierta

uint32_t statusBar_bg_color = TFT_WHITE; // El color de fondo para la barra de estatus

// Hora
static uint8_t conv2d(const char *p);                                                // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

byte ohh = 99, omm = 99, oss = 99; // Valores anteriores de minutos y segundos para imprimir solo cuando cambian

// WiFi
bool enable_wifi = true;             // Habilitar/deshabilitar el uso de wifi
bool oenable_wifi = !enable_wifi;    // Valores anterior de la habilitacion de wifi por el usuario
uint8_t owifi_connection_status = 7; // Valor anterior de la conexion wifi (7 porque ese no es un estado)
String owifi_ssid = "";              // Valor anterior del SSID
long orssi = 99;
long rssi = 0;
String localIp = "";

long last_send = 0;

// const char *ssid = "UNE_HFC_0321";
const char *ssid = "MOTOG20";
// const char *password = "4TX6ZR5T";
const char *password = "laredrusaderusia";

// Configuration for NTP
const char *ntp_primary = "pool.ntp.org";
const char *ntp_secondary = "time.nist.gov";

// Una variable para guardar las opciones del menu que se muestre en pantalla
String menu_options[6] = {"Sensores", "Actuatores", "WiFi", "SD Card", "Settings", "Graphics"};
uint32_t windowBackgrounds[6] = {TFT_WHITE, TFT_WHITE, TFT_BLUE, TFT_WHITE, TFT_BLUE, TFT_BLACK};

// // Lista de actuadores
String actuators_list[] = {
    "Lampara",
    "Celda Peltier",
    "Ventilador Celda Peltier",
    "Ventilador deshumidificador",
    "Resistencia de calor calefactor 1",
    "Resistencia de calor calefactor 2",
    "Ventilador calefactor",
    "Bomba de fluido",
    "Servomotor 1",
    "Servomotor 2"};

String sensors_list[] = {
    "Temperatura externa",
    "Temperatura interna: ",
    "Temperatura 1 deshumidificador",
    "Temperatura 2 deshumidificador",
    "Temperatura calefactor",
    "Iluminacion externa",
    "Iluminacion interna",
    "Humedad interna",
    "Tapa"};

// Variables de actuadores
float actuadores[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // igual a 'actuators_list'

// Buzzer
#define buzzer 32
uint32_t buzzerStart = 0; // Momento en el cual se enciende el buzzer

// Teclado
char key;
char okey = '.'; // Valores anteriores del teclado matricial

// Variables de Sensores
float temperatura_interior = 0;   // La temperatura al interior de la cámara
float otemperatura_interior = 99; // La temperatura al interior de la cámara

float humedad_interior = 0;   // La humedad al interior de la cámara
float ohumedad_interior = 99; // La humedad al interior de la cámara

float luminosidad_interior = 0;   // La luminosidad al interior de la cámara
float oluminosidad_interior = 99; // La luminosidad al interior de la cámara

char serial_buffer[100];
uint8_t serial_buffer_counter = 0;

// Para los menus de lista
int8_t option_list_selected = 0;   // Valor para almacenar el valor seleccionado de la lista
int8_t ooption_list_selected = 99; // Valor anterior del estado de los actuadores

// Function to extract numbers from compile time string
static uint8_t conv2d(const char *p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

// Graphic configuration
#define AXIS_COLOR TFT_WHITE

uint8_t VERTICAL_DIVIDER = 8;    // 8
uint8_t HORIZONTAL_DIVIDER = 10; // 10
uint16_t graph_width = 280;      // 300
uint8_t graph_height = 200;      // 200
uint8_t graph_box_init_x = 2;    // 2
uint8_t graph_box_init_y = 18;   // 18

bool graph_visible = false;
bool buffersReady = false;
float signal_Buffer_1[280]; // !! Poner la variable (graph_width)
float signal_Buffer_2[280]; // !! Poner la variable (graph_width)
float signal_Buffer_3[280]; // !! Poner la variable (graph_width)

uint16_t periodo_muestro = 100; // Cada cuanto mostrar el valor [ms]

uint16_t signal_1_Limits[2] = {5, 53};    // temperatura_interior
uint16_t signal_2_Limits[2] = {20, 100};  // humedad_interior
uint16_t signal_3_Limits[2] = {0, 26000}; // luminosidad_interior

uint16_t maxSignals[3] = {signal_1_Limits[1], signal_2_Limits[1], signal_3_Limits[1]};
uint16_t minSignals[3] = {signal_1_Limits[0], signal_2_Limits[0], signal_3_Limits[0]};

long lastDraw = 0;
long firstDraw = 0;

float sp1 = 100; // Set-point Temperatura
float sp2 = 20;  // Set-point Humedad
float sp3 = 0;   // Set-point Iluminacion
uint8_t sps = 1; // Set-point seleccionado 1 = humedad, 2 = , 3 = temperatura, 4 = iluminacion, 5 =

float osp1 = sp1 - 1; // Set-point Humedad
float osp2 = sp2 - 1; // Set-point Temperatura
float osp3 = sp3 - 1; // Set-point Iluminacion
uint8_t osps = 0;     // Set-point seleccionado 1 = humedad, 2 = , 3 = temperatura, 4 = iluminacion, 5 =

float setpoint_y_pos1; // Posición del triángulo en Y
float setpoint_y_pos2; // Posición del triángulo en Y
float setpoint_y_pos3; // Posición del triángulo en Y

float ec1 = 0; // Esfuerzo control Temperatura
float ec2 = 0;  // Esfuerzo control Humedad
float ec3 = 0;   // Esfuerzo control Iluminacion

uint8_t triangle_width = 8;

void restartVariables()
{
    // Restart Wifi windows variables
    owifi_connection_status = 7;
    owifi_ssid = "";
    oenable_wifi = !enable_wifi;
    // Restart Actuators window variables
    option_list_selected = 0;
    ooption_list_selected = 99;
    // Restart sensor variables
    temperatura_interior = 0;
    otemperatura_interior = 99;

    humedad_interior = 0;
    ohumedad_interior = 99;

    luminosidad_interior = 0;
    oluminosidad_interior = 99;

    buffersReady = false;
    graph_visible = false;
}
