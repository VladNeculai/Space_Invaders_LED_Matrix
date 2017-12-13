				Joc matrice – Space Invaders

	Am făcut un joc de tip Space Invaders, în care inamicii vin de sus și trebuie eliminați până să atingă nava.
	Ca hardware am folosit plăcuța Arduino Uno, o matrice de LED-uri 8x8, un driver 7219, 2 condensatoare, un joystick, 4 pushbuttons, un display LCD, rezistențe de 10 K și fire de conexiune.
	Ca abordare soft, am încercat să modularizez codul folosind clase și funcții diferite în care se află cea mai mare parte din logica jocului. Funcția de delay este folosită numai la mesajele de început și de sfârșit ale jocului, pentru a evita alte întreruperi nedorite, în rest fiind folosită funcția millis() împreună cu diverse variabile pentru a contoriza timpul în ceea ce privește mișcarea navei, a meteoriților și a rachetelor. Jocul începe prin apăsarea butonului de start și se termină atunci când ești lovit de 3 ori de un meteorit, iar la sfârșit este afișat scorul final. Joystick-ul mișcă nava pe axa oy, iar rachetele sunt trase din părțile laterale ale navei cu ajutorul a două butoane, pentru partea stângă, respectiv partea dreaptă.  De asemenea, există un buton de restart în caz că jucătorul vrea să înceapă din nou.

1.	Jocul începe cu input de la user, prin apăsarea butonului de start.
2.	În timpul jocului sunt afișate viețile și scorul, iar la final este afișat scorul.
3.	După cum am spus mai sus, jocul are scor și vieți.
4.	Dificultatea este accesibilă și se poate modifica ușor prin modificarea variabilelor de timp.
5.	Codul a fost stilizat în concordanță cu cerințele.
