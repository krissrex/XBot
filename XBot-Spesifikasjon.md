# Spesifikasjon for XBot

## Generelt
**Type**: Kosebot

### Tilstandsdiagram
<img src="res/XboxTilstand.png"/>

## Tilkoblinger

| Hva                          | Beskrivelse                                  | Pinne  |
|------------------------------|----------------------------------------------|--------|
| Bluetooth (mottaker)         | BT for å få inn hvilken hjelpebot den er nær | -      |
| Buzzer	                   | Innebygd buzzer                              | D3     |
| Sensor array                 | IR sensor                   | D4, D5, D11, A0, A2, A3 |
| ZumoButton                   | Innebygd knapp                               | D12    |
| Høyre motor retning          | Brukes av backend                            | D7     |
| Venstre motor retning        | Backend?                                     | D8     |
| Høyre motor hastighet	       | Backend?                                     | D9     |
| Venstre motor hastighet      | Backend?                                     | D10    |

Ledige:
* D13	Gul LED
* D6	Ledig
* D2	Ledig
* D0, D1 Rx/Tx