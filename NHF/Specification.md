Programozás 1 - Nagy Házi Specifikáció - Szihalmi Botond L1U7KJ
# Source to Flow specifikáció
Én egy saját ötlet alapján kezdtem el dolgozni a nagy házimon. \
A célja hogy egy beolvasott c source fileból egy megjeleníthető folyamat ábrát hozzon létre.\
A programot mind parancssorból mind grafikus felülettel lehet irányítani.

## Parancssori irányítás
Itt nem tényleges irányítás történik, csak adott lehetőségek vannak a meghívás alatt:
- help
- theme
- output file
- input file

Ha semmilyen meghívási paraméter nincs megadva csak megnyitja a program grafikus felületét. \
Ha meg van adva a bemeneti file, azt a file-ot nyitja meg a grafikus felületen \
Ha bemeneti és kimeneti file is meg van adva, rögtön kimenti a folyamat ábra képét. \
A téma paraméter ezeknek a működését nem érinti. \
A help paraméter csak kiírja hogyan kell a parancssori irányítást használni.

## Grafikus Irányítás
[Felső menü](https://app.moqups.com/brleEga77fA5GRgd3TpBNSpikZaXF3zO/view/page/ad64222d5)\
[Folyamat ábra](https://app.moqups.com/brleEga77fA5GRgd3TpBNSpikZaXF3zO/view/page/a09b4f337)

Egér irányítás:
- görgővel lehet a folyamat ábrán belül nagyítani, kisebbíteni.
- lenyomva tartva lehet vele mozogni jobbra, balra, fel, le.
- bal kattintással lehet mozgatni a folyamat ábra pontjait.

## Témák
Saját témát lehet megadni `.ini` file-ként. \
Mindegyik típusú objektumhoz (beleértve a fő képernyőt is) külön témát kell megadni. \
Egy objektumhoz két színérték tartozik:
- háttér
- szöveg

## File mentés
A létrehozott folyamati ábrát vagy `.png` vagy `.jpg`-ként lehet elmenteni. \
A mentett file nevét és helyét a felhasználó adja meg. \
A mentett file a használt téma alapján legyen színezve.

## Kilépés
Kilépésnél rákérdezünk a felhasználóra hogy biztos meg szeretné e tenni, de automatikusan nem mentünk semmit.