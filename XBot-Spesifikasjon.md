# Spesifikasjon for XBot

## Generelt
**Type**: Kosebot

### Tilstandsdiagram
<img src="res/XboxTilstand.png"/>

## Tilkoblinger

| Hva                          | Beskrivelse                                  | Pinne         |
|------------------------------|----------------------------------------------|---------------|
| Port expander                | Gir flere porter                             | A4 & A5 (I2C) |
| Bluetooth (mottaker)         | BT for å få inn hvilken hjelpebot den er nær | -             |
| Buzzer	                   | Innebygd buzzer                              | D3              |
| Sensor array                 | IR sensor                   | D4, D5, D11, A0, A2, A3        |
| ZumoButton                   | Innebygd knapp                               | D12           |
| Høyre motor retning          | Brukes av backend                            | D7            |
| Venstre motor retning        | Backend?                                     | D8            |
| Høyre motor hastighet	       | Backend?                                     | D9            |
| Venstre motor hastighet      | Backend?                                     | D10           |
| BT reciever                  | Bluetooth modul                              | D2, D6        |

Ledige:
* D13	Gul LED
* D0, D1 Rx/Tx

## BT kommunikasjon

| Sone | Melding |
|------|---------|
| Ingen|         |
| Sov  | 1       |
| Spis | 2       |
| Vask | 3       |