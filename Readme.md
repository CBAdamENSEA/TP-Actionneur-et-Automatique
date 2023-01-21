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


![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/Configuration_timer.png)


A présent nous calculons la valeur du registre du **temps mort** (Dead Time), pour cela nous utilisons les formules suivantes: 

![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/temps_mort.png)

T est le temps mort souhaité: 2 µs (minimum) .

t represente la période de notre horloge (le pas de notre horloge): t=1/f_APB1= 5.88 ns.

X est la valeur du registre Dead Time.

Algorithme: .........

Afin d'avoir un temps mort de 2us nous avons trouvé: X= 203. 

Nous visualisons les quatres signaux PWM afin de verifier le respect du temps mort et nous avons le resultats suivants: 

![architecture](https://github.com/CBAdamENSEA/TP-Actionneur-et-Automatique/blob/master/images/dead_time.png)

sur la figure ci-dessus nous pouvons voir gràce aux curseurs que le temps mort est respecté ,ce dernier vaut 2,17us ce qui respecte le cahier des charges 

une fois que cette partie a été achevé, nous pouvons à present connecter notre STM32 au hacheur 

broche hacheur		numéro		Broche STM32
CMD_B_TOP			11			PA8
CMD_Y_TOP			12			PA9
CMD_B_BOT			29			PA11
CMD_Y_BOT			30			PA12
ISO_RESET			33			PC3
ISO_GND				36			GND
ISO_+5V				37			5V

## Sequence d'allumage 
la sequence de demarrage correspond a une impulsion que l'on envoie lorsqu'on detecte un appuie sur un bouton poussoir, ce dernier genere une interruption 
dans cette interruption on envoie la sequence de demarrage qui consiste a mettre un niveau logique 1 en sortie de PC3 puis attendre 2.5 us grace a un timer et remettre cette sortie à 0. 


## Commande de vitesse et premier test
Nous recevons l'instruction de vitesse sous la forme speed=XXXX, le traitement consiste a prendre cette valeur et a la convertir en entier puis on donne une
consigne PWM qui correspond a la valeur de la vitesse. tous d'abord nous avons donné une consigne de 50% le moteur de tourne pas , puis on a donné une valeur de 70%,
le moteur dans ce cas commence a tourner puis s'arrete ,nous avons donc remarqué que lorsqu'on donne une consigne trop élevé le moteur ne suit pas et s'arrete à 
cause d'un fort appel de courant,il se met en mode securité et il faut envoyer la sequence d'allumage a nouveau, pour resoudre a ce probleme on a du creer une rampe de pwm qui monte progressivement jusqu'a atteindre la valeur de PWM souhaité 
on à aussi remarqué que l'on ne peut pas atteindre une valeur PWM de 100 % le moteur s'arrete dans ce cas. 

## mesure de vitesse et de courant 

Dans cette partie, nous avons utilisé la broche de l'ADC de la STM32 comme entrée de notre courant. 
dans un premier temps on effectue une calibration afin de relever la valeur d'offset,le calibration se fait à vitesse nulle, le courant doit etre nul lui aussi mais on s'apercoit qu'il a une certaine valeur  
une fois que nous avons trouvé cette derniere, on calcul le courant de la maniere suivante: 

current_v=((((adc_value)/4096.0)*3.3)-offset_current)*12;
4096 etant la résolution 
la multiplication par 12 provient de la datasheet. 
grace à cette formule a pu relever le courant et verifier sa valeur grace notamment à une sonde de courant. 

la vitesse quant a elle a été calculée gràce au codeur present sur les moteurs.

## Conclusion 

Ce TP nous a été trés formateur car il nous a permis d'aborder le principe de commande des machines a courant continu, cependant nous n'avons pas eu le temps de réaliser l'asservissement du moteur. 








## Authors

Adam CHEIKH BRAHIM

Karim ABDELLAZIZ
Sami ASFARY
	