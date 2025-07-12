# starglazer

## Librairies

 - TFT_eSPI
 - TinyGPSPlus (pas la version ESP32)
 - AccelStepper

### Configuration librairie TFT_eSPI

 - Copier Setup21_ILI9488_seb.h dans le dossier User_Setups de la librairie TFT_eSPI
 - Dans User_Setup_Select.h
   - Commenter #include <User_Setup.h>
   - Ajouter #include <User_Setups/Setup21_ILI9488_seb.h>

## Entrées/Sortie

 - Ecran + tactile
  - CS 5
  - RST 16
  - D/C 17
  - SDI 23
  - SCK 18
  - BL 26
  - SDO Pas branché
 - 2 moteurs pas à pas + detecteur HOME ?
 - Manette Wii
 - Laser
 - GPS
 - Buzzer
 - Led en dessous de l'horizon


 [328542][D][HTTPClient.cpp:378] disconnect(): tcp keep open for reuse
C1
[328548][D][HTTPClient.cpp:378] disconnect(): tcp keep open for reuse
C2
C3
lowPriorityLoop()
A
A1
[333991][D][NetworkClient.cpp:576] connected(): Disconnected: RES: 0, ERR: 128
[333998][D][HTTPClient.cpp:393] disconnect(): tcp is closed

[334004][V][HTTPClient.cpp:242] beginInternal(): url: http://192.168.1.169:22666/horizontal-coordinates/planets-and-moons/sun?latitude=44.81&longitude=-0.58&elevation=12.30
[334020][D][HTTPClient.cpp:293] beginInternal(): protocol: http, host: 192.168.1.169 port: 22666 url: /horizontal-coordinates/planets-and-moons/sun?latitude=44.81&longitude=-0.58&elevation=12.30
A2
A3
[334038][D][HTTPClient.cpp:574] sendRequest(): request type: 'GET' redirCount: 0

[334058][D][HTTPClient.cpp:1112] connect():  connected to 192.168.1.169:22666
[334118][V][HTTPClient.cpp:1201] handleHeaderResponse(): RX: 'HTTP/1.1 200 OK'
[334126][V][HTTPClient.cpp:1201] handleHeaderResponse(): RX: 'date: Fri, 11 Jul 2025 21:46:08 GMT'
[334136][V][HTTPClient.cpp:1201] handleHeaderResponse(): RX: 'server: uvicorn'
[334144][V][HTTPClient.cpp:1201] handleHeaderResponse(): RX: 'content-length: 88'
[334152][V][HTTPClient.cpp:1201] handleHeaderResponse(): RX: 'content-type: application/json'
[334161][V][HTTPClient.cpp:1201] handleHeaderResponse(): RX: ''
[334167][D][HTTPClient.cpp:1257] handleHeaderResponse(): code: 200
[334173][D][HTTPClient.cpp:1260] handleHeaderResponse(): size: 88
[334179][D][HTTPClient.cpp:618] sendRequest(): sendRequest code=200

B
C
[334187][V][HTTPClient.cpp:1399] writeToStreamDataBlock(): connection closed or file end (written: 88).
[334197][D][HTTPClient.cpp:378] disconnect(): tcp keep open for reuse
C1
[334203][D][HTTPClient.cpp:378] disconnect(): tcp keep open for reuse
C2
C3
lowPriorityLoop()
A
A1
[339644][D][NetworkClient.cpp:576] connected(): Disconnected: RES: 0, ERR: 128
[339651][D][HTTPClient.cpp:393] disconnect(): tcp is closed

[339657][V][HTTPClient.cpp:242] beginInternal(): url: http://192.168.1.169:22666/horizontal-coordinates/planets-and-moons/sun?latitude=44.81&longitude=-0.58&elevation=10.90
[339673][D][HTTPClient.cpp:293] beginInternal(): protocol: http, host: 192.168.1.169 port: 22666 url: /horizontal-coordinates/planets-and-moons/sun?latitude=44.81&longitude=-0.58&elevation=10.90
A2
A3
[339691][D][HTTPClient.cpp:574] sendRequest(): request type: 'GET' redirCount: 0

[339707][D][HTTPClient.cpp:1112] connect():  connected to 192.168.1.169:22666