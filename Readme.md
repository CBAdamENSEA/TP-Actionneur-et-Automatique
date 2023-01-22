# TP Actionneur et automatique applique


## But du TP 
Le TP vise à utiliser la carte STM32 (Nucleo-STM32G474RE) pour commander une machine à courant continu , la liaison entre l'étage numérique et l'étage de puissance se fera grace au PowerModule 70097.

Le but finale étant d'acquerir les données de vitesse et de courant pour asservir le moteur.

## Pinout
Nous avons configuré notre STM32 de la maniere suivante: 

![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/Pinout.PNG)

## TP1: Commande MCC basique

Cette premiere partie consiste à générer quatre signaux PWM à partir du TIMER 1, les quatre signaux PWM doivent respecter les contraintes suivantes:

Ils doivent être en mode **complémentaire décalée**.

**Fréquence** des signaux PWM : 16 kHz.

**Temps mort** minimum : 2 µs.

**Résolution** minimum : 10 bits.



## Configuration du TIMER

Pour les signaux PWM, nous avons utilisé le Timer 1 avec lequel on peut activer deux signaux PWM complémentaires par channel 
(PWM Generation CHx,CHxN). 
Donc on peut créer une **commande complémentaire** décalée de rapport cyclique alpha avec deux channels 
(channel 1: rapport cyclique=alpha, channel 2: rapport cyclique=1-alpha)

Nous avons besoin d'une **fréquence** de 16kHz pour la PWM avec une résolution de 10 bits minimum. 
Avec une ARR à (1024-1) et une fréquence f_APB à 170 MHz, on n'a pas réussi à trouver une bonne valeur de PSC pour avoir 16kHz.
Donc nous avons opté pour une ARR à (1062-1) et un PSC à (10-1) pour avoir une fréquence de 16,022 kHz qui se rapproche le plus des 16 kHz demandé.


![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/Configuration_timer_1_60.png)


A présent nous calculons la valeur du registre du **temps mort** (Dead Time), pour cela nous utilisons les formules suivantes: 

![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/temps_mort_50.png)

T est le temps mort souhaité: 2 µs (minimum) .

t represente la période de notre horloge (le pas de notre horloge): t=1/f_APB1= 5.88 ns.

X est la valeur du registre Dead Time.

D'abord, on calcule combien de pas de t il nous faut pour arriver à T : Nbre de pas=T/t=340 

C'est entre 256 et 504 donc: T=(32+X[4..0]))*8*t avec X[7..5]=110(binaire)=192(décimal) 

Donc X[4..0]=T/(8*t)-32=11 D'où X[7..0]=11+192=203

Nous visualisons les quatres signaux PWM afin de vérifier le respect du temps mort et nous avons les resultats suivants: 

![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/dead_time_65.png)

Sur la figure ci-dessus nous pouvons voir grâce aux curseurs que le temps mort a été respecté (il vaut 2,17us).

Une fois que cette partie a été achevé, nous pouvons à present connecter notre STM32 au hacheur 

| **Nom de la broche (hacheur)** | **Numéro de la broche (hacheur)** | **Broche STM32** |
| ------------------------------ | --------------------------------- | ---------------- |
| CMD_B_TOP | 11	| PA8 |
| CMD_Y_TOP | 12	| PA9 |
| CMD_B_BOT | 29	| PA11 |
| CMD_Y_BOT | 30	| PA12 |
| B_CROSSING | 6 | PA15 |
| Y_CROSSING | 7 | PA1 |
| BUS_SHUNT | 3 | PA0 |
| ISO_RESET | 33	| PC3 |
| ISO_GND | 36 | GND |
| ISO_+5V | 37 | 5V |

## Séquence d'allumage 

La séquence de démarrage correspond à une impulsion de 2us minimum sur le pin ISO_RESET du hacheur. On lance cette séquence
lorsqu'on detecte un appuie sur un bouton poussoir ou losqu'on reçoie "start" sur le shell.

On active le pin ISO_RESET, on lance le Timer 7 pour compter les 2.5us et désactive le pin ensuite.


## Commande de vitesse et premier test
Nous recevons l'instruction de vitesse sous la forme "speed=XX" (XX est un pourcentage).

Le traitement consiste à convertir en entier la valeur de la vitesse, la limiter entre 1 et 99 (RC alpha), 
l'introduire au Timer (alpha pour le registre CCR1 et 1-alpha pour le registre CCR2) pour changer les rapports cycliques.

D'abord, nous avons donné une consigne de 50% le moteur de tourne pas , puis on a donné une valeur de 70%,
le moteur dans ce cas commence a tourner puis s'arrete.

Nous avons donc remarqué que lorsqu'on donne une consigne trop élevé le moteur ne suit pas et s'arrete à 
cause d'un fort appel de courant. Il se met en mode securité et il faut envoyer la séquence d'allumage a nouveau.

Afin de resoudre ce probleme, on a créé une rampe de rapport cyclique de la PWM qui monte progressivement jusqu'a atteindre la valeur souhaitée. 
Nous avons utilisé l'interruption du Timer 6 afin de réaliser cette rampe.
On a aussi remarqué que l'on ne peut pas atteindre une valeur PWM de 100 % le moteur s'arrete dans ce cas. 

## Mesure de vitesse et de courant 

Dans cette partie, nous avons utilisé la broche de l'ADC1 de la STM32 comme entrée du retour courant du hacheur.
 
Dans un premier temps on effectue une calibration afin de relever la valeur de l'offset. La calibration se fait à vitesse nulle, 
Le courant doit être nul lui aussi, donc la tension doit être à 2.5V comme indiqué dans la datasheet. Mais on s'apercoit qu'il a une certaine
variation.

Une fois que nous avons trouvé cette dernière, on trouve le courant de la manière suivante: 
current_v=((((adc_value)/4096.0)*3.3)-offset_current)*12;

4096 est la valeur max de la tension capté par l'ADC qui correspond à 3.3V.
 
La multiplication par 12 provient de la datasheet: 12A/V. 

Pour lancer la mesure de courant, nous avons utilisé l'interruption du Timer 1 qui gèrent les PWM, afin d'avoir la valeur moyenne du courant.


Concernant la vitesse, nous l'avons calculé en utilisant le Timer 2 en mode encodeur en le connectant avec les sorties de l'encodeur. 

Le calcul de vitesse se fait dans l'interruption du Timer 8 (donc chaque 100 ms (10Hz)). On récupère la valeur du compteur du Timer 2 (encodeur)
et on la multiplie par la fréquence d'échantiollonage (10Hz) et on divise par la résolution de l'encodeur: 2048.

A la fin de chaque mesure, on remet le compteur du Timer 2 à 0 pour préparer pour la prochaine mesure.

## Conclusion 

Ce TP nous a été trés formateur car il nous a permis d'aborder le principe de commande des machines à courant continu, 
cependant nous n'avons pas eu le temps de réaliser l'asservissement du moteur. 








## Authors

Adam CHEIKH BRAHIM

Karim ABDELLAZIZ

Sami ASFARY
	