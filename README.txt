// Usturoi Flavius-Gabriel, 314CD
// Ierarhie

	Ideea generala a codului meu in vederea rezolvarii taskurilor a fost,
bineinteles, folosirea structurii de date puse la dispozitie si a operatiilor asupra
acesteia, cum ar fi parcurgerea in preordine a arborelui, obtinerea unui anumit nod,
eliberarea memoriei etc.

			REZOLVAREA TASKURILOR

HIRE - Alocarea memoriei pentru membrul din varf in cazul in care ierarhia este goala. In caz
       contrar, angajatul va fi preluat de managerul dat ca parametru. La final, se sorteaza
       angajatii alfabetic cu functia qsort.

FIRE - In principal operatii cu pointeri. Algoritmul a distins mai multe cazuri: 1) nodul de
       sters are un singur angajat, 2) nodul de sters nu are niciun angajat, 3) nodul de sters
       are 2 sau mai multi angajati. In fiecare caz, la final, memoria alocata pentru nodul
       respectiv va fi eliberata. Pentru fiecare caz in parte, am adaugat comentarii suplimen-
       tare in cod care explica ce va face blocul de instructiuni ce urmeaza.

PROMOTE, MOVE_EMPLOYEE, MOVE_TEAM, FIRE_TEAM - Algoritmul este similar cu cel de la taskul FIRE,
       fiecare task avand mici modificari corespunzatoare cerintei acestuia. De asemenea, comen-
       tarii suplimentare in sursa care explica bucatile de cod care par mai greu de interpretat.

GET_EMPLOYEES_BY_MANAGER - Am creat o functie recursiva suplimentara care adauga intr-un vector
        numele angajatilor lui "employee_name".

GET_EMPLOYEES_BY_LEVEL - Am creat o functie recursiva suplimentara care obtine un nod de pe nivelul
        dat ca parametru. Apoi, ne putem intoarce la managerul managerului nodului obtinut (node->
	manager->manager) pentru a afisa toti angajatii de pe nivelul respectiv.

GET_BEST_MANAGER - Am creat o functie recursiva suplimentara care introduce intr-un vector toti
 	membrii ierarhiei. Apoi am parcurs vectorul cautand nodul/nodurile cu numarul maxim de
 	angajati. La final am extras aceste noduri, le-am ordonat alfabetic cu qsort si apoi le-am
	scris in fisier.