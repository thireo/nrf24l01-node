EESchema Schematic File Version 4
LIBS:nrf24l01-node-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Sensor:BME280 U2
U 1 1 5C06EACC
P 5675 4600
F 0 "U2" H 5245 4646 50  0000 R CNN
F 1 "BME280" H 5245 4555 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 5675 4400 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf" H 5675 4400 50  0001 C CNN
	1    5675 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5C06EBFF
P 6400 5325
F 0 "#PWR015" H 6400 5075 50  0001 C CNN
F 1 "GND" H 6405 5152 50  0000 C CNN
F 2 "" H 6400 5325 50  0001 C CNN
F 3 "" H 6400 5325 50  0001 C CNN
	1    6400 5325
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 5325 6400 5250
Text Notes 6400 4200 0    50   ~ 0
SDO: Slave Address LSB
Text HLabel 6775 4500 2    50   Input ~ 0
BME280_SCK
$Comp
L Device:C_Small C10
U 1 1 5C06ED77
P 5875 3675
F 0 "C10" H 5900 3750 50  0000 L CNN
F 1 "100nF" H 5750 3625 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5875 3675 50  0001 C CNN
F 3 "~" H 5875 3675 50  0001 C CNN
	1    5875 3675
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C11
U 1 1 5C06EDF7
P 6025 3850
F 0 "C11" H 6117 3896 50  0000 L CNN
F 1 "100nF" H 6117 3805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6025 3850 50  0001 C CNN
F 3 "~" H 6025 3850 50  0001 C CNN
	1    6025 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5C06F198
P 5950 4025
F 0 "#PWR014" H 5950 3775 50  0001 C CNN
F 1 "GND" H 5955 3852 50  0000 C CNN
F 2 "" H 5950 4025 50  0001 C CNN
F 3 "" H 5950 4025 50  0001 C CNN
	1    5950 4025
	1    0    0    -1  
$EndComp
Wire Wire Line
	5575 3525 5575 4000
Wire Wire Line
	5875 3575 5875 3525
Connection ~ 5875 3525
Wire Wire Line
	5875 3525 5575 3525
Wire Wire Line
	5775 4000 5775 3675
Wire Wire Line
	5775 3675 6025 3675
Wire Wire Line
	6025 3675 6025 3750
Wire Wire Line
	6025 3675 6025 3525
Connection ~ 6025 3675
Wire Wire Line
	6025 3525 5950 3525
Wire Wire Line
	5875 3775 5875 4000
Wire Wire Line
	5875 4000 5950 4000
Wire Wire Line
	6025 4000 6025 3950
Wire Wire Line
	5950 4025 5950 4000
Connection ~ 5950 4000
Wire Wire Line
	5950 4000 6025 4000
$Comp
L power:VCC #PWR013
U 1 1 5C070FDB
P 5950 3475
F 0 "#PWR013" H 5950 3325 50  0001 C CNN
F 1 "VCC" H 5967 3648 50  0000 C CNN
F 2 "" H 5950 3475 50  0001 C CNN
F 3 "" H 5950 3475 50  0001 C CNN
	1    5950 3475
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3475 5950 3525
Connection ~ 5950 3525
Wire Wire Line
	5950 3525 5875 3525
Wire Wire Line
	6325 3525 6025 3525
Connection ~ 6025 3525
Wire Wire Line
	6400 5250 5775 5250
Wire Wire Line
	5775 5250 5775 5200
Wire Wire Line
	5775 5250 5575 5250
Wire Wire Line
	5575 5250 5575 5200
Connection ~ 5775 5250
Wire Wire Line
	6275 4500 6775 4500
Text HLabel 6775 4300 2    50   Input ~ 0
BME280_SDO
Wire Wire Line
	6275 4300 6775 4300
Wire Wire Line
	6275 4900 6775 4900
Text HLabel 6775 4900 2    50   Input ~ 0
BME280_CSB
Wire Wire Line
	6275 4700 6775 4700
Text HLabel 6775 4700 2    50   Input ~ 0
BME280_SDI
$EndSCHEMATC
