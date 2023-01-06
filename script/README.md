# Projet CS351 (2022-2023): Émulateur MIPS

---
title: Projet CS351 (2022-2023): Émulateur MIPS
author: GUERIN BARAL
date: 5/12/2022
---

## Rendu 1

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Vérifié que `make test-cli` ne renvoie pas d'erreur.
  - [x] Vérifié que `make tests` se plaint sur le nombre d'instructions et pas
      sur l'existence des fichiers de sortie.
  - [x] Vu que les soumissions se font avec `make tar`.


## Rendu 2

(Une dizaine de lignes devrait suffire à chaque fois)

* Comment avez-vous choisi de programmer l'analyse de texte (dont la lecture
des opérandes entières) ?

L'analyse de texte fonctionne sur le principe suivant :
  - L'assembleur va lire le fichier texte où se trouve les instructions, et s'arrêtera à la fin du fichier.
  - Il récupère puis traduit les instructions ligne par ligne, et agira différemment en fonction des instructions décodées
  à l'aide d'un switch.
    - Par exemple, dans le cas d'un ADDI rd, rs1, imm, une fois que l'assembleur aura remarqué l'instruction ADDI,
    avec en premier un registre (ici de destination), un registre (ici de source), et enfin une valeur immédiate.
    - Si jamais le cas précédent n'est pas satisfait, alors l'assembleur passe à la ligne suivante et précise que
    la syntaxe de la ligne était incorrecte.
    - Dans le cas d'un branchement, l'assembleur continuera à lire le fichier jusqu'à trouver le début de dit-branchement,
    en ignorant les autres lignes.
    - Il devra aussi être capable d'ignorer les instructions "vides" (exemple, NOP).   

* Avez-vous vu des motifs récurrents émerger ? Avez-vous "factorisé" ces motifs
pour éviter de les répéter ? Si non, serait-ce difficile ?

Nous avons vu des motifs émerger, par exemple lors de la recherche de types de motifs. En effet, si l'on regarde les registres
utilisés, ainsi que les valeurs associées pour certaines instructions, elles sont identiques. 
Ainsi, cela réduira le nombre de cas à traiter à l'aide d'un switch, car on pourra rentrer dans ce cas lorsqu'un des cas donnés
est satisfait (ici nom d'une instruction).
Pour être précis, il nous suffira de distinguer simplement trois cas d'instructions :
  - R-instruction $rd, $rs, $rt
  - I-instruction $rt, $rs, immediate
  - J-instruction target

* Comment avez-vous procédé pour écrire les tests ? Étes-vous confiant·e·s que
toutes les instructions gérées sont couvertes ? 

Nous avons écris les tests de manière à tester les cas simples, puis particuliers de notre assembleur, notamment en mettant
des instructions volontairement fausses, ou alors avec des valeurs impossibles. Nous sommes ainsi confiants que toutes les instructions gérées sont couvertes.

* Quelle a été votre expérience avec l'utilisation et la compréhension de la
documentation (ie. des annexes) ?

La compréhension et l'utilisation de la documentation n'a pas toujours été simple.

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Implémenté la traduction pour des instructions de toutes les catéories
      (calcul, comparaisons, sauts, HI/LO, mémoire)
  - [x] Implémenté la traduction pour toutes les instructions de l'annexe 2
  - [x] Pris en compte les cas particuliers : valeurs négatives, hors limites,
      noms d'instructions ou opérandes invalides...


## Rendu 3

(10/15 lignes devrait suffire à chaque fois)

* Quelle structure en modules planifiez-vous d'utiliser pour l'émulateur ?

Pour l'émulateur d'assembleur que nous avons planifié, nous utiliserons une structure en modules pour faciliter son utilisation et sa flexibilité. La structure en modules comprendra les éléments suivants :
  - Un module pour l'analyse et l'exécution des instructions MIPS (en mode interactif ou en mode fichier)
  - Un module pour la gestion de la mémoire et des registres

* Quelles seront les fonctions principales de chaque module ?

Les fonctions principales de chaque module seront les suivantes :
- Pour le module d'analyse et d'exécution des instructions MIPS :
  - Récupération des opérandes de l'instruction
  - Traduction des opérandes en héxadécimal
  - Retour du code héxadécimal complet correspondant à l'instruction
  - Retranscription de l'héxadécimal en instruction et exécution
- Pour le module de gestion de la mémoire et des registres :
  - Récupération et enregistrement des valeurs des registres
  - Application des changements aux registres en fonction des instructions
  - Retour des valeurs des registres à la fin de l'exécution des instructions

* Quels avantages voyez vous à cette structure (à comparer à un unique fichier)?

Les avantages de cette structure en modules par rapport à un fichier unique sont les suivants :
  - Permet l'exécution indépendante des modules
  - Possibilité de choisir entre les modes interactif et fichier en fonction des besoins
  - Réutilisation du code facilitée
  - Facilité de maintenance

## Rendu 4

* Avez-vous réussi à suivre la structure prévue au rendu 3 ? Quelles
modifications ont été nécessaires ? Rétrospectivement, est-ce que cette
structure était bien prévue ?

[COMPLÉTER ICI]

* Avez-vous compris le fonctionnement de chaque instruction à partir de la
documentation fournie ? Si non, quels sont les points obscurs ?

[COMPLÉTER ICI]

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les
comparaisons et sauts ? La mémoire ?

[COMPLÉTER ICI]

* Le sujet spécifie-t-il tous les détails nécessaires pour bien implémenter la
mémoire ? Quels choix avec-vous faits ?

[COMPLÉTER ICI]

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

[COMPLÉTER ICI]

* D'autres remarques sur votre programme ?

[COMPLÉTER ICI]

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [ ] Implémenté l'émulation de toutes les instructions gérées par le rendu 2.
  - [ ] Implémenté l'émulation de toutes les instructions.
  - [ ] Tous vos tests qui passent.
  - [ ] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [ ] Testé les cas particuliers : valeurs négatives, overflows...
  - [ ] Testé les cas d'erreur : division par zéro, sauts invalides...
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

[COMPLÉTER ICI]
