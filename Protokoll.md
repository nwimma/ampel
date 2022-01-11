---
Datum: 11.02.2022
Name: "Niklas Wimmer"
Gruppe: "Hamacsek, Wimmer"
Fach: "Syt Embedded Devices"
Übung: "EK713 Embedded Devices 'Hardware-Programming'"
Lehrer: "BORM"
---

Github Repo: https://github.com/nwimma/ampel
# EK713 Embedded Devices "Hardware-Programming" (BORM) 
## Theoriefragen 
##### Wo sind große Unterschiede zwischen C und anderen hohen Programiersprachen? 
- C ist nicht Objektorientiert 
	- daraus ergibt sich: keinerlei Vererbung oder all das 
- Keine Exceptions oder Generics
- Die Standard Library beinhaltet keine Collections
 - Kein Speicher Management (Speicher muss von Hand zugewiesen und natuerlich auch wieder freigegeben werden)
 - C bietet mehr flexibilität aber weniger Sicherheit, da man "näher an der Maschine arbeitet"
 
##### Was ist ein Header-File und wo werden in C die Funktionen implementiert? 
Ein Header File in C hat die Endung **.h** und darin werden Funktionsdeklarationen sowie Macro-Definitionen welche zwischen Source Files gesharet werden geschrieben. Man kann Funktionen (auch mittels eines Header-files) über `#include` einbinden. Funktionen müssen am Anfang deklariert werden, bevor sie benutzt werden können. Vergisst man dies und versucht die Funktion zu einem Zeitpunkt zu benutzen (wo die Funktion noch nicht implementiert wurde), hat man ein Problem (ich spreche aus Erfahrung), da die Funktion nicht definiert wird. 

Es gibt 2 Header Files: 
  1) File vom Entwickler 
  2) 2) Files vom Compiler  

[1] 

##### Wie kommen Toolchains bzw. Firmwares bei der Programmierung von Embedded Systems zum Einsatz?
CMake und Make ist unsere Toolchain. Sie wird benutzt um automatisch Dependencies waehrend der Kompilierung bereit zu stellen und damit der C Compiler nicht selbst aufgerufen werden muss. Also quasi ein Abstraction Layer.

##### Wie sind Datenblätter eines Mikrokontrollers und des Demo-Boards (z.B. Raspberry-Pico) aufgebaut? Wo finde ich dabei die _Interface Description_ der Toolchain? [2] 


##### Was bedeutet das Akronym _HAL_ bei der Implementierung von Embedded Devices? 
**HAL**: Hardware Abstraction Layer

Ein HAL wird von Betriebssystemen benutzt um mit unterliegenden Hardware zu kommunizieren. Der Hauptvorteil ist hierbei, dass nicht auf Hardwareebene selbst kommuniziert wird, es koennen also viel allgemeinere Programme geschrieben werden, welche auf verschiedener Hardware funktioniert.

Andere Vorteile beinhalten HAL:
 - Ermöglicht es Anwendungen, so viel Leistung wie möglich aus den Hardware-Geräten herauszuholen
 - Ermöglicht es dem Betriebssystem, unabhängig von der Hardware-Architektur zu arbeiten
 - Ermöglicht Gerätetreibern den direkten Zugriff auf jedes Hardware-Gerät, wodurch Programme geräteunabhängig sein können
 - Ermöglichung der Kommunikation von Softwareprogrammen mit den Hardware-Geräten auf einer allgemeinen Ebene
 - Erleichterung der Portabilität

[3]

##### Was macht der Befehl **make** und wie ist das Makefile im [pico-template](https://github.com/mborko/pico-template) Repository aufgebaut?
Command um erstellen von Porgrammgruppen **make** ermittelt welche Programmteile neu kompiliert werden müssen und erteilt entsprechende Befehle. Man kann make für jede Aufgabe verwenden, bei der Dateien automatisch aus anderen aktualisiert werden müssen, wenn sich diese ändern. [4] 

