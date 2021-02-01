Meno a priezvisko: Daniel Paul 
Login: xpauld00
 
## Popis a použitie
Implementácia v jazyku C++ simuluje šírenie nákazy COVID-19 s prípadným pravidelným testovaním.
Prerekvizity - OPENGL, C++, Linux.
Program je potrebne pred spustením preloziť príkazom make. Príkazom make run sa program spustí s predpripravenymi nastaveniami a odsimuluje 3 predpripravené experimenty. Program sa dá spustiť aj príkazom
./bin/main s nasledujúcimi argumentami:

• -p < intvalue > - veľkosť populacie (počet ludí, ktorí zijú v meste napr. 80000). [potrebný]

• -i < floatvalue > - percento ľudí, ktorí su infikovaní. [potrebný]

• -c < intvalue > - veľkosť mesta (<600> − > 600*600 = 360000). Pozn: veľkosť mesta/veľkosť populacie = hustota populácie [potrebný]

• -d < intvalue > - pocet dní po ktorych sa periodicky vykoná testovanie. (ak je hodnota -1, testovanie sa
nevykonáva) [potrebný]

• -t < intvalue >- pocet dní, ktore simulácia beží.[potrebný]

• -g, –graphic - umožní OPENGL grafický mód

• -q, –quick - umožní rychlejší priebeh simulácie