##### Wo ist die Methode _BSP_LED_Init()_ definiert und wo implementiert? Welche Schritte werden in der implementierten Methode durchgeführt? Was ist dabei der Pull-Mode?
Diese Methode war jene um beim stm32 die LEDs zu initialiserieren. Beim Pico heißt diese Methode **stdio_init_all()**. Diese Methode kommt von der SDK und ist in der Runtime Infrastructure. 
```
Initialize all of the present standard stdio types that are linked into the binary. Call this method once you have set up your clocks to enable the stdio support for UART, USB and semihosting based on the presence of the respective libraries in the binary. When stdio_usb is configured, this method can be optionally made to block, waiting for a connection via the variables specified in [stdio_usb_init](https://raspberrypi.github.io/pico-sdk-doxygen/group__pico__stdio__usb.html#gab87bcfa3f24e5a3fe92a944f9eecc460)
```
(i.e. PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS) [docs](https://raspberrypi.github.io/pico-sdk-doxygen/group__pico__stdio.html#gadd999f115d6f239056f3b15bfacb3726)

##### Was ist ein GPIO-Port und wieviele sind auf dem Raspberry Pico einsetzbar? 
GPIO (General Purpose Input Output) pins dienen als Schnittstellen zu anderen Systemen/Schaltungen, welche vom Raspberry Pi gesteuert werden können. Bei entsprechender Programmierung können digitale Signale von außen angenommen (Input) und welche nach außen abgegeben werden (Output) [6] 
Laut Datenblatt hat der Pico 1-22 & 25-28 GPIO Ports. [2] 
Die Ports 26-28 können als digitale GPIO oder ADC Eingänge verwendet werden. [7 - Seite 16] 

##### Auf welchem GPIO Pin liegen die einzelnen LEDs des Raspberry Pico Boards? 
Unsere LEDs liegen auf den GPIO Pins 1,2,3 (Board Pin 2,4,5). Der Port 25 ist der GPIO Pin für die On Board LED [2] 

##### Wie könnten die LEDs auch ohne der implementierten Library angesteuert werden? 

## Umsetzung 
### Aufbau
Benoetigt werden
- 3 LEDs (in Ampel Farben, also gruen, gel und rot)
- einen passenden Widerstand (zB 250 Ohm)
- ein Steckbrett
- den Raspberry Pi Pico + Verbindungskabel

Die LEDs werden mit den im Code angegeben GPIO Pins des Pico verbunden, der Widerstand wird als Vorwiderstand eingesetzt. Danach ist die Ampel einsatzbereit.

Pin belegung: ![[pins.jpeg]] Grundlegender Aufbau: ![[overview.jpeg]]

### Programmierumgebung
Um vernuenftig programmieren zu koennen, braucht man entweder ein Linux Ubuntu (oder Debian) oder ein Linux Arch. Dort muessen dann mit folgenden Befehlen (exemplarisch fuer Arch) einige Packages installiert und die Pico SDK aufgesetzt werden.

Die Befehle werden alle als root ausgefuehrt, fuer das installieren von `yay` muss allerdings ein normaler Benutzer verwendet werden (`vagrant` in diesem Fall).
```bash
# system upgrade
pacman -Suy --noconfirm --noprogressbar
# install needed packages
pacman -S   --noconfirm --noprogressbar --needed \
	git arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib cmake base-devel python3
    
# install yay
sudo -uvagrant git clone https://aur.archlinux.org/yay.git && cd yay && sudo -uvagrant makepkg -si --noconfirm --noprogressbar --needed && cd ..

# setup yay
yay -Y --gendb
yay -Suy --devel --noconfirm --noprogressbar --needed

# install AUR packages
yay -S --noconfirm --noprogressbar --needed picotool openocd-picoprobe

# setup Pico SDK
git clone -qb master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule -q update --init
echo export PICO_SDK_PATH=$PWD >> ~/.bashrc
```

Anschliessend kann das [mborko/pico-template](https://github.com/mborko/pico-template) verwendet werden, um schnell die notwendige Projektstruktur zu initialisieren.

Dort kann dann einer der Unterordner (`hello` zum Beispiel) kopiert werden um die Dateien darin fuer die Ampel anzupassen. Wichtig ist auch die `CMakeLists.txt` Datei zu bearbeiten und einen neuen `add_subdirectory` Eintrag hinzuzufuegen.

Danach kann man schon mit der Programmierung beginnen.

### Kompilieren
Es empfiehlt sich dafuer einen eigenen `build` Ordner zu erstellen, in welchem die folgenden Befehl ausgefuehrt werden. Dieser kann zum Beispiel als Unterordner im Projekt erstellt werden.

Anschliessend werden folgende Befehl ausgefuehrt, um das Projekt zu kompilieren.

```bash
cmake ..
cd ampel
make -j4
```

### Mount Pico
Mit `dmesg | tail` kann herausgefunden werden, als welches Device der Pico angeschlossen wurde (zum Beispiel `/dev/sdb1`).
Dieses kann anschliessend mitttels `mount /dev/sdb1 /mnt/pico` gemounted werden (in dem Directory `/mnt/pico`).

Dass davor erstellte `.uf2` File kann nun in diesen Ordner kopiert werden. Sobald `sync` ausgefuehrt wird, started der Pico das uebertragene Programm auszufuehren.

Danach sollte man den Pico auch wieder unmounten (mittel `umount /mnt/pico` oder `umount /dev/sdb1` - macht keinen Unterschied).

## Quellen 
- \[1] "C - Header Files"; tutorialspoint.com; Zuletzt besucht: 21.12.2021; [online](https://www.tutorialspoint.com/cprogramming/c_header_files.htm) 
- \[2] "Raspberry Pi Pico Datasheet"; datasheet.raspberrypi; Zuletzt besucht: 21.12.2021; [online](https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf) 
- \[3] "Hardware Abstraction Layer"; sciencedirect.com; Zuletzt besucht: 21.12.2021; [online](https://www.sciencedirect.com/topics/computer-science/hardware-abstraction-layer#:~:text=In%20computers%2C%20a%20hardware%20abstraction,at%20a%20detailed%20hardware%20level.) 
- \[4] "make - Unix,Linux Command"; tutorialspoint.com; Zuletzt besucht: 21.12.2021 [online](https://www.tutorialspoint.com/unix_commands/make.htm) 
- \[5] "C for Java Programmers" George Ferguson; zuletzt besucht: 21.12.2021; [online](https://www.cs.rochester.edu/u/ferguson/csc/c/c-for-java-programmers.pdf) 
- \[6] "Raspberry Pi: GPIO - General Purpose Input Output"; Elektronik Kompendium; Zuletzt besucht: 21.12.2021; [online](https://www.elektronik-kompendium.de/sites/raspberry-pi/2002191.htm) 
- \[7] "Raspberry Pi Pico"; Dogan Ibrahim; Zuletzt besucht: 21.12.2021; [online](https://issuu.com/eimworld/docs/raspberry-pi-pico-leseprobe?e=10037764/86812737)
